#ifndef  __JBXL_OAR_GENERAL_TOOL_H_
#define  __JBXL_OAR_GENERAL_TOOL_H_

/**
@brief   OARConverter用の汎用ツール
@file    GeneralTool.h

@author  Fumi.Iseki 
@date    2024 7/27
*/

#include "tools++.h"
#include "xtools.h"
#include "tlist.h"
#include "tjson.h"
#include "txml.h"
#include "Gdata.h"

#include "Jpeg2KTool.h"
#include "TgaTool.h"
#include "PngTool.h"

#include "MaterialParam.h"
#include "MeshObjectData.h"


//#define  MTRL_DEFAULT_ALPHA         0.90    // 0.99 is not Transparent
#define  MTRL_DEFAULT_COLOR         0.0001



namespace  jbxl {


extern tList* AlphaChannelList;


///////////////////////////////////////////////////////////////////////////////////////////////
//

// 有効なαチャンネルを持っているかどうか調べる．
bool  hasValidAlphaChannel(const char* texture, tList* resourceList);

// Degenerate（縮退）状態時に，ファイル名の座標情報を載せる．
void  setDegenerateFname(Buffer* out_fname, int engine, Vector<double> shift, const char* magic_str);

// XMLのJointのテンプレートから，必要な jount の情報だけ抜き出して返す
tList*  selectJointsFromXMLTemplate(SkinJointData* joints, tXML* joints_template);
tList*  _set_parent_joins(tXML* joints_template, tList* list);

void  makeFolders(const char* base, const char* sub);
void  rmFolders(const char* base, const char* tex);

}       // namespace


#endif

