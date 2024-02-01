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
    pathDAE = init_Buffer();    // Output directory for DAE
    pathSTL = init_Buffer();    // Output directory for STL
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
    shift.set(0.0, 0.0, 0.0);

    objectsNum      = 0;

    settingsFiles   = NULL;     // Files list in settings
    terrainsFiles   = NULL;     // Files list in terrains
    landdataFiles   = NULL;     // Files list in landata 
    assetsFiles     = NULL;     // Files list in assets  
    objectsFiles    = NULL;     // Files list in objects

    forUnity3D      = true;     // for Unity3D
    forUnity5       = false;    // for Unity3D v5.x
    forUnity4       = false;    // for Unity3D v4.x
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
    if (terrain!=NULL) {
        for (int i=0; i<terrainNum; i++) terrain[i].free();
        ::free(terrain);
        terrain = NULL;
        terrainNum = 0;
    }
    treeTool.free();
    clear_path();
    clear_list();
    //
    free_Buffer(&regionName);
}


char*  OARTool::get_outpath(int output)
{
    if      (output == OART_OUTPUT_DAE) return (char*)pathDAE.buf; 
    else if (output == OART_OUTPUT_OBJ) return (char*)pathOBJ.buf; 
    return (char*)pathSTL.buf;
}


//
void  OARTool::clear_path(void)
{
    free_Buffer(&pathOAR);
    free_Buffer(&pathDAE);
    free_Buffer(&pathSTL);
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
    return;
}


void  OARTool::setUnity5(bool unity5)
{
    forUnity5 = unity5;
    return;
}


void  OARTool::setUnity3D(bool unity)
{
    forUnity3D = unity;
    return;
}


void  OARTool::setUE5(bool ue5)
{
    forUE5 = ue5;
    return;
}


