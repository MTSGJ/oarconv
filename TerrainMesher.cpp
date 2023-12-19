
/**
@brief OpenSimの Terrain用Mesher．SculptMesherを参考にしている．

@file    TerrainMesher.cpp
@author  Fumi.Iseki, K.Suzuki
@date    2014 10/

@sa http://opensimulator.org/
@sa http://www.nsl.tuis.ac.jp/xoops/modules/xpwiki/?OpenSim%2Fr32file
@sa OpenSim/Region/Physics/Meshing/SculptMesh.cs
*/

#include "TerrainMesher.h"


using namespace jbxl;


///////////////////////////////////////////////////////////////////////////////////////////////
// TerrainMesh Class
//

void  TerrainMesh::init(void)
{
    clear();
    
    bound_left   = true;
    bound_right  = true;
    bound_top    = true;
    bound_bottom = true;

    xsize        = 256;
    ysize        = 256;

    max_height   = -HUGE_VALF;
    min_height   =  HUGE_VALF;
    seabed       = TERRAIN_MESH_SEABED_LEVEL;
}


//
void  TerrainMesh::clear(void)
{
    clear_data();
    clear_image();
}


//
void  TerrainMesh::clear_data(void)
{
    coords.clear();
    normals.clear();
    uvs.clear();

    terrainTriIndex.clear();
    terrainTriArray.clear();
}


//
void  TerrainMesh::clear_image(void)
{
    int rnum = (int)terrainImage.size();
    for (int i=0; i<rnum; i++) {
        terrainImage[i].clear();
    }
    terrainImage.clear();
}


//
bool  TerrainMesh::Image2Coords(MSGraph<float> grd)
{ 
    if (grd.isNull() || grd.zs!=1) return false;

    xsize = grd.xs;
    ysize = grd.ys;

    float height;
    for (int j=0; j<ysize; j++) {
        TERRAIN_VECTOR_ARRAY row;
        int jj = j*xsize;
        for (int i=0; i<xsize; i++) {
            height = float_from_little_endian(grd.gp+i+jj);
            row.push_back(Vector<float>((float)i, (float)j, height));
            if (height>max_height) max_height = height;
            if (height<min_height) min_height = height;
        }
        terrainImage.push_back(row);
    }

    return true;
}


