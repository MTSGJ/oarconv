// SetParamDLG.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OARConvWin.h"
#include "SetParamDLG.h"
#include "afxdialogex.h"


// CSetParamDLG ダイアログ

IMPLEMENT_DYNAMIC(CSetParamDLG, CDialogEx)


CSetParamDLG::CSetParamDLG(CParameterSet* param, CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetParamDLG::IDD, pParent)
{
	//outputFormat = param->outputFormat;
	startNum = param->startNum;
	stopNum  = param->stopNum;
	terrainScale = param->terrainScale;

	outputTerrain = param->outputTerrain;
	debugMode = param->debugMode;
	
	xShift = param->xShift;
	yShift = param->yShift;
	zShift = param->zShift;

	prefixOAR = param->prefixOAR;
	prefixOBJ = param->prefixOBJ;
	prefixDAE = param->prefixDAE;
	prefixSTL = param->prefixSTL;

	startNumEBox = NULL;
	stopNumEBox  = NULL;
	terrainScaleEBox = NULL;

	outputTerrainButton = NULL;
	//outputDaeButton = NULL;
	//outputObjButton = NULL;
	//outputStlButton = NULL;
	debugModeButton = NULL;

	xShiftEBox = NULL;
	yShiftEBox = NULL;
	zShiftEBox = NULL;

	prefixOarEBox = NULL;
	prefixDaeEBox = NULL;
	prefixObjEBox = NULL;
	prefixStlEBox = NULL;
}


CSetParamDLG::~CSetParamDLG()
{
}


void  CSetParamDLG::getParameters(CParameterSet* param)
{
	param->startNum = startNum;
	param->stopNum = stopNum;
	//param->outputFormat = outputFormat;
	param->terrainScale = terrainScale;

	param->outputTerrain = outputTerrain;
	param->debugMode = debugMode;
	
	param->xShift = xShift;
	param->yShift = yShift;
	param->zShift = zShift;

	param->prefixOAR = prefixOAR;
	param->prefixDAE = prefixDAE;
	param->prefixOBJ = prefixOBJ;
	param->prefixSTL = prefixSTL;

	/*
	if (outputFormat == JBXL_3D_FORMAT_DAE) {
		param->prefixOUT = prefixDAE;
	}
	else if (outputFormat == JBXL_3D_FORMAT_OBJ) {
		param->prefixOUT = prefixOBJ;
	}
	else {
		param->prefixOUT = prefixSTL;
	}*/
	return;
}


void CSetParamDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



////////////////////////////////////////////////////////////////////
// CSetParamDLG メッセージ ハンドラー

