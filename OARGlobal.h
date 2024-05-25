#ifndef  __JBXL_OAR_GLOBAL_H_
#define  __JBXL_OAR_GLOBAL_H_

/**
@brief OpenSimの OAR Global用ヘッダ

@file    OARGlobal.h
@author  Fumi.Iseki 
@date    2024 02/05
*/

#ifdef WIN32
  #define  OART_DEFAULT_INP_DIR     ".\\"
  #define  OART_DEFAULT_DAE_DIR     "DAE\\"
  #define  OART_DEFAULT_OBJ_DIR     "OBJ\\"
  #define  OART_DEFAULT_FBX_DIR     "FBX\\"
  #define  OART_DEFAULT_STL_DIR     "STL\\"
  #define  OART_DEFAULT_AST_DIR     "assets\\"
  #define  OART_DEFAULT_TEX_DIR     "Textures\\"
  #define  OART_DEFAULT_PTM_DIR     "Phantoms\\"
  #define  OART_DEFAULT_MTL_DIR     "MTL\\"
  #define  OART_DEFAULT_NOS_DIR     "_nooffset"
#else
  #define  OART_DEFAULT_INP_DIR     "./"
  #define  OART_DEFAULT_DAE_DIR     "./DAE/"
  #define  OART_DEFAULT_OBJ_DIR     "./OBJ/"
  #define  OART_DEFAULT_FBX_DIR     "./FBX/"
  #define  OART_DEFAULT_STL_DIR     "./STL/"
  #define  OART_DEFAULT_AST_DIR     "/usr/local/share/oarconv/assets/:./assets/"
  #define  OART_DEFAULT_TEX_DIR     "Textures/"
  #define  OART_DEFAULT_PTM_DIR     "Phantoms/"
  #define  OART_DEFAULT_MTL_DIR     "MTL/"
  #define  OART_DEFAULT_NOS_DIR     "_nooffset"
#endif


#define  OART_UE_PHANTOM_PREFIX      "UNC_"
#define  OART_UE_COLLIDER_PREFIX     "UCP_"

#define  OART_LOCATION_MAGIC_KEY     "metaverse.jp.net."

#endif
