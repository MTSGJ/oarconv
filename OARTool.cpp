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
    pathTEX = init_Buffer();    // Texture directory
    pathPTM = init_Buffer();    // Fhantom directory
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
    dataformat      = JBXL_3D_FORMAT_DAE;
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
    free_Buffer(&pathTEX);
    free_Buffer(&pathPTM);
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
pathTEX : テクスチャ出力用のパス
pathPTM : ファントムデータ出力用のパス
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
        if (dataformat==JBXL_3D_FORMAT_DAE) {
            pathOUT = make_Buffer_bystr(OART_DEFAULT_DAE_DIR);
        }
        // OBJ
        else if (dataformat==JBXL_3D_FORMAT_OBJ) {
            pathOUT = make_Buffer_bystr(OART_DEFAULT_OBJ_DIR);
        }
        // FBX
        else if (dataformat==JBXL_3D_FORMAT_FBX) {
            pathOUT = make_Buffer_bystr(OART_DEFAULT_FBX_DIR);
        }
        // STL
        else { 
            pathOUT = make_Buffer_bystr(OART_DEFAULT_STL_DIR);
        }
    }

    if (noOffset) {
        if (pathOUT.buf[pathOUT.vldsz-1]=='/' || pathOUT.buf[pathOUT.vldsz-1]=='\\') {
            pathOUT.buf[pathOUT.vldsz-1]='\0';
            pathOUT.vldsz--;
        }
        cat_s2Buffer(OART_DEFAULT_NOS_DIR, &pathOUT);
    }
    #ifdef WIN32
        if (pathOUT.buf[pathOUT.vldsz-1]!='\\') cat_s2Buffer("\\", &pathOUT);
    #else
        if (pathOUT.buf[pathOUT.vldsz-1]!='/')  cat_s2Buffer("/",  &pathOUT);
    #endif

    // Set Phantom Path (pathPTM)
    pathTEX = make_Buffer_bystr((char*)pathOUT.buf);
    cat_s2Buffer(OART_DEFAULT_TEX_DIR, &pathTEX);
    if (dataformat!=JBXL_3D_FORMAT_OBJ || engine!=JBXL_3D_ENGINE_UE) {
        pathPTM = make_Buffer_bystr((char*)pathOUT.buf);
        cat_s2Buffer(OART_DEFAULT_PTM_DIR, &pathPTM);
    }

    // ASSET
    if (astdir==NULL) {
        pathAST = make_Buffer_bystr(OART_DEFAULT_AST_DIR);
    }
    else {
        pathAST = make_Buffer_bystr(astdir);
        #ifdef WIN32
            if (pathAST.buf[strlen((char*)pathAST.buf)-1]!='\\') cat_s2Buffer("\\", &pathAST);
        #else
            if (pathAST.buf[strlen((char*)pathAST.buf)-1]!='/') cat_s2Buffer("/", &pathAST);
        #endif
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
        pathOAR = make_Buffer_bystr(oardir);
#ifdef WIN32
        if (pathOAR.buf[strlen((char*)pathOAR.buf) - 1] != '\\') cat_s2Buffer("\\", &pathOAR);
#else
        if (pathOAR.buf[strlen((char*)pathOAR.buf) - 1] != '/') cat_s2Buffer("/", &pathOAR);
#endif
    }

    // OUTPUT
    if (outdir != NULL) {
        pathOUT = make_Buffer_bystr(outdir);
#ifdef WIN32
        if (pathOUT.buf[strlen((char*)pathOUT.buf) - 1] != '\\') cat_s2Buffer("\\", &pathOUT);
#else
        if (pathOUT.buf[strlen((char*)pathOUT.buf) - 1] != '/') cat_s2Buffer("/", &pathOUT);
#endif
        pathTEX = make_Buffer_bystr((char*)pathOUT.buf);
        pathPTM = make_Buffer_bystr((char*)pathOUT.buf);
        cat_s2Buffer(OART_DEFAULT_TEX_DIR, &pathTEX);
        cat_s2Buffer(OART_DEFAULT_PTM_DIR, &pathPTM);
    }

    // ASSET
    if (astdir != NULL) {
        pathAST = make_Buffer_bystr(astdir);
#ifdef WIN32
        if (pathAST.buf[strlen((char*)pathAST.buf) - 1] != '\\') cat_s2Buffer("\\", &pathAST);
#else
        if (pathAST.buf[strlen((char*)pathAST.buf) - 1] != '/') cat_s2Buffer("/", &pathAST);
#endif
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
    PRINT_MESG("OARTool::GetDatanInfo: read OAR files from %s\n", pathOAR.buf);
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
        PRINT_MESG("OARTool::GetDatanInfo: ERROR: not found archive file! [%sarchive.xml]\n", pathOAR.buf);
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
        // Windowsの場合は : で区切れない
        if (pathAST.buf[strlen((char*)pathAST.buf)-1]!='\\') cat_s2Buffer("\\", &pathAST);
        assetsFiles = add_resource_list((char*)pathAST.buf, 36, assetsFiles, extn);
    #else
        Buffer* dirs = awk_Buffer_dim(pathAST, ':');
        if (dirs!=NULL) {
            int num = dirs->state;
            for (int i=0; i<num; i++) {
                if (dirs[i].buf[strlen((char*)dirs[i].buf)-1]!='/') cat_s2Buffer("/", &dirs[i]);
                assetsFiles = add_resource_list((char*)dirs[i].buf, 36, assetsFiles, extn);
                free_Buffer(&dirs[i]);
            }
            ::free(dirs);
        }
    #endif

    //
    Buffer ast_path = dup_Buffer(pathOAR);
    #ifdef WIN32
        cat_s2Buffer("assets\\", &ast_path);
    #else
        cat_s2Buffer("assets/",  &ast_path);
    #endif
    assetsFiles = add_resource_list((char*)ast_path.buf, 36, assetsFiles, extn);    // 36 is length of GUID
    free_Buffer(&ast_path);
    del_tList(&extn);

    //
    DEBUG_MODE {
        PRINT_MESG("GetDataInfo: RegionName = %s\n", (char*)regionName.buf);
        PRINT_MESG("GetDataInfo: OAR File Version = %d.%d\n", majorVersion, minorVersion);
        if (isMegaRegion) PRINT_MESG("GetDataInfo: Region is MegaRegion\n");
        PRINT_MESG("GetDataInfo: Region Size = (%d, %d)\n", xsize, ysize);
        //print_tList(stderr, settingsFiles);
        //print_tList(stderr, terrainsFiles);
        //print_tList(stderr, landdataFiles);
        //print_tList(stderr, assetsFiles);
        //print_tList(stderr, objectsFiles);
    }

    return true;
}


