/** 
    Convert OAR Files to Collada Files
*/

#include "OARTool.h"
#include "oarconv_help.h"


#define OARCONV_VERSION           "1.8.1" // 20240911 fix Unity/Editor/GLTFAutoCollider.cs, output Folder
//#define OARCONV_VERSION           "1.8.0" // 20240906 for glTF or FBX
//#define OARCONV_VERSION           "1.7.5" // 20240527 fix OAR file path include spaces
//#define OARCONV_VERSION           "1.7.4" // 20240527 fix stack size, fix spellmiss
//#define OARCONV_VERSION           "1.7.3" // 20240526 add magic string to no offset file name
//#define OARCONV_VERSION           "1.7.2" // 20240523 support object information (center coordiname)
//#define OARCONV_VERSION           "1.7.1" // 20240522 support no offset file
//#define OARCONV_VERSION           "1.7.0" // 20240520 for DAE Joints File
//#define OARCONV_VERSION           "1.6.0" // 20240421 for OBJ File
//#define OARCONV_VERSION           "1.5.0" // 20240126 less texture
//#define OARCONV_VERSION           "1.4.6" // 20240105 Unity 2022 support
//#define OARCONV_VERSION           "1.4.5" // 20201205 for latest JunkBox Library
//#define OARCONV_VERSION           "1.4.4" // 20160511 fix tree rotation bug. regenerate tree images
//#define OARCONV_VERSION           "1.4.3" // 20160428 correct Gray Scale Sculpt Image
//#define OARCONV_VERSION           "1.4.2" // 20151023 support MS Windws
//#define OARCONV_VERSION           "1.4.1" // 20150923 fix memory leak of STLTool, for VS C++
//#define OARCONV_VERSION           "1.4.0" // 20150922 support STL and BREP 
//#define OARCONV_VERSION           "1.3.4" // 20150905 add position offset
//#define OARCONV_VERSION           "1.3.3" // 20150902 fix ProfileCurve and HollowShape Bug
//#define OARCONV_VERSION           "1.3.2" // 20150831 change select shader
//#define OARCONV_VERSION           "1.3.1" // 20150826 support comps->factor of JPEG2000
//#define OARCONV_VERSION           "1.3.0" // 20150823 modify additional name of Texture, for materials
//#define OARCONV_VERSION           "1.2.0" // 20150822 for Unity Shader
//#define OARCONV_VERSION           "1.1.0" // 20150531 for hollow, phantom, texture....
//#define OARCONV_VERSION           "1.0.0" // 20141014 first version

