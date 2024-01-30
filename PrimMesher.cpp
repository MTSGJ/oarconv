
#include "PrimMesher.h"

/**
@brief   This is rewrite code from PrimMesher.cs of OpenSim for JunkBox_Lib.

@file    PrimMesher.cpp
@author  Fumi.Iseki iseki@rsch.tuis.ac.jp
@date    2014 9/7

@sa http://opensimulator.org/
@sa OpenSim/Region/Physics/Meshing/PrimMesher.cs
*/


/*
 * see also http://opensimulator.org/
 * see also OpenSim/Region/Physics/Meshing/PrimMesher.cs
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

using namespace jbxl;


///////////////////////////////////////////////////////////////////////////////////////////////
// PrimAngleList
//

const  Vector<double> PrimAngleList::normals3[4] =
{
    Vector<double>(0.25,  0.4330127, 0.0).normalize(),
    Vector<double>(-0.5,  0.0,       0.0).normalize(),
    Vector<double>(0.25, -0.4330127, 0.0).normalize(),
    Vector<double>(0.25,  0.4330127, 0.0).normalize()
};

//
const  Vector<double> PrimAngleList::normals4[5] =
{
    Vector<double>( 0.5,  0.5, 0.0).normalize(),
    Vector<double>(-0.5,  0.5, 0.0).normalize(),
    Vector<double>(-0.5, -0.5, 0.0).normalize(),
    Vector<double>( 0.5, -0.5, 0.0).normalize(),
    Vector<double>( 0.5,  0.5, 0.0).normalize()
};

//
const  PrimAngle PrimAngleList::angles3[4] =
{
    PrimAngle(0.0f,        1.0f,  0.0f),
    PrimAngle(0.3333333f, -0.5f,  0.8660254f),
    PrimAngle(0.6666667f, -0.5f, -0.8660254f),
    PrimAngle(1.0f,        1.0f,  0.0f)
};

//
const  PrimAngle PrimAngleList::angles4[5] =
{
    PrimAngle(0.0f,   1.0f,  0.0f),
    PrimAngle(0.25f,  0.0f,  1.0f),
    PrimAngle(0.5f,  -1.0f,  0.0f),
    PrimAngle(0.75f,  0.0f, -1.0f),
    PrimAngle(1.0f,   1.0f,  0.0f)
};

//
const  PrimAngle PrimAngleList::angles24[25] =
{
    PrimAngle(0.0f,        1.0f,        0.0f),
    PrimAngle(0.04166667f, 0.9659258f,  0.2588190f),
    PrimAngle(0.08333333f, 0.8660254f,  0.5f),
    PrimAngle(0.125f,      0.7071068f,  0.7071068f),
    PrimAngle(0.1666667f,  0.5f,        0.8660254f),
    PrimAngle(0.2083333f,  0.2588190f,  0.9659258f),
    PrimAngle(0.25f,       0.0f,        1.0f),
    PrimAngle(0.2916667f, -0.2588190f,  0.9659258f),
    PrimAngle(0.3333333f, -0.5f,        0.8660254f),
    PrimAngle(0.375f,     -0.7071068f,  0.7071068f),
    PrimAngle(0.4166667f, -0.8660254f,  0.5f),
    PrimAngle(0.4583333f, -0.9659258f,  0.2588190f),
    PrimAngle(0.5f,       -1.0f,        0.0f),
    PrimAngle(0.5416667f, -0.9659258f, -0.2588190f),
    PrimAngle(0.5833333f, -0.8660254f, -0.5f),
    PrimAngle(0.6250000f, -0.7071068f, -0.7071068f),
    PrimAngle(0.6666667f, -0.5f,       -0.8660254f),
    PrimAngle(0.7083333f, -0.2588190f, -0.9659258f),
    PrimAngle(0.75f,       0.0f,       -1.0f),
    PrimAngle(0.7916667f,  0.2588190f, -0.9659258f),
    PrimAngle(0.8333333f,  0.5f,       -0.8660254f),
    PrimAngle(0.875f,      0.7071068f, -0.7071068f),
    PrimAngle(0.9166667f,  0.8660254f, -0.5f),
    PrimAngle(0.9583333f,  0.9659258f, -0.2588190f),
    PrimAngle(1.0f,        1.0f,        0.0f)
};


void  PrimAngleList::Intersection(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{ 
    double denom = (y4-y3)*(x2-x1) - (x4-x3)*(y2-y1);

    if (denom != 0.0) {
        double uaNumerator = (x4-x3)*(y1-y3) - (y4-y3)*(x1-x3);
        double ua = uaNumerator/denom;
        iX = x1 + ua*(x2 - x1);
        iY = y1 + ua*(y2 - y1);
    }
}


// 角度を最大25分割して angles に格納．
void  PrimAngleList::MakeAngles(int sides, double sttAngle, double endAngle)
{
    angles.clear();
    normals.clear();

    if (sides<1 || endAngle<=sttAngle) return;

    if (sides==3 || sides==4 || sides==24) {
        sttAngle /= PI2;    // 0.0 - 1.0
        endAngle /= PI2;

        const PrimAngle* srcAngles = NULL;
        if      (sides==3) srcAngles = angles3;
        else if (sides==4) srcAngles = angles4;
        else               srcAngles = angles24;

        int sttAngleIndex = (int)(sttAngle*sides);
        int endAngleIndex = sides;
        if (endAngle<1.0) endAngleIndex = (int)(endAngle*sides) + 1;
        if (endAngleIndex==sttAngleIndex) endAngleIndex++;

        for (int index=sttAngleIndex; index<endAngleIndex+1; index++) {
            angles.push_back(srcAngles[index]);
            if      (sides==3) normals.push_back(normals3[index]);
            else if (sides==4) normals.push_back(normals4[index]);
        }

        // 最初と最後を補間値で置き換える
        if (sttAngle>0.0) {
            angles[0] = InterpolatePoints(sttAngle, angles[0], angles[1]);
        }
        if (endAngle<1.0) {
            int lastIndex = (int)angles.size() - 1;
            angles[lastIndex] = InterpolatePoints(endAngle, angles[lastIndex-1], angles[lastIndex]);
        }
    }

    //
    else {
        double stepSize = PI2/sides;
        int   sttStep  = (int)(sttAngle/stepSize);
        double angle = stepSize*sttStep;
        double endAngleTest = endAngle;

        if (endAngle<PI2) {
            endAngleTest = stepSize*((int)(endAngle/stepSize) + 1);
            if (endAngleTest<endAngle)   endAngleTest += stepSize;
            if (endAngleTest>PI2) endAngleTest = PI2;
        }

        int step = sttStep;
        while (angle<=endAngleTest) {
            PrimAngle newAngle;
            newAngle.angle = angle;
            newAngle.X = cos(angle);
            newAngle.Y = sin(angle);
            angles.push_back(newAngle);
            step++;
            angle = stepSize*step;
        }

        if (sttAngle>angles[0].angle) {
            PrimAngle newAngle;
            Intersection(angles[0].X, angles[0].Y, angles[1].X, angles[1].Y, 0.0, 0.0, cos(sttAngle), sin(sttAngle));
            newAngle.angle = sttAngle;
            newAngle.X = iX;
            newAngle.Y = iY;
            angles[0] = newAngle;
        }

        int index = (int)angles.size() - 1;
        if (endAngle<angles[index].angle) {
            PrimAngle newAngle;
            Intersection(angles[index-1].X, angles[index-1].Y, angles[index].X, angles[index].Y, 0.0, 0.0, cos(endAngle), sin(endAngle));
            newAngle.angle = endAngle;
            newAngle.X = iX;
            newAngle.Y = iY;
            angles[index] = newAngle;
        }
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////
// PrimProfile
//

void  PrimProfile::init(void)
{
    coords.clear();
    normals.clear();
    primTriIndex.clear();
    triUVs.clear();
    triNums.clear();
    us.clear();
    //
    outerCoordIndex.clear();
    hollowCoordIndex.clear();
    cut1CoordIndex.clear();
    cut2CoordIndex.clear();

    triNormal = Vector<double>(0.0, 0.0, 1.0);

    numPrimFacets   = 0;
    numOuterVertex  = 0;
    numHollowVertex = 0;
    numOuterFacet   =-1;
    numHollowFacet  =-1;
    numBottomFacet  = 0;

    calcVertexNormals = false;
}


PrimProfile::PrimProfile(int sides, PrimMeshParam mparam, double hollow, int hollowSides, bool calcNormals)
{
    init();

    int    profCurve = mparam.profCurve;
    double profBegin = mparam.profBegin;
    double profEnd   = mparam.profEnd;

    calcVertexNormals = calcNormals;
    Vector<double> center(0.0, 0.0, 0.0);

    PRIM_VECTOR_ARRAY hollowCoords;
    PRIM_VECTOR_ARRAY hollowNormals;
    PRIM_DOUBLE_ARRAY hollowUs;

    double xScale = 0.5;
    double yScale = 0.5;
    if (sides==4) {
        xScale = SQROOT_05;
        yScale = SQROOT_05;
    }
    double sttAngle = profBegin*PI2;
    double endAngle = profEnd  *PI2;

    bool hasHollow = (hollow>0.0);
    bool hasProfileCut = (profBegin>0.0  || profEnd<0.9999);
    bool simpleFacet = (sides<5 && !hasHollow && !hasProfileCut);

    PrimAngleList angles;
    PrimAngleList hollowAngles;
    angles.MakeAngles(sides, sttAngle, endAngle);

    numOuterVertex = (int)angles.angles.size();

    if (hasHollow) {
        if (sides==hollowSides) {
            hollowAngles = angles;
        }
        else {
            hollowAngles.MakeAngles(hollowSides, sttAngle, endAngle);
        }
        numHollowVertex = (int)hollowAngles.angles.size();
    }
    else if (!simpleFacet) {
        coords.push_back(center);
        if (calcVertexNormals) normals.push_back(Vector<double>(0.0, 0.0, 1.0));
        us.push_back(0.0);
    }

    Vector<double> vert;
    PrimAngle angle;

    if (hasHollow && hollowSides!=sides) {
        int numHollowAngles = (int)hollowAngles.angles.size();
        for (int i=0; i<numHollowAngles; i++) {
            angle  = hollowAngles.angles[i];
            vert.x = hollow*xScale*angle.X;
            vert.y = hollow*yScale*angle.Y;
            vert.z = 0.0f;

            hollowCoords.push_back(vert);
            if (calcVertexNormals) {
                if (hollowSides<5)  hollowNormals.push_back(-hollowAngles.normals[i]);
                else                hollowNormals.push_back(Vector<double>(-angle.X, -angle.Y, 0.0));
                if (hollowSides==4) hollowUs.push_back(angle.angle*hollow*SQROOT_05);
                else                hollowUs.push_back(angle.angle*hollow);
            }
        }
    }

    int index = 0;
    int numAngles = (int)angles.angles.size();

    for (int i=0; i<numAngles; i++) {
        angle  = angles.angles[i];
        vert.x = angle.X*xScale;
        vert.y = angle.Y*yScale;
        vert.z = 0.0f;
        coords.push_back(vert);
        //
        if (calcVertexNormals) {
            outerCoordIndex.push_back((int)coords.size()-1);
            if (sides<5) normals.push_back(angles.normals[i]);
            else         normals.push_back(Vector<double>(angle.X, angle.Y, 0.0));
            us.push_back(angle.angle);
        }

        if (hasHollow) {
            if (sides==hollowSides) {   
                vert.x *= hollow;
                vert.y *= hollow;
                vert.z = 0.0f;
                hollowCoords.push_back(vert);
                //
                if (calcVertexNormals) {
                    if (sides<5) hollowNormals.push_back(-angles.normals[i]);
                    else         hollowNormals.push_back(Vector<double>(-angle.X, -angle.Y, 0.0));
                    hollowUs.push_back(angle.angle*hollow);
                }
            }
        }
        else if (!simpleFacet && angle.angle>0.0001) {
            ContourTriIndex trindx;
            trindx.v1 = 0;
            trindx.v2 = index;
            trindx.v3 = index + 1;
            primTriIndex.push_back(trindx);
        }
        index++;
    }

    if (hasHollow) {
        std::reverse(hollowCoords.begin(), hollowCoords.end());
        if (calcVertexNormals) {
            std::reverse(hollowNormals.begin(), hollowNormals.end());
            std::reverse(hollowUs.begin(), hollowUs.end());
        }

        int numTotalVertex = numOuterVertex + numHollowVertex;
        //
        if (numOuterVertex==numHollowVertex) {
            ContourTriIndex trindx;
            for (int outvx=0; outvx<numOuterVertex-1; outvx++) {
                trindx.v1 = outvx;
                trindx.v2 = outvx + 1;
                trindx.v3 = numTotalVertex - outvx - 1;
                primTriIndex.push_back(trindx);

                trindx.v1 = outvx + 1;
                trindx.v2 = numTotalVertex - outvx - 2;
                trindx.v3 = numTotalVertex - outvx - 1;
                primTriIndex.push_back(trindx);
            }
        }
        else {
            if (numOuterVertex<numHollowVertex) {
                ContourTriIndex trindx;
                int j = 0;
                int maxJ = numOuterVertex - 1;
                for (int i=0; i<numHollowVertex; i++) {
                    if (j<maxJ) {
                        if (angles.angles[j+1].angle-hollowAngles.angles[i].angle < hollowAngles.angles[i].angle-angles.angles[j].angle+0.000001) {
                            trindx.v1 = numTotalVertex - i - 1;
                            trindx.v2 = j;
                            trindx.v3 = j + 1;
                            primTriIndex.push_back(trindx);
                            j++;
                        }
                    }
                    trindx.v1 = j;
                    trindx.v2 = numTotalVertex - i - 2;
                    trindx.v3 = numTotalVertex - i - 1;
                    primTriIndex.push_back(trindx);
                }
            }
            else { 
                ContourTriIndex trindx;
                int j = 0;
                int maxJ = numHollowVertex - 1;
                for (int i=0; i<numOuterVertex; i++) {
                    if (j<maxJ) {
                        if (hollowAngles.angles[j+1].angle-angles.angles[i].angle < angles.angles[i].angle-hollowAngles.angles[j].angle+0.000001) {
                            trindx.v1 = i;
                            trindx.v2 = numTotalVertex - j - 2;
                            trindx.v3 = numTotalVertex - j - 1;
                            primTriIndex.push_back(trindx);
                            j++;
                        }
                    }
                    trindx.v1 = numTotalVertex - j - 1;
                    trindx.v2 = i;
                    trindx.v3 = i + 1;
                    primTriIndex.push_back(trindx);
                }
            }
        }

        if (calcVertexNormals) {
            int hnum = (int)hollowCoords.size();
            for (int i=0; i<hnum; i++) {
                coords.push_back(hollowCoords[i]);
                hollowCoordIndex.push_back((int)coords.size()-1);
            }
        }
        else {
            coords.insert(coords.end(), hollowCoords.begin(), hollowCoords.end());
        }

        if (calcVertexNormals) {
            normals.insert(normals.end(), hollowNormals.begin(), hollowNormals.end());
            us.insert(us.end(), hollowUs.begin(), hollowUs.end());
        }
    }

    if (simpleFacet) {
        if (sides==3) primTriIndex.push_back(ContourTriIndex(0, 1, 2));
        else if (sides==4) {
            primTriIndex.push_back(ContourTriIndex(0, 1, 2));
            primTriIndex.push_back(ContourTriIndex(0, 2, 3));
        }
    }

    if (calcVertexNormals && hasProfileCut) {
        int lastIndex = numOuterVertex - 1;
        if (hasHollow) {
            cut1CoordIndex.push_back(0);
            cut1CoordIndex.push_back((int)coords.size()-1);
            cut2CoordIndex.push_back(lastIndex+1);
            cut2CoordIndex.push_back(lastIndex);
            cutNormal1.x =   coords[0].y - coords[(int)coords.size()-1].y;
            cutNormal1.y = -(coords[0].x - coords[(int)coords.size()-1].x);
            cutNormal2.x =   coords[lastIndex+1].y - coords[lastIndex].y;
            cutNormal2.y = -(coords[lastIndex+1].x - coords[lastIndex].x);
        }
        else {
            cut1CoordIndex.push_back(0);
            cut1CoordIndex.push_back(1);
            cut2CoordIndex.push_back(lastIndex);
            cut2CoordIndex.push_back(0);
            cutNormal1.x =  normals[1].y;
            cutNormal1.y = -normals[1].x;
            cutNormal2.x = -normals[(int)normals.size()-2].y;
            cutNormal2.y =  normals[(int)normals.size()-2].x;
        }
        cutNormal1.normalize();
        cutNormal2.normalize();
    }

    MakeTriUVs();

    hollowCoords.clear();
    hollowNormals.clear();
    hollowUs.clear();

    //
    // 面の番号を数える（重要!!）
    if (calcVertexNormals) {
        int fnum = 1;

        int sttVert = hasProfileCut && !hasHollow ? 1 : 0;
        if (sttVert>0) triNums.push_back(-1);
        for (int i=0; i<numOuterVertex-1; i++) triNums.push_back(sides<5 && i<=sides ? fnum++ : fnum);

        //triNums.push_back(hasProfileCut ? -1 : fnum++); 
        triNums.push_back(hasProfileCut ? -1 : -2);                                     // by Fumi.Iseki

        if (sides>4 && (hasHollow || hasProfileCut)) fnum++;
        //if (sides<5 && (hasHollow || hasProfileCut) && numOuterVertex<sides) fnum++;  // deleted by Fumi.Iseki 条件を調べる BOXでは不要
        if (profCurve==PRIM_PROF_CIRCLE && fnum<2) fnum++;                              // by Fumi.Iseki

        if (hasHollow) {
            //for (int i=0; i<numHollowVertex; i++) triNums.push_back(fnum);
            for (int i=0; i<numHollowVertex-1; i++) triNums.push_back(fnum);            // by Fumi.Iseki
            numHollowFacet = fnum++;
        }
        //
        numOuterFacet  = fnum - 1;                                                      // by Fumi.Iseki
        numBottomFacet = fnum++;

        if (hasHollow && hasProfileCut) triNums.push_back(fnum++);

        for (int i=0; i<(int)triNums.size(); i++) {
            if (triNums[i]==-1) triNums[i] = fnum++;
        }

        numPrimFacets = fnum;
    }
}


void  PrimProfile::MakeTriUVs(void)
{
    triUVs.clear();
    //
    int cnum = (int)coords.size();
    for (int i=0; i<cnum; i++) {
        triUVs.push_back(UVMap<double>(0.5 + coords[i].x, 0.5 - coords[i].y));
    }
}


PrimProfile  PrimProfile::Copy(bool needIndex)
{
    PrimProfile copy;

    copy.coords.insert(copy.coords.begin(), coords.begin(), coords.end());
    copy.triUVs.insert(copy.triUVs.begin(), triUVs.begin(), triUVs.end());
    if (needIndex) copy.primTriIndex.insert(copy.primTriIndex.begin(), primTriIndex.begin(), primTriIndex.end());

    copy.calcVertexNormals = calcVertexNormals;
    if (copy.calcVertexNormals) {
        copy.normals.insert(copy.normals.begin(), normals.begin(), normals.end());
        copy.triNums.insert(copy.triNums.begin(), triNums.begin(), triNums.end());
        copy.us.insert(copy.us.begin(), us.begin(), us.end());
        //
        copy.triNormal  = triNormal;
        copy.cutNormal1 = cutNormal1;
        copy.cutNormal2 = cutNormal2;
        //
        copy.outerCoordIndex  = outerCoordIndex;
        copy.hollowCoordIndex = hollowCoordIndex;
        copy.cut1CoordIndex   = cut1CoordIndex;
        copy.cut2CoordIndex   = cut2CoordIndex;
    }

    copy.numOuterVertex  = numOuterVertex;
    copy.numHollowVertex = numHollowVertex;

    copy.numPrimFacets   = numPrimFacets;
    copy.numOuterFacet   = numOuterFacet;
    copy.numHollowFacet  = numHollowFacet;
    copy.numBottomFacet  = numBottomFacet;

    return copy;
}


void  PrimProfile::execShift(double x, double y, double z)
{
    Vector<double> vert;
    //
    int cnum = (int)coords.size(); 
    for (int i=0; i<cnum; i++) {
        vert = coords[i];
        vert.x += x;
        vert.y += y;
        vert.z += z;
        coords[i] = vert;
    }
}


void  PrimProfile::execRotate(Quaternion<double> q)
{
    int cnum = (int)coords.size();
    for (int i=0; i<cnum; i++) coords[i] = VectorRotation(coords[i], q);

    if (calcVertexNormals) {
        int nnum = (int)normals.size();
        for (int i=0; i<nnum; i++) normals[i] = VectorRotation(normals[i], q);
        triNormal  = VectorRotation(triNormal,  q);
        cutNormal1 = VectorRotation(cutNormal1, q);
        cutNormal2 = VectorRotation(cutNormal2, q);
    }
}


void  PrimProfile::execScale(double x, double y)
{
    Vector<double> vert;
    //
    int cnum = (int)coords.size();
    for (int i=0; i<cnum; i++) {
        vert = coords[i];
        vert.x *= x;
        vert.y *= y;
        coords[i] = vert;
    }
}


void  PrimProfile::FlipNormals(void)
{
    int swptmp;
    ContourTriIndex trindx;

    int inum = (int)primTriIndex.size();
    for (int i=0; i<inum; i++) {
        trindx = primTriIndex[i];
        swptmp = trindx.v3;
        trindx.v3 = trindx.v1;
        trindx.v1 = swptmp;
        primTriIndex[i] = trindx;
    }

    if (calcVertexNormals) {
        int nsize = (int)normals.size();
        if (nsize>0) {
            normals[nsize-1].z = - normals[nsize-1].z;
        }
    }

    triNormal = -triNormal;

    int tnum = (int)triUVs.size();
    for (int i=0; i<tnum; i++) {
        triUVs[i].v = 1.0f - triUVs[i].v;
    }
}


void  PrimProfile::FlipUVs(void)
{
    int tnum = (int)triUVs.size();
    for (int i=0; i<tnum; i++) {
        triUVs[i].u = 1.0f - triUVs[i].u;
    }
}


void  PrimProfile::Add2ContourTriIndexVertex(int num)
{
    ContourTriIndex tri;

    int inum = (int)primTriIndex.size();
    for (int i=0; i<inum; i++) {
        tri = primTriIndex[i];
        tri.v1 += num;
        tri.v2 += num;
        tri.v3 += num;
        primTriIndex[i] = tri;
    }
}


void  PrimProfile::Add2ContourTriIndexNormal(int num)
{
    if (calcVertexNormals) {
        ContourTriIndex tri;
        int inum = (int)primTriIndex.size();
        for (int i=0; i<inum; i++) {
            tri = primTriIndex[i];
            tri.n1 += num;
            tri.n2 += num;
            tri.n3 += num;
            primTriIndex[i] = tri;
        }
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////
// PrimMeshParam
//

void  PrimMeshParam::init(bool base_init)
{
    if (base_init) PrimShapeParam::init();

    pathNodes.clear();
    stepRevol = 24;
}


//
void  PrimMeshParam::dup(PrimMeshParam p)
{
    *this = p;

    for (int i=0; i<PRIM_MATERIAL_NUM; i++) {
        materialParam[i].dup(p.materialParam[i]);
    }
}


//
void  PrimMeshParam::CreatePathNodes(int pathType, int steps)
{
    if      (pathTaperX> 0.999f) pathTaperX =  0.999f;
    else if (pathTaperX<-0.999f) pathTaperX = -0.999f;
    if      (pathTaperY> 0.999f) pathTaperY =  0.999f;
    else if (pathTaperY<-0.999f) pathTaperY = -0.999f;

    if (pathType==PRIM_PATH_LINE || pathType==PRIM_PATH_FLEXIBLE) {
        int    step = 0;
        double length = pathEnd - pathBegin;
        double twistTotal = pathTwistEnd - pathTwistBegin;
        double twistTotalAbs = fabs(twistTotal);
        if (twistTotalAbs>0.01f) steps += (int)(twistTotalAbs*3.66f);   // dahlia's magic number ?

        double start = -0.5;
        double stepSize = length/steps;

        double percentMultiplier = stepSize*0.999999;
        double xOffset = pathShearX*pathBegin;
        double yOffset = pathShearY*pathBegin;
        double zOffset = start;
        double xOffsetStepIncrement = pathShearX*length/steps;
        double yOffsetStepIncrement = pathShearY*length/steps;
        double percentOfPath = pathBegin;
        zOffset += percentOfPath;

        bool done = false;
        while (!done) {
            PrimPathNode node;
            node.xScale = 1.0;
            node.yScale = 1.0;
            if      (pathTaperX==0.0f) node.xScale = 1.0f;
            else if (pathTaperX >0.0f) node.xScale = 1.0f - percentOfPath*pathTaperX;
            else                       node.xScale = 1.0f + (1.0f - percentOfPath)*pathTaperX;
            if      (pathTaperY==0.0f) node.yScale = 1.0f;
            else if (pathTaperY >0.0f) node.yScale = 1.0f - percentOfPath*pathTaperY;
            else                       node.yScale = 1.0f + (1.0f - percentOfPath)*pathTaperY;

            double twst = pathTwistBegin + twistTotal*percentOfPath;
            node.rotation = Quaternion<double>(twst, Vector<double>(0.0, 0.0, 1.0));
            node.position = Vector<double>(xOffset, yOffset, zOffset);
            node.percent  = percentOfPath;
            pathNodes.push_back(node);

            if (step<steps) {
                step++;
                percentOfPath += percentMultiplier;
                xOffset += xOffsetStepIncrement;
                yOffset += yOffsetStepIncrement;
                zOffset += stepSize;
                if (percentOfPath>pathEnd) done = true;
            }
            else {
                done = true;
            }
        }
    }

    else {  // pathType==PRIM_PATH_CIRCLE
        double twistTotal = pathTwistEnd - pathTwistBegin;
        double twistTotalAbs = fabs(twistTotal);
        //
        if (twistTotalAbs>0.01) {
            if (twistTotalAbs>1.5*PI) steps *= 2;
            if (twistTotalAbs>3.0*PI) steps *= 2;
        }

        double yPathScale = pathScaleY*0.5;
        double totalSkew  = pathSkew*2.0*(pathEnd - pathBegin);
        double skewStart  = pathBegin*2.0*pathSkew - pathSkew;
        double xOffsetTopShearXFactor = pathShearX*(0.25 + 0.5*(0.5 - pathScaleY));
        double yShearCompensation = 1.0 + fabs(pathShearY)*0.25;

        double sttAngle = PI2*pathBegin*pathRevol - pathShearY*0.9;
        double endAngle = PI2*pathEnd  *pathRevol - pathShearY*0.9;
        double stepSize = PI2/stepRevol;

        int step = (int)(sttAngle/stepSize);
        double angle = sttAngle;

        bool done = false;
        while (!done) {
            PrimPathNode node;

            double xProfileScale = (1.0 - (double)fabs(pathSkew))*pathScaleX;
            double yProfileScale = pathScaleY;
            double percentOfPath = angle/(PI2*pathRevol);
            double percentOfAngles = (angle - sttAngle)/(endAngle - sttAngle);

            if      (pathTaperX> 0.01) xProfileScale *= 1.0 - percentOfPath*pathTaperX;
            else if (pathTaperX<-0.01) xProfileScale *= 1.0 + (1.0 - percentOfPath)*pathTaperX;
            if      (pathTaperY> 0.01) yProfileScale *= 1.0 - percentOfPath*pathTaperY;
            else if (pathTaperY<-0.01) yProfileScale *= 1.0 + (1.0 - percentOfPath)*pathTaperY;

            node.xScale = xProfileScale;
            node.yScale = yProfileScale;

            double radiusScale = 1.0;
            if      (pathRadius>0.001) radiusScale = 1.0 - percentOfPath*pathRadius;
            else if (pathRadius<0.001) radiusScale = 1.0 + (1.0 - percentOfPath)*pathRadius;

            double twst = pathTwistBegin + twistTotal*percentOfPath;
            double xOffset = 0.5*(skewStart + totalSkew*percentOfAngles) + sin(angle)*xOffsetTopShearXFactor;
            double yOffset = yShearCompensation*cos(angle)*(0.5 - yPathScale)*radiusScale;
            double zOffset = sin(angle + pathShearY)*(0.5 - yPathScale)*radiusScale;

            node.rotation = Quaternion<double>(angle+pathShearY, Vector<double>(1.0, 0.0, 0.0));
            node.position = Vector<double>(xOffset, yOffset, zOffset);

            if (twistTotal!=0.0f || pathTwistBegin!=0.0f) {
                node.rotation = node.rotation*Quaternion<double>(twst, Vector<double>(0.0, 0.0, 1.0f));
            }
            node.percent = percentOfPath;

            pathNodes.push_back(node);

            if (angle>=endAngle-0.01f) {
                done = true;
            }
            else {
                step++;
                angle = stepSize*step;
                if (angle>endAngle) angle = endAngle;
            }
        }
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////
// PrimMesh
//

void  PrimMesh::init(void)
{
    coords.clear();
    normals.clear();
    primTriIndex.clear();
    primTriArray.clear();
    uvs.clear();

    shapeType = 0;
    sides = 4;
    hollowSides = 4;

    meshParam.init();

    numPrimFacets = 0;

    calcVertexNormals = true;
    normalsProcessed  = false;

    numProfileOuterFacet  = -1;
    numProfileHollowFacet = -1;

    hasProfileCut     = false;
    hasHollow         = false;
}


PrimMesh::PrimMesh(int sds, int hsds, PrimMeshParam mesh)
{
    init();

    sides = sds;
    hollowSides = hsds;
    meshParam.dup(mesh);

    if (sides<3) sides = 3;
    if (hollowSides<3) hollowSides = 3;
    if (meshParam.profBegin<0.0f) meshParam.profBegin = 0.0f;
    if (meshParam.profEnd>1.0f)   meshParam.profEnd = 1.0f;
    if (meshParam.profEnd<0.02f)  meshParam.profEnd = 0.02f;
    if (meshParam.profBegin>=meshParam.profEnd) meshParam.profBegin = meshParam.profEnd - 0.02f;
    
    if (meshParam.profHollow>0.99f) meshParam.profHollow = 0.99f;
    if (meshParam.profHollow<0.0f ) meshParam.profHollow = 0.0f;
}


Vector<double>  PrimMesh::GetTriNormal(int index)
{
    Vector<double> normal;
    //
    if (index<0 || index>=(int)primTriIndex.size()) return normal;

    normal = GetTriNormal(primTriIndex[index]);
    return normal;
}


void  PrimMesh::Extrude(int pathType)
{
    bool needEndFacets = false;
    normalsProcessed   = false;

    coords.clear();
    primTriIndex.clear();
    primTriArray.clear();
    calcVertexNormals = true;
    if (calcVertexNormals) normals.clear();

    int steps = 1;

    if (sides==3) {
        if (fabs(meshParam.pathTaperX)>0.01f || fabs(meshParam.pathTaperY)>0.01f) {
            steps = (int)(steps*4.5f*(meshParam.pathEnd - meshParam.pathBegin));
        }
    } 

    if (shapeType==PRIM_TYPE_SPHERE) hasProfileCut = (meshParam.profEnd-meshParam.profBegin < 0.4999f);
    else                             hasProfileCut = (meshParam.profEnd-meshParam.profBegin < 0.9999f);
    hasHollow = (meshParam.profHollow>0.001f);

    double hollow     = meshParam.profHollow;
    double twistBegin = meshParam.pathTwistBegin;
    double twistEnd   = meshParam.pathTwistEnd;
    double twistTotal = twistEnd - twistBegin;
    double twistTotalAbs = fabs(twistTotal);
    if (twistTotalAbs>0.01) steps += (int)(twistTotalAbs*3.66);  // dahlia's magic number

    if (pathType==PRIM_PATH_CIRCLE) {
        needEndFacets = false;
        if      (meshParam.pathBegin!=0.0f || meshParam.pathEnd!=1.0f)    needEndFacets = true;
        else if (meshParam.pathTaperX!=0.0f|| meshParam.pathTaperY!=0.0f) needEndFacets = true;
        else if (meshParam.pathSkew!=0.0f)   needEndFacets = true;
        else if (meshParam.pathRadius!=0.0f) needEndFacets = true;
        else if (twistTotal!=0.0)            needEndFacets = true;
    }
    else needEndFacets = true;  // PRIM_PATH_LINE

    double initialProfileRot = 0.0;
    if (pathType==PRIM_PATH_CIRCLE) {
        if (sides==3) {
            initialProfileRot = PI;
            if (hollowSides==4) {
                if (hollow>SQROOT_05) hollow = SQROOT_05;
                hollow  *= SQROOT_05;
            }
            else hollow *= 0.5f;
        }
        else if (sides==4) {
            initialProfileRot = 0.25*PI;
            if (hollowSides!=4) hollow *= SQROOT_05;
        }
        else if (sides>4) {
            initialProfileRot = PI;
            if (hollowSides==4) {
                if (hollow>SQROOT_05) hollow = SQROOT_05;
                hollow  /= SQROOT_05;
            }
        }
    }
    else {      // PRIM_PATH_LINE
        if (sides==3) {
            if (hollowSides==4) {
                if (hollow>SQROOT_05) hollow = SQROOT_05;
                hollow  *= SQROOT_05;
            }
            else hollow *= 0.5f;
        }
        else if (sides==4) {
            initialProfileRot = 1.25*PI;
            if (hollowSides!=4) hollow *= SQROOT_05;
        }
        else if (sides==24 && hollowSides==4) hollow *= SQROOT_2;
    }

    PrimProfile profile(sides, meshParam, hollow, hollowSides, calcVertexNormals);
    numPrimFacets = profile.numPrimFacets;

    int numCut1Facet = profile.numBottomFacet + 1;
    int numCut2Facet = numCut1Facet + 1;
    if (!needEndFacets) {
        numCut1Facet -= 2;
        numCut2Facet -= 2;
    }

    numProfileOuterFacet = profile.numOuterFacet;
    if (!needEndFacets) numProfileOuterFacet--;

    if (hasHollow) {
        numProfileHollowFacet = profile.numHollowFacet;
        if (!needEndFacets) numProfileHollowFacet--;
    }

    int cut1Vert = -1;
    int cut2Vert = -1;
    if (hasProfileCut) {
        cut1Vert = hasHollow ? (int)profile.coords.size() -1 : 0;
        cut2Vert = hasHollow ? profile.numOuterVertex-1 : profile.numOuterVertex;
    }

    if (initialProfileRot!=0.0) {
        profile.execRotate(Quaternion<double>(initialProfileRot, Vector<double>(0.0, 0.0, 1.0)));
        profile.MakeTriUVs();
    }

    Vector<double> lastCutNormal1;
    Vector<double> lastCutNormal2;
    double thisV = 0.0;
    double lastV = 0.0;

    meshParam.CreatePathNodes(pathType, steps);

    int pathnum = (int)meshParam.pathNodes.size();
    for (int index=0; index<pathnum; index++) {
        //
        PrimPathNode node = meshParam.pathNodes[index];
        PrimProfile layer = profile.Copy();
        layer.execScale(node.xScale, node.yScale);
        layer.execRotate(node.rotation);
        layer.execShift(node.position);

        //
        // 底面
        if (needEndFacets && index==0) {
            layer.FlipNormals();
            //
            Vector<double> triNormal = layer.triNormal;
            ContourTriData primTri(profile.numBottomFacet);
            PRIM_TRIINDX_ARRAY primTriIndex = layer.primTriIndex;
            int numFacets = (int)layer.primTriIndex.size();

            for (int i=0; i<numFacets; i++) {
                ContourTriIndex trindx = primTriIndex[i];

                primTri.v1  = layer.coords[trindx.v1];
                primTri.v2  = layer.coords[trindx.v2];
                primTri.v3  = layer.coords[trindx.v3];
                primTri.n1  = triNormal;
                primTri.n2  = triNormal;
                primTri.n3  = triNormal;
                primTri.uv1 = layer.triUVs[trindx.v1];
                primTri.uv2 = layer.triUVs[trindx.v2];
                primTri.uv3 = layer.triUVs[trindx.v3];
                //
                if (pathType==PRIM_PATH_LINE) {
                    primTri.uv1.flipV();
                    primTri.uv2.flipV();
                    primTri.uv3.flipV();
                }
                else {
                    primTri.uv1.flipU();
                    primTri.uv2.flipU();
                    primTri.uv3.flipU();
                }

                primTriArray.push_back(primTri);
            }
        }

        //
        // サイド面
        int coordsLen = (int)coords.size();
        layer.Add2ContourTriIndexVertex(coordsLen);

        coords.insert(coords.end(), layer.coords.begin(), layer.coords.end());

        if (calcVertexNormals) {
            layer.Add2ContourTriIndexNormal((int)normals.size());
            normals.insert(normals.end(), layer.normals.begin(), layer.normals.end());
        }
        if (node.percent<meshParam.pathBegin+0.01 || node.percent>meshParam.pathEnd-0.01) {
            primTriIndex.insert(primTriIndex.end(), layer.primTriIndex.begin(), layer.primTriIndex.end());
        }

        int numVertex = (int)layer.coords.size();
        ContourTriIndex trindx1;
        ContourTriIndex trindx2;

        thisV = node.percent;

        if (index>0) {
            int sttVert = coordsLen + 1;
            int endVert = (int)coords.size();
            if (sides<5 || hasProfileCut || hasHollow) sttVert--;

            for (int i=sttVert; i<endVert; i++) {
                int iNext = i + 1;
                if (i==endVert-1) iNext = sttVert;
                int whichVert = i - sttVert;
                int triSize = (int)profile.triNums.size();

                if (whichVert<triSize && profile.triNums[whichVert]>=0) {               // by Fumi.Iseki
                    //
                    trindx1.v1 = i;
                    trindx1.v2 = i - numVertex;
                    trindx1.v3 = iNext;
                    trindx2.v1 = iNext;
                    trindx2.v2 = i - numVertex;
                    trindx2.v3 = iNext - numVertex;

                    trindx1.n1 = trindx1.v1;
                    trindx1.n2 = trindx1.v2;
                    trindx1.n3 = trindx1.v3;
                    trindx2.n1 = trindx2.v1;
                    trindx2.n2 = trindx2.v2;
                    trindx2.n3 = trindx2.v3;

                    primTriIndex.push_back(trindx1);
                    primTriIndex.push_back(trindx2);

                    int primTriNum = profile.triNums[whichVert];    // 面番号
                    if (!needEndFacets) primTriNum--;

                    ContourTriData primTri1(primTriNum);
                    ContourTriData primTri2(primTriNum);

                    primTri1.v1 = coords[trindx1.v1];
                    primTri1.v2 = coords[trindx1.v2];
                    primTri1.v3 = coords[trindx1.v3];
                    primTri2.v1 = coords[trindx2.v1];
                    primTri2.v2 = coords[trindx2.v2];
                    primTri2.v3 = coords[trindx2.v3];

                    //
                    // UVマップ 
                    int uIndex = whichVert;
                    if (sides>4 && !hasHollow && uIndex<(int)layer.us.size()-1) uIndex++;
                    if (shapeType==PRIM_TYPE_SPHERE || shapeType==PRIM_TYPE_CYLINDER) { // by Fumi.Iseki for deviation of Texture
                        if (!hasHollow && hasProfileCut && uIndex>0) uIndex--;
                    }

                    double u1 = layer.us[uIndex];
                    double u2 = 1.0;

                    if (uIndex<(int)layer.us.size()-1) {
                        u2 = layer.us[uIndex+1];
                    }

                    if (whichVert==cut1Vert || whichVert==cut2Vert) {
                        u1 = 0.0f;
                        u2 = 1.0f;
                    }
                    else if (sides<5) {
                        if (whichVert<profile.numOuterVertex) {
                            u1 *= sides;
                            u2 *= sides;
                            u2 -= (int)u1;
                            u1 -= (int)u1;
                            if (u2<0.1f) u2 = 1.0f;
                        }
                    }

                    if (shapeType==PRIM_TYPE_SPHERE) {
                        if (whichVert!=cut1Vert && whichVert!=cut2Vert) {
                            u1 = u1*2.0f - 1.0f;
                            u2 = u2*2.0f - 1.0f;
                            if (whichVert>=layer.numOuterVertex) {
                                u1 -= hollow;
                                u2 -= hollow;
                            }
                        }
                    }

                    primTri1.uv1.u = u1;
                    primTri1.uv2.u = u1;
                    primTri1.uv3.u = u2;
                    primTri1.uv1.v = thisV;
                    primTri1.uv2.v = lastV;
                    primTri1.uv3.v = thisV;

                    primTri2.uv1.u = u2;
                    primTri2.uv2.u = u1;
                    primTri2.uv3.u = u2;
                    primTri2.uv1.v = thisV;
                    primTri2.uv2.v = lastV;
                    primTri2.uv3.v = lastV;

                    //
                    // 法線ベクトル
                    if (whichVert==cut1Vert) {
                        primTri1.contourNum = numCut1Facet;
                        primTri2.contourNum = numCut1Facet;
                        primTri1.n1 = layer.cutNormal1;
                        primTri1.n2 = primTri1.n3 = lastCutNormal1;
                        primTri2.n1 = primTri2.n3 = layer.cutNormal1;
                        primTri2.n2 = lastCutNormal1;
                    }
                    else if (whichVert==cut2Vert) {
                        primTri1.contourNum = numCut2Facet;
                        primTri2.contourNum = numCut2Facet;
                        primTri1.n1 = layer.cutNormal2;
                        primTri1.n2 = lastCutNormal2;
                        primTri1.n3 = lastCutNormal2;
                        primTri2.n1 = layer.cutNormal2;
                        primTri2.n2 = lastCutNormal2;
                        primTri2.n3 = layer.cutNormal2;
                    }
                    else {
                        if ((sides<5 && whichVert<layer.numOuterVertex) || (hollowSides<5 && whichVert>=layer.numOuterVertex)) {
                            primTri1.ComputeTriNormal();
                            primTri2.ComputeTriNormal();
                        }
                        else {
                            primTri1.n1 = normals[trindx1.n1];
                            primTri1.n2 = normals[trindx1.n2];
                            primTri1.n3 = normals[trindx1.n3];
                            primTri2.n1 = normals[trindx2.n1];
                            primTri2.n2 = normals[trindx2.n2];
                            primTri2.n3 = normals[trindx2.n3];
                        }
                    }

                    primTriArray.push_back(primTri1);
                    primTriArray.push_back(primTri2);
                }
            }
        }

        lastCutNormal1 = layer.cutNormal1;
        lastCutNormal2 = layer.cutNormal2;
        lastV = thisV;

        //
        // 上面
        if (needEndFacets && index==pathnum-1) {
            Vector<double> triNormal = layer.triNormal;
            ContourTriData primTri(0);
            PRIM_TRIINDX_ARRAY primTriIndex = layer.primTriIndex;
            int numFacets = (int)layer.primTriIndex.size();
            layer.FlipUVs();            // by Fumi.Iseki

            for (int i=0; i<numFacets; i++) {
                ContourTriIndex trindx = primTriIndex[i];
                primTri.v1 = layer.coords[trindx.v1-coordsLen];
                primTri.v2 = layer.coords[trindx.v2-coordsLen];
                primTri.v3 = layer.coords[trindx.v3-coordsLen];

                primTri.n1  = triNormal;
                primTri.n2  = triNormal;
                primTri.n3  = triNormal;
                primTri.uv1 = layer.triUVs[trindx.v1-coordsLen];
                primTri.uv2 = layer.triUVs[trindx.v2-coordsLen];
                primTri.uv3 = layer.triUVs[trindx.v3-coordsLen];

                if (pathType==PRIM_PATH_LINE) {
                    primTri.uv1.flip();
                    primTri.uv2.flip();
                    primTri.uv3.flip();
                }

                primTriArray.push_back(primTri);
            }
        }

    }
}


PrimMesh  PrimMesh::Copy(void)
{
    PrimMesh copy(sides, hollowSides, meshParam);

    copy.calcVertexNormals = calcVertexNormals;
    copy.normalsProcessed = normalsProcessed;
    copy.shapeType = shapeType;
    copy.numPrimFacets = numPrimFacets;

    copy.coords  = PRIM_VECTOR_ARRAY(coords);
    copy.normals = PRIM_VECTOR_ARRAY(normals);
    copy.uvs     = PRIM_UVMAP_ARRAY(uvs);
    copy.primTriIndex = PRIM_TRIINDX_ARRAY(primTriIndex);
    copy.primTriArray = PRIM_TRIDATA_ARRAY(primTriArray);

    copy.numProfileOuterFacet  = numProfileOuterFacet;
    copy.numProfileHollowFacet = numProfileHollowFacet;
    copy.hasHollow = hasHollow;
    copy.hasProfileCut = hasProfileCut;

    return copy;
}


void  PrimMesh::ComputeTriNormals(void)
{
    if (normalsProcessed) return;
    normalsProcessed = true;

    if (!calcVertexNormals) normals.clear();

    int inum = (int)primTriIndex.size();
    for (int i=0; i<inum; i++) {
        ContourTriIndex trindx = primTriIndex[i];
        normals.push_back(GetTriNormal(i).normalize());

        int normIndx = (int)normals.size() - 1;
        trindx.n1 = normIndx;
        trindx.n2 = normIndx;
        trindx.n3 = normIndx;
        primTriIndex[i] = trindx;
    }
}


void  PrimMesh::execShift(double x, double y, double z)
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

    int tnum = (int)primTriArray.size();
    for (int i=0; i<tnum; i++) {
        ContourTriData tri = primTriArray[i];
        tri.execShift(x, y, z);
        primTriArray[i] = tri;
    }
}


void  PrimMesh::execRotate(Quaternion<double> q)
{
    int cnum = (int)coords.size();
    for (int i=0; i<cnum; i++) coords[i]  = VectorRotation(coords[i], q);

    int nnum = (int)normals.size();
    for (int i=0; i<nnum; i++) normals[i] = VectorRotation(normals[i], q);

    int tnum = (int)primTriArray.size();
    for (int i=0; i<tnum; i++) {
        ContourTriData tri = primTriArray[i];
        tri.v1 = VectorRotation(tri.v1, q);
        tri.v2 = VectorRotation(tri.v2, q);
        tri.v3 = VectorRotation(tri.v3, q);
        tri.n1 = VectorRotation(tri.n1, q);
        tri.n2 = VectorRotation(tri.n2, q);
        tri.n3 = VectorRotation(tri.n3, q);
        primTriArray[i] = tri;
    }
}


void  PrimMesh::execScale(double x, double y, double z)
{
    Vector<double> m(x, y, z);

    int cnum = (int)coords.size();
    for (int i=0; i<cnum; i++) {
        coords[i].x *= m.x;
        coords[i].y *= m.y;
        coords[i].z *= m.z;
    }

    int tnum = (int)primTriArray.size();
    for (int i=0; i<tnum; i++) {
        ContourTriData tri = primTriArray[i];
        tri.v1.x *= m.x;
        tri.v1.y *= m.y;
        tri.v1.z *= m.z;
        tri.v2.x *= m.x;
        tri.v2.y *= m.y;
        tri.v2.z *= m.z;
        tri.v3.x *= m.x;
        tri.v3.y *= m.y;
        tri.v3.z *= m.z;
        primTriArray[i] = tri;
    }
}

