#ifndef  __JBXL_OAR_TOOL_H_
#define  __JBXL_OAR_TOOL_H_

/**
@brief OpenSimの OAR用ツール

@file    OARTool.h
@author  Fumi.Iseki 
@date    2014 10/5
*/

#include "tools++.h"
#include "xtools.h"
#include "txml.h"
#include "Gdata.h"

#include "Jpeg2KTool.h"
#include "TgaTool.h"
#include "BrepLib.h"

#include "MeshMaker.h"
#include "MaterialTool.h"
#include "TreeTool.h"
#include "TerrainTool.h"
#include "LogDocTool.h"

#include "OARGlobal.h"


namespace  jbxl {


#ifdef WIN32
  #define  OART_JP2_DECOMP_COM      "opj_decompress.exe -i \"%s\" -o \"%s\""
#else
  #if OPENJPEG_VER >= JP2K_VER_20
    #define  OART_JP2_DECOMP_COM    "/usr/local/bin/opj_decompress -i \"%s\" -o \"%s\" >/dev/null 2>&1"
  #else
    #define  OART_JP2_DECOMP_COM    "/usr/local/bin/j2k_to_image -i \"%s\" -o \"%s\" >/dev/null 2>&1"
  #endif
#endif

#define  OART_FLAGS_PHANTOM         "Phantom"

//#define  OART_JOINT_TEMPLATE_FILE   "f53b84ae-322f-455d-9447-457e8a4e22ec"
#define  OART_JOINT_TEMPLATE_FILE   "f5df7c06-6636-44c1-8406-68da93408cde"



///////////////////////////////////////////////////////////////////////////////////////
// OARTool
//

class OARTool
{
public:
    Buffer regionName;          //

    bool   isMegaRegion;        //
    int    majorVersion;        // OAR format major version
    int    minorVersion;        // OAR format minor version
    int    xsize, ysize;        //
    float  waterHeight;         //

private:
    Buffer pathOAR;             // OAR directory
    Buffer pathOUT;             // Output directory for output
    Buffer pathTEX;             // Texture directory
    Buffer pathPTM;             // Phantom directory
    Buffer pathAST;             // Adding assets directory

    Vector<double> terrainShift;// Terrain座標

    tList* settingsFiles;       // Files list in settings
    tList* terrainsFiles;       // Files list in terrains
    tList* landdataFiles;       // Files list in landata    
    tList* assetsFiles;         // Files list in assets     
    tList* objectsFiles;        // Files list in objects

    TerrainTool* terrain;       // pointer to TerrainTool
    TreeTool treeTool;

public:
    int    terrainNum;
    int    objectsNum;

public:
    bool   forUnity4;           // Unity4.x 用
    bool   forUnity5;           // Unity5.x 用
    bool   forUnity;            // Unityサポート
    bool   forUE;               // Unreal Engine

public:
    OARTool(void) { init();}
    virtual ~OARTool(void) {}

    void   init(void);
    void   free(void);

    void   clear_terrain(void);
    void   clear_path(void);
    void   clear_list(void);

    char*  get_outpath(void);
    void   set_outpath(char* path);

    void   setUnity4(bool); 
    void   setUnity5(bool);
    void   setUnity(bool);
    void   setUE(bool);

private:
    int    engine;
    int    dataformat;
    bool   noOffset;        // UE用 縮退
    bool   procJoints;

public:
    void   SetPathInfo(const char* oardir, const char* outdir, const char* astdir);
    void   ChangePathInfo(const char* oardir, const char* outdir, const char* astdir);
    bool   GetDataInfo(void);
    void   MakeOutputFolder(void);

    void   SetEngine(int e);
    void   SetDataFormat(int f) { dataformat = f; }
    void   SetNoOffset(bool b){ noOffset = b; }
    void   SetProcJoints(bool b){ procJoints = b; }
    void   SetTerrainShift(Vector<float> vt) { terrainShift = vt;}
    void   SetTerrainShift(float x, float y, float z) { terrainShift.set(x, y, z);}
    void   SetOutPath(char* path) { set_outpath(path);}

    int    GetEngine(void) { return engine;}
    int    GetDataFormat(void) { return dataformat;}
    bool   GetNoOffset(void) { return noOffset;}
    bool   GetProcJoints(void) { return procJoints;}
    tList* GetObjectsList(void) { return objectsFiles;}
    char*  GetOutPath(void) { return get_outpath(); }    // not free

    void   ReadTerrainData(void);

    // DAE/OBJ/STL
    int    GenerateObjectFromDataIndex(int startnum=1, int stopnum=-1, bool useBrep=true, char* command=NULL);
    int    GenerateObjectFromDataList(int* objlist, int objnum, bool useBrep=true, char* command=NULL);
    void   GenerateObjectFromDataFile(char* fname, bool useBrep=true, char* command=NULL);

    int    GenerateTerrainDataFile (void);

    // オブジェクト データ
    void*  generateSolidData(int format, const char* fname, int num=1, bool useBrep=true, char* command=NULL);
    void   outputSolidData(int format, const char* fname, void* solid);
    void   freeSolidData(int format, void* solid);

    bool   isRequiredTexture(int format) { return (format!=JBXL_3D_FORMAT_STL_A && format!=JBXL_3D_FORMAT_STL_B);} 
    void   ConvertTexture(const char* texture, const char* addname, const char* exename, const char* path=NULL, const char* command=NULL);
    PrimBaseShape  getAbstractObject(const char* fname);

    // ReadTerrainData と GenerateTerrainDataFile の間で呼ぶこと．
    void   SetTerrainTextureScale(float sc) { if(terrain!=NULL) for(int i=0; i<terrainNum; i++) terrain[i].set_scale((float)sc);}

    TreeTool* GetTreeTool(void) { return &treeTool;}

//  CVCounter* counter;
};


}

#endif