//
void  TerrainMesh::GenerateMeshData(Vector<float> shift, bool autoseabed)
{
    clear_data();

    if (terrainImage.size()==0) {
        PRINT_MESG("TerrainMesh::GenerateMeshData(): WARNING: image data size is 0!\n");
        return;
    }

    int xs  = (int)terrainImage[0].size();
    int ys  = (int)terrainImage.size();
    int oxs = xs;
    int oys = ys;
    int stx = 0;
    int sty = 0;

    if (autoseabed && min_height<=max_height) seabed = min_height - 1.0f;

    // X方向境界処理
    // Left
    if (bound_left) {
        Vector<float> left (0.0f, 0.0f, seabed);
        for (int j=0; j<ys; j++) {
            left.y  = (float)j;
            terrainImage[j].insert(terrainImage[j].begin(), left);
        }
        xs += 1;
        stx = -1;
    }
    // Right
    if (bound_right) {
        Vector<float> right(xs-1.0f, 0.0f, seabed);
        for (int j=0; j<ys; j++) {
            right.y = (float)j;
            terrainImage[j].push_back(right);
        }
        xs += 1;
    }

    // Y方向境界処理
    // Top
    if (bound_top) {
        Vector<float> top(0.0f, 0.0f, seabed);
        TERRAIN_VECTOR_ARRAY topSeabed;
        topSeabed.push_back(top);
        for (int i=1; i<xs-1; i++) {
            top.x    = (float)(i-1);
            topSeabed.push_back(top);
        }
        top.x = (float)(oxs-1);
        topSeabed.push_back(top);
        terrainImage.insert(terrainImage.begin(), topSeabed);
        ys += 1;
        sty = -1;
    }
    // Bottom
    if (bound_bottom) {
        Vector<float> bottom(0.0f, ys-1.0f, seabed);
        TERRAIN_VECTOR_ARRAY bottomSeabed;
        bottomSeabed.push_back(bottom);
        for (int i=1; i<xs-1; i++) {
            bottom.x = (float)(i-1);
            bottomSeabed.push_back(bottom);
        }
        bottom.x = (float)(oxs-1);
        bottomSeabed.push_back(bottom);
        terrainImage.push_back(bottomSeabed);
        ys += 1;
    }

    //
    float du = 1.0f/(oxs-1);
    float dv = 1.0f/(oys-1);
    int p1, p2, p3, p4;

    for (int j=0; j<ys; j++) {
        int jj = j*xs;
        for (int i=0; i<xs; i++) {
            p4 = i + jj;
            p3 = p4 - 1;
            p2 = p4 - xs;
            p1 = p3 - xs;

            coords.push_back(terrainImage[j][i]);
            normals.push_back(Vector<float>(0.0f, 0.0f, 0.0f));
            uvs.push_back(UVMap<float>(du*(i+stx), dv*(j+sty)));

            if (i>0 && j>0) {
                FacetTriIndex t1, t2;
                t1.mlt_set(p1, p4, p3);
                t2.mlt_set(p1, p2, p4);
                terrainTriIndex.push_back(t1);
                terrainTriIndex.push_back(t2);
            }
        }
    }

    if (autoseabed) {
        // 海底面処理
        int cnum = (int)coords.size();  // データ（頂点）の数

        FacetTriIndex s1, s2;
        s1.mlt_set(cnum, cnum+2, cnum+3);
        s2.mlt_set(cnum, cnum+3, cnum+1);
        terrainTriIndex.push_back(s1);
        terrainTriIndex.push_back(s2);

        coords.push_back(Vector<float>(0.0f, 0.0f, seabed));
        coords.push_back(Vector<float>((float)xsize, 0.0f, seabed));
        coords.push_back(Vector<float>(0.0f, (float)ysize, seabed));
        coords.push_back(Vector<float>((float)xsize, (float)ysize, seabed));

        for (int i=0; i<4; i++) {
            normals.push_back(Vector<float>(0.0f, 0.0f, 0.0f));
            uvs.push_back(UVMap<float>(0.0f, 0.0f));
        }
    }

    ComputeTriNormals();
    SetupTriArray(shift);
}



void  TerrainMesh::ComputeTriNormals(void)
{
    int tnum = (int)terrainTriIndex.size();
    for (int i=0; i<tnum; i++) {
        FacetTriIndex indx = terrainTriIndex[i];
        Vector<float> normal = indx.SurfaceNormal(&coords);
        normals[indx.n1] = normals[indx.n1] + normal;
        normals[indx.n2] = normals[indx.n2] + normal;
        normals[indx.n3] = normals[indx.n3] + normal;
    }
    int nnum = (int)normals.size();
    for (int i=0; i<nnum; i++) normals[i] = normals[i].normalize();

    return;
}



void  TerrainMesh::SetupTriArray(Vector<float> shift)
{
    int size = (int)coords.size();
    for (int i=0; i<size; i++) coords[i] = coords[i] + shift;

    int tnum = (int)terrainTriIndex.size();
    for (int i=0; i<tnum; i++) {
        FacetTriIndex indx = terrainTriIndex[i];
        FacetTriData  tridat;
        tridat.facetNum = 0;
        tridat.v1  = coords [indx.v1];
        tridat.v2  = coords [indx.v2];
        tridat.v3  = coords [indx.v3];
        tridat.n1  = normals[indx.n1];
        tridat.n2  = normals[indx.n2];
        tridat.n3  = normals[indx.n3];
        tridat.uv1 = uvs    [indx.uv1];
        tridat.uv2 = uvs    [indx.uv2];
        tridat.uv3 = uvs    [indx.uv3];
        terrainTriArray.push_back(tridat);
    }

    return;
}

