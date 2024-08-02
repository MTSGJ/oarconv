/**
@brief  OAR用の 汎用ツール
@file   GeneralTool.cpp

@author Fumi.Iseki 
@date   2024 7/27
*/

#include "GeneralTool.h"


using namespace jbxl;


tList*  jbxl::AlphaChannelList = NULL;  // テクスチャがアルファチャンネルを持つかどうかの情報を格納したリスト


///////////////////////////////////////////////////////////////////////////////////////////////////
//

bool  jbxl::hasValidAlphaChannel(const char* texture, tList* resourceList)
{
    bool ret = false;
    if (texture==NULL) return ret;

    Buffer inppath = make_Buffer_bystr(texture);
#ifndef WIN32
    rewrite_sBuffer_str(&inppath, " ", "\\ ");
#endif

    tList* pp = strncasecmp_tList(AlphaChannelList, (char*)inppath.buf, 0, 1);
    if (pp!=NULL) {
        if (pp->ldat.lv) return true;
        else             return false;
    }

    //
    char* path = get_resource_path((char*)inppath.buf, resourceList);
    char* extn = get_file_extension(path);
    //
    if (path!=NULL && extn!=NULL) {
        // for Jpeg
        if (!strcasecmp(extn, "jpeg") || !strcasecmp(extn, "jpg")) {
            ret = false;
        }

        // for PNG
        if (!strcasecmp(extn, "png")) {
            PNGImage png = readPNGFile(path);
            MSGraph<uByte> vp = PNGImage2MSGraph<uByte>(png);

            if (vp.zs==4 || vp.zs==2) {
                int   psz = vp.xs*vp.ys;
                uByte* pp = vp.gp + psz*(vp.zs-1);
                for (int i=0; i<psz; i++) {
                    if (pp[i]!=255) {
                        ret = true;
                        break;
                    }
                }
            }
            vp.free();
            png.free();
        }

        // for TGA
        else if (!strcasecmp(extn, "tga")) {
            TGAImage tga = readTGAFile(path);
            MSGraph<uByte> vp = TGAImage2MSGraph<uByte>(tga);

            if (vp.zs==4 || vp.zs==2) {
                int   psz = vp.xs*vp.ys;
                uByte* pp = vp.gp + psz*(vp.zs-1);
                for (int i=0; i<psz; i++) {
                    if (pp[i]!=255) {
                        ret = true;
                        break;
                    }
                }
            }
            vp.free();
            tga.free();
        }

        // for Jpeg2000
        else if (extn[0]=='j' || extn[0]=='J') {
            JPEG2KImage jpg = readJPEG2KFile(path);
            MSGraph<uByte> vp = JPEG2KImage2MSGraph<uByte>(jpg);

            if (vp.zs==4 || vp.zs==2) {
                int   psz = vp.xs*vp.ys;
                uByte* pp = vp.gp + psz*(vp.zs-1);
                for (int i=0; i<psz; i++) {
                    if (pp[i]!=255) {
                        ret = true;
                        break;
                    }
                }
            }
            vp.free();
            jpg.free();
        }
    }

    // List
    int lv = FALSE;
    if (ret) lv = TRUE;
    pp = add_tList_node_bystr(AlphaChannelList, 0, lv, (char*)inppath.buf, NULL, NULL, 0);
    if (AlphaChannelList==NULL) AlphaChannelList = pp;

    free_Buffer(&inppath);
    return ret;
}


void  jbxl::setDegenerateFname(Buffer* out_fname, int engine, Vector<double> shift, const char* magic_str)
{
    float offset[3];
    int len = sizeof(float) * 3;
    memset(offset, 0, len);
    if (engine==JBXL_3D_ENGINE_UE) {  // UE
        offset[0] =  (float)(shift.x * 100.);    // 100 is Unreal Unit
        offset[1] = -(float)(shift.y * 100.);
        offset[2] =  (float)(shift.z * 100.);
    }
    else {                                  // Unity or others
        offset[0] = -(float)(shift.x);
        offset[1] =  (float)(shift.z);
        offset[2] = -(float)(shift.y);
    }
    //
    char* params = (char*)encode_base64_filename((unsigned char*)offset, len, '-');
    del_file_extension_Buffer(out_fname);
    cat_s2Buffer("_", out_fname);
    if (magic_str!=NULL) cat_s2Buffer(magic_str, out_fname);
    cat_s2Buffer(params, out_fname);
    cat_s2Buffer(".", out_fname);
    DEBUG_MODE PRINT_MESG("jbxl::setDegenerateFname: offset (%f, %f, %f) to filename (%s).\n", offset[0], offset[1], offset[2], params);
    ::free(params);

    return;
}


/**
tList*  jbxl::selctJointsFromXMLTemplate(SkinJointData* joints, tXML* joints_template)

XMLのJointのテンプレート joints_template から，必要な jount の情報だけ抜き出して tListにして返す．

@param  joints  ジョイントの名前の配列パラメータ．
@param  joints_template  ジョイントのテンプレートデータ．XML形式．

@retval list->ldat.id   ジョイントの ID
@retval list->ldat.lv   ジョイントの親の ID
@retval list->ldat.key  "name" 固定．
@retval list->ldat.val  ジョイントの名前．文字の最初と最後に " が付く．例："\"mPelvis\""
*/
tList*  jbxl::selctJointsFromXMLTemplate(SkinJointData* joints, tXML* joints_template)
{
    if (joints==NULL || joints_template==NULL) return NULL;
    Buffer name = make_Buffer(L_32);

    int id = 0;
    for (int i=0; i<joints->joint_num; i++) {
        copy_s2Buffer("\"", &name);
        cat_s2Buffer(joints->joint_names.get_value(i), &name);
        cat_s2Buffer("\"", &name);
        tXML* xml = get_xml_attr_node(joints_template, "name", (char*)name.buf);
        if (xml!=NULL) {
            //
            xml->ldat.id = id;
            copy_s2Buffer("name", &xml->ldat.key);
            copy_Buffer(&name, &xml->ldat.val);
            del_xml(&(xml->ldat.lst));
            xml->ldat.lst = NULL;

            Vector<float>* vec = new Vector<float>();
            if (vec!=NULL) {
                vec->x = get_xml_float_content_bystr(xml, "<extra><technique><tip_x>");
                vec->y = get_xml_float_content_bystr(xml, "<extra><technique><tip_y>");
                vec->z = get_xml_float_content_bystr(xml, "<extra><technique><tip_z>");
                xml->ldat.ptr = (void*)vec;
                xml->ldat.sz  = sizeof(Vector<float>);
            }
            //
            xml->ctrl_alt = TREE_KEEP_NODE;
            id++;
        }
    }
    free_Buffer(&name);

    del_non_keep_node_tTree(&joints_template);          // see ttree.h

    tList* list = new_tList_anchor_node();
    _set_parent_joins((tTree*)joints_template, list);
    return list;
}


tList*  jbxl::_set_parent_joins(tTree* pp, tList* list)
{
    if (pp!=NULL) {
        while(pp->esis!=NULL) pp = pp->esis;
        do {
            if (pp->ldat.id!=TREE_ANCHOR_NODE) {
                tList* lt = find_tList_end(list);
                //
                tList_data ldat = dup_tList_data(pp->ldat);
                ldat.lv = pp->prev->ldat.id;
                add_tList_node_bydata(lt, ldat);
            }
            //
            if (pp->next!=NULL) _set_parent_joins(pp->next, list);
            pp = pp->ysis;
        } while(pp!=NULL);
    }

    return list;
}

