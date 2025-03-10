﻿/**
@brief  OpenSimの Terrain用ツール

@file   TerrainTool.cpp
@author Fumi.Iseki 
@date   2014 10/5
*/

#include "TerrainTool.h"


using namespace jbxl;


///////////////////////////////////////////////////////////////////////////////////////////////
//

void  TerrainTool::init(char* name, int xs, int ys)
{
    terrainName = make_Buffer_bystr(name);

    xsize = xs;
    ysize = ys;
    x_region_num = xs/256;
    y_region_num = ys/256;
    scale = (float)TRNT_DEFAULT_TEX_SCALE;

    engine        = JBXL_3D_ENGINE_UNITY;
    dataFormat    = JBXL_3D_FORMAT_DAE;
    textureFormat = JBXL_TEXTURE_PNG;

    r32.init();
    for (int i=0; i<4; i++) texture[i] = init_Buffer();

    defaultTexture[0] = make_Buffer_bystr("b8d3965a-ad78-bf43-699b-bff8eca6c975");      // Terrain Dirt
    defaultTexture[1] = make_Buffer_bystr("abb783e6-3e93-26c0-248a-247666855da3");      // Terrain Grass
    defaultTexture[2] = make_Buffer_bystr("179cdabd-398a-9b6b-1391-4dc333ba321f");      // Terrain Mountain
    defaultTexture[3] = make_Buffer_bystr("beb169c7-11ea-fff2-efe5-0f24dc881df2");      // Terrain Rock

    minNW = 10.0f;
    maxNW = 60.0f;
    minNE = 10.0f;
    maxNE = 60.0f;
    minSW = 10.0f;
    maxSW = 60.0f;
    minSE = 10.0f;
    maxSE = 60.0f;

    waterHeight = 20.0f;
}


//
void  TerrainTool::free(void)
{
    r32.free();

    free_Buffer(&terrainName);

    for (int i=0; i<4; i++) {
        free_Buffer(&texture[i]);
        free_Buffer(&defaultTexture[i]);
    }
}


//
void  TerrainTool::ReadSettings(const char* path)
{
    if (path==NULL) return;

    tXML* xml = xml_parse_file(path);
    if (xml==NULL) {
        PRINT_MESG("TerrainTool::ReadSettings:   ERROR: file read error! [%s]\n", path);
        return;
    }

    char* text[4];
    text[0] = get_xml_char_content_bystr(xml, "<RegionSettings><GroundTextures><Texture1>");
    text[1] = get_xml_char_content_bystr(xml, "<RegionSettings><GroundTextures><Texture2>");
    text[2] = get_xml_char_content_bystr(xml, "<RegionSettings><GroundTextures><Texture3>");
    text[3] = get_xml_char_content_bystr(xml, "<RegionSettings><GroundTextures><Texture4>");
    for (int i=0; i<4; i++) {
        free_Buffer(&texture[i]);
        if (text[i]!=NULL) {
            texture[i] = make_Buffer_bystr(text[i]);
        }
        else {
            texture[i] = dup_Buffer(defaultTexture[i]);
        }           
    }

    minNW = get_xml_float_content_bystr(xml, "<RegionSettings><GroundTextures><ElevationLowNW>");
    maxNW = get_xml_float_content_bystr(xml, "<RegionSettings><GroundTextures><ElevationHighNW>");
    minNE = get_xml_float_content_bystr(xml, "<RegionSettings><GroundTextures><ElevationLowNE>");
    maxNE = get_xml_float_content_bystr(xml, "<RegionSettings><GroundTextures><ElevationHighNE>");
    minSW = get_xml_float_content_bystr(xml, "<RegionSettings><GroundTextures><ElevationLowSW>");
    maxSW = get_xml_float_content_bystr(xml, "<RegionSettings><GroundTextures><ElevationHighSW>");
    minSE = get_xml_float_content_bystr(xml, "<RegionSettings><GroundTextures><ElevationLowSE>");
    maxSE = get_xml_float_content_bystr(xml, "<RegionSettings><GroundTextures><ElevationHighSE>");
    waterHeight = get_xml_float_content_bystr(xml, "<RegionSettings><Terrain><WaterHeight>");

    del_all_xml(&xml);
    return;
}


