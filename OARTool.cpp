/**
@brief  OpenSimの OAR用ツール

@file   OARTool.cpp
@author Fumi.Iseki 
@date   2014 10/5
*/

#include "OARTool.h"


using namespace jbxl;


///////////////////////////////////////////////////////////////////////////////////////
// OARTool Class
//

void  OARTool::init(void)
{
    pathOAR = init_Buffer();    // OAR directory
    pathOUT = init_Buffer();    // Output directory
    pathSLD = init_Buffer();    // Solid directory
    pathPTM = init_Buffer();    // Fhantom directory
    pathTRR = init_Buffer();    // Terrain directory
    //pathTEX = init_Buffer();    // Texture directory
    pathAST = init_Buffer();    // Adding assets directory

    regionName = init_Buffer();

    isMegaRegion    = false;    //
    majorVersion    = 0;        // OAR format major version
    minorVersion    = 0;        // OAR format minor version
    xsize           = 256;      //
    ysize           = 256;      //
    waterHeight     = 20.0;     //
    terrainShift.set(0.0, 0.0, 0.0);

    objectsNum      = 0;

    settingsFiles   = NULL;     // Files list in settings
    terrainsFiles   = NULL;     // Files list in terrains
    landdataFiles   = NULL;     // Files list in landata 
    assetsFiles     = NULL;     // Files list in assets  
    objectsFiles    = NULL;     // Files list in objects

    forUE           = false;    // for UE
    forUnity4       = false;    // for Unity v4.x
    forUnity5       = false;    // for Unity v5.x
    forUnity        = true;     // for Unity

    engine          = JBXL_3D_ENGINE_UNITY;
    dataFormat      = JBXL_3D_FORMAT_DAE;
    textureFormat   = JBXL_TEXTURE_PNG;
    noOffset        = false;
    procJoints      = false;
    terrainNum      = 0;
    terrain         = NULL;     // pointer to TerrainSetteings

    treeTool.init();

    // for BREP
    SetVertexTolerance(1.0e-8f);
    SetAbsVertexTolerance(1.0e-6f);
}


//
void  OARTool::free(void)
{
    treeTool.free();
    clear_terrain();
    clear_path();
    clear_list();
    //
    free_Buffer(&regionName);
}


void  OARTool::clear_terrain(void)
{
    if (terrain!=NULL) {
        for (int i=0; i<terrainNum; i++) terrain[i].free();
        ::free(terrain);
        terrain = NULL;
        terrainNum = 0;
    }
}


char*  OARTool::get_outpath(void)
{
    return (char*)pathOUT.buf;
}


void   OARTool::set_outpath(char* path)
{
    if (path==NULL) return;

    copy_s2Buffer(path, &pathOUT);

#ifdef WIN32
    if (pathOUT.buf[pathOUT.vldsz-1]!='\\') cat_s2Buffer("\\", &pathOUT);
#else
    if (pathOUT.buf[pathOUT.vldsz-1]!='/')  cat_s2Buffer("/",  &pathOUT);
#endif
}


//
void  OARTool::clear_path(void)
{
    free_Buffer(&pathOAR);
    free_Buffer(&pathOUT);
    free_Buffer(&pathSLD);
    free_Buffer(&pathPTM);
    free_Buffer(&pathTRR);
    //free_Buffer(&pathTEX);
    free_Buffer(&pathAST);
}


//
void  OARTool::clear_list(void)
{
    del_tList(&settingsFiles);
    del_tList(&terrainsFiles);
    del_tList(&landdataFiles);
    del_tList(&assetsFiles);
    del_tList(&objectsFiles);
}


void  OARTool::setUnity4(bool unity4)
{
    forUnity4 = unity4;
    if (unity4) forUnity = true;
    return;
}


void  OARTool::setUnity5(bool unity5)
{
    forUnity5 = unity5;
    if (unity5) forUnity = true;
    return;
}


void  OARTool::setUnity(bool unity)
{
    forUnity = unity;
    if (!unity) {
        forUnity4 = false;
        forUnity5 = false;
    }
    return;
}


void  OARTool::setUE(bool ue)
{
    forUE = ue;
    return;
}


void  OARTool::SetEngine(int e)
{
    setUnity(false);
    setUE(false);
    //
    engine = e;
    if      (e == JBXL_3D_ENGINE_UNITY)  setUnity(true);
    else if (e == JBXL_3D_ENGINE_UE)     setUE(true);
    return;
}


/**
データ入出力用のパス情報を設定する．

設定されるパス情報は以下の通り．
pathOAR : データ入力用（OAR）のパス
pathSLD : オブジェクト（コライダー）データ出力用のパス
pathPTM : ファントムデータ出力用のパス
pathTRR : 地形データ出力用のパス
pathAST : 追加のアセットデータ用のパス

@param oardir  読み込むOARファイルのトップディレクトリ
@param outdir  データ書き出し用ディレクトリ
@param astdir  アセットデータの追加ディレクトリ（OARに含まれていないアッセットデータ用）
*/
void  OARTool::SetPathInfo(const char* oardir, const char* outdir, const char* astdir)
{
    clear_path();

    // OAR
    if (oardir==NULL) {
        pathOAR = make_Buffer_bystr(OART_DEFAULT_INP_DIR);
    }
    else {
        pathOAR = make_Buffer_bystr(oardir);
    #ifdef WIN32
        if (pathOAR.buf[strlen((char*)pathOAR.buf)-1]!='\\') cat_s2Buffer("\\", &pathOAR);
    #else
        if (pathOAR.buf[strlen((char*)pathOAR.buf)-1]!='/') cat_s2Buffer("/", &pathOAR);
    #endif
    }

    // OUTPUT
    if (outdir!=NULL) {
        pathOUT = make_Buffer_bystr(outdir);
    }
    else {
        // DAE
        if (dataFormat==JBXL_3D_FORMAT_DAE) {
            pathOUT = make_Buffer_bystr(OART_DEFAULT_DAE_DIR);
        }
        // OBJ
        else if (dataFormat==JBXL_3D_FORMAT_OBJ) {
            pathOUT = make_Buffer_bystr(OART_DEFAULT_OBJ_DIR);
        }
        // GLTF
        else if (dataFormat==JBXL_3D_FORMAT_GLTF) {
            pathOUT = make_Buffer_bystr(OART_DEFAULT_GLTF_DIR);
        }
        // GLB
        else if (dataFormat==JBXL_3D_FORMAT_GLB) {
            pathOUT = make_Buffer_bystr(OART_DEFAULT_GLB_DIR);
        }
        // FBX
        else if (dataFormat==JBXL_3D_FORMAT_FBX) {
            pathOUT = make_Buffer_bystr(OART_DEFAULT_FBX_DIR);
        }
        // STL
        else { 
            pathOUT = make_Buffer_bystr(OART_DEFAULT_STL_DIR);
            noOffset = false;
        }
        //
        if (noOffset) {
            if (pathOUT.buf[pathOUT.vldsz-1]=='/' || pathOUT.buf[pathOUT.vldsz-1]=='\\') {
                pathOUT.buf[pathOUT.vldsz-1]='\0';
                pathOUT.vldsz--;
            }
            cat_s2Buffer(OART_DEFAULT_NOFST_DIR, &pathOUT);
        }
    }

#ifdef WIN32
    if (pathOUT.buf[pathOUT.vldsz-1]!='\\')  cat_s2Buffer("\\",  &pathOUT);
#else
    if (pathOUT.buf[pathOUT.vldsz-1]!='/')  cat_s2Buffer("/",  &pathOUT);
#endif
    // Set Path
    //pathTEX = make_Buffer_bystr((char*)pathOUT.buf);
    pathPTM = make_Buffer_bystr((char*)pathOUT.buf);
    pathSLD = make_Buffer_bystr((char*)pathOUT.buf);
    pathTRR = make_Buffer_bystr((char*)pathOUT.buf);
    cat_s2Buffer(OART_DEFAULT_PTM_DIR, &pathPTM);
    cat_s2Buffer(OART_DEFAULT_SLD_DIR, &pathSLD);
    cat_s2Buffer(OART_DEFAULT_TRR_DIR, &pathTRR);

/*
    cat_s2Buffer(OART_DEFAULT_TEX_DIR, &pathTEX);
    if (dataFormat!=JBXL_3D_FORMAT_OBJ || engine!=JBXL_3D_ENGINE_UE) {
        pathPTM = make_Buffer_bystr((char*)pathOUT.buf);
        cat_s2Buffer(OART_DEFAULT_PTM_DIR, &pathPTM);
    }
*/

    // ASSET
    if (astdir==NULL) {
        pathAST = make_Buffer_bystr(OART_DEFAULT_AST_DIR);
    }
    else {
        pathAST = make_Buffer_bystr(astdir);
    }
}


