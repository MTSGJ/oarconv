/**
@brief   This is rewrite code of SculptMesh.cs of OpenSim for JunkBox_Lib.

@file    SculptMesher.cpp
@author  Fumi.Iseki  iseki@solar-system.tuis.ac.jp
@version 1.1
@date    2015 5/27

@sa http://opensimulator.org/
@sa http://wiki.secondlife.com/wiki/Sculpted_Prims:_Technical_Explanation
@sa OpenSim/Region/Physics/Meshing/SculptMesh.cs
@sa Viewer_Source: indra\llmath\llvolume.cpp: sculptGenerateMapVertices()
*/


/*
 * see also http://opensimulator.org/
 * see also http://wiki.secondlife.com/wiki/Sculpted_Prims:_Technical_Explanation
 * see also OpenSim/Region/Physics/Meshing/SculptMesh.cs
 * see also Viewer_Source: indra\llmath\llvolume.cpp: sculptGenerateMapVertices()
 * We got advice from Janus Dugong
 *
 * OpenSim CONTRIBUTORS.TXT is in OpenSim.License directory.
 */

 /*
  * Copyright (c) Contributors
  * See CONTRIBUTORS.TXT for a full list of copyright holders.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met:
  *     * Redistributions of source code must retain the above copyright
  *       notice, this list of conditions and the following disclaimer.
  *     * Redistributions in binary form must reproduce the above copyright
  *       notice, this list of conditions and the following disclaimer in the
  *       documentation and/or other materials provided with the distribution.
  *     * Neither the name of the OpenSimulator Project nor the
  *       names of its contributors may be used to endorse or promote products
  *       derived from this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE DEVELOPERS ``AS IS'' AND ANY
  * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY
  * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  */

#include "SculptMesher.h"


using namespace jbxl;



///////////////////////////////////////////////////////////////////////////////////////////////
//
// ScupltMesh Class
//

void  SculptMesh::init(int typ)
{
    clear();

    xscale = 2;
    yscale = 2;

    flipUV = false;
    flipU  = false;
    flipV  = false;

    sizetype = SCULPT_SIZE_OTHER;
    //
    set_type(typ);
}


void  SculptMesh::clear(void)
{
    clear_data();
    clear_image();
}


void  SculptMesh::clear_data(void)
{
    coords.clear();
    normals.clear();
    uvs.clear();

    sculptTriIndex.clear();
    sculptTriArray.clear();
}


void  SculptMesh::clear_image(void)
{
    int rnum = (int)sculptImage.size();
    for (int i=0; i<rnum; i++) {
        sculptImage[i].clear();
    }
    sculptImage.clear();
}


void  SculptMesh::set_type(int stype)
{
    int chkinv, chkmir;
    invert = false;
    mirror = false;

    type   = stype & 0x07;      // 0000 0111    
    chkinv = stype & 0x40;      // 0100 0000 (64)
    chkmir = stype & 0x80;      // 1000 0000 (128)

    if (chkinv!=0) invert = true;
    if (chkmir!=0) mirror = true;
    if (mirror) invert = !invert;
}


MSGraph<double> SculptMesh::MakeSculptImage(MSGraph<uByte> grd)
{
    MSGraph<double> sculpt;
    sculpt.state = SCULPT_IMAGE_ERROR;
    if (grd.isNull() || grd.zs<3) return sculpt;

    int psize = grd.xs*grd.ys;

    double pixScale = 1.0/256.0;
    int    offsetA = 0;            // アルファチャンネルが存在する場合のオフセット
    bool   reverse = false;        // true: BGR

    //
    if (grd.color==GRAPH_COLOR_RGB || grd.color==GRAPH_COLOR_RGBA) {
        // nop
    }
    else if (grd.color==GRAPH_COLOR_XRGB || grd.color==GRAPH_COLOR_ARGB) {
        if (grd.zs==4) offsetA = psize;
    }
    else if (grd.color==GRAPH_COLOR_BGR  || grd.color==GRAPH_COLOR_BGRA) {
        reverse = true;
    }
    else if (grd.color==GRAPH_COLOR_XBGR || grd.color==GRAPH_COLOR_ABGR) {
        if (grd.zs==4) offsetA = psize;
        reverse = true;
    }
    else {
        return sculpt;        // none supported color mode
    }

    sculpt.getm(grd.xs, grd.ys, 3);

    for (int j=0; j<grd.ys; j++) {
        int offsetYS = j * grd.xs;
        int offsetYY = offsetYS + offsetA;
        //
        for (int i=0; i<grd.xs; i++) {
            int offsetS = i + offsetYS;
            int offsetX = i + offsetYY;
            double rval = (double)grd.gp[offsetX];              // R
            double gval = (double)grd.gp[offsetX + psize];      // G
            double bval = (double)grd.gp[offsetX + psize * 2];  // B
            //
            if (reverse) {
                double swap = rval;
                rval = bval;
                bval = swap;
            }

            sculpt.gp[offsetS          ] = rval * pixScale - 0.5f;
            sculpt.gp[offsetS + psize  ] = gval * pixScale - 0.5f;
            sculpt.gp[offsetS + psize*2] = bval * pixScale - 0.5f;
        }
    }
    sculpt.state = SCULPT_IMAGE_NORMAL;

    return sculpt;
}