//
void  TerrainTool::ReadHeightData(const char* path)
{
    if (path==NULL) return;

    int sz = (int)file_size(path);
    if (sz!=xsize*ysize*sizeof(float)) {
        if (sz==0)  PRINT_MESG("ReadHeightData: ERROR: file read error! [%s]\n", path);
        else        PRINT_MESG("ReadHeightData: ERROR: file size missmatch %d != %dx%dx4 [%s]\n", sz, xsize, ysize, path);
        return;
    }

    DEBUG_MODE PRINT_MESG("ReadHeightData: reading terrain r32 file %s\n", path);
    FILE* fp = fopen(path, "rb");
    if (fp!=NULL) {
        if (!r32.isNull()) r32.free();
        r32.getm(xsize, ysize);
        if (r32.isNull()) return;
        fread(r32.gp, r32.xs*r32.ys*sizeof(float), 1, fp);
        fclose(fp);
    }

    return;
}


//
TerrainTexWeight  TerrainTool::GetTextureWeight(int x, int y)
{
    float h = height(x, y);
    float alph = (float)x/xsize;
    float beta = (float)y/ysize;

    TerrainTexWeight nw = GetTerrainBaseTextureWeight(h, minNW, maxNW);
    TerrainTexWeight ne = GetTerrainBaseTextureWeight(h, minNE, maxNE);
    TerrainTexWeight sw = GetTerrainBaseTextureWeight(h, minSW, maxSW);
    TerrainTexWeight se = GetTerrainBaseTextureWeight(h, minSE, maxSE);

    TerrainTexWeight tweight;
    for (int i=0; i<4; i++) {
        tweight.w[i] = (1.0f-alph)*(1.0f-beta)*sw.w[i] + alph*(1.0f-beta)*se.w[i] + (1.0f-alph)*beta*nw.w[i] + alph*beta*ne.w[i];
    }

    return tweight;
}


/**
@sa https://polaris.star-dust.jp/pukiwiki/?OpenSim/Diorama2
*/
int  TerrainTool::GetDioramaMode(void)
{
    int mode1 = 0;
    int mode2 = 0;

    if (minNW==0.0 && maxNW==0.0) {
        mode1 = 1;
        mode2++;
    }
    if (minNE==0.0 && maxNE==0.0) {
        mode1 = 2;
        mode2++;
    }
    if (minSW==0.0 && maxSW==0.0) {
        mode1 = 3;
        mode2++;
    }
    if (minSE==0.0 && maxSE==0.0) {
        mode1 = 4;
        mode2++;
    }

    if (mode2==4) {
        mode2++;
        return mode2;
    }
    return mode1;
}


MSGraph<uByte>  TerrainTool::GenerateDioramaTexture(MSGraph<uByte>* vp, int mode)
{
    MSGraph<uByte> msg;
    if (mode<=0) return msg;

    int  x_lsize = TRNT_TEXTURE_SIZE * x_region_num;
    int  y_lsize = TRNT_TEXTURE_SIZE * y_region_num;
    //
    if (mode>4) {
        if (vp[0].xs!=vp[1].xs || vp[0].xs!=vp[2].xs || vp[0].xs!=vp[3].xs) return msg;
        if (vp[0].ys!=vp[1].ys || vp[0].ys!=vp[2].ys || vp[0].ys!=vp[3].ys) return msg;

        MSGraph<uByte> gd;
        int xo = vp[0].xs;
        int yo = vp[0].ys;
        int xs = xo*2;
        int ys = yo*2;
        int zs = vp[0].zs;
        int po = xo*yo;
        int ps = xs*ys;

        gd.getm(xs, ys, zs);
        gd.color = vp[0].color;

        for (int k=0; k<zs; k++) {
            int posk = k*ps;
            int posn = k*po;
            //
            for (int j=0; j<yo;  j++) {
                int posj = j*xs + posk;
                int posm = j*xo + posn;
                //
                for (int i=0; i<xo; i++) {
                    gd.gp[i+posj] = vp[0].gp[i+posm];
                }
                for (int i=xo; i<xs; i++) {
                    gd.gp[i+posj] = vp[1].gp[i-xo+posm];
                }
            }
            //
            for (int j=yo; j<ys; j++) {
                int posj = j*xs + posk;
                int posm = (j-yo)*xo + posn;
                //
                for (int i=0; i<xo; i++) {
                    gd.gp[i+posj] = vp[2].gp[i+posm];
                }
                for (int i=xo; i<xs; i++) {
                    gd.gp[i+posj] = vp[3].gp[i-xo+posm];
                }
            }
        }
        
        msg = scalingMSGraph2D(gd, 4.0);
        gd.free();
    }
    else {
        float scale = Max(x_lsize/(float)vp[mode-1].xs, y_lsize/(float)vp[mode-1].ys);
        msg = scalingMSGraph2D(vp[mode-1], scale);
    }
    return msg;
}


