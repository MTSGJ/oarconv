
/**
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
    trnName = make_Buffer_bystr(name);

    xsize = xs;
    ysize = ys;
    rgnum = xs/256;
    scale = (float)TRNT_DEFAULT_TEX_SCALE;

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

    free_Buffer(&trnName);

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

    del_xml(&xml);
    return;
}


//
void  TerrainTool::ReadHeightData(const char* path)
{
    if (path==NULL) return;

    int sz = (int)file_size(path);
    if (sz!=xsize*ysize*4) {   // 4==sizeof(float)
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

    TerrainTexWeight weight;
    for (int i=0; i<4; i++) {
        weight.w[i] = (1.0f-alph)*(1.0f-beta)*sw.w[i] + alph*(1.0f-beta)*se.w[i] + (1.0f-alph)*beta*nw.w[i] + alph*beta*ne.w[i];
    }

    return weight;
}



/**
@sa http://www.nsl.tuis.ac.jp/xoops/modules/xpwiki/?OpenSim%2FDiorama2
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

    int lsize = TRNT_TEXTURE_SIZE*rgnum;

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
        msg = scalingMSGraph2D(vp[mode-1], lsize/(float)vp[mode-1].xs);
    }
        
    return msg;
}



MSGraph<uByte>  TerrainTool::GenerateWeightedTexture(MSGraph<uByte>* vp)
{
    MSGraph<uByte> msg;

    int lsize = TRNT_TEXTURE_SIZE*rgnum;
    msg.getm(lsize, lsize, 3);
    if (msg.isNull()) return msg;

    TerrainTexWeight weight;
    float vscale = scale/xsize;
    for (int k=0; k<3; k++) {
        PRINT_MESG("GenerateWeightedTexture: generating region texture. %d/3\n", k+1);
#ifdef WIN32
        DisPatcher();
#endif
        int kk = k*msg.xs*msg.ys;
        for (int j=0; j<lsize; j++) {
            int jj = j*msg.xs + kk;
            float dy = (float)j/lsize;
            float ry = dy*ysize;                                    // Region Y座標
            for (int i=0; i<lsize; i++) {
                float dx = (float)i/lsize;
                float rx = dx*xsize;                                // Region X座標
                weight = GetTextureWeight((int)rx, (int)ry);
                //
                float sum = 0.0f;
                for (int m=0; m<4; m++) {
                    int cl = k;
                    if (vp[m].zs==1) cl = 0;
                    sum += weight.w[m] * GetScalingPixel(&vp[m], vscale*vp[m].xs, dx*vp[m].xs, dy*vp[m].ys, cl);
                }
                if (sum>=256.0f) sum = 255.0f;
                msg.gp[jj+i] = (int)sum;
            }
        }
    }
    //
    return msg;
}



void  TerrainTool::GenerateTexture(tList* assets, const char* outpath, bool add_param)
{
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
            return;
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
    if (msg.isNull()) return;

    //
    // Textureファイルの生成（256x256以上のTerrainは分割される）
    for (int rj=0; rj<rgnum; rj++) {
        int yy = (rgnum-rj-1)*TRNT_TEXTURE_SIZE*msg.xs;
        for (int ri=0; ri<rgnum; ri++) {
            int xx = ri*TRNT_TEXTURE_SIZE + yy;

            DEBUG_MODE {
                if (rgnum>1) PRINT_MESG("TerrainTool::GenerateTexture: generating sub region texture. %02d/%d\n", count, rgnum*rgnum-1);
                else         PRINT_MESG("TerrainTool::GenerateTexture: generating region texture file.\n");
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
                        region.gp[jj+i] = msg.gp[ww+i];
                    }
                }
            }
            region.color = GRAPH_COLOR_RGB;

            Buffer trrntex = dup_Buffer(trnName);
            if (rgnum>1) {
                snprintf(num, L_OCT-1, "_%02d", count);
                cat_s2Buffer(num, &trrntex);
            }
            rewrite_sBuffer_str(&trrntex, "?", "_");
            rewrite_sBuffer_str(&trrntex, " ", "_");

            Buffer texfile = make_Buffer_bystr(outpath);

            MaterialParam param;
            param.setTextureName((char*)trrntex.buf);
            param.setColor(1.0f, 1.0f, 1.0f, 1.0f);
            param.setTransparent(1.0f);
            //
            if (add_param) {
                char* paramstr = param.getBase64Params('E');   // E: Earth
                param.setParamString(paramstr);
                param.setFullName(MTRL_IMAGE_TYPE);
                if (paramstr!=NULL) ::free(paramstr);
            }

            char* filename = param.getTextureName();
            cat_s2Buffer(filename, &texfile);
            TGAImage tga = MSGraph2TGAImage(region);
            writeTGAFile((char*)texfile.buf, tga);
            tga.free();

            param.free();
            region.free();
            free_Buffer(&trrntex);
            free_Buffer(&texfile);

            count++;
        }
    }

    msg.free();

    return;
}



void  TerrainTool::GenerateDae(const char* outpath, Vector<float> offset, bool add_param)
{
    if (r32.isNull()) return;

    char num[L_OCT];
    int  count = 0;

    bool left;
    bool right;
    bool top;
    bool bottom;

    //
    DEBUG_MODE PRINT_MESG("TerrainTool::GenerateDae: generating sub region mesh.\n");
    for (int rj=0; rj<rgnum; rj++) {
        top = bottom = false;
        int jsize = 257;                // 境界用
        if (rj==0) top = true;
        if (rj==rgnum-1) {
            bottom = true;
            jsize = 256;
        }
        int yy = rj*256*xsize;
        //
        for (int ri=0; ri<rgnum; ri++) {
            left = right = false;
            int isize = 257;            // 境界用
            if (ri==0) left = true;
            if (ri==rgnum-1) {
                right = true;
                isize = 256;
            }
            int xx = ri*256 + yy;

            DEBUG_MODE if (rgnum>1) PRINT_MESG("TerrainTool::GenerateDae: generating sub region mesh. %02d/%d\n", count, rgnum*rgnum-1);
            Vector<float> shift = Vector<float>(ri*256.0f-xsize/2.0f+offset.x, rj*256.0f-ysize/2.0f+offset.y, -waterHeight+offset.z);
            MSGraph<float> region;

            region.getm(isize, jsize);
            for (int j=0; j<jsize; j++) {
                int jj = j*isize;
                int ww = j*xsize + xx;
                for (int i=0; i<isize; i++) {
                    region.gp[jj+i] = r32.gp[ww+i];
                }
            }
            FacetBaseData* facetdata = FacetBaseDataFromTerrainImage(region, shift, left, right, top, bottom, false);

            //
            Buffer daename = dup_Buffer(trnName);
            if (rgnum>1) {
                snprintf(num, L_OCT-1, "_%02d", count);
                cat_s2Buffer(num, &daename);
            }
            rewrite_sBuffer_str(&daename, "?", "_");
            rewrite_sBuffer_str(&daename, " ", "_");

            Buffer daepath = make_Buffer_bystr(outpath);
            Buffer texfile = dup_Buffer(daename);

            MaterialParam param;
            param.setTextureName(get_file_name((char*)texfile.buf));
            param.setColor(1.0f, 1.0f, 1.0f, 1.0f);
            param.setTransparent(1.0f);
            //
            if (add_param) {
                char* paramstr = param.getBase64Params('E');   // E: Earth
                param.setParamString(paramstr);
                param.setFullName(MTRL_IMAGE_TYPE);
                if (paramstr!=NULL) ::free(paramstr);
            }

            MeshObjectData* data = new MeshObjectData();
            data->addData(facetdata, &param);
            data->setMaterialParam(param);

            ColladaXML* dae = new ColladaXML();
            dae->setBlankTexture(PRIM_OS_BLANK_TEXTURE);
            dae->addObject(data, true);
            dae->outputFile((char*)daename.buf, (char*)daepath.buf);

            param.free();
            region.free();
            free_Buffer(&daename);
            free_Buffer(&daepath);
            free_Buffer(&texfile);
            freeFacetBaseData(facetdata);
            freeColladaXML(dae);
            freeMeshObjectData(data);

            count++;
        }
    }

    return;
}



void  TerrainTool::GenerateSTL(const char* outpath, Vector<float> offset, bool binfile)
{
    if (r32.isNull()) return;

    char num[L_OCT];
    int  count = 0;

    bool left;
    bool right;
    bool top;
    bool bottom;

    //
    DEBUG_MODE PRINT_MESG("TerrainTool::GenerateSTL: generating sub region mesh.\n");
    for (int rj=0; rj<rgnum; rj++) {
        top = bottom = false;
        int jsize = 257;                // 境界用
        if (rj==0) top = true;
        if (rj==rgnum-1) {
            bottom = true;
            jsize = 256;
        }
        int yy = rj*256*xsize;
        //
        for (int ri=0; ri<rgnum; ri++) {
            left = right = false;
            int isize = 257;            // 境界用
            if (ri==0) left = true;
            if (ri==rgnum-1) {
                right = true;
                isize = 256;
            }
            int xx = ri*256 + yy;

            DEBUG_MODE if (rgnum>1) PRINT_MESG("TerrainTool::GenerateSTL: generating sub region mesh. %02d/%d\n", count, rgnum*rgnum-1);
            Vector<float> shift = Vector<float>(ri*256.0f-xsize/2.0f+offset.x, rj*256.0f-ysize/2.0f+offset.y, -waterHeight+offset.z);
            MSGraph<float> region;

            region.getm(isize, jsize);
            for (int j=0; j<jsize; j++) {
                int jj = j*isize;
                int ww = j*xsize + xx;
                for (int i=0; i<isize; i++) {
                    region.gp[jj+i] = r32.gp[ww+i];
                }
            }
            FacetBaseData* facetdata = FacetBaseDataFromTerrainImage(region, shift, left, right, top, bottom, true);

            //
            Buffer stlname = dup_Buffer(trnName);
            if (rgnum>1) {
                snprintf(num, L_OCT-1, "_%02d", count);
                cat_s2Buffer(num, &stlname);
            }
            rewrite_sBuffer_str(&stlname, " ", "_");
            Buffer stlpath = make_Buffer_bystr(outpath);

            MeshObjectData* data = new MeshObjectData();
            data->addData(facetdata, NULL);

            BrepSolidList* solids = new BrepSolidList();
            solids->addObject(data);
            solids->outputFile((char*)stlname.buf, (char*)stlpath.buf, binfile);

            region.free();
            free_Buffer(&stlname);
            free_Buffer(&stlpath);
            freeFacetBaseData(facetdata);
            freeMeshObjectData(data);
            freeBrepSolidList(solids);

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
    TerrainTexWeight weight;

    float hsz = (max - min)/2.0f;
    float dh  = hsz/4.0f;

    float h0  = min;
    float h1  = min + hsz;
    float h2  = max;

    if      (h<h0-dh) weight.w[0] = 1.0f;
    else if (h>h0+dh) weight.w[0] = 0.0f;
    else              weight.w[0] = 1.0f - 0.5f/dh*(h - h0+dh);

    if      (h<h0-dh) weight.w[1] = 0.0f;
    else if (h>h1+dh) weight.w[1] = 0.0f;
    else if (h>=h0-dh && h<=h0+dh) weight.w[1] = 0.5f/dh*(h - h0+dh);
    else if (h>=h1-dh && h<=h1+dh) weight.w[1] = 1.0f - 0.5f/dh*(h - h1+dh);
    else              weight.w[1] = 1.0f;
    
    if      (h<h1-dh) weight.w[2] = 0.0f;
    else if (h>h2+dh) weight.w[2] = 0.0f;
    else if (h>=h1-dh && h<=h1+dh) weight.w[2] = 0.5f/dh*(h - h1+dh);
    else if (h>=h2-dh && h<=h2+dh) weight.w[2] = 1.0f - 0.5f/dh*(h - h2+dh);
    else              weight.w[2] = 1.0f;
        
    if      (h<h2-dh) weight.w[3] = 0.0f;
    else if (h>h2+dh) weight.w[3] = 1.0f;
    else              weight.w[3] = 0.5f/dh*(h - h2+dh);

    float ttl = weight.w[0] + weight.w[1] + weight.w[2] + weight.w[3];
    if (ttl<=0.0f) {
        weight.w[1] = 1.0f;
        weight.w[0] = weight.w[2] = weight.w[3] = 0.0f;
    }
    else {
        weight.w[0] /= ttl;
        weight.w[1] /= ttl;
        weight.w[2] /= ttl;
        weight.w[3] /= ttl;
    }

    return weight;
}



/**
画像データ vpからスケーリングファクターに従って,座標 (x, y, col) の輝度値を取り出す．
x, yが intでなのは，スケーリングファクタとの演算があるから．@n
縮尺は X方向 scale/vp.xs倍, Y方向 scale/vp.ys倍となる．
従って，スケーリングファクタが vpのサイズと同じ場合は縮尺は 1になる．@n
 
@param vp      画像データの入ったMSGraph<uByte>変数へのポインタ．
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




///////////////////////////////////////////////////////////////////////////////////////////////
// Junk
//