//
//
bool  SculptMesh::Image2Coords(MSGraph<uByte> grd)
{
    if (grd.isNull() || grd.zs<3) return false;

    sizetype = GetSculptScale(grd.xs, grd.ys, &xscale, &yscale, &xshift, &yshift);

    MSGraph<double> sculpt = MakeSculptImage(grd);
    if (sculpt.isNull()) return false;

    int xsize = sculpt.xs;
    int ysize = sculpt.ys;
    int psize = sculpt.xs*sculpt.ys;
    //
    for (int j=0; j<ysize; j++) {
        if ((j-yshift)%yscale==0 || j==0 || j==ysize-1) {
            SCULPT_VECTOR_ARRAY row;
            int offsetY = j*sculpt.xs;
            //
            for (int i=0; i<xsize; i++) {
                if ((i-xshift)%xscale==0 || i==0 || i==xsize-1) {   // xshift is not used
                    //
                    int offset  = i + offsetY;
                    double rval = sculpt.gp[offset];                // R
                    double gval = sculpt.gp[offset + psize];        // G
                    double bval = sculpt.gp[offset + psize * 2];    // B
                    //
                    if (mirror) row.push_back(Vector<double>(-rval, gval, bval));
                    else        row.push_back(Vector<double>(rval, gval, bval));
                }
            }
            if (row.size() > 0) sculptImage.push_back(row);
        }
    }
    sculpt.free();
    return true;
}


void  SculptMesh::GenerateMeshData(void)
{
    clear_data();

    if (sculptImage.size()==0) {
        PRINT_MESG("SculptMesh::GenerateMeshData(): WARNING: image data size is 0!\n");
        return;
    }

    int xs = (int)sculptImage[0].size();
    int ys = (int)sculptImage.size();
    int ox = xs;
    int oy = ys;

    // X方向境界処理
    if (type!=SCULPT_TYPE_PLANE) {
        if (ys%2==0) {
            for (int j=0; j<ys; j++) sculptImage[j].push_back(sculptImage[j][0]);
        }
        else {
            for (int j=0; j<ys; j++) sculptImage[j][0] = sculptImage[j][xs-1];
        }
    }

    Vector<double> topPole = sculptImage[0][xs/2];
    Vector<double> bottomPole = sculptImage[ys-1][xs/2];
    xs = (int)sculptImage[0].size();

    // Y方向境界処理
    if (type==SCULPT_TYPE_SPHERE) {
        if (ys%2==0) {
            SCULPT_VECTOR_ARRAY bottomPoleRow;
            //
            for (int i=0; i<xs; i++) {
                sculptImage[0][i] = topPole;
                bottomPoleRow.push_back(bottomPole);
            }
            sculptImage.push_back(bottomPoleRow);
        }
        else {
            for (int i=0; i<xs; i++) {
                sculptImage[0][i] = topPole;
                sculptImage[ys-1][i] = bottomPole;
            }
        }
    }
    else if (type==SCULPT_TYPE_TORUS) {
        sculptImage.push_back(sculptImage[0]);
    }
    ys = (int)sculptImage.size();

    DEBUG_MODE PRINT_MESG("SculptMesh::GenerateMeshData(): Sculpt Type = %d, Size = (%d, %d)\n", type, xs, ys);

    //
    double du = 1.0f/(ox-1);
    double dv = 1.0f/(oy-1);
    int   p1, p2, p3, p4;

    for (int j=0; j<ys; j++) {
        for (int i=xs-1; i>=0; i--) {
            coords.push_back(sculptImage[j][i]);
            normals.push_back(Vector<double>(0.0, 0.0, 0.0));
            uvs.push_back(UVMap<double>(du*i, 1.0-dv*j));
        }
    }

    for (int j=1; j<ys; j++) {
        int jj = j*xs;
        for (int i=xs-1; i>0; i--) {
            p4 = i + jj;
            p3 = p4 - 1;
            p2 = p4 - xs;
            p1 = p3 - xs;

            ContourTriIndex t1, t2;
            if (!invert) {
                t1.mlt_set(p1, p4, p3);
                t2.mlt_set(p1, p2, p4);
            }
            else {
                t1.mlt_set(p1, p3, p4);
                t2.mlt_set(p1, p4, p2);
            }
            //
            sculptTriIndex.push_back(t1);
            sculptTriIndex.push_back(t2);
        }
    }

    if (flipUV) {
        execFlipUV();
    }
    else {
        if (flipU) execFlipU();
        if (flipV) execFlipV();
    }

    ComputeTriNormals(xs, ys);
    SetupTriArray();
}


