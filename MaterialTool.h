#ifndef  __JBXL_OS_MATERIAL_TOOL_H_
#define  __JBXL_OS_MATERIAL_TOOL_H_

/**
@brief OARマテリアル用のツール
@file  MaterialTool.h

@author  Fumi.Iseki 
@date    2015 6/27
*/

#include "tools++.h"
#include "xtools.h"
#include "tlist.h"
#include "Gdata.h"

#include "Jpeg2KTool.h"
#include "TgaTool.h"

#include "MaterialParam.h"


//#define  MTRL_DEFAULT_ALPHA         0.90    // 0.99 is not Transparent
#define  MTRL_DEFAULT_COLOR         0.0001

#define  MTRL_IMAGE_TYPE            ".tga"


namespace  jbxl {


extern tList* AlphaChannelList;


///////////////////////////////////////////////////////////////////////////////////////////////
//

bool  HasValidAlphaChannel(const char* texture, tList* resourceList);

}       // namespace


#endif