/**
データ入出力用のパス情報を修正する．

修正するものだけ指定する．
*/
void  OARTool::ChangePathInfo(const char* oardir, const char* outdir, const char* astdir)
{
    // OAR
    if (oardir != NULL) {
        free_Buffer(&pathOAR);
        pathOAR = make_Buffer_bystr(oardir);
#ifdef WIN32
        if (pathOAR.buf[strlen((char*)pathOAR.buf) - 1] != '\\') cat_s2Buffer("\\", &pathOAR);
#else
        if (pathOAR.buf[strlen((char*)pathOAR.buf) - 1] != '/') cat_s2Buffer("/", &pathOAR);
#endif
    }
    // OUTPUT
    if (outdir != NULL) {
        free_Buffer(&pathOUT);
        free_Buffer(&pathSLD);
        free_Buffer(&pathPTM);
        free_Buffer(&pathTRR);
        //free_Buffer(&pathTEX);

        pathOUT = make_Buffer_bystr(outdir);
#ifdef WIN32
        if (pathOUT.buf[strlen((char*)pathOUT.buf) - 1] != '\\') cat_s2Buffer("\\", &pathOUT);
#else
        if (pathOUT.buf[strlen((char*)pathOUT.buf) - 1] != '/') cat_s2Buffer("/", &pathOUT);
#endif
        //pathTEX = make_Buffer_bystr((char*)pathOUT.buf);
        pathSLD = make_Buffer_bystr((char*)pathOUT.buf);
        pathPTM = make_Buffer_bystr((char*)pathOUT.buf);
        pathTRR = make_Buffer_bystr((char*)pathOUT.buf);
        //cat_s2Buffer(OART_DEFAULT_TEX_DIR, &pathTEX);
        cat_s2Buffer(OART_DEFAULT_SLD_DIR, &pathSLD);
        cat_s2Buffer(OART_DEFAULT_PTM_DIR, &pathPTM);
        cat_s2Buffer(OART_DEFAULT_TRR_DIR, &pathTRR);
    }
    // ASSET
    if (astdir != NULL) {
        free_Buffer(&pathAST);
        pathAST = make_Buffer_bystr(astdir);
    }
}


/**
入力用データ（OAR）の一部を読み込んで，情報を得る．
OARデータでない場合は false を返す．

得られる情報は以下の通り．
regionName   : リージョンの名前
majorVersion : データファイルのメジャーバージョン．
minorVersion : データファイルのマイナーバージョン．
xsize, ysize : リージョンの xyサイズ．通常 xsize, ysize は等しい．
isMegaRegion : メガリージョンかどうか？
objectsNum   : オブジェクト数（Terrainを除く）
terrainNum   : Terrain (settings ファイル) の数
settingsFiles: Files list in settings
terrainsFiles: Files list in terrains
landdataFiles: Files list in landata    
assetsFiles  : Files list in assets     
objectsFiles : Files list in objects

@return  pathOARの指すパスがOARのデータならば true, そうでないなら falseを返す．
*/
bool  OARTool::GetDataInfo()
{
    clear_list();
    PRINT_MESG("OARTool::GetDataInfo: read OAR files from %s\n", pathOAR.buf);
    //
    // archive.xml
    Buffer arc_path = dup_Buffer(pathOAR);
    cat_s2Buffer("archive.xml", &arc_path);
    tXML* arc_xml = xml_parse_file((char*)arc_path.buf);
    free_Buffer(&arc_path);

    if (arc_xml!=NULL) {
        char* ismega = get_xml_str_content_bystr(arc_xml, "<archive><region_info><is_megaregion>");
        if (ismega!=NULL) if (!strcmp(ismega, "TRUE")) isMegaRegion = true;
        majorVersion = get_xml_int_attr_bystr(arc_xml, "<archive>", "major_version");
        minorVersion = get_xml_int_attr_bystr(arc_xml, "<archive>", "minor_version");
        char* xysize = get_xml_str_content_bystr(arc_xml, "<archive><region_info><size_in_meters>");
        if (xysize!=NULL) {
            char* xstrsz = awk(xysize, ',', 1);
            char* ystrsz = awk(xysize, ',', 2);
            xsize = atoi(xstrsz);
            ysize = atoi(ystrsz);
            ::free(xstrsz);
            ::free(ystrsz);
        }
        del_all_xml(&arc_xml);
    }
    else {
        PRINT_MESG("OARTool::GetDataInfo: ERROR: not found archive file! [archive.xml at %s]\n", pathOAR.buf);
        return false;
    }

    // Settings Files
    Buffer set_path = dup_Buffer(pathOAR);
    #ifdef WIN32
        cat_s2Buffer("settings\\", &set_path);
    #else
        cat_s2Buffer("settings/", &set_path);
    #endif
    settingsFiles = get_dir_files((char*)set_path.buf);
    free_Buffer(&set_path);
    //
    int snum = 0;
    tList* lp = settingsFiles;
    while (lp!=NULL) {
        if (ex_strncaservscmp((char*)lp->ldat.val.buf, ".xml" , 4)) {
            Buffer name = make_Buffer_bystr(get_file_name((char*)lp->ldat.val.buf));
            del_file_extension_Buffer(&name);
            lp->ldat.key = name;
            if (snum==0) regionName = dup_Buffer(name);
            lp = lp->next;
            snum++;
        }
        else {
            if (lp==settingsFiles) settingsFiles = lp->next;
            lp = del_tList_node(&lp);
        }
    }
    if (snum==0) {
        DEBUG_MODE PRINT_MESG("OARTool::SetupPathInfo: WARNING: not found settings file! [%ssettings/]\n", pathOAR.buf);
        return false;
    }

    // Terrain R32 Files
    Buffer trn_path = dup_Buffer(pathOAR);
    #ifdef WIN32
        cat_s2Buffer("terrains\\", &trn_path);
    #else
        cat_s2Buffer("terrains/", &trn_path);
    #endif
    terrainsFiles = get_dir_files((char*)trn_path.buf);
    free_Buffer(&trn_path);
    //
    int tnum = 0;
    lp = terrainsFiles;
    while (lp!=NULL) {
        if (ex_strncaservscmp((char*)lp->ldat.val.buf, ".r32" , 4)) {
            Buffer name = make_Buffer_bystr(get_file_name((char*)lp->ldat.val.buf));
            del_file_extension_Buffer(&name);
            lp->ldat.key = name;
            lp = lp->next;
            tnum++;
        }
        else {
            if (lp==terrainsFiles) terrainsFiles = lp->next;
            lp = del_tList_node(&lp);
        }
    }
    if (tnum==0) {
        DEBUG_MODE PRINT_MESG("OARTool::SetupPathInfo: WARNING: not found terrains file! [%sterrains/]\n", pathOAR.buf);
        return false;
    }

    terrainNum = tnum;      // number of terrains

    // Landdata Files
    Buffer lnd_path = dup_Buffer(pathOAR);
    #ifdef WIN32
        cat_s2Buffer("landdata\\", &lnd_path);
    #else
        cat_s2Buffer("landdata/",  &lnd_path);
    #endif
    landdataFiles = get_dir_files((char*)lnd_path.buf);
    free_Buffer(&lnd_path);
    /*
    if (landdataFiles!=NULL) {
        tXML* land_xml = xml_parse_file((char*)landdataFiles->ldat.val.buf);
        if (land_xml!=NULL) {
            char* name = get_xml_str_content_bystr(land_xml, "<LandData><Name>"); // パーセル名
            regionName = make_Buffer_bystr(name);
            del_all_xml(&land_xml);
        }
    }*/

    // Objects Files
    Buffer obj_path = dup_Buffer(pathOAR);
    #ifdef WIN32
        cat_s2Buffer("objects\\", &obj_path);
    #else
        cat_s2Buffer("objects/",  &obj_path);
    #endif
    objectsFiles = get_dir_files_rcsv((char*)obj_path.buf);
    free_Buffer(&obj_path);

    objectsNum = 0;
    lp = objectsFiles;
    while (lp!=NULL) {
        if (ex_strncaservscmp((char*)lp->ldat.val.buf, ".xml" , 4)) {
            objectsNum++;
            lp = lp->next;
        }
        else {
            if (lp==objectsFiles) objectsFiles = lp->next;
            lp = del_tList_node(&lp);
        }
    }

    // Assets Files
    tList*  extn = add_tList_node_str(NULL, "txt", NULL);   // 除外拡張子
    //
#ifdef WIN32
    // Windowsの場合は : で区切れない．'|' で区切る．
    Buffer* dirs = awk_Buffer_dim(pathAST, '|');
#else
    Buffer* dirs = awk_Buffer_dim(pathAST, ':');
#endif
    if (dirs != NULL) {
        int num = dirs->state;
        for (int i = 0; i < num; i++) {
#ifdef WIN32
            if (dirs[i].buf[strlen((char*)dirs[i].buf) - 1] != '\\') cat_s2Buffer("\\", &dirs[i]);
#else
            if (dirs[i].buf[strlen((char*)dirs[i].buf) - 1] != '/') cat_s2Buffer("/", &dirs[i]);
#endif
            assetsFiles = add_resource_list((char*)dirs[i].buf, 36, assetsFiles, extn, 2);  // 後優先（上書きモード）
            free_Buffer(&dirs[i]);
        }
    }

    //
    Buffer ast_path = dup_Buffer(pathOAR);
    #ifdef WIN32
        cat_s2Buffer("assets\\", &ast_path);
    #else
        cat_s2Buffer("assets/",  &ast_path);
    #endif
    assetsFiles = add_resource_list((char*)ast_path.buf, 36, assetsFiles, extn, 1);    // 前優先．36 is length of GUID
    free_Buffer(&ast_path);
    del_tList(&extn);

    //
    DEBUG_MODE {
        PRINT_MESG("GetDataInfo: RegionName = %s\n", (char*)regionName.buf);
        PRINT_MESG("GetDataInfo: OAR File Version = %d.%d\n", majorVersion, minorVersion);
        if (isMegaRegion) PRINT_MESG("GetDataInfo: Region is MegaRegion\n");
        PRINT_MESG("GetDataInfo: Region Size = (%d, %d)\n", xsize, ysize);
        print_tList(stderr, settingsFiles);
        print_tList(stderr, terrainsFiles);
        print_tList(stderr, landdataFiles);
        //print_tList(stderr, assetsFiles);
        //print_tList(stderr, objectsFiles);
/*
        PRINT_MESG("*** ASSETS LIST ***\n");
        tList* lst = assetsFiles;
        if (lst!=NULL && lst->ldat.id==TLIST_ANCHOR_NODE) lst = lst->next;
        while (lst!=NULL) {
            PRINT_MESG("KEY:%s  VAL:%s\n", (char*)lst->ldat.key.buf, (char*)lst->ldat.val.buf);
            lst = lst->next;
        }
*/
    }

    return true;
}


