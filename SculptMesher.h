#ifndef  __JBXL_SCULPT_MESHER_H_
#define  __JBXL_SCULPT_MESHER_H_

/**
@brief   This is rewrite code from SculptMesh.cs of OpenSim for JunkBox_Lib.

@file    SculptMesher.h
@author  Fumi.Iseki  iseki@rsch.tuis.ac.jp
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

#include "ContourBaseData.h"
#include "Gdata.h"

#include "LogDocTool.h"


#define  SCULPT_TYPE_NONE       0
#define  SCULPT_TYPE_SPHERE     1
#define  SCULPT_TYPE_TORUS      2
#define  SCULPT_TYPE_PLANE      3
#define  SCULPT_TYPE_CYLINDER   4
#define  SCULPT_TYPE_MESH       5
#define  SCULPT_TYPE_INVERT     64
#define  SCULPT_TYPE_MIRROR     128

#define  SCULPT_SIZE_32x32      0

#define  SCULPT_SIZE_64x64      10
#define  SCULPT_SIZE_32x128     11
#define  SCULPT_SIZE_128x32     12
#define  SCULPT_SIZE_8x512      13
#define  SCULPT_SIZE_512x8      14
#define  SCULPT_SIZE_16x256     15
#define  SCULPT_SIZE_256x16     16

#define  SCULPT_SIZE_64x128     20
#define  SCULPT_SIZE_128x64     21

#define  SCULPT_SIZE_128x128    30
#define  SCULPT_SIZE_64x256     31
#define  SCULPT_SIZE_256x64     32  // not
#define  SCULPT_SIZE_32x512     33
#define  SCULPT_SIZE_512x32     34
#define  SCULPT_SIZE_16x1024    35  // not
#define  SCULPT_SIZE_1024x16    36

#define  SCULPT_SIZE_256x256    40

#define  SCULPT_SIZE_256x512    50  // not
#define  SCULPT_SIZE_512x256    50

#define  SCULPT_SIZE_512x512    60

#define  SCULPT_SIZE_1024x1024  70

#define  SCULPT_SIZE_OTHER      99

#define  SCULPT_IMAGE_ERROR     -1
#define  SCULPT_IMAGE_NORMAL    0
#define  SCULPT_IMAGE_MIRROR    1



namespace  jbxl {


typedef std::vector<UVMap<double> >       SCULPT_UVMAP_ARRAY;
typedef std::vector<Vector<double> >      SCULPT_VECTOR_ARRAY;
typedef std::vector<SCULPT_VECTOR_ARRAY>  SCULPT_IMAGE_ARRAY;



//
// データの保持は２通り
// データ毎に保持： coords, normals, uvs
// 三角ポリゴンとして保持: sculptTriIndex, sculptTriArray
//
class SculptMesh
{
public:
    SCULPT_VECTOR_ARRAY coords;
    SCULPT_VECTOR_ARRAY normals;
    SCULPT_UVMAP_ARRAY  uvs;

    CONTOUR_TRIINDX_ARRAY sculptTriIndex;
    CONTOUR_TRIDATA_ARRAY sculptTriArray;

    SCULPT_IMAGE_ARRAY  sculptImage;

    int   type;
    int   sizetype;

    int   xscale;
    int   yscale;
    int   xshift;
    int   yshift;
    int   xbndry;
    int   ybndry;

    bool  invert;
    bool  mirror;

    // UV Map
    bool  flipUV;
    bool  flipU;
    bool  flipV;

public:
    SculptMesh(int typ=SCULPT_TYPE_SPHERE) { init(typ);}
    virtual ~SculptMesh(void) {}

    void  init(int typ=SCULPT_TYPE_SPHERE);
    void  free(void) { clear();}
    void  clear(void);
    void  clear_data(void);
    void  clear_image(void);

    void  set_type (int typ);
    void  set_scale(int xscl, int yscl) { xscale = xscl, yscale = yscl;}

public:
    MSGraph<double> MakeSculptImage(MSGraph<uByte> grd);

    bool  Image2Coords(MSGraph<uByte> grd);
    void  GenerateMeshData(void);

    void  ComputeTriNormals(int xs, int ys);
    void  SetupTriArray(void);

    SculptMesh  Copy(void) { return SculptMesh(*this);}

    void  execShift(double x, double y, double z);
    void  execScale(double x, double y, double z);
    void  execRotate(Quaternion<double> q);

    void  execFlipUV(void){int sz=(int)uvs.size();for(int i=0;i<sz;i++){uvs[i].u=1.0f-uvs[i].u;uvs[i].v=1.0f-uvs[i].v;}}
    void  execFlipU(void) {int sz=(int)uvs.size();for(int i=0;i<sz;i++){uvs[i].u=1.0f-uvs[i].u;}}
    void  execFlipV(void) {int sz=(int)uvs.size();for(int i=0;i<sz;i++){uvs[i].v=1.0f-uvs[i].v;}}

    int   GetSculptScale(int width, int height);
    void  GetSculptResolution(int width, int height, int* s, int* t);
};


}


#endif