MSGraph<uByte>  TerrainTool::GenerateWeightedTexture(MSGraph<uByte>* vp)
{
    MSGraph<uByte> msg;

    int x_lsize = TRNT_TEXTURE_SIZE * x_region_num;
    int y_lsize = TRNT_TEXTURE_SIZE * y_region_num;
    msg.getm(x_lsize, y_lsize, 3);
    if (msg.isNull()) return msg;

    TerrainTexWeight tweight;
    memset(&tweight, 0, sizeof(tweight));

    float vscale = scale/xsize;
    for (int k=0; k<3; k++) {
        PRINT_MESG("GenerateWeightedTexture: generating region texture. %d/3\n", k+1);
#ifdef WIN32
        DisPatcher();
#endif
        int kk = k*msg.xs*msg.ys;
        for (int j=0; j<y_lsize; j++) {
#ifdef WIN32
            DisPatcher();
#endif
            int jj = j*msg.xs + kk;
            float dy = (float)j/y_lsize;
            float ry = dy*ysize;                                    // Region Y座標
            for (int i=0; i<x_lsize; i++) {
#ifdef WIN32
                DisPatcher();
#endif
                float dx = (float)i/x_lsize;
                float rx = dx*xsize;                                // Region X座標
                tweight = GetTextureWeight((int)rx, (int)ry);
                //
                float sum = 0.0f;
                for (int m=0; m<4; m++) {
                    int cl = k;
                    if (vp[m].zs==1) cl = 0;
                    sum += tweight.w[m] * GetScalingPixel(&vp[m], vscale*vp[m].xs, dx*vp[m].xs, dy*vp[m].ys, cl);
                }
                if (sum>=256.0f) sum = 255.0f;
                msg.gp[jj+i] = (int)sum;
            }
        }
    }
    return msg;
}