void  OARTool::MakeOutputFolder(void)
{
    mkdir((char*)pathOUT.buf, 0700);                                    // OUTPUT Folder

    // Texture, Material and Phantom Folder
    if (dataFormat==JBXL_3D_FORMAT_DAE  || dataFormat==JBXL_3D_FORMAT_OBJ || dataFormat==JBXL_3D_FORMAT_FBX || 
        dataFormat==JBXL_3D_FORMAT_GLTF || dataFormat==JBXL_3D_FORMAT_GLB) {
/*
        if (pathTEX.buf!=NULL) {
            mkdir((char*)pathTEX.buf, 0700);                            // Texture Folder
        }

        mkdir((char*)pathSLD.buf, 0700);                                // Solid Folder
        mkdir((char*)pathPTM.buf, 0700);                                // Phantom Folder
        mkdir((char*)pathTRR.buf, 0700);                                // Terrain Folder
        
        Buffer tex_path = dup_Buffer(pathSLD);
        cat_s2Buffer(OART_DEFAULT_TEX_DIR, &tex_path);
        mkdir((char*)tex_path.buf, 0700);                                // Solid/Texture Folder
        free_Buffer(&tex_path);
        tex_path = dup_Buffer(pathPTM);
        cat_s2Buffer(OART_DEFAULT_TEX_DIR, &tex_path);
        mkdir((char*)tex_path.buf, 0700);                                // Phantom/Texture Folder
        free_Buffer(&tex_path);
        tex_path = dup_Buffer(pathTRR);
        cat_s2Buffer(OART_DEFAULT_TEX_DIR, &tex_path);
        mkdir((char*)tex_path.buf, 0700);                                // Terrain/Texture Folder
        free_Buffer(&tex_path);
*/

        makeFolders((char*)pathSLD.buf, OART_DEFAULT_TEX_DIR);
        makeFolders((char*)pathPTM.buf, OART_DEFAULT_TEX_DIR);
        makeFolders((char*)pathTRR.buf, OART_DEFAULT_TEX_DIR);

        // OBJ
        if (dataFormat==JBXL_3D_FORMAT_OBJ) {
            makeFolders((char*)pathSLD.buf, OART_DEFAULT_MTL_DIR);
            makeFolders((char*)pathPTM.buf, OART_DEFAULT_MTL_DIR);
            makeFolders((char*)pathTRR.buf, OART_DEFAULT_MTL_DIR);
/*
            Buffer mtl = dup_Buffer(pathSLD);
            cat_s2Buffer(OART_DEFAULT_MTL_DIR, &mtl);
            mkdir((char*)mtl.buf, 0700);
            free_Buffer(&mtl);
            mtl = dup_Buffer(pathPTM);
            cat_s2Buffer(OART_DEFAULT_MTL_DIR, &mtl);
            mkdir((char*)mtl.buf, 0700);
            free_Buffer(&mtl);
*/
        }

        // GLTF
        if (dataFormat==JBXL_3D_FORMAT_GLTF) {
            makeFolders((char*)pathSLD.buf, OART_DEFAULT_BIN_DIR);
            makeFolders((char*)pathPTM.buf, OART_DEFAULT_BIN_DIR);
            makeFolders((char*)pathTRR.buf, OART_DEFAULT_BIN_DIR);
/*
            Buffer bin = dup_Buffer(pathSLD);
            cat_s2Buffer(OART_DEFAULT_BIN_DIR, &bin);
            mkdir((char*)bin.buf, 0700);                                // Solid/Binary Folder
            free_Buffer(&bin);
            bin = dup_Buffer(pathPTM);
            cat_s2Buffer(OART_DEFAULT_BIN_DIR, &bin);
            mkdir((char*)bin.buf, 0700);                                // Phantom/Binary Folder
            free_Buffer(&bin);
*/
        }

        /*
        //  Phantom Folder
        if (engine == JBXL_3D_ENGINE_UNITY) {
            if (pathPTM.buf!=NULL) {
                mkdir((char*)pathPTM.buf, 0700);                        // Phantom Folder
            }
            if (dataFormat==JBXL_3D_FORMAT_OBJ) {                       // Phantom/Material Folder
                Buffer mtl = dup_Buffer(pathPTM);
                cat_s2Buffer(OART_DEFAULT_MTL_DIR, &mtl);
                mkdir((char*)mtl.buf, 0700);
                free_Buffer(&mtl);
            }
            if (dataFormat==JBXL_3D_FORMAT_GLTF) {
                Buffer bin = dup_Buffer(pathPTM);                       // Phantom/Binary Folder
                cat_s2Buffer(OART_DEFAULT_BIN_DIR, &bin);
                mkdir((char*)bin.buf, 0700);
                free_Buffer(&bin);
            }
        }
        */
    }

    return;
}


