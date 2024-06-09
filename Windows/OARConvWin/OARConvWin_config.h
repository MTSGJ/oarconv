/**
	OARConvWin_config.h  by Fumi.Iseki (C)


*/
#pragma once


#ifdef WIN64
#define		OARCONV_VERSION			"OARConvWin 1.7.8 (x64)"
#else
#define		OARCONV_VERSION			"OARConvWin 1.7.8 (x86)"
#endif
// change also OARConvWin.rc2 -> Version


#define		OARCONV_CONFIG_PATH		"\\NSL\\OARConvWin\\"
#define		OARCONV_CONFIG_FILE		"OARConvWin.cfg"
#define		OARCONV_WINSIZE_FILE	"OARConvWin.size"

// 初期ウィンドウサイズ
#define	    OARCONV_WND_XSIZE		480
#define	    OARCONV_WND_YSIZE		320


//
// OpenJpeg openjpeg-1.2 の DllOpenJPEG.vcxproj を使用
//#define     OPENJPEG_VER             12