int  TerrainTool::GenerateTexture(tList* assets, const char* outpath)
{
    if (assets==NULL)  return JBXL_ARGS_ERROR;
    if (outpath==NULL) return JBXL_ARGS_ERROR;

    MSGraph<uByte> vp[4];
    MSGraph<uByte> msg;

    char num[L_OCT];
    int  count = 0;

    //
    // Textureデータの読み込みと変換
    for (int i=0; i<4; i++) {
        char* asset_path = get_resource_path((char*)texture[i].buf, assets);
        if (asset_path==NULL) asset_path = get_resource_path((char*)defaultTexture[i].buf, assets);
        if (asset_path==NULL) { // texture genaration is failed
            PRINT_MESG("TerrainTool::GenerateTexture: ERROR: texture %s is lost!\n", defaultTexture[i].buf);
            for (int j=0; j<=i; j++) vp[j].free();
            return JBXL_ERROR;
        }
        DEBUG_MODE PRINT_MESG("Terrain Texture %d is %s\n", i, asset_path);
        //
        JPEG2KImage jpg = readJPEG2KFile(asset_path);
        vp[i] = JPEG2KImage2MSGraph<uByte>(jpg);
        jpg.free();
    }

    //
    // Texture の生成
    int mode = GetDioramaMode();
    if (mode==0) msg = GenerateWeightedTexture(vp);
    else         msg = GenerateDioramaTexture (vp, mode);
    //
    for (int i=0; i<4; i++) vp[i].free();
    if (msg.isNull()) return JBXL_ERROR;

    //
    // Textureファイルの生成（256x256以上のTerrainは分割される）
    char* image_type = GetTextureExtension();
    for (int rj=0; rj<y_region_num; rj++) {
        int yy = (y_region_num-rj-1)*TRNT_TEXTURE_SIZE*msg.xs;
        for (int ri=0; ri<x_region_num; ri++) {
            int xx = ri*TRNT_TEXTURE_SIZE + yy;

            DEBUG_MODE {
                if (x_region_num>1 || y_region_num>1) {
                    PRINT_MESG("TerrainTool::GenerateTexture: generating sub region texture. %02d/%d\n", count, x_region_num*y_region_num);
                }
                else {
                    PRINT_MESG("TerrainTool::GenerateTexture: generating region texture file.\n");
                }
            }
#ifdef WIN32
            DisPatcher();   // Windows用ディスパッチャー
#endif
            MSGraph<uByte> region;
            region.set(TRNT_TEXTURE_SIZE, TRNT_TEXTURE_SIZE, 3);

            for (int k=0; k<3; k++) {
                int kk = k*region.xs*region.ys;
                int vv = k*msg.xs*msg.ys;
                for (int j=0; j<region.ys; j++) {
                    int jj = j*region.xs + kk;
                    int ww = j*msg.xs + xx + vv;
                    for (int i=0; i<region.xs; i++) {
                        region.gp[jj + i] = msg.gp[ww + i];
                    }
                }
            }
            region.color = GRAPH_COLOR_RGB;

            Buffer trrntex = dup_Buffer(terrainName);
            if (x_region_num>1 || y_region_num>1) {
                snprintf(num, L_OCT-1, "_%02d", count);
                cat_s2Buffer(num, &trrntex);
            }
            Buffer texfile = make_Buffer_bystr(outpath);

            MaterialParam param;
            param.setTextureName((char*)trrntex.buf);
            //param.setTransparent(1.0f);
            param.texture.setColor(1.0f, 1.0f, 1.0f, 1.0f);
            //
            param.setKind('E');                         // E: Earth
            char* paramstr = param.getBase64Params();
            param.setParamString(paramstr);
            param.setFullName(image_type);
            if (paramstr!=NULL) ::free(paramstr);

            Buffer filename = make_Buffer_str(param.getTextureName());
            canonical_filename_Buffer(&filename, TRUE);
            cat_Buffer(&filename, &texfile);

            if (textureFormat==JBXL_TEXTURE_TGA) {
                TGAImage tga = MSGraph2TGAImage(region, true);
                writeTGAFile((char*)texfile.buf, &tga);
                tga.free();
            }
            else {  // default
                PNGImage png = MSGraph2PNGImage(region);
                writePNGFile((char*)texfile.buf, &png);
                png.free();
            }

            param.free();
            region.free();
            free_Buffer(&filename);
            free_Buffer(&trrntex);
            free_Buffer(&texfile);

            count++;
        }
    }
    msg.free();
    return JBXL_NORMAL;
}


