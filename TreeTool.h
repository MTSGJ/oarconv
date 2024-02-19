#ifndef  __JBXL_OS_TREE_TOOL_H_
#define  __JBXL_OS_TREE_TOOL_H_

/**
@brief OpenSimの Tree/Grassのサポート
@file  TreeTool.h

@author  Fumi.Iseki 
@date    2014/10/9 
*/

#include "Vector.h"
#include "Rotation.h"
#include "AffineTrans.h"
#include "txml.h"
#include "ContourBaseData.h"

#include "TerrainTool.h"


#define  TREM_GRASS_NUM_RATE        0.1

#define  TREM_TREE_NUM              22
#define  TREM_GRASS_NUM             7

#define  TREM_TREE_PINE1            0
#define  TREM_TREE_OAK              1
#define  TREM_TREE_TROPICAL_BUSH1   2
#define  TREM_TREE_PALM1            3               
#define  TREM_TREE_DOGWOOD          4               
#define  TREM_TREE_TROPICAL_BUSH2   5   
#define  TREM_TREE_PALM2            6               
#define  TREM_TREE_CYPRESS1         7               
#define  TREM_TREE_CYPRESS2         8               
#define  TREM_TREE_PINE2            9               
#define  TREM_TREE_PLUMERIA         10              
#define  TREM_TREE_WINTER_PINE1     11
#define  TREM_TREE_WINTER_ASPEN     12
#define  TREM_TREE_WINTER_PINE2     13
#define  TREM_TREE_EUCALYPTUS       14
#define  TREM_TREE_FERN             15
#define  TREM_TREE_EELGRASS         16
#define  TREM_TREE_SEA_SWORD        17
#define  TREM_TREE_KELP1            18
#define  TREM_TREE_BEACH_GRASS1     19
#define  TREM_TREE_KELP2            20
#define  TREM_TREE_TEST             21  // for TEST Tree

#define  TREM_GRASS_GRASS0          0
#define  TREM_GRASS_GRASS1          1
#define  TREM_GRASS_GRASS2          2
#define  TREM_GRASS_GRASS3          3
#define  TREM_GRASS_GRASS4          4
#define  TREM_GRASS_GRASS5          5
#define  TREM_GRASS_TEST            6   // for TEST Grass 


namespace  jbxl {


//
class TreeParam
{
public:
    int    num_panel;

    TextureParam texture;

    Vector<double> size;
    UVMap<double>  shift;
    UVMap<double>  scale;

public:
//  TreeParam(void) { init();}
    TreeParam(void) {}
    virtual ~TreeParam(void) {}

    void init(void);
    void free(void) { texture.free();}
};


//
class GrassParam
{
public:
    TextureParam texture;

    Vector<double> size;

public:
//  GrassParam(void) { init();}
    GrassParam(void) {}
    virtual ~GrassParam(void) {}

    void init(void);
    void free(void) { texture.free();}
};


//
class  TreeTool
{
public:
    PrimBaseShape  shape;
    Buffer  addname;

    TextureParam alphaTexture;
    TreeParam  treeParam [TREM_TREE_NUM];
    GrassParam grassParam[TREM_GRASS_NUM];

public:
//  TreeTool(void) { init();}
    TreeTool(void) {} 
    virtual ~TreeTool(void) {}

    void    init(void);
    void    free(void);
    void    set_shape(PrimBaseShape pbs);

public:
    char*   GetAlphaTexture(void) { return (char*)alphaTexture.getName();}
    char*   GetTreeTexture (int kind=-1);
    char*   GetGrassTexture(int kind=-1);

    MeshObjectData*  GenerateTree (PrimBaseShape pbs, int ndiv);
    MeshObjectData*  GenerateGrass(PrimBaseShape pbs, TerrainTool* terrain);
};


}


#endif

