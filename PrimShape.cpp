/*
 * Source code of OpenMetaverse is used in part. 
 *
 * see also http://openmetaverse.org/
 * see also OpenMetaverse.Primitive.TextureEntry Class
 */

/*
 * Copyright (c) 2007-2009, openmetaverse.org
 * All rights reserved.
 * 
 * - Redistribution and use in source and binary forms, with or without 
 *   modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * - Neither the name of the openmetaverse.org nor the names 
 *   of its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "PrimShape.h"


using namespace jbxl;


static const std::string _PrimProfType[PRIM_PROF_TYPE_NUM] = 
{
    PRIM_PROF_CIRCLE_STR,
    PRIM_PROF_SQUARE_STR,
    PRIM_PROF_ISOTRIANGLE_STR,
    PRIM_PROF_EQLTRIANGLE_STR,
    PRIM_PROF_RGTTRIANGLE_STR,
    PRIM_PROF_HALFCIRCLE_STR
};


//
static const std::string _PrimHoleType[PRIM_HOLE_TYPE_NUM] = 
{
    PRIM_HOLE_SAME_STR,
    PRIM_HOLE_CIRCLE_STR,
    PRIM_HOLE_SQUARE_STR,
    PRIM_HOLE_TRIANGLE_STR
};


//
static const std::string _PrimBumpMapUUID[PRIM_BUMPMAP_NUM] = 
{
    "00000000-0000-0000-0000-000000000000",     // Brightness
    "ffffffff-ffff-ffff-ffff-ffffffffffff",     // Darkness
    "058c75c0-a0d5-f2f8-43f3-e9699a89c2fc",     // woodgrain
    "6c9fa78a-1c69-2168-325b-3e03ffa348ce",     // bark
    "b8eed5f0-64b7-6e12-b67f-43fa8e773440",     // briks
    "9deab416-9c63-78d6-d558-9a156f12044c",     // checker
    "db9d39ec-a896-c287-1ced-64566217021e",     // concrete
    "f2d7b6f6-4200-1e9a-fd5b-96459e950f94",     // crustytile
    "d9258671-868f-7511-c321-7baef9e948a4",     // cutstone 
    "d21e44ca-ff1c-a96e-b2ef-c0753426b7d9",     // discs        
    "4726f13e-bd07-f2fb-feb0-bfa2ac58ab61",     // gravel       
    "e569711a-27c2-aad4-9246-0c910239a179",     // petridish    
    "073c9723-540c-5449-cdd4-0e87fdc159e3",     // siding       
    "ae874d1a-93ef-54fb-5fd3-eb0cb156afc0",     // stonetile    
    "92e66e00-f56f-598a-7997-048aa64cde18",     // stucco       
    "83b77fc6-10b4-63ec-4de7-f40629f238c5",     // suction      
    "735198cf-6ea0-2550-e222-21d3c6a341ae"      // weave        
};



//////////////////////////////////////////////////////////////////////////////////////////
//

void  PrimBaseShape::init(void) 
{
    ObjectName      = init_Buffer();

    PCode           = PRIM_PCODE_PRIM;
    State           = 0;

    ProfileCurve    = PRIM_PROF_CIRCLE;
    ProfileBegin    = 0;
    ProfileEnd      = 0;
    ProfileHollow   = 0;

    PathCurve       = PRIM_PATH_LINE;
    PathBegin       = 0;
    PathEnd         = 0;
    PathRadiusOffset= 0;
    PathRevolutions = 0;
    PathScaleX      = 100;
    PathScaleY      = 100;
    PathShearX      = 0;
    PathShearY      = 0;
    PathSkew        = 0;
    PathTaperX      = 0;
    PathTaperY      = 0;
    PathTwist       = 0;
    PathTwistBegin  = 0;

    ProfileShape    = PRIM_PROF_CIRCLE;     // same ProfileCurve?
    HollowShape     = PRIM_HOLE_SAME;
    
    SculptType      = 0;
    SculptTexture   = init_Buffer();
    SculptEntry     = false;

    ObjFlags        = init_Buffer();

    // AffineTrans は自身のコンストラクタで初期化される．

    for (int i=0; i<PRIM_MATERIAL_NUM; i++) materialParam[i].init();
    for (int i=0; i<PRIM_MATERIAL_NUM; i++) materialName[i] = init_Buffer();

    return;
}


//
void  PrimBaseShape::free(void)
{
    free_Buffer(&ObjectName);
    free_Buffer(&SculptTexture);
    free_Buffer(&ObjFlags);
    affineTrans.free();

    for (int i=0; i<PRIM_MATERIAL_NUM; i++) materialParam[i].free();
    for (int i=0; i<PRIM_MATERIAL_NUM; i++) free_Buffer(&materialName[i]);
}


//
void  PrimBaseShape::dup(PrimBaseShape p)
{
    *this = p; 

    ObjectName    = dup_Buffer(p.ObjectName);
    SculptTexture = dup_Buffer(p.SculptTexture);
    ObjFlags      = dup_Buffer(p.ObjFlags);

    affineTrans.dup(p.affineTrans);

    for (int i=0;i<PRIM_MATERIAL_NUM;i++) {
        materialParam[i].dup(p.materialParam[i]);
        materialName[i] = dup_Buffer(p.materialName[i]);
    }
}


//
void  PrimBaseShape::GenerateBaseParam(int type)
{
    init();

    switch(type) {
        case PRIM_TYPE_BOX:
            ProfileShape = PRIM_PROF_SQUARE;
            ProfileCurve = PRIM_PROF_SQUARE;
            PathCurve    = PRIM_PATH_LINE;
            PathScaleX   = 100;
            PathScaleY   = 100;
            break;
        case PRIM_TYPE_SPHERE:
            ProfileShape = PRIM_PROF_HALFCIRCLE;
            ProfileCurve = PRIM_PROF_HALFCIRCLE;
            PathCurve    = PRIM_PATH_CIRCLE;
            PathScaleX   = 100;
            PathScaleY   = 100;
            break;
        case PRIM_TYPE_CYLINDER:
            ProfileShape = PRIM_PROF_CIRCLE;
            ProfileCurve = PRIM_PROF_CIRCLE;
            PathCurve    = PRIM_PATH_LINE;
            PathScaleX   = 100;
            PathScaleY   = 100;
            break;
        case PRIM_TYPE_TORUS:
            ProfileShape = PRIM_PROF_CIRCLE;
            ProfileCurve = PRIM_PROF_CIRCLE;
            PathCurve    = PRIM_PATH_CIRCLE;
            PathScaleX   = 100;
            PathScaleY   = 175;
            break;
        case PRIM_TYPE_TUBE:
            ProfileShape = PRIM_PROF_SQUARE;
            ProfileCurve = PRIM_PROF_SQUARE;
            PathCurve    = PRIM_PATH_CIRCLE;
            PathScaleX   = 100;
            PathScaleY   = 175;
            break;
        case PRIM_TYPE_RING:
            ProfileShape = PRIM_PROF_EQUALTRIANGLE;
            ProfileCurve = PRIM_PROF_EQUALTRIANGLE;
            PathCurve    = PRIM_PATH_CIRCLE;
            PathScaleX   = 100;
            PathScaleY   = 175;
            break;
        case PRIM_TYPE_SCULPT:
            SculptType   = 1;
            SculptEntry  = true;
            ProfileShape = PRIM_PROF_CIRCLE;
            ProfileCurve = PRIM_PROF_CIRCLE;
            PathCurve    = PRIM_PATH_CIRCLE;
            PathScaleX   = 100;
            PathScaleY   = 150;
            break;
        default:
            break;
    }
    return;
}


/**
OARの object用 XMLデータをから基本形状データを読み込む．

@param xml 処理する XMLデータ．
@param base ルートプリムのアフィン変換へのポインタ．回転部分を使用する．
*/
void  PrimBaseShape::GetBaseParamFromXML(tXML* xml, AffineTrans<double>* base)
{
    if (xml==NULL) return;
    init();

    // Object Name
    char* objname = get_xml_str_content_bystr(xml, "<Name>");
    if (objname!=NULL) ObjectName = make_Buffer_bystr(objname); 

    PCode = get_xml_int_content_bystr(xml, "<Shape><PCode>");
    State = get_xml_int_content_bystr(xml, "<Shape><State>");

//  ProfileCurve    = get_xml_int_content_bystr(xml, "<Shape><ProfileCurve>");
    ProfileBegin    = get_xml_int_content_bystr(xml, "<Shape><ProfileBegin>");
    ProfileEnd      = get_xml_int_content_bystr(xml, "<Shape><ProfileEnd>");
    ProfileHollow   = get_xml_int_content_bystr(xml, "<Shape><ProfileHollow>");

    PathCurve       = get_xml_int_content_bystr(xml, "<Shape><PathCurve>");
    PathBegin       = get_xml_int_content_bystr(xml, "<Shape><PathBegin>");
    PathEnd         = get_xml_int_content_bystr(xml, "<Shape><PathEnd>");
    PathScaleX      = get_xml_int_content_bystr(xml, "<Shape><PathScaleX>");
    PathScaleY      = get_xml_int_content_bystr(xml, "<Shape><PathScaleY>");
    PathShearX      = get_xml_int_content_bystr(xml, "<Shape><PathShearX>");
    PathShearY      = get_xml_int_content_bystr(xml, "<Shape><PathShearY>");
    PathSkew        = get_xml_int_content_bystr(xml, "<Shape><PathSkew>");
    PathTaperX      = get_xml_int_content_bystr(xml, "<Shape><PathTaperX>");
    PathTaperY      = get_xml_int_content_bystr(xml, "<Shape><PathTaperY>");
    PathTwist       = get_xml_int_content_bystr(xml, "<Shape><PathTwist>");
    PathTwistBegin  = get_xml_int_content_bystr(xml, "<Shape><PathTwistBegin>");

    PathRadiusOffset= get_xml_int_content_bystr(xml, "<Shape><PathRadiusOffset>");
    PathRevolutions = get_xml_int_content_bystr(xml, "<Shape><PathRevolutions>");

    char* cntnt = get_xml_str_content_bystr(xml, "<Shape><ProfileShape>");
    if (cntnt!=NULL) {
        for (int i=0; i<PRIM_PROF_TYPE_NUM; i++) {
            if (!strcasecmp(_PrimProfType[i].c_str(), cntnt)) {
                ProfileShape = i;
                break;
            }
        }
    }
    ProfileCurve = ProfileShape;

    cntnt = get_xml_str_content_bystr(xml, "<Shape><HollowShape>");
    if (cntnt!=NULL) {
        for (int i=0; i<PRIM_HOLE_TYPE_NUM; i++) {
            if (!strcasecmp(_PrimHoleType[i].c_str(), cntnt)) {
                HollowShape = i*16;
                break;
            }
        }
    }

    // Sculpted Prim
    SculptType = get_xml_int_content_bystr(xml, "<Shape><SculptType>");
    char* sculptTex = get_xml_str_content_bystr(xml, "<Shape><SculptTexture><UUID>");
    SculptTexture = make_Buffer_bystr(sculptTex);
    char* entry = get_xml_str_content_bystr(xml, "<Shape><SculptEntry>");
    if (!strcasecmp(entry, "true")) SculptEntry = true;
    else                            SculptEntry = false;

    //
    if (PCode==PRIM_PCODE_AVATAR) {
        //
    }
    else if (PCode==PRIM_PCODE_GRASS) { 
        //
    }
    else if (PCode==PRIM_PCODE_PARTICLE) {  
        //
    }
    else if (PCode==PRIM_PCODE_NEWTREE || PCode==PRIM_PCODE_TREE) {     // Tree
        // State が木の種類を表す
    }

    // Texture Entry
    GetTextureEntry(xml);

    // Extra Params
    GetExtraParams(xml);

    int stype = SculptType & 0x07;
    if (SculptEntry && stype<SCULPT_TYPE_MESH) {    // Sculpted Prim: Texture(Matrial) is one
        for (int i=1; i<PRIM_MATERIAL_NUM; i++) materialParam[i].enable = false;
    }
    for (int i=0; i<PRIM_MATERIAL_NUM; i++) materialParam[i].texture.setAlphaMode(MATERIAL_ALPHA_BLENDING);

    //
    double gposx = (double)get_xml_float_content_bystr(xml, "<GroupPosition><X>");
    double gposy = (double)get_xml_float_content_bystr(xml, "<GroupPosition><Y>");
    double gposz = (double)get_xml_float_content_bystr(xml, "<GroupPosition><Z>");
    double oposx = (double)get_xml_float_content_bystr(xml, "<OffsetPosition><X>");
    double oposy = (double)get_xml_float_content_bystr(xml, "<OffsetPosition><Y>");
    double oposz = (double)get_xml_float_content_bystr(xml, "<OffsetPosition><Z>");
    if (base!=NULL) {
        affineTrans.shift.set(oposx, oposy, oposz);
        affineTrans.shift = base->execRotate(affineTrans.shift);
        affineTrans.shift = affineTrans.shift + Vector<double>(gposx, gposy, gposz);
    }
    else {
        affineTrans.shift.set(gposx+oposx, gposy+oposy, gposz+oposz);
    }
        
    double rotx = (double)get_xml_float_content_bystr(xml, "<RotationOffset><X>");
    double roty = (double)get_xml_float_content_bystr(xml, "<RotationOffset><Y>");
    double rotz = (double)get_xml_float_content_bystr(xml, "<RotationOffset><Z>");
    double rots = (double)get_xml_float_content_bystr(xml, "<RotationOffset><W>");
    affineTrans.rotate.set(rots, rotx, roty, rotz);
    if (base!=NULL) {
        affineTrans.rotate = base->rotate*affineTrans.rotate;
    }
    
    double sclx = (double)get_xml_float_content_bystr(xml, "<Scale><X>");
    double scly = (double)get_xml_float_content_bystr(xml, "<Scale><Y>");
    double sclz = (double)get_xml_float_content_bystr(xml, "<Scale><Z>");
    affineTrans.scale.set(sclx, scly, sclz);
    affineTrans.computeMatrix();

    // Flags  "Physics Phantom TemporaryOnRez"
    char* objflags = get_xml_str_content_bystr(xml, "<Flags>");
    ObjFlags = make_Buffer_bystr(objflags);

    /*
    DEBUG_MODE {
        PRINT_MESG("[DEBUG INFO] in PrimBaseShape::GetBaseParamFromXML\n");
        PrintBaseParam();
        for (int i=0; i<PRIM_MATERIAL_NUM; i++) {
            if (materialParam[i].enable)   PRINT_MESG("Texture : %d %s\n", i, materialParam[i].getTextureName());
            if (materialName[i].buf!=NULL) PRINT_MESG("Material: %d %s\n", i, materialName[i].buf);
        }
        //if (SculptType!=0) PRINT_MESG("Sculpt: %s\n", SculptTexture.buf);
        if (SculptEntry) PRINT_MESG("Sculpt: %s\n", SculptTexture.buf);
        PRINT_MESG("Flags: %s\n", ObjFlags.buf);
    }*/

    return;
}


