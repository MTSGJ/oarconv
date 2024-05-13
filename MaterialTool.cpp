/**
@brief OAR マテリアル用のツール
@file  MaterialTool.cpp

@author  Fumi.Iseki 
@date    2015 6/27
*/

#include "MaterialTool.h"


using namespace jbxl;


tList*  jbxl::AlphaChannelList = NULL;  // テクスチャがアルファチャンネルを持つかどうかの情報を格納したリスト


///////////////////////////////////////////////////////////////////////////////////////////////////
//

bool  jbxl::CheckAlphaChannel(const char* texture, tList* resourceList)
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

    char* path = get_resource_path((char*)inppath.buf, resourceList);
    char* extn = get_file_extension(path);
    //
    if (path!=NULL && extn!=NULL && (extn[0]=='j' || extn[0]=='J')) {   // for Jpeg2000
        //
        JPEG2KImage jpg = readJPEG2KFile(path);
        MSGraph<uByte> vp = JPEG2KImage2MSGraph<uByte>(jpg);
        //
        if (vp.zs>0 && (vp.zs==4 || vp.zs==2)) {
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

    // List
    int lv = FALSE;
    if (ret) lv = TRUE;
    pp = add_tList_node_bystr(AlphaChannelList, 0, lv, (char*)inppath.buf, NULL, NULL, 0);
    if (AlphaChannelList==NULL) AlphaChannelList = pp;

    free_Buffer(&inppath);

    return ret;
}

