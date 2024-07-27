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
#include "Gdata.h"

#include "Jpeg2KTool.h"
#include "TgaTool.h"
#include "PngTool.h"

#include "MaterialParam.h"


//#define  MTRL_DEFAULT_ALPHA         0.90    // 0.99 is not Transparent
#define  MTRL_DEFAULT_COLOR         0.0001



namespace  jbxl {


extern tList* AlphaChannelList;


///////////////////////////////////////////////////////////////////////////////////////////////
//

bool  hasValidAlphaChannel(const char* texture, tList* resourceList);


void  setDegenerateFname(Buffer* out_fname, int engine, Vector<double> shift, const char* magic_str);

}       // namespace


#endif