/**
Texture Entry. 

@sa OpenMetaverse.Primitive.TextureEntry Class of libopenmetaverse
*/
void  PrimBaseShape::GetTextureEntry(tXML* xml)
{
    if (xml==NULL) return;
    
    tXML* texttag = get_xml_content_bystr(xml, "<Shape><TextureEntry>");
    if (texttag!=NULL) {
        int size;
        uByte* textdata = decode_base64(texttag->ldat.key.buf, &size);
    
        /*
        DEBUG_MODE {
            PRINT_MESG("================================================================================== \n");
            fdump(stderr, textdata, size);
            PRINT_MESG("================================================================================== \n");
        }*/
        //
        if (textdata!=NULL) {
            ///////////////////////////////////////////////////////////////
            // Default Entry.
            MaterialParam param = GetDefaultTextureEntry(textdata, size);
            for (int i=0; i<PRIM_MATERIAL_NUM; i++) {
                materialParam[i].free();
                materialParam[i].dup(param);
                if (i>0) materialName[i] = dup_Buffer(materialName[0]);
            }
            param.free();

            //
            uByte* end = textdata + size - 1;
            uByte* ptr = textdata;
            int facetBits, fieldSize;

            // Texture
            ptr += 16;      // default値の読み飛ばし
            while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) {
                uByte* guid = uuid2guid(ptr);
                for (int facet=0, bit=1; facet<fieldSize; facet++, bit<<=1) {
                    if ((facetBits&bit)!=0) {
                        materialParam[facet].setTextureName((char*)guid);
                    }
                }
                ::free(guid);
                ptr += 16;
            }
            // Color
            ptr += 4;       // default値の読み飛ばし
            while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) {
                double red = (255 - ptr[0])/255.0;
                double grn = (255 - ptr[1])/255.0;
                double blu = (255 - ptr[2])/255.0;
                double alp = (255 - ptr[3])/255.0;
                for (int facet=0, bit=1; facet<fieldSize; facet++, bit<<=1) {
                    if ((facetBits&bit)!=0) {
                        if (red==0.0) red = MTRL_DEFAULT_COLOR;
                        if (grn==0.0) grn = MTRL_DEFAULT_COLOR;
                        if (blu==0.0) blu = MTRL_DEFAULT_COLOR;
                        materialParam[facet].texture.setColor(red, 0);
                        materialParam[facet].texture.setColor(grn, 1);
                        materialParam[facet].texture.setColor(blu, 2);
                        materialParam[facet].texture.setColor(alp, 3);
                    }
                }
                ptr += 4;
            }
            // Scale UV
            ptr += 4;       // default値の読み飛ばし
            while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) {
                for (int facet=0, bit=1; facet<fieldSize; facet++, bit<<=1) {
                    if ((facetBits&bit)!=0) {
                        materialParam[facet].texture.setScaleU((double)(*(float*)ptr));
                    }
                }
                ptr += 4;
            }
            ptr += 4;       // default値の読み飛ばし
            while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) {
                for (int facet=0, bit=1; facet<fieldSize; facet++, bit<<=1) {
                    if ((facetBits&bit)!=0) {
                        materialParam[facet].texture.setScaleV((double)(*(float*)ptr));
                    }
                }
                ptr += 4;
            }
            // Shift UV
            ptr += 2;       // default値の読み飛ばし
            while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) {
                double shftu = (*(short int*)ptr)/32767.0;
                for (int facet=0, bit=1; facet<fieldSize; facet++, bit<<=1) {
                    if ((facetBits&bit)!=0) {
                        materialParam[facet].texture.setShiftU(shftu);
                    }
                }
                ptr += 2;
            }
            ptr += 2;       // default値の読み飛ばし
            while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) {
                double shftv = (*(short int*)ptr)/32767.0;
                for (int facet=0, bit=1; facet<fieldSize; facet++, bit<<=1) {
                    if ((facetBits&bit)!=0) {
                        materialParam[facet].texture.setShiftV(shftv);
                    }
                }
                ptr += 2;
            }
            // Rotation
            ptr += 2;       // default値の読み飛ばし
            while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) {
                double rot = (*(short int*)ptr)/32768.0*PI2;
                for (int facet=0, bit=1; facet<fieldSize; facet++, bit<<=1) {
                    if ((facetBits&bit)!=0) {
                        materialParam[facet].texture.setRotate(rot);
                    }
                }
                ptr += 2;
            }
            // Material
            ptr++;          // default値の読み飛ばし    
            while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) {
                for (int facet=0, bit=1; facet<fieldSize; facet++, bit<<=1) {
                    if ((facetBits&bit)!=0) {
                        uByte material = (uByte)(*ptr);
                        int shininess = (int)((material & 0xc0)>>6);
                        materialParam[facet].setShininess(shininess/3.0f);
                        int bright = (int)((material & 0x20)>>5);   // 0 or 1
                        if (bright==1) materialParam[facet].setBright(1.0f);
                        int bumpno = (int)(material & 0x1f);
                        if (bumpno>0 && bumpno<=PRIM_BUMPMAP_NUM) {
                            materialParam[facet].setBumpMapName(_PrimBumpMapUUID[bumpno-1].c_str());
                        }
                    }
                }
                ptr++;
            }
            // Media
            ptr++;          // default値の読み飛ばし
            while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) {
                for (int facet=0, bit=1; facet<fieldSize; facet++, bit<<=1) {
                    if ((facetBits&bit)!=0) {
                        uByte media = (uByte)(*ptr);
                        materialParam[facet].mapping = (int)(media & 0x06); // テクスチャのマッピング方法: MATERIAL_MAPPING_*
                        materialParam[facet].mflags  = (int)(media & 0x01); // Media Flags
                        materialParam[facet].others  = (int)(media & 0xf8); // Unused
                    }
                }
                ptr++;
            }
            // Glow
            ptr++;          // default値の読み飛ばし
            while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) {
                double glow = (*ptr)/255.0;
                for (int facet=0, bit=1; facet<fieldSize; facet++, bit<<=1) {
                    if ((facetBits&bit)!=0) {
                        materialParam[facet].setGlow(glow);
                    }
                }
                ptr++;
            }
            // Material UUID 16Byte
            ptr += 16;      // default値の読み飛ばし
            while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) {
                uByte* guid = uuid2guid(ptr);
                for (int facet=0, bit=1; facet<fieldSize; facet++, bit<<=1) {
                    if ((facetBits&bit)!=0) {
                        // indicate to asset/GUID_material.xml that includes texture bumpmap
                        free_Buffer(&materialName[facet]);
                        materialName[facet] = make_Buffer_bystr((char*)guid);
                    }
                }
                ::free(guid);
                ptr += 16;
            }

            ::free(textdata);
        }
    }

    return;
}