BOOL CSetParamDLG::OnInitDialog()
{
	TCHAR buf[LNAME];

	CDialogEx::OnInitDialog();

	//
	startNumEBox = (CEdit*)GetDlgItem(IDC_EDIT_START_NUM);
	stopNumEBox  = (CEdit*)GetDlgItem(IDC_EDIT_STOP_NUM);

	//outputDaeButton = (CButton*)GetDlgItem(IDC_RADIO_DAE);
	//outputObjButton = (CButton*)GetDlgItem(IDC_RADIO_OBJ);
	//outputStlButton = (CButton*)GetDlgItem(IDC_RADIO_STL);

	outputTerrainButton = (CButton*)GetDlgItem(IDC_CHECK_TERRAIN);
	terrainScaleEBox = (CEdit*)GetDlgItem(IDC_EDIT_TERRAIN_SCALE);
	debugModeButton = (CButton*)GetDlgItem(IDC_CHECK_DEBUGMODE);

	xShiftEBox = (CEdit*)GetDlgItem(IDC_EDIT_SHIFT_X);
	yShiftEBox = (CEdit*)GetDlgItem(IDC_EDIT_SHIFT_Y);
	zShiftEBox = (CEdit*)GetDlgItem(IDC_EDIT_SHIFT_Z);

	prefixOarEBox = (CEdit*)GetDlgItem(IDC_EDIT_PREFIX_OAR);
	prefixDaeEBox = (CEdit*)GetDlgItem(IDC_EDIT_PREFIX_DAE);
	prefixObjEBox = (CEdit*)GetDlgItem(IDC_EDIT_PREFIX_OBJ);
	prefixStlEBox = (CEdit*)GetDlgItem(IDC_EDIT_PREFIX_STL);

	sntprintf(buf, LNAME, _T("%d"), startNum);
	startNumEBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T("%d"), stopNum);
	stopNumEBox->SetWindowText(buf);

	sntprintf(buf, LNAME, _T("%g"), terrainScale);
	terrainScaleEBox->SetWindowText(buf);

	/*
	if (outputFormat == JBXL_3D_FORMAT_DAE) {
		outputDaeButton->SetCheck(1);
		outputObjButton->SetCheck(0);
		outputStlButton->SetCheck(0);
	}
	else if (outputFormat == JBXL_3D_FORMAT_OBJ) {
		outputDaeButton->SetCheck(0);
		outputObjButton->SetCheck(1);
		outputStlButton->SetCheck(0);
	}
	else {
		outputDaeButton->SetCheck(0);
		outputObjButton->SetCheck(0);
		outputStlButton->SetCheck(1);
	}*/

	if (outputTerrain) outputTerrainButton->SetCheck(1);
	else               outputTerrainButton->SetCheck(0);
	if (debugMode)     debugModeButton->SetCheck(1);
	else               debugModeButton->SetCheck(0);

	sntprintf(buf, LNAME, _T("%g"), xShift);
	xShiftEBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T("%g"), yShift);
	yShiftEBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T("%g"), zShift);
	zShiftEBox->SetWindowText(buf);

	sntprintf(buf, LNAME, _T("%s"), prefixOAR.GetString());
	prefixOarEBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T("%s"), prefixDAE.GetString());
	prefixDaeEBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T("%s"), prefixOBJ.GetString());
	prefixObjEBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T("%s"), prefixSTL.GetString());
	prefixStlEBox->SetWindowText(buf);

	return TRUE;
}


void CSetParamDLG::OnOK()
{
	TCHAR buf[LNAME];

	startNumEBox->GetWindowText(buf, LNAME);
	startNum = ttoi(buf);
	if (startNum<1) startNum = 1;

	stopNumEBox->GetWindowText(buf, LNAME);
	stopNum = ttoi(buf);
	terrainScaleEBox->GetWindowText(buf, LNAME);
	terrainScale = (float)ttof(buf);

	//if (outputDaeButton->GetCheck()) outputFormat = JBXL_3D_FORMAT_DAE;
	//if (outputObjButton->GetCheck()) outputFormat = JBXL_3D_FORMAT_OBJ;
	//if (outputStlButton->GetCheck()) outputFormat = JBXL_3D_FORMAT_STL_A;

	if (outputTerrainButton->GetCheck()) outputTerrain = TRUE;
	else                                 outputTerrain = FALSE;
	if (debugModeButton->GetCheck())     debugMode = TRUE;
	else                                 debugMode = FALSE;

	xShiftEBox->GetWindowText(buf, LNAME);
	xShift = (float)ttof(buf);
	yShiftEBox->GetWindowText(buf, LNAME);
	yShift = (float)ttof(buf);
	zShiftEBox->GetWindowText(buf, LNAME);
	zShift = (float)ttof(buf);

	prefixOarEBox->GetWindowText(buf, LNAME);
	prefixOAR = buf;
	prefixDaeEBox->GetWindowText(buf, LNAME);
	prefixDAE = buf;
	prefixObjEBox->GetWindowText(buf, LNAME);
	prefixOBJ = buf;
	prefixStlEBox->GetWindowText(buf, LNAME);
	prefixSTL = buf;

	CDialogEx::OnOK();
}

BEGIN_MESSAGE_MAP(CSetParamDLG, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_OBJ, &CSetParamDLG::OnBnClickedRadioObj)
END_MESSAGE_MAP()


void CSetParamDLG::OnBnClickedRadioObj()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}