//
// for DAE/OBJ/STL/glTF
//
void  TerrainTool::GenerateTerrain(const char* outpath, Vector<double> offset)
{
    if (r32.isNull()) return;

    char num[L_OCT];
    int  count = 0;

    bool left;
    bool right;
    bool top;
    bool bottom;

    //
    DEBUG_MODE PRINT_MESG("TerrainTool::GenerateTerrain: generating sub region mesh.\n");
    char* image_type = GetTextureExtension();
    for (int rj=0; rj<y_region_num; rj++) {
        top = bottom = false;
        int jsize = 257;                // 境界用
        if (rj==0) top = true;
        if (rj==y_region_num-1) {
            bottom = true;
            jsize = 256;
        }
        int yy = rj*256*xsize;
        //
        for (int ri=0; ri<x_region_num; ri++) {
            left = right = false;
            int isize = 257;            // 境界用
            if (ri==0) left = true;
            if (ri==x_region_num-1) {
                right = true;
                isize = 256;
            }
            int xx = ri*256 + yy;

            if (x_region_num>1 || y_region_num>1) {
                PRINT_MESG("TerrainTool::GenerateTerrain: generating sub region mesh. %02d/%d\n", count, x_region_num*y_region_num);
            }

            MSGraph<float> region;
            region.getm(isize, jsize);
            for (int j=0; j<jsize; j++) {
                int jj = j*isize;
                int ww = j*xsize + xx;
                for (int i=0; i<isize; i++) {
                    region.gp[jj + i] = r32.gp[ww + i];
                }
            }

            Vector<float>  shift = Vector<float>(ri*256.0f + (float)offset.x, rj*256.0f + (float)offset.y, (float)offset.z);
            Vector<float> center = Vector<float>(128.0f, 128.0f, waterHeight);
            shift = shift + center;
            ContourBaseData* facetdata = ContourBaseDataFromTerrainImage(region, -center, left, right, top, bottom, false);
            //
            Buffer objname = dup_Buffer(terrainName);
            if (x_region_num>1 || y_region_num>1) {
                snprintf(num, L_OCT-1, "_%02d", count);
                cat_s2Buffer(num, &objname);
            }

            Buffer path = make_Buffer_bystr(outpath);
            Buffer texfile = dup_Buffer(objname);

            MaterialParam param;
            param.setTextureName(get_file_name((char*)texfile.buf));
            //param.setTransparent(1.0f);
            param.texture.setColor(1.0f, 1.0f, 1.0f, 1.0f);
            //
            param.setKind('E');                             // E: Earth
            char* paramstr = param.getBase64Params();
            param.setParamString(paramstr);
            param.setFullName(image_type);                  // + .png/.tga
            if (paramstr!=NULL) ::free(paramstr);

            // create Geometory Data
            MeshObjectData* data = new MeshObjectData();
            data->addData(facetdata, &param);
            data->setMaterialParam(param);
            data->data_name = dup_Buffer(objname);
            data->alt_name  = dup_Buffer(objname);
            cat_s2Buffer("_Node", &data->alt_name);
            //if (engine==JBXL_3D_ENGINE_UE) ins_s2Buffer(OART_UE_COLLIDER_PREFIX, &objname);
            //
            data->affineTrans = new AffineTrans<double>();
            data->affineTrans->setShift((double)shift.x, (double)shift.y, (double)shift.z);
            data->affineTrans->computeMatrix();

            // 縮退状態
            if (noOffset) {
                setDegenerateFname(&objname, engine, data->affineTrans->getShift(), OART_LOCATION_MAGIC_STR);
            }
/*
            if (noOffset) {
                float position[3];
                int len = sizeof(float) * 3;
                memset(position, 0, len);
                //
                if (engine==JBXL_3D_ENGINE_UE) {
                    position[0] =  (float)(data->affineTrans->shift.x*100.0);    // 100 is Unreal Unit
                    position[1] = -(float)(data->affineTrans->shift.y*100.0);
                    position[2] =  (float)(data->affineTrans->shift.z*100.0);
                }
                else {
                    position[0] = -(float)(data->affineTrans->shift.x);
                    position[1] =  (float)(data->affineTrans->shift.z);
                    position[2] = -(float)(data->affineTrans->shift.y);
                }
                char* params = (char*)encode_base64_filename((unsigned char*)position, len, '-');
                del_file_extension_Buffer(&objname);
                cat_s2Buffer("_", &objname);
                cat_s2Buffer(OART_LOCATION_MAGIC_STR, &objname);
                cat_s2Buffer(params, &objname);
                cat_s2Buffer(".", &objname);
                DEBUG_MODE PRINT_MESG("TerrainTool::GenerateTerrain: offset (%f, %f, %f) to filename (%s).\n", position[0], position[1], position[2], params);
                ::free(params);
            }
*/
            //
            ColladaXML*    dae  = NULL;
            OBJData*       obj  = NULL;
            GLTFData*      gltf = NULL;
            FBXData*       fbx  = NULL;
            BrepSolidList* stl  = NULL;

            // DAE
            if (dataFormat==JBXL_3D_FORMAT_DAE) {
                dae = new ColladaXML();
                dae->no_offset   = noOffset;
                dae->phantom_out = false;
                dae->has_joints  = false;
                dae->setBlankTexture(PRIM_OS_BLANK_TEXTURE);
                //
                dae->addShell(data, true, NULL);
                dae->closeSolid();
                dae->outputFile((char*)objname.buf, (char*)path.buf, XML_SPACE_FORMAT);
                freeColladaXML(dae);
            }
            // OBJ
            else if (dataFormat==JBXL_3D_FORMAT_OBJ) {
                obj = new OBJData();        // アンカー
                obj->no_offset   = noOffset;
                obj->phantom_out = false;
                obj->setEngine(engine);
                //
                obj->addShell(data, true);
                Vector<double> offset = obj->execAffineTrans();         // no_offset==true の場合，原点縮退
                obj->affineTrans->setShift(offset);
                obj->affineTrans->computeMatrix();
                obj->closeSolid();
                //obj->outputFile((char*)objname.buf, (char*)path.buf, OART_DEFAULT_PTM_DIR, OART_DEFAULT_TEX_DIR, OART_DEFAULT_MTL_DIR);
                obj->outputFile((char*)objname.buf, (char*)path.buf, OART_DEFAULT_TEX_DIR, OART_DEFAULT_MTL_DIR);
                freeOBJData(obj);
            }
            // GLTF
            else if (dataFormat==JBXL_3D_FORMAT_GLTF || dataFormat==JBXL_3D_FORMAT_GLB) {
                gltf = new GLTFData();
                gltf->no_offset   = noOffset;
                gltf->phantom_out = false;
                gltf->setEngine(engine);
                gltf->glb_out = false;
                if (dataFormat==JBXL_3D_FORMAT_GLB) gltf->glb_out = true;
                gltf->addShell(data, true);
                gltf->closeSolid();
                //
                //gltf->outputFile((char*)objname.buf, (char*)path.buf, OART_DEFAULT_PTM_DIR, OART_DEFAULT_TEX_DIR, OART_DEFAULT_BIN_DIR);
                gltf->outputFile((char*)objname.buf, (char*)path.buf, OART_DEFAULT_TEX_DIR, OART_DEFAULT_BIN_DIR);
                freeGLTFData(gltf);
            }
            // FBX
            else if (dataFormat==JBXL_3D_FORMAT_FBX) {
                // Not Implemented yet
                fbx = new FBXData();
                fbx->no_offset   = noOffset;
                fbx->phantom_out = false;
                fbx->setEngine(engine);
                fbx->addShell(data, true);
                fbx->closeSolid();
                //
                //fbx->outputFile((char*)objname.buf, (char*)path.buf, OART_DEFAULT_PTM_DIR, OART_DEFAULT_TEX_DIR, OART_DEFAULT_BIN_DIR);
                fbx->outputFile((char*)objname.buf, (char*)path.buf, OART_DEFAULT_TEX_DIR, OART_DEFAULT_BIN_DIR);
                freeFBXData(fbx);
            }
            // STL
            else if (dataFormat==JBXL_3D_FORMAT_STL_A || dataFormat==JBXL_3D_FORMAT_STL_B) {
                bool ascii = true;
                if (dataFormat==JBXL_3D_FORMAT_STL_B) ascii = false;
                stl = new BrepSolidList();
                stl->addShell(data);
                stl->closeSolid();
                stl->outputFile(get_file_name((char*)objname.buf), (char*)path.buf, ascii);
                freeBrepSolidList(stl);
            }
            //
            free_Buffer(&objname);
            free_Buffer(&path);
            param.free();
            region.free();
            free_Buffer(&texfile);
            freeContourBaseData(facetdata);
            freeMeshObjectData(data);
            count++;
        }
    }
    return;
}