void  SculptMesh::ComputeTriNormals(int xs, int ys)
{
    int tnum = (int)sculptTriIndex.size();

    for (int i=0; i<tnum; i++) {
        ContourTriIndex indx = sculptTriIndex[i];
        Vector<double> normal = indx.SurfaceNormal(&coords);
        normals[indx.n1] = normals[indx.n1] + normal;
        normals[indx.n2] = normals[indx.n2] + normal;
        normals[indx.n3] = normals[indx.n3] + normal;
    }
    int nnum = (int)normals.size();
    for (int i=0; i<nnum; i++) normals[i] = normals[i].normalize();

    // X方向境界（繋ぎ目）の処理
    if (type != SCULPT_TYPE_PLANE) {
        for (int j=0; j<ys; j++) {
            int jj = j*xs;
            Vector<double> normal = (normals[jj] + normals[xs-1+jj]).normalize();
            normals[jj] = normal;
            normals[xs-1+jj] = normal;
        }
    }

    return;
}


void  SculptMesh::SetupTriArray(void)
{
    int tnum = (int)sculptTriIndex.size();

    for (int i=0; i<tnum; i++) {
        ContourTriIndex indx = sculptTriIndex[i];
        ContourTriData  tridat;
        tridat.contourNum = 0;
        tridat.v1  = coords [indx.v1];
        tridat.v2  = coords [indx.v2];
        tridat.v3  = coords [indx.v3];
        tridat.n1  = normals[indx.n1];
        tridat.n2  = normals[indx.n2];
        tridat.n3  = normals[indx.n3];
        tridat.uv1 = uvs    [indx.uv1];
        tridat.uv2 = uvs    [indx.uv2];
        tridat.uv3 = uvs    [indx.uv3];
        sculptTriArray.push_back(tridat);
    }
    return;
}


void  SculptMesh::execShift(double x, double y, double z)
{
    Vector<double> vert;

    int cnum = (int)coords.size();
    for (int i=0; i<cnum; i++) {
        vert = coords[i];
        vert.x += x;
        vert.y += y;
        vert.z += z;
        coords[i] = vert;
    }

    int tnum = (int)sculptTriArray.size();
    for (int i=0; i<tnum; i++) {
        ContourTriData tri = sculptTriArray[i];
        tri.execShift(x, y, z);
        sculptTriArray[i] = tri;
    }
}


void  SculptMesh::execRotate(Quaternion<double> q)
{
    int cnum = (int)coords.size();
    for (int i=0; i<cnum; i++) coords[i] = VectorRotation(coords[i], q);

    int nnum = (int)normals.size();
    for (int i=0; i<nnum; i++) normals[i] = VectorRotation(normals[i], q);

    int tnum = (int)sculptTriArray.size();
    for (int i=0; i<tnum; i++) {
        ContourTriData tri = sculptTriArray[i];
        tri.v1 = VectorRotation(tri.v1, q);
        tri.v2 = VectorRotation(tri.v2, q);
        tri.v3 = VectorRotation(tri.v3, q);
        tri.n1 = VectorRotation(tri.n1, q);
        tri.n2 = VectorRotation(tri.n2, q);
        tri.n3 = VectorRotation(tri.n3, q);
        sculptTriArray[i] = tri;
    }
}


