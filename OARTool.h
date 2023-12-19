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




namespace  jbxl {


#ifdef WIN32
  #define  OART_DEFAULT_INP_DIR     ".\\"
  #define  OART_DEFAULT_DAE_DIR     "DAE\\"
  #define  OART_DEFAULT_STL_DIR     "STL\\"
  #define  OART_DEFAULT_AST_DIR     "assets\\"
  #define  OART_DEFAULT_TEX_DIR     "Textures\\"
  #define  OART_DEFAULT_PTM_DIR     "Phantoms\\"
  #define  OART_JP2_DECOMP_COM      "opj_decompress.exe -i %s -o %s"
#else
  #define  OART_DEFAULT_INP_DIR     "./"
  #define  OART_DEFAULT_DAE_DIR     "./DAE/"
  #define  OART_DEFAULT_STL_DIR     "./STL/"
  #define  OART_DEFAULT_AST_DIR     "/usr/local/share/oarconv/assets/:./assets/"
  #define  OART_DEFAULT_TEX_DIR     "Textures/"
  #define  OART_DEFAULT_PTM_DIR     "Phantoms/"
  #define  OART_JP2_DECOMP_COM      "/usr/local/bin/opj_decompress -i %s -o %s >/dev/null 2>&1"
#endif

#define  OART_FLAGS_PHANTOM         "Phantom"



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
    Buffer pathDAE;             // Output directory for DAE
    Buffer pathSTL;             // Output directory for STL
    Buffer pathTEX;             // Texture directory
    Buffer pathPTM;             // Phantom directory
    Buffer pathAST;             // Adding assets directory

    Vector<float> shift;        //

    tList* settingsFiles;       // Files list in settings
    tList* terrainsFiles;       // Files list in terrains
    tList* landdataFiles;       // Files list in landata    
    tList* assetsFiles;         // Files list in assets     
    tList* objectsFiles;        // Files list in objects

    TerrainTool* terrain;       // pointer to TerrainTool
    TreeTool treeTool;

public:
    bool   forUnity3D;          // Unity3Dサポート
    bool   forUnity5;           // Unity5.x 用
    bool   forUnity4;           // Unity4.x 用
    int    terrainNum;
    int    objectsNum;

    OARTool(void) { init();}
    virtual ~OARTool(void) {}

    void   init(void);
    void   free(void);

    void   setUnity4(bool); 
    void   setUnity5(bool);
    void   setUnity3D(bool);

    void   clear_path(void);
    void   clear_list(void);
    char*  get_outpath(bool outdae) { if(outdae) return (char*)pathDAE.buf; else return (char*)pathSTL.buf;}

public:
    void   SetPathInfo(const char* oardir, const char* daedir, const char* stldir, const char* astdir);
    bool   GetDataInfo(void);
    void   MakeOutputFolder(bool outdae=true);

    void   SetShift(Vector<float> vt) { shift = vt;}
    void   SetShift(float x, float y, float z) { shift.set(x, y, z);}

    tList* GetObjectsList(void) { return objectsFiles;}
    void   ReadTerrainData(void);

    int    GenerateTerrainDae (void);
    int    GenerateObjectsDae (int startnum=1, int stopnum=-1, bool useBrep=true, bool phantom=false, char* command=NULL);
    int    GenerateSelectedDae(int objnum, int* objlist, bool useBrep=true, bool phantom=false, char* command=NULL);
    void   GenerateDae(const char* fname, int num=1, bool useBrep=true, bool phantom=false, char* command=NULL);

    void   ConvertTexture(const char* texture, const char* addname, const char* exename, const char* path=NULL, const char* command=NULL);
    void   MakeDummyTexture(const char* texture, const char* addname, const char* exename, const char* path=NULL);

    int    GenerateTerrainSTL (bool binfile=true);
    int    GenerateObjectsSTL (int startnum=1, int stopnum=-1, bool binfile=true);
    int    GenerateSelectedSTL(int objnum, int* objlist, bool binfile=true);
    void   GenerateSTL(const char* fname, int num=1, bool binfile=true);
    //
    BrepSolidList*  GenerateSolidList(const char* fname);

    // ReadTerrainData と GenerateTerrainDae の間で呼ぶこと．
    void  SetTerrainTextureScale(float sc) { if(terrain!=NULL) for(int i=0; i<terrainNum; i++) terrain[i].set_scale((float)sc);}

    TreeTool* GetTreeTool(void) { return &treeTool;}

//  CVCounter* counter;
};


}

#endif