void  OARTool::MakeOutputFolder(void)
{
    if (pathOUT.buf!=NULL) mkdir((char*)pathOUT.buf, 0700);             // OUTPUT Folder

    // Texture, Material and Phantom Folder
    if (dataformat==JBXL_3D_FORMAT_DAE || dataformat==JBXL_3D_FORMAT_OBJ) {
        if (pathTEX.buf!=NULL) mkdir((char*)pathTEX.buf, 0700);         // Texture Folder
        if (dataformat==JBXL_3D_FORMAT_OBJ) {                           // Material Folder
            Buffer mtl = dup_Buffer(pathOUT);
            cat_s2Buffer(OART_DEFAULT_MTL_DIR, &mtl);
            mkdir((char*)mtl.buf, 0700);
            free_Buffer(&mtl);
        }
        //  Phantom Folder
        if (engine != JBXL_3D_ENGINE_UE) {
            if (pathPTM.buf!=NULL) mkdir((char*)pathPTM.buf, 0700);     // Phantom Folder
            if (dataformat==JBXL_3D_FORMAT_OBJ) {                       // Phantom/Material Folder
                //Buffer mtl = dup_Buffer(pathOUT);
                Buffer mtl = dup_Buffer(pathPTM);
                cat_s2Buffer(OART_DEFAULT_MTL_DIR, &mtl);
                mkdir((char*)mtl.buf, 0700);
                free_Buffer(&mtl);
            }
        }
    }
    return;
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
            terrain[count].SetFormat(dataformat);
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
            terrain[count].SetFormat(dataformat);
            terrain[count].SetNoOffset(noOffset);
            terrain[count].ReadHeightData((char*)lpt->ldat.val.buf);
            lpt = lpt->next;
            count++;
        }
    }

    if (count>0) waterHeight = terrain[0].water_height();
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// for DAE/OBJ/STL