void  OARTool::RmTextureFolder(void)
{
    if (dataFormat==JBXL_3D_FORMAT_GLB) {
        rmFolders((char*)pathSLD.buf, OART_DEFAULT_TEX_DIR);
        rmFolders((char*)pathPTM.buf, OART_DEFAULT_TEX_DIR);
        rmFolders((char*)pathTRR.buf, OART_DEFAULT_TEX_DIR);
    }
}


void  OARTool::ReadTerrainData(void)
{
    if (terrainNum==0) return;
    terrain = (TerrainTool*)malloc(terrainNum*sizeof(TerrainTool));
    if (terrain==NULL) return;
    memset(terrain, 0, terrainNum*sizeof(TerrainTool));

    int count  = 0;
    tList* lps = settingsFiles;
    tList* lpt = terrainsFiles;

    if (lps!=NULL) {
        while (count<terrainNum && lps!=NULL && lpt!=NULL) {
            terrain[count] = TerrainTool((char*)lps->ldat.key.buf, xsize, ysize);
            terrain[count].SetEngine(engine);
            terrain[count].SetDataFormat(dataFormat);
            terrain[count].SetTextureFormat(textureFormat);
            terrain[count].SetNoOffset(noOffset);
            terrain[count].ReadSettings((char*)lps->ldat.val.buf);
            terrain[count].ReadHeightData((char*)lpt->ldat.val.buf);
            lps = lps->next;
            lpt = lpt->next;
            count++;
        }
    }
    else {
        while (count<terrainNum && lpt!=NULL) {
            terrain[count] = TerrainTool((char*)lpt->ldat.key.buf, xsize, ysize);
            terrain[count].SetEngine(engine);
            terrain[count].SetDataFormat(dataFormat);
            terrain[count].SetTextureFormat(textureFormat);
            terrain[count].SetNoOffset(noOffset);
            terrain[count].ReadHeightData((char*)lpt->ldat.val.buf);
            lpt = lpt->next;
            count++;
        }
    }

    if (count>0) waterHeight = terrain[0].water_height();
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// for DAE/OBJ/glTF/STL

int  OARTool::GenerateTerrainDataFile(void)
{
    if (terrainNum==0) return 0;
    PRINT_MESG("GenerateTerrainSolid: converting terrain datafile file (%d)\n", dataFormat);

    Buffer tex_path = dup_Buffer(pathTRR);
    cat_s2Buffer(OART_DEFAULT_TEX_DIR, &tex_path);
    int num = 0;
    while (num<terrainNum) {
        int ret = terrain[num].GenerateTexture(assetsFiles, (char*)tex_path.buf);
        if (ret>0) {
            terrain[num].GenerateTerrain((char*)pathTRR.buf, terrainShift);
            num++;
#ifdef WIN32
            DisPatcher(); 
#endif
        }
        else break;
    }
    free_Buffer(&tex_path);
    return num;
}


/**
int  OARTool::GenerateObjectFromDataIndex(int startnum, int stopnum, bool useBrep, char* command)

オブジェクトファイルリスト this->objectFiles から指定された Objectデータを生成する．

*/
int  OARTool::GenerateObjectFromDataIndex(int startnum, int stopnum, bool useBrep, char* command)
{
    DEBUG_MODE PRINT_MESG("OARTool::GenerateObjectFromDataIndex: start.\n");

    tList* lp = this->objectsFiles;
    CVCounter* counter = GetUsableGlobalCounter();

    if (stopnum<0) stopnum = objectsNum;
    int num = 0;
    int cnt = 0;
    while (lp!=NULL) {
        num++;
        if (num>=startnum && num<=stopnum) {
            //
            char* file_path = (char*)lp->ldat.val.buf;
            void* solid = generateSolidData(dataFormat, file_path, num, useBrep, command);
            outputSolidData(dataFormat, get_file_name(file_path), solid);
            freeSolidData(dataFormat, solid);
            //
            if (counter!=NULL) {
                if (counter->cancel) break;
                counter->StepIt();
            }
            //
            del_tList(&AlphaChannelList);
            cnt++;
        }
        lp = lp->next;
    }
    if (counter!=NULL && counter->cancel) cnt = -1;

    return cnt;
}


/**
int  OARTool::GenerateObjectFromDataList(int* objlist, int objnum, bool useBrep, char* command)

オブジェクトファイルリスト this->objectFiles から，objlist（要素数 objnum）で指定されたオブジェクトを生成する．

$retval 生成したオブジェクト数
*/
int  OARTool::GenerateObjectFromDataList(int* objlist, int objnum, bool useBrep, char* command)
{
    DEBUG_MODE PRINT_MESG("OARTool::GenerateObjectFromDataList: start.\n");

    if (objnum<=0 || objlist==NULL) return 0;

    tList* lp = this->objectsFiles;
    CVCounter* counter = GetUsableGlobalCounter();

    int num = 0;
    int cnt = 0;
    while (lp!=NULL) {
        if (num==objlist[cnt]) {
            //
            char* file_path = (char*)lp->ldat.val.buf;
            void* solid = generateSolidData(dataFormat, file_path, num + 1, useBrep, command);
            outputSolidData(dataFormat, file_path, solid);
            freeSolidData(dataFormat, solid);
            //
            if (counter!=NULL) {
                if (counter->cancel) break;
                counter->StepIt();
            }
            //
            del_tList(&AlphaChannelList);
            cnt++;
            if (cnt==objnum) break;
        }
        num++;
        lp = lp->next;
    }
    if (counter!=NULL && counter->cancel) cnt = -1;

    return cnt;
}


/**
void  OARTool::GenerateObjectFromDataFile(char* file_path, bool useBrep, char* command)

指定されたファイルから Objectデータ１個を生成する．

*/
void  OARTool::GenerateObjectFromDataFile(char* file_path, bool useBrep, char* command)
{
    DEBUG_MODE PRINT_MESG("OARTool::GenerateObjectFromDataFile: start.\n");
    if (file_path==NULL) return;

    void* solid = generateSolidData(dataFormat, file_path, 1, useBrep, command);
    outputSolidData(dataFormat, get_file_name(file_path), solid);
    freeSolidData(dataFormat, solid);
    del_tList(&AlphaChannelList);

    return;
}


/**
void*  OARTool::generateSolidData(int format, const char* fname, int num, bool useBrep, char* command)

Tree, Grass, Prim(Sculpt, Meshを含む) の XMLデータ(オブジェクト１個分) から指定された形式で SOLIDデータを生成する．

@param format     生成データフォーマット
@param fname      オブジェクト名（xmlファイル名）
@param num        表示用の処理番号．何個目のオブジェクトか指定する．1 から数える．
@param useBrep    頂点の配置にBREPを使用するか？ 使用すると処理時間はかかるが，データサイズが小さくなる．
@param command    JPEG2000（テクスチャ）の内部処理が失敗した場合の外部コマンド．
@retval 生成されたデータへのポインタ．それぞれのデータ型でキャストして使用する．
*/
void*  OARTool::generateSolidData(int format, const char* fname, int num, bool useBrep, char* command)
{
    if (fname==NULL) return NULL;

#ifdef WIN32
    PRINT_MESG("[%d/%d] GenerateSolid: converting %s\n", num, objectsNum, fname);
#else
    PRINT_MESG("[%d/%d] GenerateSolid: converting %s (%'ldkB)\n", num, objectsNum, fname, memory_check());
#endif
    
    // read XML
    int shno = 0;
    PrimBaseShape* shapes;
    //
    tXML* sxml = xml_parse_file(fname);
    if (sxml!=NULL) {
        shapes = CreatePrimBaseShapesFromXML(sxml, assetsFiles, &shno); // Shapeデータ．shnoはデータの数
        del_all_xml(&sxml);
        if (shapes==NULL || shno<=0) {
            PRINT_MESG("OARTool::GenerateSolid: WARNING: not found shape data in %s (skip)\n", fname);
            return NULL;
        }
    }
    else {
        PRINT_MESG("OARTool::GenerateSolid: WARNING: XML File %s Read Error.(skip)\n", fname);
        return NULL;
    }

    int  collider_flag = OFF;
    bool collider = true;

    //
    ColladaXML*    dae  = NULL;
    OBJData*       obj  = NULL;
    FBXData*       fbx  = NULL;
    GLTFData*      gltf = NULL;
    BrepSolidList* stl  = NULL;

    // DAE
    if (format==JBXL_3D_FORMAT_DAE) {
        dae = new ColladaXML();
        dae->no_offset = noOffset;
        dae->forUnity5 = forUnity5;
        dae->forUnity  = forUnity;
        dae->setBlankTexture(PRIM_OS_BLANK_TEXTURE);
    }
    // OBJ
    else if (format==JBXL_3D_FORMAT_OBJ) {
        obj = new OBJData(); 
        obj->no_offset = noOffset;
        obj->setEngine(engine);
    }
    // GLTF
    else if (format==JBXL_3D_FORMAT_GLTF || format==JBXL_3D_FORMAT_GLB) {
        gltf = new GLTFData();
        gltf->no_offset = noOffset;
        gltf->setEngine(engine);
        gltf->glb_out = false;
        if (format==JBXL_3D_FORMAT_GLB) gltf->glb_out = true;
    }
    // FBX
    else if (format==JBXL_3D_FORMAT_FBX) {
        fbx = new FBXData();
        fbx->no_offset = noOffset;
        fbx->setEngine(engine);
    }
    // STL
    else if (format==JBXL_3D_FORMAT_STL_A || format==JBXL_3D_FORMAT_STL_B) {
        useBrep = false;
        command = NULL;
        stl = new BrepSolidList();
    }
    else {
        PRINT_MESG("OARTool::GenerateSolid: WARNING: unknown data format (%d)\n", format);
        return NULL;
    }

    int  count = 0;
    for (int s=0; s<shno; s++) {    // SHELL
        // Tree
        if (shapes[s].PCode==PRIM_PCODE_NEWTREE || shapes[s].PCode==PRIM_PCODE_TREE) {
            //
            //shapes[s].affineTrans.addShift(-xsize/2.0f + terrainShift.x, -ysize/2.0f + terrainShift.y, -waterHeight + terrainShift.z);
            shapes[s].affineTrans.addShift(terrainShift);
            shapes[s].affineTrans.computeMatrix();
            MeshObjectData* mesh = treeTool.GenerateTree(shapes[s], 0);
            //
            if (mesh!=NULL) {
                if (isRequiredTexture(format)) {    // STLの場合は不必要
                    MeshFacetNode* facet = mesh->facet;
                    char* image_type = GetTextureExtension();
                    Buffer tex_path = dup_Buffer(pathPTM);
                    cat_s2Buffer(OART_DEFAULT_TEX_DIR, &tex_path);
                    while (facet!=NULL) {
                        if (facet->material_param.enable) {
                            ConvertTexture(facet->material_param.getTextureName(), NULL, image_type, (char*)tex_path.buf, command);
                            facet->material_param.setFullName(image_type);
                        }
                        facet = facet->next;
                    }
                    free_Buffer(&tex_path);
                }

                // DAE
                if (format==JBXL_3D_FORMAT_DAE) {
                    dae->phantom_out = true;
                    dae->addShell(mesh, false);
                }
                // OBJ
                else if (format==JBXL_3D_FORMAT_OBJ) {
                    obj->phantom_out = true;
                    obj->addShell(mesh, false);
                }
                // GLTF
                else if (format==JBXL_3D_FORMAT_GLTF || format==JBXL_3D_FORMAT_GLB) {
                    gltf->phantom_out = true;
                    gltf->addShell(mesh, false);
                }
                // FBX
                else if (format==JBXL_3D_FORMAT_FBX) {
                    fbx->phantom_out = true;
                    fbx->addShell(mesh, false);
                }
                // STL
                else if (format==JBXL_3D_FORMAT_STL_A || format==JBXL_3D_FORMAT_STL_B) {
                    stl->addShell(mesh);
                }
                //
                freeMeshObjectData(mesh);
                count++;
            }
        }

        //
        // Grass
        else if (shapes[s].PCode==PRIM_PCODE_GRASS){ 
            //
            //shapes[s].affineTrans.addShift(-xsize/2.0f, -ysize/2.0f, -waterHeight);
            //shapes[s].affineTrans.addShift(terrainShift.x, terrainShift.y, terrainShift.z);
            shapes[s].affineTrans.computeMatrix();
            MeshObjectData* mesh = treeTool.GenerateGrass(shapes[s], terrain);  // 1個の Terrainのみサポート．範囲チェックあり
            if (mesh!=NULL && mesh->affineTrans!=NULL) {
                mesh->affineTrans->addShift(terrainShift);
                mesh->affineTrans->computeMatrix();
            }
            //
            if (mesh!=NULL) {
                if (isRequiredTexture(format)) {    // STLの場合は不必要
                    MeshFacetNode* facet = mesh->facet;
                    char* image_type = GetTextureExtension();
                    Buffer tex_path = dup_Buffer(pathPTM);
                    cat_s2Buffer(OART_DEFAULT_TEX_DIR, &tex_path);
                    while (facet!=NULL) {
                        if (facet->material_param.enable) {
                            ConvertTexture(facet->material_param.getTextureName(), NULL, image_type, (char*)tex_path.buf, command);
                            facet->material_param.setFullName(image_type);
                        }
                        facet = facet->next;
                    }
                    free_Buffer(&tex_path);
                }

                // DAE
                if (format==JBXL_3D_FORMAT_DAE) {
                    dae->phantom_out = true;
                    dae->addShell(mesh, false);
                }
                // OBJ
                else if (format==JBXL_3D_FORMAT_OBJ) {
                    obj->phantom_out = true;
                    obj->addShell(mesh, false);
                }
                // GLTF
                else if (format==JBXL_3D_FORMAT_GLTF || format==JBXL_3D_FORMAT_GLB) {
                    gltf->phantom_out = true;
                    gltf->addShell(mesh, false);
                }
                // FBX
                else if (format==JBXL_3D_FORMAT_FBX) {
                    fbx->phantom_out = true;
                    fbx->addShell(mesh, false);
                }
                // STL
                else if (format==JBXL_3D_FORMAT_STL_A || format==JBXL_3D_FORMAT_STL_B) {
                    stl->addShell(mesh);
                }
                //
                freeMeshObjectData(mesh);
                count++;
            }
        }

        //
        // Prim (Sculpt, Meshを含む)
        else if (shapes[s].PCode==PRIM_PCODE_PRIM) { 
            //
            SkinJointData*   skin_joint = NULL;
            SkinJointData**  ptr_skin_joint = &skin_joint;
            if (!procJoints) ptr_skin_joint = NULL;

            //shapes[s].affineTrans.addShift(-xsize/2.0f + terrainShift.x, -ysize/2.0f + terrainShift.y, -waterHeight + terrainShift.z);
            shapes[s].affineTrans.addShift(terrainShift);
            shapes[s].affineTrans.computeMatrix();
            MeshObjectData* mesh = MeshObjectDataFromPrimShape(shapes[s], assetsFiles, useBrep, ptr_skin_joint);
            //
            if (mesh!=NULL) {
                if (collider_flag==OFF) {
                    collider_flag = ON;
                    if (strstr((const char*)shapes[s].ObjFlags.buf, OART_FLAGS_PHANTOM)!=NULL) {    // Phantom
                        collider = false;
                    }
                }
                //
                if (isRequiredTexture(format)) {    // STLの場合は不必要
                    MeshFacetNode* facet = mesh->facet;
                    char* image_type = GetTextureExtension();
                    Buffer tex_path;
                    if (collider) tex_path = dup_Buffer(pathSLD);
                    else          tex_path = dup_Buffer(pathPTM);
                    cat_s2Buffer(OART_DEFAULT_TEX_DIR, &tex_path);
                    while (facet!=NULL) {
                        if (facet->material_param.enable) {
                            // MeshObjectDataFromPrimShape へ移動
                            //facet->material_param.setAlphaChannel(CheckAlphaChannel(facet->material_param.getTextureName()));
                            //if (facet->material_param.isSetAlpha()) facet->material_param.setTransparent(MTRL_DEFAULT_ALPHA);
                            //if (unity3D) {
                            //  char* paramstr = GetBase64ParamsString(facet->material_param, 'O');    // O: Object
                            //  if (paramsstr!=NULL) {
                            //      facet->material_param.setParamString(paramstr);
                            //      ::free(paramstr);
                            //  }
                            //}
                            int ret = ConvertTexture(facet->material_param.getTextureName(), NULL, image_type, (char*)tex_path.buf, command);
                            if (ret<=0 && ret!=OART_TEXCNVT_WRITE_ERR) {
                                // 代替テクスチャ
                                facet->material_param.setTextureName(OART_TEXCNVT_ALT_TEXTURE);
                                ret = ConvertTexture(facet->material_param.getTextureName(), NULL, image_type, (char*)tex_path.buf, command);
                                if (ret==OART_TEXCNVT_NORMAL) {
                                    PRINT_MESG("OARTool::generateSolidData: tetxure was changed to %s\n", facet->material_param.getTextureName());
                                }
                            }
                            ConvertTexture(facet->material_param.getBumpMapName(), NULL, image_type, (char*)tex_path.buf, command);
                            //
                            facet->material_param.setFullName(image_type);
                        }
                        facet = facet->next;
                    }
                    free_Buffer(&tex_path);
                }

                tXML* joints_template = NULL;
                // DAE
                if (format==JBXL_3D_FORMAT_DAE) {
                    if (collider) dae->phantom_out = false;
                    if (this->procJoints) {
                        char* path = get_resource_path(OART_JOINT_TEMPLATE_FILE, assetsFiles);
                        if (path!=NULL) joints_template = xml_parse_file(path);     // not free
                        else PRINT_MESG("OARTool::generateSolidData: WARNING: Joints template xml file is not found!\n");
                    }
                    dae->addShell(mesh, collider, skin_joint, joints_template);
                    joints_template = NULL;         // joints_tempalte は出力用のデータに組み込まれ，最後に dae 内で freeされる
                }
                // OBJ
                else if (format==JBXL_3D_FORMAT_OBJ) {
                    if (collider) obj->phantom_out = false;
                    obj->addShell(mesh, collider);
                }
                // GLTF or GLB
                else if (format==JBXL_3D_FORMAT_GLTF || format==JBXL_3D_FORMAT_GLB) {
                    if (collider) gltf->phantom_out = false;
                    if (this->procJoints) {
                        char* path = get_resource_path(OART_JOINT_TEMPLATE_FILE, assetsFiles);
                        if (path!=NULL) joints_template = xml_parse_file(path);     // not free
                        else PRINT_MESG("OARTool::generateSolidData: WARNING: Joints template xml file is not found!\n");
                    }
                    tList* jl = selectJointsFromXMLTemplate(skin_joint, joints_template);
                    gltf->addShell(mesh, collider, skin_joint, jl);
                }
                // FBX
                else if (format==JBXL_3D_FORMAT_FBX) {
                    if (collider) fbx->phantom_out = false;
                    if (this->procJoints) {
                        char* path = get_resource_path(OART_JOINT_TEMPLATE_FILE, assetsFiles);
                        if (path!=NULL) joints_template = xml_parse_file(path);     // not free
                        else PRINT_MESG("OARTool::generateSolidData: WARNING: Joints template xml file is not found!\n");
                    }
                    tTree* jl = selectJointsFromXMLTemplate(skin_joint, joints_template);
                    fbx->addShell(mesh, collider, skin_joint, jl);
                }
                // STL
                else if (format==JBXL_3D_FORMAT_STL_A || format==JBXL_3D_FORMAT_STL_B) {
                    stl->addShell(mesh);
                }
                //
                freeMeshObjectData(mesh);
                if (joints_template!=NULL) del_all_xml(&joints_template);
                count++;
            }
            if (skin_joint!=NULL) skin_joint->free();
        }
    }

    for (int s=0; s<shno; s++) shapes[s].free();
    ::free(shapes);

    // Object の終了処理
    if (count>0) {
        // DAE
        if (format==JBXL_3D_FORMAT_DAE) {
            if (count==1 && forUnity4) dae->addCenterObject();      // for Unity4.x
            dae->closeSolid();
            return (void*)dae;
        }
        //  OBJ
        else if (format==JBXL_3D_FORMAT_OBJ) {
            Vector<double> offset = obj->execAffineTrans();         // no_offset==true の場合，原点縮退
            if (obj->affineTrans==NULL) obj->affineTrans = new AffineTrans<double>();
            obj->affineTrans->setShift(offset);
            obj->affineTrans->computeMatrix();
            obj->closeSolid();
            return (void*)obj;
        }
        //  GLTF or GLB
        else if (format==JBXL_3D_FORMAT_GLTF || format==JBXL_3D_FORMAT_GLB) {
            gltf->closeSolid();
            return (void*)gltf;
        }
        //  FBX
        else if (format==JBXL_3D_FORMAT_FBX) {
            Vector<double> offset = fbx->execAffineTrans();         // no_offset==true の場合，原点縮退
            if (fbx->affineTrans==NULL) fbx->affineTrans = new AffineTrans<double>();
            fbx->affineTrans->setShift(offset);
            fbx->closeSolid();
            return (void*)fbx;
        }
        // STL
        else if (format==JBXL_3D_FORMAT_STL_A || format==JBXL_3D_FORMAT_STL_B) {
            stl->closeSolid();
            return (void*)stl;
        }
    }
    return NULL;
}


/**
void  OARTool::outputSolidData(int format, const char* fname, void* solid)

それぞれの形式の SOLIDデータからファイルを出力する．
出力先は 大域変数 pathOUT, pathPTM で指定されたディレクトリ．

@param format   出力用データフォーマット
@param fname    出力ファイル名
@param solid    出力データ
@param affine   アフィン変換 (solid->affineTrans)
*/
void  OARTool::outputSolidData(int format, const char* fname, void* solid)
{
    DEBUG_MODE {
        PRINT_MESG("OARTool::outputSolidData: start.\n");
        if (solid==NULL) PRINT_MESG("OARTool::outputSolidData: solid data is NULL. return.\n");
    }
    if (solid==NULL || fname==NULL) return;
    //
    fname = get_file_name(fname);
    Buffer out_fname = make_Buffer_str(fname);
    Buffer out_path  = init_Buffer();

    //
    // fnameの拡張子は自動的に変換される
    // DAE
    if (format==JBXL_3D_FORMAT_DAE) {
        ColladaXML* dae = (ColladaXML*)solid;
        //
        if (noOffset && dae->affineTrans!=NULL) setDegenerateFname(&out_fname, engine, dae->affineTrans->getShift(), OART_LOCATION_MAGIC_STR);
        if (dae->phantom_out) out_path = dup_Buffer(pathPTM);
        else                  out_path = dup_Buffer(pathSLD);
        //
        dae->outputFile((char*)out_fname.buf, (char*)out_path.buf, XML_SPACE_FORMAT);
    }

    // OBJ
    else if (format==JBXL_3D_FORMAT_OBJ) {
        OBJData* obj = (OBJData*)solid;

        if (noOffset && obj->affineTrans!=NULL) setDegenerateFname(&out_fname, engine, obj->affineTrans->getShift(), OART_LOCATION_MAGIC_STR);
        if (obj->phantom_out) out_path = dup_Buffer(pathPTM);
        else                  out_path = dup_Buffer(pathSLD);
        //
        obj->outputFile((char*)out_fname.buf, (char*)out_path.buf, OART_DEFAULT_TEX_DIR, OART_DEFAULT_MTL_DIR);
        /*
        if (obj->engine==JBXL_3D_ENGINE_UE) {
            //if (obj->phantom_out) ins_s2Buffer(OART_UE_PHANTOM_PREFIX,  &out_fname);
            //else                  ins_s2Buffer(OART_UE_COLLIDER_PREFIX, &out_fname);
            obj->outputFile((char*)out_fname.buf, (char*)pathOUT.buf, NULL, OART_DEFAULT_TEX_DIR, OART_DEFAULT_MTL_DIR);
        }
        else {
            obj->outputFile((char*)out_fname.buf, (char*)pathOUT.buf, OART_DEFAULT_PTM_DIR, OART_DEFAULT_TEX_DIR, OART_DEFAULT_MTL_DIR);
        }
        */
    }

    // GLTF or GLB
    else if (format==JBXL_3D_FORMAT_GLTF || format==JBXL_3D_FORMAT_GLB) {
        GLTFData* gltf = (GLTFData*)solid;
        //
        if (noOffset) setDegenerateFname(&out_fname, engine, gltf->center, OART_LOCATION_MAGIC_STR);
        if (gltf->phantom_out) out_path = dup_Buffer(pathPTM);
        else                   out_path = dup_Buffer(pathSLD);
        //
        gltf->outputFile((char*)out_fname.buf, (char*)out_path.buf, OART_DEFAULT_TEX_DIR, OART_DEFAULT_BIN_DIR);


/*
        if (gltf->engine==JBXL_3D_ENGINE_UE) {
            //if (gltf->phantom_out) ins_s2Buffer(OART_UE_PHANTOM_PREFIX,  &out_fname);
            //else                   ins_s2Buffer(OART_UE_COLLIDER_PREFIX, &out_fname);
            gltf->outputFile((char*)out_fname.buf, (char*)pathOUT.buf, NULL, OART_DEFAULT_TEX_DIR, OART_DEFAULT_BIN_DIR);
        }
        else {
            gltf->outputFile((char*)out_fname.buf, (char*)pathOUT.buf, OART_DEFAULT_PTM_DIR, OART_DEFAULT_TEX_DIR, OART_DEFAULT_BIN_DIR);
        }
*/
    }

    // FBX
    else if (format==JBXL_3D_FORMAT_FBX) {
        FBXData* fbx = (FBXData*)solid;
        //
        if (noOffset && fbx->affineTrans!=NULL) setDegenerateFname(&out_fname, engine, fbx->affineTrans->getShift(), OART_LOCATION_MAGIC_STR);
        if (fbx->phantom_out) out_path = dup_Buffer(pathPTM);
        else                  out_path = dup_Buffer(pathSLD);
        //
        fbx->outputFile((char*)out_fname.buf, (char*)pathOUT.buf, OART_DEFAULT_TEX_DIR, OART_DEFAULT_BIN_DIR);

/*
        if (fbx->engine==JBXL_3D_ENGINE_UE) {
            //if (fbx->phantom_out) ins_s2Buffer(OART_UE_PHANTOM_PREFIX,  &out_fname);
            //else                  ins_s2Buffer(OART_UE_COLLIDER_PREFIX, &out_fname);
            fbx->outputFile((char*)out_fname.buf, (char*)pathOUT.buf, NULL, OART_DEFAULT_TEX_DIR, OART_DEFAULT_BIN_DIR);
        }
        else {
            fbx->outputFile((char*)out_fname.buf, (char*)pathOUT.buf, OART_DEFAULT_PTM_DIR, OART_DEFAULT_TEX_DIR, OART_DEFAULT_BIN_DIR);
        }
*/
    }

    // STL
    else if (format==JBXL_3D_FORMAT_STL_A || format==JBXL_3D_FORMAT_STL_B) {
        BrepSolidList* stl = (BrepSolidList*)solid;
        bool ascii = true;
        if (format==JBXL_3D_FORMAT_STL_B) ascii = false;
        out_path = dup_Buffer(pathOUT);
        stl->outputFile((char*)out_fname.buf, (char*)out_path.buf, ascii);
    }

    free_Buffer(&out_path);
    free_Buffer(&out_fname);
    return;
}


void  OARTool::freeSolidData(int format, void* solid)
{
    if (solid==NULL) return;

    // DAE
    if (format==JBXL_3D_FORMAT_DAE) {
        ColladaXML* dae = (ColladaXML*)solid;
        freeColladaXML(dae);
    }
    // OBJ
    else if (format==JBXL_3D_FORMAT_OBJ) {
        OBJData* obj = (OBJData*)solid;
        freeOBJData(obj);
    }
    // GLTF
    else if (format==JBXL_3D_FORMAT_GLTF || format==JBXL_3D_FORMAT_GLB) {
        GLTFData* gltf = (GLTFData*)solid;
        freeGLTFData(gltf);
    }
    // FBX
    else if (format==JBXL_3D_FORMAT_FBX) {
        FBXData* fbx = (FBXData*)solid;
        freeFBXData(fbx);
    }
    // STL
    else if (format==JBXL_3D_FORMAT_STL_A || format==JBXL_3D_FORMAT_STL_B) {
        BrepSolidList* stl = (BrepSolidList*)solid;
        freeBrepSolidList(stl);
    }
    return;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 

/**
PrimBaseShape  OARTool::getAbstractObject(const char* fname)

ルートプリムの情報を得る．
*/
PrimBaseShape  OARTool::getAbstractObject(const char* fname)
{
    PrimBaseShape rparam;
    rparam.init();
    if (fname==NULL) return rparam;

    tXML* xml = xml_parse_file(fname);
    if (xml==NULL) return rparam;

    tList* lp = get_xml_node_list_bystr(xml, "<SceneObjectPart><Shape>");
    if (lp!=NULL) {
        int count = count_tList(lp);
        if (count!=0) rparam.GetBaseParamFromXML(lp->altp, NULL);
    }
    del_xml(&xml);

    return rparam;
}


/**
int  OARTool::ConvertTexture(const char* texture, const char* add_name, const char* ext_name, const char* dist, const char* comformat)

@param texture    コンバート元データのUUID．
@param add_name   コンバート先ファイル名の追加文字列．
@param ext_name   コンバート先ファイル名の拡張子． "." 付き
@param dist       コンバート先のパス．
@param comformat  内部処理が失敗した場合の外部処理コマンド．NULL の場合はデフォルトを使用する．
*/
int  OARTool::ConvertTexture(const char* texture, const char* add_name, const char* ext_name, const char* dist, const char* comformat)
{
    if (texture==NULL) return OART_TEXCNVT_UNKNOWN;

    Buffer outpath;
    if (dist==NULL) {
        outpath = make_Buffer_bystr((char*)pathSLD.buf);
    }
    else {
        outpath = make_Buffer_bystr(dist);
    }

    int ret = OART_TEXCNVT_UNKNOWN;
    bool converted = false;

    cat_s2Buffer(texture, &outpath);
    /**/
    if (add_name!=NULL) {
        cat_s2Buffer("_", &outpath);
        cat_s2Buffer(add_name, &outpath);
    }
    /**/
    if (ext_name!=NULL) {
        if (ext_name[0]!='.') cat_s2Buffer(".", &outpath);
        cat_s2Buffer(ext_name, &outpath);
    }
    #ifndef WIN32
        rewrite_sBuffer_str(&outpath, " ", "\\ ");
        rewrite_sBuffer_str(&outpath, ";", "\\;");
    #endif

    // 下記 if文をコメントアウトすると上書き可（ただし凄く遅い）
    if (file_exist((char*)outpath.buf)) {
        ret = OART_TEXCNVT_NORMAL;
    }
    else {
        //
        char* path = get_resource_path((char*)texture, assetsFiles);
        char* extn = get_file_extension(path);
        int filesz = file_size(path);

        if (filesz>0 && extn!=NULL) {
            char* check_ext = (char*)ext_name;
            if (check_ext[0]=='.') check_ext = (char*)ext_name + 1;

            // 同じ形式のファイル -> コピー
            if (!strcasecmp(extn, check_ext)) {
                int flsz = file_size(path);
                int cpsz = copy_file(path, (char*)outpath.buf);
                if (flsz==cpsz) {
                    DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: copy from %s to %s\n", path, (char*)outpath.buf);
                    ret = OART_TEXCNVT_NORMAL;
                }
                else {
                    DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: ERROR: copy from %s to %s failed!\n", path, (char*)outpath.buf);
                    ret = OART_TEXCNVT_COPY_ERR;
                }
            }
            // JPEG2000
            else if (extn[0]=='j' || extn[0]=='J') {
                JPEG2KImage jpg = readJPEG2KFile(path);
                // Repair 効果は無いかも
                if (jpg.state!=0) {
                    PRINT_MESG("OARTool::ConvertTexture: failure to read %s. attempt to repair the file.\n", path);
                    Buffer repair_file = make_Buffer_bystr(texture);
                    cat_s2Buffer(OART_JP2_REPAIR_EXTN, &repair_file);
                    //
                    int err = repairJPEG2KFile(path, (char*)repair_file.buf);
                    if (err==JBXL_NORMAL) {
                        jpg = readJPEG2KFile((char*)repair_file.buf);
                        unlink((char*)repair_file.buf);
                        ret = OART_TEXCNVT_NORMAL;
                    }
                    else {
                        DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: ERROR: failure to repair the file.\n", err);
                        ret = OART_TEXCNVT_DSTRY_ERR;
                    }
                    free_Buffer(&repair_file);
                }
                //
                if (jpg.state==0) {
                    MSGraph<uByte> vp = JPEG2KImage2MSGraph<uByte>(jpg);
                    DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: texture = %s [size = (%4d,%4d,%2d), mode = %d]\n", texture, jpg.ws, jpg.hs, jpg.col, jpg.cmode);
                    //
                    if (vp.zs>0) {
                        if (textureFormat==JBXL_TEXTURE_TGA) {
                            //TGAImage tga = MSGraph2TGAImage(vp, true);
                            TGAImage tga = MSGraph2TGAImage(vp, false);
                            int err = writeTGAFile((char*)outpath.buf, &tga);
                            if (!err) {
                                converted = true;
                                ret = OART_TEXCNVT_NORMAL;
                            }
                            else  {
                                DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: ERROR: writeTGAFile error (%d) [%s].\n", err, (char*)outpath.buf);
                                ret = OART_TEXCNVT_WRITE_ERR;
                            }
                            tga.free();
                        }
                        else {
                            PNGImage png = MSGraph2PNGImage(vp);
                            int err = writePNGFile((char*)outpath.buf, &png);
                            if (!err) {
                                converted = true;
                                ret = OART_TEXCNVT_NORMAL;
                            }
                            else {
                                PRINT_MESG("OARTool::ConvertTexture: ERROR: writePNGFile error (%d) [%s].\n", err, (char*)outpath.buf);
                                ret = OART_TEXCNVT_WRITE_ERR;
                            }
                            png.free();
                        }
                    }
                    else {
                        DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: ERROR: color num of %s is %d [%s].\n", texture, vp.zs, (char*)outpath.buf);
                        ret = OART_TEXCNVT_INVD_ERR;
                    }
                    vp.free();
                    jpg.free();
                }
                else {
                    if (jpg.state==JBXL_GRAPH_IVDDATA_ERROR) {
                        DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: ERROR: texture %s is invalid [%s].\n", texture, (char*)outpath.buf);
                        ret = OART_TEXCNVT_INVD_ERR;
                    }
                    else {
                        DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: ERROR: texture %s convert error (%d) [%s].\n", texture, jpg.state, (char*)outpath.buf);
                        ret = OART_TEXCNVT_CNVT_ERR;
                    }
                }
                //
                // Retry convert using external command
                if (!converted) {
                    PRINT_MESG("OARTool::ConvertTexture: RETRY: convert %s to %s\n", path, (char*)outpath.buf); 
                    //
                    char command[LMESG];
                    memset(command, 0, LMESG);
                    if (comformat!=NULL) {
                        snprintf(command, LMESG-1, comformat, path, (char*)outpath.buf);
                    }
                    else {
                        snprintf(command, LMESG-1, OART_JP2_DECOMP_COM, path, (char*)outpath.buf);
                    }
                    //
                    int err = 0;
                    #ifdef WIN32
                        DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: convert command: %s\n", command);
                        STARTUPINFOA sinfo;             // コマンドの実行
                        PROCESS_INFORMATION pinfo;
                        memset(&sinfo, 0, sizeof(STARTUPINFO));
                        sinfo.cb = sizeof(STARTUPINFO); 
                        sinfo.wShowWindow = SW_HIDE;
                        CreateProcessA(NULL, (LPSTR)command, NULL, NULL, FALSE, CREATE_NO_WINDOW ,NULL, NULL, &sinfo, &pinfo);
                        CloseHandle(pinfo.hThread);
                        DWORD ret = WaitForSingleObject(pinfo.hProcess, INFINITE);
                        CloseHandle(pinfo.hProcess);
                        if (ret!=WAIT_OBJECT_0) err = 1;
                    #else
                        DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: convert command: %s\n", command);
                        int rslt = system(command);
                        err = WEXITSTATUS(rslt);
                    #endif
                    if (err!=0) {
                        PRINT_MESG("OARTool::ConvertTexture: ERROR: texture %s convert error (%d).\n", texture, err);
                        ret = OART_TEXCNVT_CNVT_ERR;
                    }
                    else {
                        if (file_exist((char*)outpath.buf)) {
                            PRINT_MESG("OARTool::ConvertTexture: SUCCESS: texture %s is converted.\n", texture);
                            ret = OART_TEXCNVT_NORMAL;
                        }
                        else {
                            PRINT_MESG("OARTool::ConvertTexture: ERROR: texture %s convert error.\n", texture);
                            ret = OART_TEXCNVT_CNVT_ERR;
                        }
                    }
                }
            }
            // unknown file extension
            else {
                PRINT_MESG("OARTool::ConvertTexture: ERROR: unsupported file %s found!", path);
                ret = OART_TEXCNVT_INVD_ERR;
            }
        }
        // Lost
        else {
            if (filesz==0) {
                PRINT_MESG("OARTool::ConvertTexture: ERROR: texture %s size is zero!\n", texture);
                ret = OART_TEXCNVT_ZERO_ERR;
            }
            else {
                PRINT_MESG("OARTool::ConvertTexture: ERROR: texture %s is lost!\n", texture);
                ret = OART_TEXCNVT_LOST_ERR;
            }
        }
    }
    free_Buffer(&outpath);

    return ret;
}