MaterialParam  PrimBaseShape::GetDefaultTextureEntry(uByte* ptr, int size)
{
    MaterialParam param;
    uByte* end = ptr + size - 1;
    int facetBits, fieldSize;

    // Texture
    uByte* guid = uuid2guid(ptr);
    param.setTextureName((char*)guid);
    ::free(guid);
    ptr += 16;
    while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) ptr += 16;

    // Color
    double red = (255 - ptr[0])/255.0;      // R
    double grn = (255 - ptr[1])/255.0;      // G
    double blu = (255 - ptr[2])/255.0;      // B
    double alp = (255 - ptr[3])/255.0;      // A
    if (red==0.0) red = MTRL_DEFAULT_COLOR;
    if (grn==0.0) grn = MTRL_DEFAULT_COLOR;
    if (blu==0.0) blu = MTRL_DEFAULT_COLOR;
    param.texture.setColor(red, 0);                 // R
    param.texture.setColor(grn, 1);                 // G
    param.texture.setColor(blu, 2);                 // B
    param.texture.setColor(alp, 3);                 // A
    ptr += 4;
    while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) ptr += 4;

    // Scale UV
    param.texture.setScaleU((double)(*(float*)ptr));
    ptr += 4;
    while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) ptr += 4;
    param.texture.setScaleV((double)(*(float*)ptr));
    ptr += 4;
    while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) ptr += 4;

    // Shift UV
    param.texture.setShiftU((*(short int*)ptr)/32767.0);
    ptr += 2;
    while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) ptr += 2;
    param.texture.setShiftV(((*(short int*)ptr)/32767.0));
    ptr += 2;
    while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) ptr += 2;

    // Rotation
    param.texture.setRotate(((*(short int*)ptr)/32768.0*PI2));
    ptr += 2;
    while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) ptr += 2;

    // Material
    uByte material = (uByte)(*ptr);
    int shininess = (int)((material & 0xc0)>>6);
    param.setShininess(shininess/3.0f);
    int bright = (int)((material & 0x20)>>5);   // 0 or 1
    if (bright==1) param.setBright(1.0f);
    int bumpno = (int)(material & 0x1f);
    if (bumpno>0 && bumpno<=PRIM_BUMPMAP_NUM) {
        param.setBumpMapName(_PrimBumpMapUUID[bumpno-1].c_str());
    }
    ptr++;
    while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) ptr++;

    // Media
    uByte media = (uByte)(*ptr);
    param.mapping = (int)(media & 0x06);        // テクスチャのマッピング方法: MATERIAL_MAPPING_*
    param.mflags  = (int)(media & 0x01);        // Media Flags
    param.others  = (int)(media & 0xf8);        // Unused
    ptr++;
    while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) ptr++;

    // Glow
    param.setGlow((double)(*ptr)/255.0f);
    ptr++;
    while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) ptr++;

    // Material UUID
    if (ptr+16<=end) {
        guid = uuid2guid(ptr);
        materialName[0] = make_Buffer_bystr((char*)guid);   // indicate to asset/GUID_material.xml that includes texture bumpmap
        ::free(guid);
        ptr+=16;
    }
    //while (ptr<end && ReadFacetBitField(&ptr, &facetBits, &fieldSize)) ptr++;

    param.enable = true;

    return param;
}