int  OARTool::GenerateTerrainDataFile(void)
{
    if (terrainNum==0) return 0;
    PRINT_MESG("GenerateTerrainSolid: generating terrain datafile file (%d)\n", dataformat);
    int num = 0;
    while (num<terrainNum) {
        terrain[num].GenerateTexture(assetsFiles, (char*)pathTEX.buf);
        terrain[num].GenerateTerrain((char*)pathOUT.buf, terrainShift);
        num++;
#ifdef WIN32
        DisPatcher(); 
#endif
    }
    return num;
}


/**
int  OARTool::GenerateObjectFromDataIndex(int startnum, int stopnum, bool useBrep, char* command)

オブジェクトファイルリスト this->objectFiles から指定された Objectデータを生成する．

*/
int  OARTool::GenerateObjectFromDataIndex(int startnum, int stopnum, bool useBrep, char* command)
{
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
            void* solid = generateSolidData(dataformat, file_path, num, useBrep, command);
            outputSolidData(dataformat, get_file_name(file_path), solid);
            freeSolidData(dataformat, solid);
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
    if (objnum<=0 || objlist==NULL) return 0;

    tList* lp = this->objectsFiles;
    CVCounter* counter = GetUsableGlobalCounter();

    int num = 0;
    int cnt = 0;
    while (lp!=NULL) {
        if (num==objlist[cnt]) {
            //
            char* file_path = (char*)lp->ldat.val.buf;
            void* solid = generateSolidData(dataformat, file_path, num + 1, useBrep, command);
            outputSolidData(dataformat, file_path, solid);
            freeSolidData(dataformat, solid);
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
    if (file_path==NULL) return;

    void* solid = generateSolidData(dataformat, file_path, 1, useBrep, command);
    outputSolidData(dataformat, get_file_name(file_path), solid);
    freeSolidData(dataformat, solid);
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
    PRINT_MESG("[%d/%d] GenerateSolid: generating %s\n", num, objectsNum, fname);
#else
    PRINT_MESG("[%d/%d] GenerateSolid: generating %s (%'ldkB)\n", num, objectsNum, fname, memory_check());
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

    //
    ColladaXML*    dae = NULL;
    OBJData*       obj = NULL;
    BrepSolidList* stl = NULL;

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
    // FBX
    else if (format==JBXL_3D_FORMAT_FBX) {
        obj = NULL;
        obj->no_offset = noOffset;
        //obj->setEngine(engine);
    }
    // STL
    else if (format==JBXL_3D_FORMAT_STL_A || format==JBXL_3D_FORMAT_STL_B) {
        useBrep = true;
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
            shapes[s].affineTrans.addShift(-xsize/2.0f + terrainShift.x, -ysize/2.0f + terrainShift.y, -waterHeight + terrainShift.z);
            MeshObjectData* mesh = treeTool.GenerateTree(shapes[s], 0);
            //
            if (mesh!=NULL) {
                if (isRequiredTexture(format)) {    // STLの場合は不必要
                    MeshFacetNode* facet = mesh->facet;
                    while (facet!=NULL) {
                        if (facet->material_param.enable) {
                            ConvertTexture(facet->material_param.getTextureName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                            facet->material_param.setFullName(MTRL_IMAGE_TYPE);
                        }
                        facet = facet->next;
                    }
                }
                // DAE
                if (format==JBXL_3D_FORMAT_DAE) {
                    dae->phantom_out = true;
                    dae->addObject(mesh, false);
                }
                // OBJ
                else if (format==JBXL_3D_FORMAT_OBJ) {
                    obj->phantom_out = true;
                    obj->addObject(mesh, false);
                }
                // FBX
                else if (format==JBXL_3D_FORMAT_FBX) {
                    //obj->phantom_out = true;
                    //obj->addObject(mesh, false);
                }
                // STL
                else if (format==JBXL_3D_FORMAT_STL_A || format==JBXL_3D_FORMAT_STL_B) {
                    stl->addObject(mesh);
                }
                freeMeshObjectData(mesh);
                //
                count++;
            }
        }

        //
        // Grass
        else if (shapes[s].PCode==PRIM_PCODE_GRASS){ 
            //
            shapes[s].affineTrans.addShift(-xsize/2.0f, -ysize/2.0f, -waterHeight);
            MeshObjectData* mesh = treeTool.GenerateGrass(shapes[s], terrain);  // 1個の Terrainのみサポート．範囲チェックあり
            if (mesh!=NULL && mesh->affineTrans!=NULL) mesh->affineTrans->addShift(terrainShift);
            //
            if (mesh!=NULL) {
                if (isRequiredTexture(format)) {    // STLの場合は不必要
                    MeshFacetNode* facet = mesh->facet;
                    while (facet!=NULL) {
                        if (facet->material_param.enable) {
                            ConvertTexture(facet->material_param.getTextureName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                            facet->material_param.setFullName(MTRL_IMAGE_TYPE);
                        }
                        facet = facet->next;
                    }
                }
                // DAE
                if (format==JBXL_3D_FORMAT_DAE) {
                    dae->phantom_out = true;
                    dae->addObject(mesh, false);
                }
                // OBJ
                else if (format==JBXL_3D_FORMAT_OBJ) {
                    obj->phantom_out = true;
                    obj->addObject(mesh, false);
                }
                // FBX
                else if (format==JBXL_3D_FORMAT_FBX) {
                    //obj->phantom_out = true;
                    //obj->addObject(mesh, false);
                }
                // STL
                else if (format==JBXL_3D_FORMAT_STL_A || format==JBXL_3D_FORMAT_STL_B) {
                    stl->addObject(mesh);
                }
                freeMeshObjectData(mesh);
                //
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

            shapes[s].affineTrans.addShift(-xsize/2.0f + terrainShift.x, -ysize/2.0f + terrainShift.y, -waterHeight + terrainShift.z);
            MeshObjectData* mesh = MeshObjectDataFromPrimShape(shapes[s], assetsFiles, useBrep, ptr_skin_joint);
            //
            if (mesh!=NULL) {
                if (isRequiredTexture(format)) {    // STLの場合は不必要
                    MeshFacetNode* facet = mesh->facet;
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
                            ConvertTexture(facet->material_param.getTextureName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                            ConvertTexture(facet->material_param.getBumpMapName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                            ConvertTexture(facet->material_param.getSpecMapName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                            facet->material_param.setFullName(MTRL_IMAGE_TYPE);
                        }
                        facet = facet->next;
                    }
                }

                bool collider = true;
                if (strstr((const char*)shapes[s].ObjFlags.buf, OART_FLAGS_PHANTOM)!=NULL) {    // Phantom
                    collider = false;
                }
                // DAE
                if (format==JBXL_3D_FORMAT_DAE) {
                    if (collider) dae->phantom_out = false;
                    tXML*  joints_template = NULL;
                    //
                    if (count==0 && this->procJoints) {
                        char* path = get_resource_path(OART_JOINT_TEMPLATE_FILE, assetsFiles);
                        if (path != NULL) {
                            joints_template = xml_parse_file(path);     // not free
                        }
                        else {
                            PRINT_MESG("OARTool::generateSolidData: WARNING: Joints template xml file is not found!\n");
                        }
                    }
                    dae->addObject(mesh, collider, skin_joint, joints_template);
                }
                // OBJ
                else if (format==JBXL_3D_FORMAT_OBJ) {
                    if (collider) obj->phantom_out = false;
                    obj->addObject(mesh, collider);
                }
                // FBX
                else if (format==JBXL_3D_FORMAT_FBX) {
                    //if (collider) obj->phantom_out = false;
                    //obj->addObject(mesh, collider, skin_joint);
                }
                // STL
                else if (format==JBXL_3D_FORMAT_STL_A || format==JBXL_3D_FORMAT_STL_B) {
                    stl->addObject(mesh);
                }
                freeMeshObjectData(mesh);
                //
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
            obj->closeSolid();
            return (void*)obj;
        }
        //  FBX
        else if (format==JBXL_3D_FORMAT_FBX) {
            //Vector<double> offset = obj->execAffineTrans();         // no_offset==true の場合，原点縮退
            //if (obj->affineTrans==NULL) fbx->affineTrans = new AffineTrans<double>();
            //fbx->affineTrans->setShift(offset);
            //fbx->closeSolid();
            return NULL;
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
*/
void  OARTool::outputSolidData(int format, const char* fname, void* solid)
{
    if (solid==NULL || fname==NULL) return;
    //
    fname = get_file_name(fname);
    Buffer dae_fname = make_Buffer_str(fname);
    Buffer out_path = init_Buffer();

    // fnameの拡張子は自動的に変換される
    // DAE
    if (format==JBXL_3D_FORMAT_DAE) {
        ColladaXML* dae = (ColladaXML*)solid;
        // 縮退状態
        if (noOffset && dae->affineTrans!=NULL) {
            float offset[3];
            int len = sizeof(float) * 3;
            memset(offset, 0, len);
            offset[0] = -(float)(dae->affineTrans->shift.x);
            offset[1] =  (float)(dae->affineTrans->shift.z);
            offset[2] = -(float)(dae->affineTrans->shift.y);
            char* params = (char*)encode_base64_filename((unsigned char*)offset, len, '-');
            del_file_extension_Buffer(&dae_fname);
            cat_s2Buffer("_", &dae_fname);
            cat_s2Buffer(params, &dae_fname);
            cat_s2Buffer(".", &dae_fname);
        }
        //
        if (dae->phantom_out) out_path = dup_Buffer(pathPTM);
        else                  out_path = dup_Buffer(pathOUT);
        //
        dae->outputFile((char*)dae_fname.buf, (char*)out_path.buf, XML_SPACE_FORMAT);
        free_Buffer(&dae_fname);
    }

    // OBJ
    else if (format==JBXL_3D_FORMAT_OBJ) {
        OBJData* obj = (OBJData*)solid;
        Buffer obj_fname = make_Buffer_str(fname);
        // 縮退状態
        if (noOffset && obj->affineTrans != NULL) {
            float offset[3];
            int len = sizeof(float) * 3;
            memset(offset, 0, len);
            if (obj->engine==JBXL_3D_ENGINE_UE) {   // UE
                offset[0] =  (float)(obj->affineTrans->shift.x * 100.);    // 100 is Unreal Unit
                offset[1] = -(float)(obj->affineTrans->shift.y * 100.);
                offset[2] =  (float)(obj->affineTrans->shift.z * 100.);
            }
            else {                                  // Unity
                offset[0] = -(float)(obj->affineTrans->shift.x);
                offset[1] =  (float)(obj->affineTrans->shift.z);
                offset[2] = -(float)(obj->affineTrans->shift.y);
            }
            char* params = (char*)encode_base64_filename((unsigned char*)offset, len, '-');
            del_file_extension_Buffer(&obj_fname);
            cat_s2Buffer("_", &obj_fname);
            cat_s2Buffer(params, &obj_fname);
            cat_s2Buffer(".", &obj_fname);
        }
        //
        if (obj->engine==JBXL_3D_ENGINE_UE) {
            if (obj->phantom_out) ins_s2Buffer(OART_UE_PHANTOM_PREFIX,  &obj_fname);
            else                  ins_s2Buffer(OART_UE_COLLIDER_PREFIX, &obj_fname);
            out_path = dup_Buffer(pathOUT);
        }
        else {
            if (obj->phantom_out) out_path = dup_Buffer(pathPTM);
            else                  out_path = dup_Buffer(pathOUT);
        }
        //
        obj->outputFile((char*)obj_fname.buf, (char*)out_path.buf, OART_DEFAULT_TEX_DIR, OART_DEFAULT_MTL_DIR);
        free_Buffer(&obj_fname);
    }

    // FBX
    else if (format==JBXL_3D_FORMAT_FBX) {
        //
    }

    // STL
    else if (format==JBXL_3D_FORMAT_STL_A || format==JBXL_3D_FORMAT_STL_B) {
        bool ascii = true;
        if (format==JBXL_3D_FORMAT_STL_B) ascii = false;
        out_path = dup_Buffer(pathOUT);
        BrepSolidList* stl = (BrepSolidList*)solid;
        stl->outputFile(fname, (char*)out_path.buf, ascii);
    }
    free_Buffer(&out_path);

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
    // FBX
    else if (format==JBXL_3D_FORMAT_FBX) {
        //OBJData* obj = (OBJData*)solid;
        //freeOBJData(obj);
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
void  OARTool::ConvertTexture(const char* texture, const char* add_name, const char* ext_name, const char* dist, const char* comformat)

@param texture    コンバート元データのUUID．
@param add_name   コンバート先ファイル名の追加文字列．
@param ext_name   コンバート先ファイル名の拡張子． "." 付き
@param dist       コンバート先のパス．
@param comformat  内部処理が失敗した場合の外部処理コマンド．NULL の場合はデフォルトを使用する．
*/
void  OARTool::ConvertTexture(const char* texture, const char* add_name, const char* ext_name, const char* dist, const char* comformat)
{
    if (texture==NULL) return;

    Buffer outpath;
    if (dist==NULL) outpath = make_Buffer_bystr((char*)pathTEX.buf);
    else            outpath = make_Buffer_bystr(dist);

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

    if (!file_exist((char*)outpath.buf)) {
        //
        char* path = get_resource_path((char*)texture, assetsFiles);
        char* extn = get_file_extension(path);
        //
        if (path!=NULL && extn!=NULL) {
            char* check_ext = (char*)ext_name;
            if (check_ext[0]=='.') check_ext = (char*)ext_name + 1;

            // 同じ形式のファイル -> コピー
            if (!strcasecmp(extn, check_ext)) {
                int flsz = file_size(path);
                int cpsz = copy_file(path, (char*)outpath.buf);
                DEBUG_MODE {
                    if (flsz==cpsz) {
                        PRINT_MESG("OARTool::ConvertTexture: copy from %s to %s\n", path, (char*)outpath.buf);
                    }
                    else {
                        PRINT_MESG("OARTool::ConvertTexture: ERROR: copy from %s to %s failed!\n", path, (char*)outpath.buf);
                    }
                }
            }
            // JPEG2000
            else if (extn[0]=='j' || extn[0]=='J') {
                JPEG2KImage jpg = readJPEG2KFile(path);
                if (jpg.state==0) {
                    MSGraph<uByte> vp = JPEG2KImage2MSGraph<uByte>(jpg);
                    DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: texture = %s [size = (%4d,%4d,%2d), mode = %d]\n", texture, jpg.ws, jpg.hs, jpg.col, jpg.cmode);
                    //
                    if (vp.zs>0) {
                        TGAImage tga = MSGraph2TGAImage(vp, true);
                        int err = writeTGAFile((char*)outpath.buf, tga);
                        if (!err) converted = true;
                        else      PRINT_MESG("OARTool::ConvertTexture: ERROR: write error (%d).\n", err);
                        tga.free();
                    }
                    else {
                        PRINT_MESG("OARTool::ConvertTexture: ERROR: color num of %s is %d\n", texture, vp.zs);
                    }
                    vp.free();
                    jpg.free();
                }
                else {
                    if (jpg.state==JBXL_GRAPH_IVDDATA_ERROR) {
                        DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: ERROR: texture %s is invalid.\n", texture);
                    }
                    else {
                        DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: ERROR: texture %s convert error (%d).\n", texture, jpg.state);
                    }
                }
                //
                // Retry convert using external command
                if (!converted) {
                    DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: RETRY: convert %s to %s\n", path, (char*)outpath.buf); 
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
                        int ret = system(command);
                        err = WEXITSTATUS(ret);
                    #endif
                    if (err!=0) {
                        PRINT_MESG("OARTool::ConvertTexture: ERROR: texture %s convert error (%d).\n", texture, err);
                    }
                    else {
                        DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: SUCCESS: texture %s is converted.\n", texture);
                    }
                }
            }
            // unknown file extension
            else {
                PRINT_MESG("OARTool::ConvertTexture: ERROR: unsupported file %s\n found!", path);
            }
        }
        // Lost
        else {
            PRINT_MESG("OARTool::ConvertTexture: ERROR: texture %s is lost!\n", texture);
        }
    }
    free_Buffer(&outpath);

    return;
}


