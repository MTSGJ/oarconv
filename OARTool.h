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
#include "GeneralTool.h"
#include "TreeTool.h"
#include "TerrainTool.h"
#include "LogDocTool.h"

#include "OARGlobal.h"


namespace  jbxl {


#ifdef WIN32
  #define  OART_JP2_DECOMP_COM      "opj_decompress.exe -i \"%s\" -o \"%s\" -allow-partial"
#else
  #if OPENJPEG_VER >= JP2K_VER_20
    #define  OART_JP2_DECOMP_COM    "/usr/local/bin/opj_decompress -i \"%s\" -o \"%s\"  -allow-partial >/dev/null 2>&1"
  #else
    #define  OART_JP2_DECOMP_COM    "/usr/local/bin/j2k_to_image -i \"%s\" -o \"%s\" >/dev/null 2>&1"
  #endif
#endif
#define  OART_JP2_REPAIR_EXTN       ".repair_temp.jp2"

#define  OART_FLAGS_PHANTOM         "Phantom"

//#define  OART_JOINT_TEMPLATE_FILE   "f53b84ae-322f-455d-9447-457e8a4e22ec"
#define  OART_JOINT_TEMPLATE_FILE   "f5df7c06-6636-44c1-8406-68da93408cde"


// Texture Convert ERROR for ConvertTexture()
#define  OART_TEXCNVT_NORMAL        1
#define  OART_TEXCNVT_UNKNOWN       0
#define  OART_TEXCNVT_NULL_ERR      -1      // データがNULL
#define  OART_TEXCNVT_COPY_ERR      -100    // コピーモードで失敗
#define  OART_TEXCNVT_INVD_ERR      -101    // 無効なファイル
#define  OART_TEXCNVT_CNVT_ERR      -102    // 変換エラー
#define  OART_TEXCNVT_DSTRY_ERR     -103    // ファイルが壊れている
#define  OART_TEXCNVT_LOST_ERR      -200    // Texture が無い
#define  OART_TEXCNVT_ZERO_ERR      -201    // Texture のサイズが 0
#define  OART_TEXCNVT_WRITE_ERR     -300    // 書き込みエラー

#define  OART_TEXCNVT_ALT_TEXTURE   "89556747-24cb-43ed-920b-47caed15465f"      // 木目画像

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
    Buffer pathSLD;             // Solid (with Collider) directory
    Buffer pathPTM;             // Phantom directory
    Buffer pathTRR;             // Terrain directory
    //Buffer pathTEX;             // Texture directory
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
    int    dataFormat;
    int    textureFormat;
    bool   noOffset;        // UE用 縮退
    bool   procJoints;

public:
    void   SetPathInfo(const char* oardir, const char* outdir, const char* astdir);
    void   ChangePathInfo(const char* oardir, const char* outdir, const char* astdir);
    bool   GetDataInfo(void);
    void   MakeOutputFolder(void);

    void   SetEngine(int e);
    void   SetDataFormat(int f) { dataFormat = f; }
    void   SetTextureFormat(int f) { textureFormat = f; }
    void   SetNoOffset(bool b){ noOffset = b; }
    void   SetProcJoints(bool b){ procJoints = b; }
    void   SetOutPath(char* path) { set_outpath(path);}

    int    GetEngine(void) { return engine;}
    int    GetDataFormat(void) { return dataFormat;}
    int    GetTextureFormat(void) { return textureFormat;}
    char*  GetTextureExtension(void) { return get_graphic_extension(textureFormat);}  // not free
    bool   GetNoOffset(void) { return noOffset;}
    bool   GetProcJoints(void) { return procJoints;}
    tList* GetObjectsList(void) { return objectsFiles;}
    char*  GetOutPath(void) { return get_outpath(); }   // not free

    // DAE/OBJ/STL/glTF
    int    GenerateObjectFromDataIndex(int startnum=1, int stopnum=-1, bool useBrep=false, char* command=NULL);
    int    GenerateObjectFromDataList(int* objlist, int objnum, bool useBrep=false, char* command=NULL);
    void   GenerateObjectFromDataFile(char* fname, bool useBrep=false, char* command=NULL);

    // オブジェクト データ
    void*  generateSolidData(int format, const char* fname, int num=1, bool useBrep=false, char* command=NULL);
    void   outputSolidData(int format, const char* fname, void* solid);
    void   freeSolidData(int format, void* solid);

    bool   isRequiredTexture(int format) { return (format!=JBXL_3D_FORMAT_STL_A && format!=JBXL_3D_FORMAT_STL_B);} 
    int    ConvertTexture(const char* texture, const char* addname, const char* exename, const char* path=NULL, const char* command=NULL);
    PrimBaseShape  getAbstractObject(const char* fname);

    // Terrain
    void   SetTerrainShift(Vector<float> vt) { terrainShift = vt;}
    void   SetTerrainShift(float x, float y, float z) { terrainShift.set(x, y, z);}

    void   ReadTerrainData(void);
    int    GenerateTerrainDataFile(void);

    void   SetTerrainTextureScale(float sc) { if(terrain!=NULL) for(int i=0; i<terrainNum; i++) terrain[i].set_scale((float)sc);}
        // SetTerrainTextureScale() は ReadTerrainData() と GenerateTerrainDataFile() の間で呼ぶこと．

    // Tree
    TreeTool* GetTreeTool(void) { return &treeTool;}

//  CVCounter* counter;
};


}

#endif
