#ifndef  __JBXL_OS_TERRAIN_MESHER_H_
#define  __JBXL_OS_TERRAIN_MESHER_H_

/**
@brief OpenSimのTerrain用 Mesher．SculptMesherを参考にしている．

@file    TerrainMesher.h
@author  Fumi.Iseki, K.Suzuki 
@date    2014 10/

@sa http://opensimulator.org/
@sa http://www.nsl.tuis.ac.jp/xoops/modules/xpwiki/?OpenSim%2Fr32file
@sa OpenSim/Region/Physics/Meshing/SculptMesh.cs
*/

#include <vector>
#include <algorithm>

#include "ContourBaseData.h"
#include "Gdata.h"

#include "LogDocTool.h"


#define  TERRAIN_MESH_SEABED_LEVEL  -50.0


namespace  jbxl {


typedef std::vector<UVMap<float> >        TERRAIN_UVMAP_ARRAY;
typedef std::vector<Vector<float> >       TERRAIN_VECTOR_ARRAY;
typedef std::vector<TERRAIN_VECTOR_ARRAY> TERRAIN_IMAGE_ARRAY;


//
class TerrainMesh
{
public:
    TERRAIN_VECTOR_ARRAY coords;
    TERRAIN_VECTOR_ARRAY normals;
    TERRAIN_UVMAP_ARRAY  uvs;

    CONTOUR_TRIINDX_ARRAY terrainTriIndex;
    CONTOUR_TRIDATA_ARRAY terrainTriArray;
    TERRAIN_IMAGE_ARRAY   terrainImage;

    int   xsize;
    int   ysize;

    float max_height;
    float min_height;
    float seabed;

    bool  bound_left;
    bool  bound_right;
    bool  bound_top;
    bool  bound_bottom;

public:
    TerrainMesh(void) { init();}
    virtual ~TerrainMesh(void) {}

    void  init(void);
    void  free(void) { clear();}
    void  clear(void);
    void  clear_data(void);
    void  clear_image(void);
    void  set_boundary(bool l, bool r, bool t, bool b) { bound_left = l; bound_right = r; bound_top = t; bound_bottom = b;} 

public:
    bool  Image2Coords(MSGraph<float> grd);
    void  GenerateMeshData(Vector<float> shift, bool autoseabed);
    void  ComputeTriNormals(void);
    void  SetupTriArray(Vector<float> shift);
};


}   // namespace


#endif
