#ifndef  __JBXL_OS_TERRAIN_TOOL_H_
#define  __JBXL_OS_TERRAIN_TOOL_H_

/**
@brief OpenSimの Terrain用ツール

@file    TerrainTool.h
@author  Fumi.Iseki 
@date    2014 10/5
*/

#include "tools++.h"
#include "xtools.h"
#include "txml.h"

#include "ContourBaseData.h"
#include "Gdata.h"
#include "BrepLib.h"

#include "LogDocTool.h"
#include "MeshMaker.h"
#include "GeneralTool.h"

#include "OARGlobal.h"


namespace  jbxl {


#define  TRNT_TEXTURE_SIZE          4096
#define  TRNT_DEFAULT_TEX_SCALE     7.0f


///////////////////////////////////////////////////////////////////////////////////////

struct TerrainTexWeight
{
    float w[4];
};



///////////////////////////////////////////////////////////////////////////////////////
// Terrain Tool Class
//

class TerrainTool
{
public:
    Buffer terrainName;     // Terrain name

    int    xsize;           // Region X size
    int    ysize;           // Region Y size
    int    x_region_num;    // X Region Number
    int    y_region_num;    // Y Region Number
    //int    region_num;      // Region Number
    float  scale;           // Texture Scale

    int    engine;
    int    dataFormat;      //
    int    textureFormat;
    bool   noOffset;

    MSGraph<float> r32;      // r32 data

    Buffer texture[4];
    Buffer defaultTexture[4];

    float  minNW;
    float  maxNW;
    float  minNE;
    float  maxNE;
    float  minSW;
    float  maxSW;
    float  minSE;
    float  maxSE;

    float  waterHeight;      // Water Height

public:
    TerrainTool(char* name, int xs = 256, int ys = 256) { init(name, xs, ys);}
    virtual ~TerrainTool(void) {}

    void   init(char* name, int xs = 256, int ys = 256);
    void   free(void);

    void   set_size (int xs, int ys) { xsize = xs; ysize = ys;}
    void   set_scale(float sc) { scale = sc;}

    float  height(int x, int y) { return r32.gp[(r32.ys - 1 - y)*r32.xs + x];}
    float  water_height(void)   { return waterHeight;}

public:
    void   SetEngine(int e) { engine = e;}
    void   SetDataFormat(int f) { dataFormat = f;}
    void   SetTextureFormat(int f) { textureFormat = f;}
    void   SetNoOffset(bool b) { noOffset = b;}

    char*  GetTextureExtension(void) { return get_graphic_extension(textureFormat);}

    void   ReadSettings(const char* path);
    void   ReadHeightData(const char* path);

    void   GenerateTexture(tList* assets, const char* outpath);
    void   GenerateTerrain(const char* outpath, Vector<double> offset);

    TerrainTexWeight  GetTextureWeight(int x, int y);

    int    GetDioramaMode(void);
    MSGraph<uByte>  GenerateDioramaTexture (MSGraph<uByte>* vp, int mode);
    MSGraph<uByte>  GenerateWeightedTexture(MSGraph<uByte>* vp);
};



///////////////////////////////////////////////////////////////////////////////////////

TerrainTexWeight  GetTerrainBaseTextureWeight(float h, float min, float max);
int    GetScalingPixel(MSGraph<uByte>* vp, float scale, float x, float y, int col);


}

#endif

