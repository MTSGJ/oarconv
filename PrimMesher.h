#ifndef  __JBXL_OS_PRIM_MESHER_H_
#define  __JBXL_OS_PRIM_MESHER_H_

/**
@brief   This is rewrite code from PrimMesher.cs of OpenSim for JunkBox_Lib.

@file    PrimMesher.h
@author  Fumi.Iseki  iseki@solar-system.tuis.ac.jp
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

#include <vector>
#include <algorithm>

#include "FacetBaseData.h"
#include "PrimShape.h"



namespace  jbxl {


class   PrimAngle;
class   PrimAngleList;
class   PrimProfile;
class   PrimMeshParam;
class   PrimMesh;

struct  PrimPathNode;


typedef std::vector<Vector<double> > PRIM_VECTOR_ARRAY;
typedef std::vector<UVMap<double> >  PRIM_UVMAP_ARRAY;
typedef std::vector<FacetTriIndex>   PRIM_TRIINDX_ARRAY;
typedef std::vector<FacetTriData>    PRIM_TRIDATA_ARRAY;
typedef std::vector<PrimAngle>       PRIM_ANGLE_ARRAY;
typedef std::vector<PrimPathNode>    PRIM_PATHNODE_ARRAY;

typedef std::vector<int>             PRIM_INT_ARRAY;
typedef std::vector<double>          PRIM_DOUBLE_ARRAY;



///////////////////////////////////////////////////////////////////////////////////////////////
//

class  PrimAngle
{
public:
    double angle;
    double X;
    double Y;

public:
    PrimAngle(double a=0.0, double x=0.0, double y=0.0) { init(a, x, y);}
    virtual ~PrimAngle(void) {}

    void  init(double a=0.0, double x=0.0, double y=0.0) { angle = a; X = x; Y = y;}
};



///////////////////////////////////////////////////////////////////////////////////////////////
//

class  PrimAngleList
{
public:
    PRIM_ANGLE_ARRAY  angles;
    PRIM_VECTOR_ARRAY normals;
    double iX, iY;

public:
    PrimAngleList(void) {}
    virtual ~PrimAngleList(void) {}

public:
    void  MakeAngles(int sides, double sttAngle, double endAngle);

private:
    static const Vector<double> normals3[4];
    static const Vector<double> normals4[5];
    static const PrimAngle angles3[4];
    static const PrimAngle angles4[5];
    static const PrimAngle angles24[25];

    PrimAngle InterpolatePoints(double newPoint, PrimAngle p1, PrimAngle p2) {
        double m = (newPoint-p1.angle)/(p2.angle-p1.angle);
        return PrimAngle(newPoint, p1.X+m*(p2.X-p1.X), p1.Y+m*(p2.Y-p1.Y));
    }

    void Intersection(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
};



///////////////////////////////////////////////////////////////////////////////////////////////
//

/**
Extrusion用のプロファイルを生成

*/
class  PrimProfile
{
public:
    PRIM_VECTOR_ARRAY   coords;
    PRIM_VECTOR_ARRAY   normals;
    PRIM_TRIINDX_ARRAY  primTriIndex;
    PRIM_UVMAP_ARRAY    triUVs;
    PRIM_INT_ARRAY      triNums;
    PRIM_DOUBLE_ARRAY   us;         // U of UV

    PRIM_INT_ARRAY      outerCoordIndex;
    PRIM_INT_ARRAY      hollowCoordIndex;
    PRIM_INT_ARRAY      cut1CoordIndex;
    PRIM_INT_ARRAY      cut2CoordIndex;

    Vector<double>      triNormal;
    Vector<double>      cutNormal1;
    Vector<double>      cutNormal2;

    int   numPrimFacets;
    int   numOuterVertex;
    int   numOuterFacet;
    int   numHollowVertex;
    int   numHollowFacet;
    int   numBottomFacet;

    bool  calcVertexNormals;

public:
    PrimProfile() { init();}
    PrimProfile(int sides, PrimMeshParam mparam, double hollow, int hollowSides, bool calcNormals);
    virtual ~PrimProfile(void) {}

    void init(void);

public:
    void MakeTriUVs(void);

    PrimProfile Copy(void) { return Copy(true);}
    PrimProfile Copy(bool needIndex);

    void execShift(Vector<double> v) { execShift(v.x, v.y, v.z);}
    void execShift(double x, double y, double z);
    void execScale(double x, double y);
    void execRotate(Quaternion<double> q);

    void FlipNormals(void);
    void FlipUVs(void);

    void Add2FacetTriIndexVertex(int num);
    void Add2FacetTriIndexNormal(int num);
};



///////////////////////////////////////////////////////////////////////////////////////////////
//

class  PrimMeshParam : public PrimShapeParam 
{
public:
    PRIM_PATHNODE_ARRAY pathNodes;
    int     stepRevol;

public:
    PrimMeshParam(void) { init(false);}
    virtual ~PrimMeshParam(void) {}

    void    free(void) { PrimShapeParam::free();}
    void    init(bool base_init=true);
    void    dup(PrimMeshParam p);

public:
    void  CreatePathNodes(int pathType, int steps);
};



struct  PrimPathNode
{
    Vector<double>     position;
    Quaternion<double> rotation;
    double             xScale;
    double             yScale;
    double             percent;
};



///////////////////////////////////////////////////////////////////////////////////////////////
//

class  PrimMesh
{
public:
    PRIM_VECTOR_ARRAY   coords;
    PRIM_VECTOR_ARRAY   normals;
    PRIM_UVMAP_ARRAY    uvs;
    PRIM_TRIINDX_ARRAY  primTriIndex;
    PRIM_TRIDATA_ARRAY  primTriArray;

    int   shapeType;
    int   sides;
    int   hollowSides;
    PrimMeshParam   meshParam;

    int   numPrimFacets;

    bool  calcVertexNormals;
    bool  normalsProcessed;

    int   numProfileOuterFacet;
    int   numProfileHollowFacet;
    bool  hasHollow;
    bool  hasProfileCut;

public:
    PrimMesh(void) { init();}
    PrimMesh(int sides, int hollowsides, PrimMeshParam mesh);
    virtual ~PrimMesh(void) {}

    void  free(void) {}
    void  init(void);

public:
    void  Extrude(int pathType);

    Vector<double> GetTriNormal(Vector<double> v1, Vector<double> v2, Vector<double> v3) { return NewellMethod(v1, v2, v3).normalize();}
    Vector<double> GetTriNormal(FacetTriIndex itri) { return GetTriNormal(coords[itri.v1], coords[itri.v2], coords[itri.v3]);}
    Vector<double> GetTriNormal(int index);

    PrimMesh  Copy(void);

    void ComputeTriNormals(void);
    void execShift(double x, double y, double z);
    void execScale(double x, double y, double z);
    void execRotate(Quaternion<double> q);
};


}

#endif
