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
	CString	oarFolder;
	CString	daeFolder;
	CString	stlFolder;
	CString	logFolder;

	int     startNum;
	int     stopNum;
	BOOL    outputDae;
	BOOL    outputTerrain;
	BOOL    debugMode;
	
	float   terrainScale;
	float   xShift;
	float   yShift;
	float   zShift;

	CString prefixOAR;
	CString prefixDAE;
	CString prefixSTL;
};



