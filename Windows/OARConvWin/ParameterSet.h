#pragma once

#include  "OARConvWin_config.h"
#include  "WinTools.h"

#include  "TerrainTool.h"


using namespace  jbxwl;


////////////////////////////////////////////////////////////////////////////////////
//
// CParameterSet
//

class CParameterSet
{
public:
	CParameterSet() { init();}
	virtual	~CParameterSet() {}

	void	init(void);
	void	free(void) {}

	void	readConfigFile(void);
	void	saveConfigFile(void);

	void	readWindowSize(RECT* winsz);
	void	saveWindowSize(RECT  winsz);

	CString	makeConfigFilePath(void);
	CString	makeConfigSizePath(void);

//
public:
	CString	configFilePath;
	CString	configSizePath;

//
// Parameters
public:
	CString oarName;	// OAR名．拡張なし．
	CString baseFolder;	// oarFolder, outFolder の親フォルダ
	CString	oarFolder;
	CString	outFolder;
	CString	logFolder;

	int     startNum;
	int     stopNum;
	BOOL    outputObject;
	BOOL    outputTerrain;
	BOOL    debugMode;
	
	int     outputEngine;		// Unity or UE
	int     outputFormat;		// ファイルフォーマット DAE, OBJ, STL
	float   terrainScale;
	float   xShift;
	float   yShift;
	float   zShift;

	CString prefixOAR;
	CString prefixOUT;
	CString prefixDAE;
	CString prefixOBJ;
	CString prefixSTL;
};