/**
@sa OpenMetaverse.Primitive.TextureEntry Class
*/
bool  PrimBaseShape::ReadFacetBitField(uByte** data, int* facetBits, int* fieldSize)
{
    *facetBits = 0;
    *fieldSize = 0;
    uByte b = 0;

    do {
        b = **data;
        *facetBits = (*facetBits<<7) | (b&0x7f);
        *fieldSize += 7;
        (*data)++;
    } while ((b&0x80)!=0);

    return (*facetBits!=0);
}


/**
Extra Params of Textures. 

@sa http://wiki.secondlife.com/wiki/ExtraParams
@sa http://www.nsl.tuis.ac.jp/xoops/modules/xpwiki/?OAR%2Fobjects%2FExtraParams

 for Flexible, Light, Sculpted, Light Image.
*/
void  PrimBaseShape::GetExtraParams(tXML* xml)
{
    if (xml==NULL) return;
    
    tXML* prmstag = get_xml_content_bystr(xml, "<Shape><ExtraParams>");
    if (prmstag!=NULL) {
        int size;
        uByte* prmsdata = decode_base64(prmstag->ldat.key.buf, &size);

        /*
        DEBUG_MODE {
            PRINT_MESG("==================================================================================\n");
            fdump(stderr, prmsdata, size);
            PRINT_MESG("==================================================================================\n");
        }*/

        int pos = 0;
        uByte* ptr = prmsdata;
        if (ptr!=NULL) {
            if (*ptr!=0x00) {
                pos++;
                unsigned short type = (unsigned short)(*((unsigned short*)(ptr+pos)));
                //
                if (type==16) {     // 0x0010 Flexible
                    pos += 2;
                    int len = (int)(*((int*)(ptr+pos)));
                    //
                    // not implemented
                    //
                    pos += len + 4;
                    if (pos+1<size) type = (unsigned short)(*((unsigned short*)(ptr+pos)));
                    else            type = 0;
                }
                //
                if (type==32) {     // 0x0020 Light
                    for (int i=0; i<PRIM_MATERIAL_NUM; i++) materialParam[i].setLight(1.0);
                    //
                    pos += 2;
                    int len = (int)(*((int*)(ptr+pos)));
                    //
                    // not implemented
                    //
                    pos += len + 4;
                    if (pos+1<size) type = (unsigned short)(*((unsigned short*)(ptr+pos)));
                    else            type = 0;
                }
                //
                // not implemented
                //
            }
            ::free(prmsdata);
        }
    }

    return;
}