/**
データ入出力用のパス情報を設定する．

設定されるパス情報は以下の通り．
pathOAR : データ入力用（OAR）のパス
pathDAE : DAEデータ出力用のパス
pathOBJ : OBJデータ出力用のパス
pathSTL : STLデータ出力用のパス
pathTEX : テクスチャ出力用のパス
pathPTM : ファントムデータ出力用のパス
pathAST : 追加のアセットデータ用のパス

@param oardir  読み込むOARファイルのトップディレクトリ
@param outdir  データ書き出し用ディレクトリ
@param astdir  アセットデータの追加ディレクトリ（OARに含まれていないアッセットデータ用）
@param output  出力データフォーマット（OART_OUTPUT_DAE, OART_OUTPUT_OBJ, OART_OUTPUT_STL）
*/
void  OARTool::SetPathInfo(const char* oardir, const char* outdir, const char* astdir, int output)
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
        pathDAE = make_Buffer_bystr(outdir);
        pathOBJ = make_Buffer_bystr(outdir);
        pathSTL = make_Buffer_bystr(outdir);
        #ifdef WIN32
            if (pathDAE.buf[strlen((char*)pathDAE.buf)-1]!='\\') cat_s2Buffer("\\", &pathDAE);
            if (pathOBJ.buf[strlen((char*)pathOBJ.buf)-1]!='\\') cat_s2Buffer("\\", &pathOBJ);
            if (pathSTL.buf[strlen((char*)pathSTL.buf)-1]!='\\') cat_s2Buffer("\\", &pathSTL);
        #else
            if (pathDAE.buf[strlen((char*)pathDAE.buf)-1]!='/') cat_s2Buffer("/", &pathDAE);
            if (pathOBJ.buf[strlen((char*)pathOBJ.buf)-1]!='/') cat_s2Buffer("/", &pathOBJ);
            if (pathSTL.buf[strlen((char*)pathSTL.buf)-1]!='/') cat_s2Buffer("/", &pathSTL);
        #endif
    }
    else {
        pathDAE = make_Buffer_bystr(OART_DEFAULT_DAE_DIR);
        pathOBJ = make_Buffer_bystr(OART_DEFAULT_OBJ_DIR);
        pathSTL = make_Buffer_bystr(OART_DEFAULT_STL_DIR);
    }

    if (output==OART_OUTPUT_DAE) {
        pathTEX = make_Buffer_bystr((char*)pathDAE.buf);
        pathPTM = make_Buffer_bystr((char*)pathDAE.buf);
        cat_s2Buffer(OART_DEFAULT_TEX_DIR, &pathTEX);
        cat_s2Buffer(OART_DEFAULT_PTM_DIR, &pathPTM);
    }
    else if (output==OART_OUTPUT_OBJ) {
        pathTEX = make_Buffer_bystr((char*)pathOBJ.buf);
        pathPTM = make_Buffer_bystr((char*)pathOBJ.buf);
        cat_s2Buffer(OART_DEFAULT_TEX_DIR, &pathTEX);
        cat_s2Buffer(OART_DEFAULT_PTM_DIR, &pathPTM);
    }

    // ASSE
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
        del_xml(&arc_xml);
    }
    else {
        DEBUG_MODE PRINT_MESG("OARTool::GetDatanInfo: WARNING: not found archive  file! [%sarchive.xml]\n", pathOAR.buf);
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
            del_xml(&land_xml);
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
    tList*  extn = add_tList_node_str(NULL, "txt", NULL);
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


void  OARTool::MakeOutputFolder(int output)
{
    // mkdir for output
    if (output == OART_OUTPUT_DAE) {
        mkdir((char*)pathDAE.buf, 0700);
        mkdir((char*)pathTEX.buf, 0700);
        mkdir((char*)pathPTM.buf, 0700);
    }
    else if (output == OART_OUTPUT_OBJ) {
        mkdir((char*)pathOBJ.buf, 0700);
        mkdir((char*)pathTEX.buf, 0700);
        mkdir((char*)pathPTM.buf, 0700);
    }
    else if (output & OART_OUTPUT_STL) {
        mkdir((char*)pathSTL.buf, 0700);
    }

    return;
}


void  OARTool::ReadTerrainData(void)
{
    if (terrainNum==0) return;
    terrain = (TerrainTool*)malloc(terrainNum*sizeof(TerrainTool));
    if (terrain==NULL) return;

    int count  = 0;
    tList* lps = settingsFiles;
    tList* lpt = terrainsFiles;

    if (lps!=NULL) {
        while (count<terrainNum && lps!=NULL && lpt!=NULL) {
            terrain[count] = TerrainTool((char*)lps->ldat.key.buf, xsize, ysize);
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
            terrain[count].ReadHeightData((char*)lpt->ldat.val.buf);
            lpt = lpt->next;
            count++;
        }
    }

    if (count>0) waterHeight = terrain[0].water_height();
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// for DAE

int  OARTool::GenerateTerrainDae(void)
{
    if (terrainNum==0) return 0;

    PRINT_MESG("GenerateTerrainDae: generating terrain dae file\n");
    int num = 0;
    while (num<terrainNum) {
        terrain[num].GenerateTexture(assetsFiles, (char*)pathTEX.buf, forUnity3D);
        terrain[num].GenerateDae((char*)pathDAE.buf, shift, forUnity3D);
        num++;
#ifdef WIN32
        DisPatcher(); 
#endif
    }

    return num;
}


int  OARTool::GenerateObjectsDae(int startnum, int stopnum, bool useBrep, bool phantom, char* command)
{
    tList* lp = objectsFiles;
    CVCounter* counter = GetUsableGlobalCounter();

    if (stopnum<0) stopnum = objectsNum;
    int num = 0;
    int cnt = 0;
    while (lp!=NULL) {
        num++;
        if (num>=startnum && num<=stopnum) {
            GenerateDae((char*)lp->ldat.val.buf, num, useBrep, phantom, command);
            if (counter!=NULL) {
                if (counter->cancel) break;
                counter->StepIt();
            }
            cnt++;
        }
        lp = lp->next;
    }
    if (counter!=NULL && counter->cancel) cnt = -1;

    return cnt;
}


int  OARTool::GenerateSelectedDae(int objnum, int* objlist, bool useBrep, bool phantom, char* command)
{
    tList* lp = objectsFiles;
    CVCounter* counter = GetUsableGlobalCounter();

    int num = 0;
    int cnt = 0;
    while (lp!=NULL) {
        if (num==objlist[cnt]) {
            GenerateDae((char*)lp->ldat.val.buf, num+1, useBrep, phantom, command);
            if (counter!=NULL) {
                if (counter->cancel) break;
                counter->StepIt();
            }
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
Tree, Grass, Prim(Sculpt, Meshを含む) のXMLデータ(オブジェクト１個分) を Collada形式で書きだす．
出力先は outPathで指定されたディレクトリ．

@param fname    オブジェクト名（xmlファイル名）
@param useBrep  頂点の配置にBREPを使用するか？ 使用すると処理時間はかかるが，データサイズが小さくなる．
@param num      表示用の処理番号．
@paeam phantom  オブジェクト中に１個でもファントムがある場合，全体をファントムとするか？
@param command  JPEG2000（テクスチャ）の内部処理が失敗した場合の外部コマンド．
@param outPath  大域変数．データの出力先．
*/
void  OARTool::GenerateDae(const char* fname, int num, bool useBrep, bool phantom, char* command)
{
    PRINT_MESG("[%d/%d] GenerateDae: converting %s\n", num, objectsNum, fname);

    int shno = 0;
    PrimBaseShape* shapes;

    tXML* sxml = xml_parse_file(fname);
    if (sxml!=NULL) {
        shapes = CreatePrimBaseShapesFromXML(sxml, assetsFiles, &shno); // Shapeデータ．shnoはデータの数
        del_xml(&sxml);
        if (shapes==NULL || shno<=0) {
            PRINT_MESG("OARTool::GenerateDae: WARNING: not found shape data in %s (skip)\n", fname);
            return;
        }
    }
    else {
        PRINT_MESG("OARTool::GenerateDae: WARNING: XML File %s Read Error.(skip)\n", fname);
        return;
    }

    ColladaXML* dae = new ColladaXML();
    dae->forUnity5  = forUnity5;
    dae->forUnity4  = forUnity4;
    dae->forUnity3D = forUnity3D;
    dae->setBlankTexture(PRIM_OS_BLANK_TEXTURE);

    bool phantom_out, collider;
    if (phantom) phantom_out = false;
    else         phantom_out = true;
 
    int  count = 0;
    for (int s=0; s<shno; s++) {
        //
        // Tree
        if (shapes[s].PCode==PRIM_PCODE_NEWTREE || shapes[s].PCode==PRIM_PCODE_TREE) {
            //
            shapes[s].affineTrans.addShift(-xsize/2.0f+shift.x, -ysize/2.0f+shift.y, -waterHeight+shift.z);
            MeshObjectData* data = treeTool.GenerateTree(shapes[s], 0, forUnity3D);
            //
            if (data!=NULL) {
                collider = false;
                if (phantom) phantom_out = true;
                //
                MeshFacetNode* facet = data->facet;
                while (facet!=NULL) {
                    if (facet->material_param.enable) {
                        // convert texture
                        //char* addname = facet->material_param.getAdditionalName();
                        ConvertTexture(facet->material_param.getTextureName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                        facet->material_param.setFullName(MTRL_IMAGE_TYPE);
                    }
                    facet = facet->next;
                }
                dae->addObject(data, collider);
                freeMeshObjectData(data);
                //
                count++;
            }
        }

        //
        // Grass
        else if (shapes[s].PCode==PRIM_PCODE_GRASS){ 
            //
            shapes[s].affineTrans.addShift(-xsize/2.0f, -ysize/2.0f, -waterHeight);
            MeshObjectData* data = treeTool.GenerateGrass(shapes[s], terrain, forUnity3D);  // 1個の Terrainのみサポート．範囲チェックあり
            shapes[s].affineTrans.addShift(shift.x, shift.y, shift.z);
            //
            if (data!=NULL) {
                collider = false;
                if (phantom)  phantom_out = true;
                //
                MeshFacetNode* facet = data->facet;
                while (facet!=NULL) {
                    if (facet->material_param.enable) {
                        // convert texture
                        //char* addname = facet->material_param.getAdditionalName();
                        ConvertTexture(facet->material_param.getTextureName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                        facet->material_param.setFullName(MTRL_IMAGE_TYPE);
                    }
                    facet = facet->next;
                }
                dae->addObject(data, collider);
                freeMeshObjectData(data);
                //
                count++;
            }
        }

        //
        // Prim (Sculpt, Meshを含む)
        else if (shapes[s].PCode==PRIM_PCODE_PRIM) { 
            //
            shapes[s].affineTrans.addShift(-xsize/2.0f+shift.x, -ysize/2.0f+shift.y, -waterHeight+shift.z);
            MeshObjectData* data = MeshObjectDataFromPrimShape(shapes[s], assetsFiles, useBrep, forUnity3D);
            //
            if (data!=NULL) {
                if (strstr((const char*)shapes[s].ObjFlags.buf, OART_FLAGS_PHANTOM)!=NULL) {    // Phantom
                    collider = false;
                    if (phantom)  phantom_out = true;
                }
                else {
                    collider = true;
                    if (!phantom) phantom_out = false;
                }
                //
                MeshFacetNode* facet = data->facet;
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
                        
                        // convert texture
                        //char* addname = facet->material_param.getAdditionalName();
                        ConvertTexture(facet->material_param.getTextureName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                        ConvertTexture(facet->material_param.getBumpMapName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                        ConvertTexture(facet->material_param.getSpecMapName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                        facet->material_param.setFullName(MTRL_IMAGE_TYPE);
                    }
                    facet = facet->next;
                }

                dae->addObject(data, collider);
                freeMeshObjectData(data);
                //
                count++;
            }
        }
    }

    // Output file
    if (count>0) {
        if (count==1 && forUnity4) dae->addCenterObject();  // for Unity4.x
        //
        Buffer out_path = dup_Buffer(pathDAE);
        if (phantom_out) cat_Buffer(&pathPTM, &out_path);
        dae->outputFile(fname, (char*)out_path.buf, XML_INDENT_FORMAT); // fnameの拡張子は自動的に変換される
        free_Buffer(&out_path);
    }

    freeColladaXML(dae);
    for (int s=0; s<shno; s++) shapes[s].free();
    ::free(shapes);

    return;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// for OBJ

int  OARTool::GenerateTerrainOBJ(void)
{
    if (terrainNum==0) return 0;

    PRINT_MESG("GenerateTerrainOBJ: generating terrain OBJ file\n");
    int num = 0;
    while (num<terrainNum) {
        terrain[num].GenerateTexture(assetsFiles, (char*)pathTEX.buf, forUnity3D);
        terrain[num].GenerateOBJ((char*)pathOBJ.buf, shift, forUnity3D);
        num++;
#ifdef WIN32
        DisPatcher(); 
#endif
    }

    return num;
}


int  OARTool::GenerateObjectsOBJ(int startnum, int stopnum, bool useBrep, bool phantom, char* command)
{
    tList* lp = objectsFiles;
    CVCounter* counter = GetUsableGlobalCounter();

    if (stopnum<0) stopnum = objectsNum;
    int num = 0;
    int cnt = 0;
    while (lp!=NULL) {
        num++;
        if (num>=startnum && num<=stopnum) {
            GenerateOBJ((char*)lp->ldat.val.buf, num, useBrep, phantom, command);
            if (counter!=NULL) {
                if (counter->cancel) break;
                counter->StepIt();
            }
            cnt++;
        }
        lp = lp->next;
    }
    if (counter!=NULL && counter->cancel) cnt = -1;

    return cnt;
}


int  OARTool::GenerateSelectedOBJ(int objnum, int* objlist, bool useBrep, bool phantom, char* command)
{
    tList* lp = objectsFiles;
    CVCounter* counter = GetUsableGlobalCounter();

    int num = 0;
    int cnt = 0;
    while (lp!=NULL) {
        if (num==objlist[cnt]) {
            GenerateOBJ((char*)lp->ldat.val.buf, num+1, useBrep, phantom, command);
            if (counter!=NULL) {
                if (counter->cancel) break;
                counter->StepIt();
            }
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
Tree, Grass, Prim(Sculpt, Meshを含む) のXMLデータ(オブジェクト１個分) を Collada形式で書きだす．
出力先は outPathで指定されたディレクトリ．

@param fname    オブジェクト名（xmlファイル名）
@param useBrep  頂点の配置にBREPを使用するか？ 使用すると処理時間はかかるが，データサイズが小さくなる．
@param num      表示用の処理番号．
@paeam phantom  オブジェクト中に１個でもファントムがある場合，全体をファントムとするか？
@param command  JPEG2000（テクスチャ）の内部処理が失敗した場合の外部コマンド．
@param outPath  大域変数．データの出力先．
*/
void  OARTool::GenerateOBJ(const char* fname, int num, bool useBrep, bool phantom, char* command)
{
    PRINT_MESG("[%d/%d] GenerateOBJ: converting %s\n", num, objectsNum, fname);

    int shno = 0;
    PrimBaseShape* shapes;

    tXML* sxml = xml_parse_file(fname);
    if (sxml!=NULL) {
        shapes = CreatePrimBaseShapesFromXML(sxml, assetsFiles, &shno); // Shapeデータ．shnoはデータの数
        del_xml(&sxml);
        if (shapes==NULL || shno<=0) {
            PRINT_MESG("OARTool::GenerateOBJ: WARNING: not found shape data in %s (skip)\n", fname);
            return;
        }
    }
    else {
        PRINT_MESG("OARTool::GenerateOBJ: WARNING: XML File %s Read Error.(skip)\n", fname);
        return;
    }

    /*
    ColladaXML* dae = new ColladaXML();
    dae->forUnity5  = forUnity5;
    dae->forUnity4  = forUnity4;
    dae->forUnity3D = forUnity3D;
    dae->setBlankTexture(PRIM_OS_BLANK_TEXTURE);
    */

    OBJData* obj = new OBJData();

    bool phantom_out, collider;
    if (phantom) phantom_out = false;
    else         phantom_out = true;
 
    int  count = 0;
    for (int s=0; s<shno; s++) {
        //
        // Tree
        if (shapes[s].PCode==PRIM_PCODE_NEWTREE || shapes[s].PCode==PRIM_PCODE_TREE) {
            //
            shapes[s].affineTrans.addShift(-xsize/2.0f+shift.x, -ysize/2.0f+shift.y, -waterHeight+shift.z);
            MeshObjectData* data = treeTool.GenerateTree(shapes[s], 0, forUnity3D);
            //
            if (data!=NULL) {
                collider = false;
                if (phantom) phantom_out = true;
                //
                MeshFacetNode* facet = data->facet;
                while (facet!=NULL) {
                    if (facet->material_param.enable) {
                        // convert texture
                        //char* addname = facet->material_param.getAdditionalName();
                        ConvertTexture(facet->material_param.getTextureName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                        facet->material_param.setFullName(MTRL_IMAGE_TYPE);
                    }
                    facet = facet->next;
                }
                obj->addObject(data, collider);
                freeMeshObjectData(data);
                //
                count++;
            }
        }

        //
        // Grass
        else if (shapes[s].PCode==PRIM_PCODE_GRASS){ 
            //
            shapes[s].affineTrans.addShift(-xsize/2.0f, -ysize/2.0f, -waterHeight);
            MeshObjectData* data = treeTool.GenerateGrass(shapes[s], terrain, forUnity3D);  // 1個の Terrainのみサポート．範囲チェックあり
            shapes[s].affineTrans.addShift(shift.x, shift.y, shift.z);
            //
            if (data!=NULL) {
                collider = false;
                if (phantom)  phantom_out = true;
                //
                MeshFacetNode* facet = data->facet;
                while (facet!=NULL) {
                    if (facet->material_param.enable) {
                        // convert texture
                        //char* addname = facet->material_param.getAdditionalName();
                        ConvertTexture(facet->material_param.getTextureName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                        facet->material_param.setFullName(MTRL_IMAGE_TYPE);
                    }
                    facet = facet->next;
                }
                obj->addObject(data, collider);
                freeMeshObjectData(data);
                //
                count++;
            }
        }

        //
        // Prim (Sculpt, Meshを含む)
        else if (shapes[s].PCode==PRIM_PCODE_PRIM) { 
            //
            shapes[s].affineTrans.addShift(-xsize/2.0f+shift.x, -ysize/2.0f+shift.y, -waterHeight+shift.z);
            MeshObjectData* data = MeshObjectDataFromPrimShape(shapes[s], assetsFiles, useBrep, forUnity3D);
            //
            if (data!=NULL) {
                if (strstr((const char*)shapes[s].ObjFlags.buf, OART_FLAGS_PHANTOM)!=NULL) {    // Phantom
                    collider = false;
                    if (phantom)  phantom_out = true;
                }
                else {
                    collider = true;
                    if (!phantom) phantom_out = false;
                }
                //
                MeshFacetNode* facet = data->facet;
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
                        
                        // convert texture
                        //char* addname = facet->material_param.getAdditionalName();
                        ConvertTexture(facet->material_param.getTextureName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                        ConvertTexture(facet->material_param.getBumpMapName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                        ConvertTexture(facet->material_param.getSpecMapName(), NULL, MTRL_IMAGE_TYPE, NULL, command);
                        facet->material_param.setFullName(MTRL_IMAGE_TYPE);
                    }
                    facet = facet->next;
                }

                obj->addObject(data, collider);
                freeMeshObjectData(data);
                //
                count++;
            }
        }
    }

    // Output file
    if (count>0) {
        //if (count==1 && forUnity4) obj->addCenterObject();  // for Unity4.x
        //
        Buffer out_path = dup_Buffer(pathOBJ);
        if (phantom_out) cat_Buffer(&pathPTM, &out_path);
        obj->execAffineTrans();
        obj->outputFile(fname, (char*)out_path.buf); // fnameの拡張子は自動的に変換される
        free_Buffer(&out_path);
    }

    freeOBJData(obj);
    for (int s=0; s<shno; s++) shapes[s].free();
    ::free(shapes);

    return;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// for STL
//

int  OARTool::GenerateTerrainSTL(bool binfile)
{
    if (terrainNum==0) return 0;

    PRINT_MESG("GenerateTerrainSTL: generating terrain stl file\n");
    int num = 0;
    while (num<terrainNum) {
        terrain[num].GenerateSTL((char*)pathSTL.buf, shift, binfile);
        num++;
#ifdef WIN32
        DisPatcher();
#endif
    }

    return num;
}


int  OARTool::GenerateObjectsSTL(int startnum, int stopnum, bool binfile)
{
    tList* lp = objectsFiles;
    CVCounter* counter = GetUsableGlobalCounter();

    if (stopnum<0) stopnum = objectsNum;
    int num = 0;
    int cnt = 0;
    while (lp!=NULL) {
        num++;
        if (num>=startnum && num<=stopnum) {
            GenerateSTL((char*)lp->ldat.val.buf, num, binfile);
            if (counter!=NULL) {
                if (counter->cancel) break;
                counter->StepIt();
            }
            cnt++;
        }
        lp = lp->next;
    }
    if (counter!=NULL && counter->cancel) cnt = -1;

    return cnt;
}


int  OARTool::GenerateSelectedSTL(int objnum, int* objlist, bool binfile)
{
    tList* lp = objectsFiles;
    CVCounter* counter = GetUsableGlobalCounter();

    int num = 0;
    int cnt = 0;
    while (lp!=NULL) {
        if (num==objlist[cnt]) {
            GenerateSTL((char*)lp->ldat.val.buf, num+1, binfile);
            if (counter!=NULL) {
                if (counter->cancel) break;
                counter->StepIt();
            }
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
Tree, Grass, Prim(Sculpt, Meshを含む) のXMLデータ(ファイル１個分) を STL形式で書きだす.
頂点の配置には BREPが使用される．

@param fname    オブジェクト名
@param num      表示用の処理番号．
@param binfile  データをバイナリ形式で出力するか？
*/
void  OARTool::GenerateSTL(const char* fname, int num, bool binfile)
{
    PRINT_MESG("[%d/%d] GenerateSTL: converting %s\n", num, objectsNum, fname);

    BrepSolidList* slist = GenerateSolidList(fname);

    // Output file
    if (slist!=NULL) {
        Buffer out_path = dup_Buffer(pathSTL);
        slist->outputFile(fname, (char*)out_path.buf, binfile); // fnameの拡張子は自動的に変換される
        free_Buffer(&out_path);
        freeBrepSolidList(slist);
    }

    return;
}


BrepSolidList*  OARTool::GenerateSolidList(const char* fname)
{
    int shno = 0;
    PrimBaseShape* shapes;

    tXML* sxml = xml_parse_file(fname);
    if (sxml!=NULL) {
        shapes = CreatePrimBaseShapesFromXML(sxml, assetsFiles, &shno); // Shapeデータ．shnoはデータの数
        del_xml(&sxml);
        if (shapes==NULL || shno<=0) {
            PRINT_MESG("OARTool::GenerateBrepSolidList: WARNING: not found shape data in %s (skip)\n", fname);
            return NULL;
        }
    }
    else {
        PRINT_MESG("OARTool::GenerateBrepSolidList: WARNING: XML File %s Read Error.(skip)\n", fname);
        return NULL;
    }

    BrepSolidList* slist = new BrepSolidList();

    int count = 0;
    for (int s=0; s<shno; s++) {
        // Tree
        if (shapes[s].PCode==PRIM_PCODE_NEWTREE || shapes[s].PCode==PRIM_PCODE_TREE) {
            //
            shapes[s].affineTrans.addShift(-xsize/2.0f+shift.x, -ysize/2.0f+shift.y, -waterHeight+shift.z);
            MeshObjectData* data = treeTool.GenerateTree(shapes[s], 0, forUnity3D);
            //
            if (data!=NULL) {
                slist->addObject(data);
                freeMeshObjectData(data);
                //
                count++;
            }
        }

        // Grass
        else if (shapes[s].PCode==PRIM_PCODE_GRASS){ 
            //
            shapes[s].affineTrans.addShift(-xsize/2.0f, -ysize/2.0f, -waterHeight);
            MeshObjectData* data = treeTool.GenerateGrass(shapes[s], terrain, forUnity3D);  // 1個の Terrainのみサポート．範囲チェックあり
            shapes[s].affineTrans.addShift(shift.x, shift.y, shift.z);
            //
            if (data!=NULL) {
                slist->addObject(data);
                freeMeshObjectData(data);
                //
                count++;
            }
        }

        // Prim (Sculpt, Meshを含む)
        else if (shapes[s].PCode==PRIM_PCODE_PRIM) { 
            //
            shapes[s].affineTrans.addShift(-xsize/2.0f+shift.x, -ysize/2.0f+shift.y, -waterHeight+shift.z);
            MeshObjectData* data = MeshObjectDataFromPrimShape(shapes[s], assetsFiles, false, forUnity3D);
            //
            if (data!=NULL) {
                slist->addObject(data);
                freeMeshObjectData(data);
                //
                count++;
            }
        }
    }

    for (int s=0; s<shno; s++) shapes[s].free();
    ::free(shapes);

    if (count<=0) {
        freeBrepSolidList(slist);
        slist = NULL;
    }
    return slist;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 

/**
@param texture    コンバート元データのUUID．
@param add_fname  コンバート先ファイル名の追加文字列．
@param ext_fname  コンバート先ファイル名の拡張子．
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

    bool make_dummy = true;

    if (!file_exist((char*)outpath.buf)) {
        //
        char* path = get_resource_path((char*)texture, assetsFiles);
        char* extn = get_file_extension(path);
        //
        if (path!=NULL && extn!=NULL && (extn[0]=='j' || extn[0]=='J')) {   // for Jpeg2000
            //
            JPEG2KImage jpg = readJPEG2KFile(path);
            if (jpg.state==0) {
                MSGraph<uByte> vp = JPEG2KImage2MSGraph<uByte>(jpg);
                DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: texture = %s [size = (%4d,%4d,%2d), mode = %d]\n", texture, jpg.ws, jpg.hs, jpg.col, jpg.cmode);
                //
                if (vp.zs>0) {
                    TGAImage tga = MSGraph2TGAImage(vp);
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
                    make_dummy = false;
                    PRINT_MESG("OARTool::ConvertTexture: ERROR: texture %s convert error (%d).\n", texture, err);
                }
                else {
                    DEBUG_MODE PRINT_MESG("OARTool::ConvertTexture: SUCCESS: texture %s is converted.\n", texture);
                }
            }
        }
        else {
            make_dummy = false;
            PRINT_MESG("OARTool::ConvertTexture: ERROR: texture %s is lost!\n", texture);
        }
    }
    free_Buffer(&outpath);

    //
//  if (make_dummy) MakeDummyTexture(texture, add_name, ext_name, dist);

    return;
}


/**
テスト中！
@param texture    コンバート元データのUUID．
@param add_fname  コンバート先ファイル名の追加文字列．
@param ext_fname  コンバート先ファイル名の拡張子．
@param dist       コンバート先のパス．
*/
void  OARTool::MakeDummyTexture(const char* texture, const char* add_name, const char* ext_name, const char* dist)
{
    if (texture==NULL) return;

    Buffer outpath;
    if (dist==NULL) outpath = make_Buffer_bystr((char*)pathTEX.buf);
    else            outpath = make_Buffer_bystr(dist);

    cat_s2Buffer(texture, &outpath);
    if (add_name!=NULL) {
        cat_s2Buffer("_", &outpath);
        cat_s2Buffer(add_name, &outpath);
    }
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
        Buffer inppath = dup_Buffer(pathAST);
        cat_s2Buffer("dummy.tga", &inppath);

        int ret = copy_file((char*)inppath.buf, (char*)outpath.buf);
        //PRINT_MESG("====> %d %s\n", ret, inppath.buf);
        free_Buffer(&inppath);
    }
    free_Buffer(&outpath);

    return;
}