void  SculptMesh::execScale(double x, double y, double z)
{
    Vector<double> m(x, y, z);

    int cnum = (int)coords.size();
    for (int i=0; i<cnum; i++) {
        coords[i].x *= m.x;
        coords[i].y *= m.y;
        coords[i].z *= m.z;
    }

    int tnum = (int)sculptTriArray.size();
    for (int i=0; i<tnum; i++) {
        ContourTriData tri = sculptTriArray[i];
        tri.v1.x *= m.x;
        tri.v1.y *= m.y;
        tri.v1.z *= m.z;
        tri.v2.x *= m.x;
        tri.v2.y *= m.y;
        tri.v2.z *= m.z;
        tri.v3.x *= m.x;
        tri.v3.y *= m.y;
        tri.v3.z *= m.z;
        sculptTriArray[i] = tri;
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////
//

/**
　Sculpted Prim のスケールを決定する．

*/
int   jbxl::GetSculptScale(int width, int height, int* xscale, int* yscale, int* xshift, int* yshift)
{
    int type = SCULPT_SIZE_OTHER;
    int s, t;

    // default
    *xscale = 2;
    *yscale = 2;
    *xshift = 0;
    *yshift = 0;

    if (width ==0) width  = 1;
    if (height==0) height = 1;

    GetSculptResolution(width, height, &s, &t);

    if (width==32 && height==32) {
        type = SCULPT_SIZE_32x32;
        *xscale = 1;
        *yscale = 1;
    }
    //
    else if (width==64 && height== 64) {
        type = SCULPT_SIZE_64x64;
    }
    else if (width==32 && height==128) {
        type = SCULPT_SIZE_32x128;
    }
    else if (width==128 && height==32) {
        type = SCULPT_SIZE_128x32;
    }
    else if (width==8 && height==512) {
        type = SCULPT_SIZE_8x512;
    }
    else if (width==512 && height==8) {
        type = SCULPT_SIZE_512x8;
    }
    else if (width==16 && height==256) {
        type = SCULPT_SIZE_16x256;
    }
    else if (width==256 && height==16) {
        type = SCULPT_SIZE_256x16;
        *xscale = 2;
        *yscale = 2;
    }
    //
     else if (width==64 && height==128) {
        //????
        type = SCULPT_SIZE_64x128;
        *xscale = 2;
        *yscale = 2;
    }
    else if (width==128 && height==64) {
        //????
        type = SCULPT_SIZE_128x64;
        *xscale = 2;
        *yscale = 2;
    }
    //
    else if (width==128 && height==128) {
        type = SCULPT_SIZE_128x128;
        *xscale = 4;
        *yscale = 4;
    }
    else if (width==64 && height==256) {
        type = SCULPT_SIZE_64x256;
        *xscale = 4;
        *yscale = 16;
    }/*
    else if (width==256 && height==64) {
        type = SCULPT_SIZE_256x64;
        *xscale = 4;
        *yscale = 16;
    }*/
    else if (width==32 && height==512) {
        type = SCULPT_SIZE_32x512;
        *xscale = 4;
        *yscale = 4;
    }
    else if (width==512 && height==32) {
        type = SCULPT_SIZE_512x32;
        *xscale = 4;
        *yscale = 4;
    }/*
     else if (width==16 && height==1024) {
        type = SCULPT_SIZE_16x1024;
        *xscale = 4;
        *yscale = 4;
    }*/
    else if (width==1024 && height==16) {
        type = SCULPT_SIZE_1024x16;
        *xscale = 4;
        *yscale = 4;
    }
    //
    else if (width==256 && height==256) {
        type = SCULPT_SIZE_256x256;
        *xscale = 8;
        *yscale = 8;
    }

    /*
    else if (width==256 && height==512) {
        type = SCULPT_SIZE_256x512;
        *xscale = 11;
        *yscale = 11;
    }*/
    else if (width==512 && height==256) {
        type = SCULPT_SIZE_512x256;
        *xscale = 11;
        *yscale = 11;
    }
    //
    else if (width==512 && height==512) {
        type = SCULPT_SIZE_512x512;
        *xscale = 16;
        *yscale = 16;
    }
    //
    else if (width==1024 && height==1024) {
        type = SCULPT_SIZE_1024x1024;
        *xscale = 32;
        *yscale = 32;
    }
    //
    else {
        *xscale = width/s;
        *yscale = height/t;
    }
    if (width*height<=4096) *yshift = 1;
    
    PRINT_MESG("JBXL::GetSculptScale: type = %d, size = (%d, %d), st = (%d, %d), scale = (%d, %d)/(%d, %d), yshift = %d\n", 
                                                                type, width, height, s, t, *xscale, *yscale, width/s, height/t, *yshift);
    return type;
}



void  jbxl::GetSculptResolution(int width, int height, int* s, int* t)
{
    int v = Min(1024, width*height/4);
    double r = (double)width/(double)height;
    *s = (int)(sqrt(v/r) /* + 0.5*/);
    *s = Max(*s, 4);
    *t = v/(*s);
    *t = Max(*t, 4);
    *s = v/(*t);
    if (*s==0) *s = 1;
    if (*t==0) *t = 1;
    
    int xs, ys;
    if (width > height) {
        xs = Max(*s, *t);
        ys = Min(*s, *t);
    }
    else {
        xs = Min(*s, *t);
        ys = Max(*s, *t);
    }
    *s = xs;
    *t = ys;

    return;
}