//
//
#define  SPEC_COLOR_TAG_R  "<key>SpecColor</key><array><integer>"
#define  SPEC_COLOR_TAG_G  "<key>SpecColor</key><array><integer></integer><integer>"
#define  SPEC_COLOR_TAG_B  "<key>SpecColor</key><array><integer></integer><integer></integer><integer>"
#define  SPEC_COLOR_TAG_A  "<key>SpecColor</key><array><integer></integer><integer></integer><integer></integer><integer>"

//
void  PrimBaseShape::GetMaterialParams(tList* resourceList)
{
    if (resourceList==NULL) return;

    for (int i=0; i<PRIM_MATERIAL_NUM; i++) {
        char* matname = (char*)materialName[i].buf;
        if (matname!=NULL) {
            char* path = get_resource_path(matname, resourceList);  
            if (path!=NULL && strcmp(matname, "00000000-0000-0000-0000-000000000000")) {
                DEBUG_MODE PRINT_MESG("PrimBaseShape::GetMaterialParams: reading material file %s\n", path);
                //
                tXML* xml = xml_parse_file(path);
                if (xml!=NULL) {
                    materialParam[i].texture.setAlphaMode  (get_xml_int_content_bystr(xml, "<key>DiffuseAlphaMode</key><integer>"));
                    materialParam[i].texture.setAlphaCutoff(get_xml_int_content_bystr(xml, "<key>AlphaMaskCutoff</key><integer>")/255.0);
                    materialParam[i].setGlossiness (get_xml_int_content_bystr(xml, "<key>SpecExp</key><integer>")/255.0);
                    materialParam[i].setEnvironment(get_xml_int_content_bystr(xml, "<key>EnvIntensity</key><integer>")/255.0);
                    //
                    double colorR = get_xml_int_content_bystr(xml, SPEC_COLOR_TAG_R)/255.0;
                    double colorG = get_xml_int_content_bystr(xml, SPEC_COLOR_TAG_G)/255.0;
                    double colorB = get_xml_int_content_bystr(xml, SPEC_COLOR_TAG_B)/255.0;
                    double colorA = get_xml_int_content_bystr(xml, SPEC_COLOR_TAG_A)/255.0;
                    if (colorR==0.0) colorR = MTRL_DEFAULT_COLOR;
                    if (colorG==0.0) colorG = MTRL_DEFAULT_COLOR;
                    if (colorB==0.0) colorB = MTRL_DEFAULT_COLOR;
                    materialParam[i].specmap.setColor(colorR, colorG, colorB, colorA);

                    char* specmap = get_xml_str_content_bystr(xml, "<key>SpecMap</key><uuid>");
                    if (specmap!=NULL && strcmp(specmap, "00000000-0000-0000-0000-000000000000")) {
                        materialParam[i].specmap.setName(specmap);
                        materialParam[i].specmap.setShiftU(get_xml_int_content_bystr(xml, "<key>SpecOffsetX</key><integer>") /10000.0);
                        materialParam[i].specmap.setShiftV(get_xml_int_content_bystr(xml, "<key>SpecOffsetY</key><integer>") /10000.0);
                        materialParam[i].specmap.setScaleU(get_xml_int_content_bystr(xml, "<key>SpecRepeatX</key><integer>") /10000.0);
                        materialParam[i].specmap.setScaleV(get_xml_int_content_bystr(xml, "<key>SpecRepeatY</key><integer>") /10000.0);
                        materialParam[i].specmap.setRotate(get_xml_int_content_bystr(xml, "<key>SpecRotation</key><integer>")/10000.0);
                    }
                    //
                    char* bumpmap = get_xml_str_content_bystr(xml, "<key>NormMap</key><uuid>");
                    if (bumpmap!=NULL && strcmp(bumpmap, "00000000-0000-0000-0000-000000000000")) {
                        materialParam[i].bumpmap.setName(bumpmap);
                        materialParam[i].bumpmap.setShiftU(get_xml_int_content_bystr(xml, "<key>NormOffsetX</key><integer>") /10000.0);
                        materialParam[i].bumpmap.setShiftV(get_xml_int_content_bystr(xml, "<key>NormOffsetY</key><integer>") /10000.0);
                        materialParam[i].bumpmap.setScaleU(get_xml_int_content_bystr(xml, "<key>NormRepeatX</key><integer>") /10000.0);
                        materialParam[i].bumpmap.setScaleV(get_xml_int_content_bystr(xml, "<key>NormRepeatY</key><integer>") /10000.0);
                        materialParam[i].bumpmap.setRotate(get_xml_int_content_bystr(xml, "<key>NormRotation</key><integer>")/10000.0);
                    }
                    //
                    materialParam[i].enable = true;
                    del_all_xml(&xml);
                }

            }
        }
    }
    return;
}