///////////////////////////////////////////////////////////////////////////////////////////////
// Tools
//

/**
リージョンの各コーナーでのテクスチャの重みを計算する．
*/
TerrainTexWeight  jbxl::GetTerrainBaseTextureWeight(float h, float min, float max)
{
    TerrainTexWeight tweight;
    memset(&tweight, sizeof(tweight), 0);

/*
    float dh = (max - min)/7.0;

    // Low
    if      (h <= min)    tweight.w[0] = 1.0f;
    else if (h >= min+dh) tweight.w[0] = 0.0f;
    else                  tweight.w[0] = (min + dh - h)/dh;
    //
    if      (h <= min)                       tweight.w[1] = 0.0f;
    else if (h >= min      && h <= min+  dh) tweight.w[1] = (h - min)/dh;
    else if (h >= min+  dh && h <= min+3*dh) tweight.w[1] = 1.0f;
    else if (h >= min+3*dh && h <= min+4*dh) tweight.w[1] = (min + 4*dh - h)/dh;
    else if (h >= min+4*dh)                  tweight.w[1] = 0.0f;

    if      (h <= min+3*dh)                  tweight.w[2] = 0.0f;
    else if (h >= min+3*dh && h <= min+4*dh) tweight.w[2] = (h - min - 3*dh)/dh;
    else if (h >= min+4*dh && h <= min+6*dh) tweight.w[2] = 1.0f;
    else if (h >= min+6*dh && h <= min+7*dh) tweight.w[2] = (min + 7*dh - h)/dh;
    else if (h >= min+7*dh)                  tweight.w[2] = 0.0f;
    // High
    if      (h >= min+7*dh) tweight.w[3] = 1.0f;
    else if (h <= min+6*dh) tweight.w[3] = 0.0f;
    else                    tweight.w[3] = (h - min - 6*dh)/dh;
*/

    // Referring to Singularity Viewer
    float hsz = (max - min)/2.0f;
    float dh  = hsz/4.0f;

    float h0  = min;
    float h1  = min + hsz;
    float h2  = max;

    if      (h<h0-dh) tweight.w[0] = 1.0f;
    else if (h>h0+dh) tweight.w[0] = 0.0f;
    else              tweight.w[0] = 1.0f - 0.5f/dh*(h - h0+dh);

    if      (h<h0-dh) tweight.w[1] = 0.0f;
    else if (h>h1+dh) tweight.w[1] = 0.0f;
    else if (h>=h0-dh && h<=h0+dh) tweight.w[1] = 0.5f/dh*(h - h0+dh);
    else if (h>=h1-dh && h<=h1+dh) tweight.w[1] = 1.0f - 0.5f/dh*(h - h1+dh);
    else              tweight.w[1] = 1.0f;
    
    if      (h<h1-dh) tweight.w[2] = 0.0f;
    else if (h>h2+dh) tweight.w[2] = 0.0f;
    else if (h>=h1-dh && h<=h1+dh) tweight.w[2] = 0.5f/dh*(h - h1+dh);
    else if (h>=h2-dh && h<=h2+dh) tweight.w[2] = 1.0f - 0.5f/dh*(h - h2+dh);
    else              tweight.w[2] = 1.0f;
        
    if      (h<h2-dh) tweight.w[3] = 0.0f;
    else if (h>h2+dh) tweight.w[3] = 1.0f;
    else              tweight.w[3] = 0.5f/dh*(h - h2+dh);

    //
    float ttl = tweight.w[0] + tweight.w[1] + tweight.w[2] + tweight.w[3];
    if (ttl<=0.0f) {
        tweight.w[1] = 1.0f;
        tweight.w[0] = tweight.w[2] = tweight.w[3] = 0.0f;
    }
    else {
        tweight.w[0] /= ttl;
        tweight.w[1] /= ttl;
        tweight.w[2] /= ttl;
        tweight.w[3] /= ttl;
    }

    return tweight;
}


/**
画像データ vpからスケーリングファクターに従って,座標 (x, y, col) の輝度値を取り出す．
x, yが intでなのは，スケーリングファクタとの演算があるから．@n
縮尺は X方向 scale/vp.xs倍, Y方向 scale/vp.ys倍となる．
従って，スケーリングファクタが vpのサイズと同じ場合は縮尺は 1になる．@n
 
@param vp     画像データの入ったMSGraph<uByte>変数へのポインタ．
@param scale  スケーリングファクタ．
@param x      画像の X座標
@param y      画像の Y座標
@param col    画像の Z座標．または色

@return  縮尺の位置の輝度値．
*/
int  jbxl::GetScalingPixel(MSGraph<uByte>* vp, float scale, float x, float y, int col)
{
    float xx = x/scale;
    float yy = y/scale;
    
    int ii = (int)((xx-(int)xx)*vp->xs);
    int jj = (int)((yy-(int)yy)*vp->ys);
    if (ii>=vp->xs) ii = vp->xs - 1;
    if (jj>=vp->ys) jj = vp->ys - 1;

    return (int)(vp->gp[col*vp->xs*vp->ys + jj*vp->xs + ii]);
}

