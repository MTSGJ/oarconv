#ifndef  __JBXL_OS_PRIMSHAPE_H_
#define  __JBXL_OS_PRIMSHAPE_H_

/**
@brief OpenSimの PrimMecher用のサポートデータ．
@file  PrimShape.h

@author  Fumi.Iseki 
@date    2014 9/16

@sa http://opensimulator.org/
@sa OpenSim/Region/Physics/Meshing/PrimMesher.cs

メモ
1. PrimBaseShape で基本的なShapeデータを生成（XMLからの読み込みを含む）
2. PrimBaseShape からパラメータ(PrimShapeParam)を抽出
3. PrimShapeParam を使用してプリムを生成
*/


/*
 * Source code of OpenMetaverse is used in part. 
 *
 * see also http://openmetaverse.org/
 * see also OpenMetaverse.Primitive.TextureEntry Class
 */

/*
 * Copyright (c) 2007-2009, openmetaverse.org
 * All rights reserved.
 *
 * - Redistribution and use in source and binary forms, with or without 
 *   modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * - Neither the name of the openmetaverse.org nor the names 
 *   of its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <vector>
#include <algorithm>

#include "Vector.h"
#include "Rotation.h"
#include "AffineTrans.h"
#include "txml.h"
#include "xtools.h"

#include "ContourBaseData.h"
#include "MaterialParam.h"

#include "MaterialTool.h"
#include "SculptMesher.h"
#include "LogDocTool.h"


#define  PRIM_PROF_CIRCLE           0                       // 0x00
#define  PRIM_PROF_SQUARE           1                       // 0x01
#define  PRIM_PROF_ISOTRIANGLE      2                       // 0x02
#define  PRIM_PROF_EQUALTRIANGLE    3                       // 0x03
#define  PRIM_PROF_RIGHTTRIANGLE    4                       // 0x04
#define  PRIM_PROF_HALFCIRCLE       5                       // 0x05

#define  PRIM_PATH_LINE             16                      // 0x10
#define  PRIM_PATH_CIRCLE           32                      // 0x20
#define  PRIM_PATH_CIRCLE2          48                      // 0x30
#define  PRIM_PATH_FLEXIBLE         128                     // 0x80

#define  PRIM_HOLE_SAME             0                       // 0x00
#define  PRIM_HOLE_CIRCLE           16                      // 0x10
#define  PRIM_HOLE_SQUARE           32                      // 0x20
#define  PRIM_HOLE_TRIANGLE         48                      // 0x30

#define  PRIM_PROF_TYPE_NUM         6
#define  PRIM_PROF_CIRCLE_STR       "Circle"                // 0
#define  PRIM_PROF_SQUARE_STR       "Square"                // 1
#define  PRIM_PROF_ISOTRIANGLE_STR  "IsometricTriangle"     // 2
#define  PRIM_PROF_EQLTRIANGLE_STR  "EquilateralTriangle"   // 3
#define  PRIM_PROF_RGTTRIANGLE_STR  "RightTrianble"         // 4
#define  PRIM_PROF_HALFCIRCLE_STR   "HalfCircle"            // 5

#define  PRIM_HOLE_TYPE_NUM          4
#define  PRIM_HOLE_SAME_STR         "Same"                  // 0
#define  PRIM_HOLE_CIRCLE_STR       "Circle"                // 16
#define  PRIM_HOLE_SQUARE_STR       "Square"                // 32
#define  PRIM_HOLE_TRIANGLE_STR     "Triangle"              // 48

#define  PRIM_TYPE_BOX              1
#define  PRIM_TYPE_CYLINDER         2
#define  PRIM_TYPE_SPHERE           3
#define  PRIM_TYPE_TORUS            4
#define  PRIM_TYPE_TUBE             5
#define  PRIM_TYPE_RING             6
#define  PRIM_TYPE_SCULPT           7
#define  PRIM_TYPE_MESH             8

#define  PRIM_CUT_QUANTA            0.00002f
#define  PRIM_SCALE_QUANTA          0.01f
#define  PRIM_SHEAR_QUANTA          0.01f
#define  PRIM_TAPER_QUANTA          0.01f
#define  PRIM_REV_QUANTA            0.015f
#define  PRIM_HOLLOW_QUANTA         0.00002f

#define  PRIM_PCODE_NONE            0
#define  PRIM_PCODE_PRIM            9
#define  PRIM_PCODE_AVATAR          47
#define  PRIM_PCODE_GRASS           95
#define  PRIM_PCODE_NEWTREE         111
#define  PRIM_PCODE_PARTICLE        143
#define  PRIM_PCODE_TREE            255

#define  PRIM_MATERIAL_NUM          33      // default + 32
#define  PRIM_BUMPMAP_NUM           17      // bright, dark + 15

#define  PRIM_OS_BLANK_TEXTURE      "5748decc-f629-461c-9a36-a35a221fe21f"



namespace  jbxl {


class  PrimBaseShape;
class  PrimShapeParam;



///////////////////////////////////////////////////////////////////////////////////////////////
// PrimBaseShape
//

class  PrimBaseShape
{
public:
    Buffer  ObjectName;

    int     PCode;
    int     State;

    int     ProfileCurve;
    int     PathCurve;

    int     ProfileBegin;
    int     ProfileEnd;
    int     ProfileHollow;

    int     PathBegin;
    int     PathEnd;
    int     PathScaleX;
    int     PathScaleY;
    int     PathShearX;
    int     PathShearY;
    int     PathTaperX;
    int     PathTaperY;
    int     PathTwist;
    int     PathTwistBegin;
    int     PathSkew;
    int     PathRadiusOffset;
    int     PathRevolutions;

    int     ProfileShape;
    int     HollowShape;

    int     SculptType;
    Buffer  SculptTexture;
    bool    SculptEntry;

    Buffer  ObjFlags;

    AffineTrans<double> affineTrans;
    MaterialParam materialParam[PRIM_MATERIAL_NUM];
    Buffer  materialName[PRIM_MATERIAL_NUM];

public:
    PrimBaseShape(void) { init();}
    virtual ~PrimBaseShape(void) {}

    void    init(void);
    void    free(void);
    void    dup(PrimBaseShape p);

public:
    void    GenerateBaseParam(int type);
    void    PrintTextureEntry(void);
    void    PrintBaseParam(void);

public:
    //char* GetMaterialName(int n) { return (char*)materialName[n].buf;}
    //void  SetMaterialName(char* name, int n) { if(name!=NULL) materialName[n] = make_Buffer_bystr(name);}

    void    GetBaseParamFromXML(tXML* xml, AffineTrans<double>* base);
    void    GetMaterialParams(tList* resourceList);

    void    GetTextureEntry(tXML* xml);
    MaterialParam GetDefaultTextureEntry(uByte* ptr, int size);
    bool    ReadFacetBitField(uByte** data, int* faceBits, int* bitfieldSize);

    void    GetExtraParams(tXML* xml);
};


PrimBaseShape*  CreatePrimBaseShapesFromXML(tXML* xml, tList* rsclist, int* sno);



///////////////////////////////////////////////////////////////////////////////////////////////
// PrimShapeParam
//

class  PrimShapeParam
{
public:
    Buffer  objectName;

    int     pCode;
    int     state;

    int     profCurve;
    int     pathCurve;
    int     hollowType;

    double  profBegin;          // Dimple Begin
    double  profEnd;            // Dimple End
    double  profHollow;

    double  pathBegin;          // Path Cut Begin
    double  pathEnd;            // Path Cut End
    double  pathScaleX;
    double  pathScaleY;
    double  pathShearX;
    double  pathShearY;
    double  pathTaperX;
    double  pathTaperY;
    double  pathTwistBegin;     // Radian
    double  pathTwistEnd;       // Radian
    double  pathSkew;
    double  pathRadius;
    double  pathRevol;

    int     sculptType;
    Buffer  sculptTexture;
    bool    sculptEntry;

    Buffer  objFlags;

    AffineTrans<double> affineTrans;
    MaterialParam materialParam[PRIM_MATERIAL_NUM];

public:
    PrimShapeParam(void) { init();}
    virtual ~PrimShapeParam(void) {}

    void    init(void);
    void    free(void);
    void    dup(PrimShapeParam p);

public:
    void    GetParamFromBaseShape(PrimBaseShape shape);     // BaseShape からパラメータを取り出す
    void    PrintParam(void);
};


}


#endif