//
void  PrimBaseShape::PrintTextureEntry(void)
{
    PRINT_MESG("[DEBUG INFO] in PrimBaseShape::PrintTextureEntry\n");
    for (int i=0; i<PRIM_MATERIAL_NUM; i++) {
        if (materialParam[i].enable) {
            PRINT_MESG("num         : %d\n", i); 
            PRINT_MESG("texture     : %s\n", materialParam[i].getTextureName()); 
            PRINT_MESG("material    : %s\n", materialName[i].buf);
            //
            PRINT_MESG("color       : %f, %f, %f, %f\n", (float)materialParam[i].texture.getColor(0), (float)materialParam[i].texture.getColor(1),
                                                         (float)materialParam[i].texture.getColor(2), (float)materialParam[i].texture.getColor(3));
            PRINT_MESG("scale       : %f, %f\n", (float)materialParam[i].texture.getScaleU(), (float)materialParam[i].texture.getScaleV());
            PRINT_MESG("shift       : %f, %f\n", (float)materialParam[i].texture.getShiftU(), (float)materialParam[i].texture.getShiftV());
            PRINT_MESG("rotate      : %f\n", (float)materialParam[i].texture.getRotate());
            PRINT_MESG("transparent : %f\n", (float)materialParam[i].texture.getColor(3));
            PRINT_MESG("shininess   : %f\n", (float)materialParam[i].getShininess());
            PRINT_MESG("glow        : %f\n", (float)materialParam[i].getGlow());
            PRINT_MESG("bright      : %f\n", (float)materialParam[i].getBright());
            PRINT_MESG("light       : %f\n", (float)materialParam[i].getLight());
            //
            PRINT_MESG("mapping     : %d\n", materialParam[i].mapping);
            PRINT_MESG("flags       : %d\n", materialParam[i].mflags);
            if (materialParam[i].texture.getAlphaChannel()) PRINT_MESG("alphaChannel: true\n");
            else                                            PRINT_MESG("alphaChannel: false\n");
            PRINT_MESG("alphaMode   : %d\n", materialParam[i].texture.getAlphaMode());
            PRINT_MESG("alphaCutoff : %f\n", (float)materialParam[i].texture.getAlphaCutoff());
            PRINT_MESG("\n");
        }
    }
    PRINT_MESG("\n");
}


//
void  PrimBaseShape::PrintBaseParam(void)
{
    PRINT_MESG("PrimBaseShape.ObjectName   = %s\n", ObjectName.buf);
    PRINT_MESG("PrimBaseShape.PCode        = %d\n", PCode);
    PRINT_MESG("PrimBaseShape.State        = %d\n", State);
    PRINT_MESG("PrimBaseShape.ProfileCurve = %d\n", ProfileCurve);
    PRINT_MESG("PrimBaseShape.PathCurve    = %d\n", PathCurve);

    int n = ProfileShape;
    if (n>=0 && n<PRIM_PROF_TYPE_NUM) PRINT_MESG("PrimBaseShape.ProfileShape = %s\n", _PrimProfType[n].c_str());
    n = HollowShape/16;
    if (n>=0 && n<PRIM_HOLE_TYPE_NUM) PRINT_MESG("PrimBaseShape.HollowShape  = %s\n", _PrimHoleType[n].c_str());

    PRINT_MESG("----------------------------------\n");
    PRINT_MESG("PrimBaseShape.ProfileBegin  = %d\n", ProfileBegin);
    PRINT_MESG("PrimBaseShape.ProfileEnd    = %d\n", ProfileEnd);
    PRINT_MESG("PrimBaseShape.ProfileHollow = %d\n", ProfileHollow);

    PRINT_MESG("----------------------------------\n");
    PRINT_MESG("PrimBaseShape.PathBegin  = %d\n", PathBegin);
    PRINT_MESG("PrimBaseShape.PathEnd    = %d\n", PathEnd);
    PRINT_MESG("PrimBaseShape.PathScaleX = %d\n", PathScaleX);
    PRINT_MESG("PrimBaseShape.PathScaleY = %d\n", PathScaleY);
    PRINT_MESG("PrimBaseShape.PathShearX = %d\n", PathShearX);
    PRINT_MESG("PrimBaseShape.PathShearY = %d\n", PathShearY);
    PRINT_MESG("PrimBaseShape.PathSkew   = %d\n", PathSkew);
    PRINT_MESG("PrimBaseShape.PathTaperX = %d\n", PathTaperX);
    PRINT_MESG("PrimBaseShape.PathTaperY = %d\n", PathTaperY);
    PRINT_MESG("PrimBaseShape.PathTwist  = %d\n", PathTwist);
    PRINT_MESG("PrimBaseShape.PathTwistBegin = %d\n", PathTwistBegin);

    PRINT_MESG("PrimBaseShape.PathRadiusOffset = %d\n", PathRadiusOffset);
    PRINT_MESG("PrimBaseShape.PathRevolutions  = %d\n", PathRevolutions);

    PRINT_MESG("----------------------------------\n");
    PRINT_MESG("PrimBaseShape.SculptType = %d\n", SculptType);
    if (SculptTexture.buf!=NULL) PRINT_MESG("PrimBaseShape.SculptTexture = %s\n", SculptTexture.buf);
    PRINT_MESG("PrimBaseShape.ObjFlags = %s\n", ObjFlags.buf);

    return;
}



//////////////////////////////////////////////////////////////////////////////////////////

/**
OARの object用 XMLデータから プリムの基本形状データを読み込む．

@param xml      検索を行う xmlデータ．
@param rsclist  key部にリソース名，val部に assetリソースのパスを格納したリスト．
@param *sno     読み込んだ基本形状データの個数．

@return  読み込んだプリムの基本形状データの配列．複数の場合は最初のプリムが ルートプリムである．
*/
PrimBaseShape*  jbxl::CreatePrimBaseShapesFromXML(tXML* xml, tList* rsclist, int* sno)
{
    if (sno!=NULL) *sno = 0;
    if (xml==NULL) return NULL;

    tList* lp = get_xml_node_list_bystr(xml, "<SceneObjectPart><Shape>");
    int count = count_tList(lp);
    if (count==0) return NULL;

    PrimBaseShape* shapes = (PrimBaseShape*)malloc(count*sizeof(PrimBaseShape));
    if (shapes==NULL) return NULL;
    memset(shapes, 0, count*sizeof(PrimBaseShape));

    AffineTrans<double> base;   // ROOTプリムのアフィン変換
    int n = 0;
    while (lp!=NULL) {
        PrimBaseShape rparam;
        if (n==0) {     // 最初の(ROOT)プリム
            rparam.GetBaseParamFromXML(lp->altp, NULL);
            base.dup(rparam.affineTrans);
        }
        else {          // 子プリム
            rparam.GetBaseParamFromXML(lp->altp, &base);
        }
        rparam.GetMaterialParams(rsclist);
        shapes[n].dup(rparam);

        ///////////////////////////////////////////
        //DEBUG_MODE rparam.PrintTextureEntry();
        ///////////////////////////////////////////
        
        rparam.free();
        lp = lp->next;
        n++;
    }
    base.free();

    if (sno!=NULL) *sno = count;
    return shapes;
}



//////////////////////////////////////////////////////////////////////////////////////////
// PrimShapeParam
//

void  PrimShapeParam::init(void)
{
    objectName     = init_Buffer();

    pCode          = PRIM_PCODE_PRIM;
    state          = 0;

    profCurve      = PRIM_PROF_CIRCLE;
    pathCurve      = PRIM_PATH_LINE;
    hollowType     = PRIM_HOLE_SAME;

    profBegin      = 0.0;
    profEnd        = 1.0;
    profHollow     = 0.0;

    pathBegin      = 0.0;
    pathEnd        = 1.0;
    pathScaleX     = 1.0;
    pathScaleY     = 0.25;
    pathShearX     = 0.0;
    pathShearY     = 0.0;
    pathTaperX     = 0.0;
    pathTaperY     = 0.0;
    pathTwistBegin = 0.0;
    pathTwistEnd   = 0.0;
    pathSkew       = 0.0;
    pathRadius     = 0.0;
    pathRevol      = 1.0;

    sculptType     = 0;
    sculptTexture  = init_Buffer();
    sculptEntry    = false;

    objFlags       = init_Buffer();

    // AffineTrans は自身のコンストラクタで初期化される．

    for (int i=0; i<PRIM_MATERIAL_NUM; i++) materialParam[i].init();

    return;
}


//
void  PrimShapeParam::free(void)
{
    free_Buffer(&objectName);
    free_Buffer(&sculptTexture);
    free_Buffer(&objFlags);
    affineTrans.free();

    for (int i=0; i<PRIM_MATERIAL_NUM; i++) materialParam[i].free();
}


//
void  PrimShapeParam::dup(PrimShapeParam p)
{
    *this = p; 

    objectName    = dup_Buffer(p.objectName);
    sculptTexture = dup_Buffer(p.sculptTexture);
    objFlags      = dup_Buffer(p.objFlags);

    affineTrans.dup(p.affineTrans);

    for (int i=0;i<PRIM_MATERIAL_NUM;i++) {
        materialParam[i].dup(p.materialParam[i]);
    }
}


//
void  PrimShapeParam::GetParamFromBaseShape(PrimBaseShape shape)
{
    init();

    objectName  = dup_Buffer(shape.ObjectName);

    pCode       = shape.PCode;
    state       = shape.State;
    profCurve   = shape.ProfileCurve;
    pathCurve   = shape.PathCurve;

    profBegin   = shape.ProfileBegin*PRIM_CUT_QUANTA;
    profEnd     = 1.0f - shape.ProfileEnd*PRIM_CUT_QUANTA;
    profHollow  = shape.ProfileHollow*PRIM_HOLLOW_QUANTA;
    if (profHollow>0.95f) profHollow = 0.95;
    hollowType  = shape.HollowShape;

    pathBegin   = shape.PathBegin*PRIM_CUT_QUANTA;
    pathEnd     = 1.0f - shape.PathEnd*PRIM_CUT_QUANTA;

    pathScaleX  = 2.0f - shape.PathScaleX*PRIM_SCALE_QUANTA;
    pathScaleY  = 2.0f - shape.PathScaleY*PRIM_SCALE_QUANTA;

    // Taper
    pathTaperX  = shape.PathTaperX*PRIM_TAPER_QUANTA;
    pathTaperY  = shape.PathTaperY*PRIM_TAPER_QUANTA;
    if (pathCurve==PRIM_PATH_LINE || pathCurve==PRIM_PATH_FLEXIBLE) {   // by Fumi.Iseki for OAR 0.8
        pathTaperX  = 1.0f - pathScaleX;
        pathTaperY  = 1.0f - pathScaleY;
    }

    pathShearX  = shape.PathShearX < 128 ? (double)shape.PathShearX : shape.PathShearX-256.0;
    pathShearY  = shape.PathShearY < 128 ? (double)shape.PathShearY : shape.PathShearY-256.0;
    pathShearX *= PRIM_SHEAR_QUANTA;
    pathShearY *= PRIM_SHEAR_QUANTA;

    pathTwistBegin = shape.PathTwistBegin*PRIM_SCALE_QUANTA*PI2;    // Radian
    pathTwistEnd   = shape.PathTwist*PRIM_SCALE_QUANTA*PI2;         // Radian

    pathRadius  = shape.PathRadiusOffset*PRIM_SCALE_QUANTA;
    pathSkew    = shape.PathSkew*PRIM_SCALE_QUANTA;
    pathRevol   = shape.PathRevolutions*PRIM_REV_QUANTA + 1.0f;

    sculptType  = shape.SculptType;
    sculptTexture = dup_Buffer(shape.SculptTexture);
    sculptEntry = shape.SculptEntry;
    objFlags    = dup_Buffer(shape.ObjFlags);

    affineTrans.free();
    affineTrans.dup(shape.affineTrans);
    affineTrans.shift.set(affineTrans.shift.x, affineTrans.shift.y, affineTrans.shift.z);

    for (int i=0; i<PRIM_MATERIAL_NUM; i++) {
        if (shape.materialParam[i].enable) {
            materialParam[i].free();
            materialParam[i].dup(shape.materialParam[i]);
        }
    }

    /*
    DEBUG_MODE {
        PRINT_MESG("[DEBUG INFO] in PrimShapeParam::GetParamFromBaseShape\n");
        PrintParam();
        PRINT_MESG("\n");
        for (int i=0; i<PRIM_MATERIAL_NUM; i++) {
            if (materialParam[i].enable) PRINT_MESG("%d: %s\n", i, materialParam[i].getTextureName());
        }
        PRINT_MESG("\n");
    }*/

    return;
}
    

//
void  PrimShapeParam::PrintParam(void)
{
    PRINT_MESG("PrimShapeParam.objectName  = %s\n", objectName.buf);
    PRINT_MESG("PrimShapeParam.pCode       = %d\n", pCode);
    PRINT_MESG("PrimShapeParam.state       = %d\n", state);
    PRINT_MESG("PrimShapeParam.profCurve   = %d\n", profCurve);
    PRINT_MESG("PrimShapeParam.pathCurve   = %d\n", pathCurve);

    PRINT_MESG("----------------------------------\n");
    PRINT_MESG("PrimShapeParam.profBegin   = %f\n", (float)profBegin);
    PRINT_MESG("PrimShapeParam.profEnd     = %f\n", (float)profEnd);
    PRINT_MESG("PrimShapeParam.profHollow  = %f\n", (float)profHollow);

    PRINT_MESG("----------------------------------\n");
    PRINT_MESG("PrimShapeParam.pathBegin   = %f\n", (float)pathBegin);
    PRINT_MESG("PrimShapeParam.pathEnd     = %f\n", (float)pathEnd);
    PRINT_MESG("PrimShapeParam.pathScaleX  = %f\n", (float)pathScaleX);
    PRINT_MESG("PrimShapeParam.pathScaleY  = %f\n", (float)pathScaleY);
    PRINT_MESG("PrimShapeParam.pathShearX  = %f\n", (float)pathShearX);
    PRINT_MESG("PrimShapeParam.pathShearY  = %f\n", (float)pathShearY);
    PRINT_MESG("PrimShapeParam.pathSkew    = %f\n", (float)pathSkew);
    PRINT_MESG("PrimShapeParam.pathTaperX  = %f\n", (float)pathTaperX);
    PRINT_MESG("PrimShapeParam.pathTaperY  = %f\n", (float)pathTaperY);
    PRINT_MESG("PrimShapeParam.pathTwistBegin = %f\n", (float)pathTwistBegin);
    PRINT_MESG("PrimShapeParam.pathTwistEnd   = %f\n", (float)pathTwistEnd);

    PRINT_MESG("PrimShapeParam.pathRadius  = %f\n", (float)pathRadius);
    PRINT_MESG("PrimShapeParam.pathRevol   = %f\n", (float)pathRevol);

    PRINT_MESG("----------------------------------\n");
    if (sculptEntry) PRINT_MESG("PrimShapeParam.sculptEntry = true\n");
    else             PRINT_MESG("PrimShapeParam.sculptEntry = false\n");
    PRINT_MESG("PrimShapeParam.sculptType  = %d\n", sculptType);
    if (sculptTexture.buf!=NULL) PRINT_MESG("PrimShapeParam.sculptTexture = %s\n", sculptTexture.buf);
    PRINT_MESG("PrimShapeParam.objFlags    = %s\n", objFlags.buf);

    return;
}

