// OutFormatDLG.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OARConvWin.h"
#include "OutFormatDLG.h"
#include "afxdialogex.h"


// COutFormatDLG ダイアログ

IMPLEMENT_DYNAMIC(COutFormatDLG, CDialogEx)


COutFormatDLG::COutFormatDLG(CParameterSet* param, CWnd* pParent /*=NULL*/)
	: CDialogEx(COutFormatDLG::IDD, pParent)
{
	outputEngine = param->outputEngine;
	outputFormat = param->outputFormat;
	degeneracy   = param->degeneracy;
	procJoints   = param->procJoints;

	outputDaeButton   = NULL;
	outputObjButton   = NULL;
	outputONOButton   = NULL;
	outputFbxButton   = NULL;
	outputFNOButton   = NULL;
	outputStlButton   = NULL;

	outputUnityButton = NULL;
	outputUEButton    = NULL;

	procJointsButton  = NULL;
}


COutFormatDLG::~COutFormatDLG()
{
}


void  COutFormatDLG::getParameters(CParameterSet* param)
{
	param->outputEngine = outputEngine;
	param->outputFormat = outputFormat;
	param->degeneracy   = degeneracy;
	param->procJoints   = procJoints;

	if (outputFormat == JBXL_3D_FORMAT_DAE) {
		param->prefixOUT = param->prefixDAE;
	}
	else if (outputFormat == JBXL_3D_FORMAT_OBJ) {
		param->prefixOUT = param->prefixOBJ;
	}
	else if (outputFormat == JBXL_3D_FORMAT_FBX) {
		param->prefixOUT = param->prefixFBX;
	}
	else {
		param->prefixOUT = param->prefixSTL;
	}
	return;
}


void COutFormatDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


////////////////////////////////////////////////////////////////////
// COutFormatDLG メッセージ ハンドラー

BOOL COutFormatDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	outputDaeButton   = (CButton*)GetDlgItem(IDC_RADIO_DAE);
	outputObjButton   = (CButton*)GetDlgItem(IDC_RADIO_OBJ);
	outputONOButton   = (CButton*)GetDlgItem(IDC_RADIO_OBJ_NO);
	outputFbxButton   = (CButton*)GetDlgItem(IDC_RADIO_FBX);
	outputFNOButton   = (CButton*)GetDlgItem(IDC_RADIO_FBX_NO);
	outputStlButton   = (CButton*)GetDlgItem(IDC_RADIO_STL);
	outputUnityButton = (CButton*)GetDlgItem(IDC_RADIO_UNITY);
	outputUEButton    = (CButton*)GetDlgItem(IDC_RADIO_UE);
	procJointsButton  = (CButton*)GetDlgItem(IDC_CHECK_JOINTS);

	outputDaeButton->SetCheck(0);
	outputObjButton->SetCheck(0);
	outputONOButton->SetCheck(0);
	outputFbxButton->SetCheck(0);
	outputFNOButton->SetCheck(0);
	outputStlButton->SetCheck(0);

	// Not Implement yet
	outputFbxButton->EnableWindow(FALSE);
	outputFNOButton->EnableWindow(FALSE);

	if (outputFormat == JBXL_3D_FORMAT_DAE) {
		outputDaeButton->SetCheck(1);
		OnBnClickedRadioDae();
	}
	else if (outputFormat == JBXL_3D_FORMAT_OBJ) {
		if (!degeneracy) {
			outputObjButton->SetCheck(1);
			OnBnClickedRadioObj();
		}
		else {
			outputONOButton->SetCheck(1);
			degeneracy = TRUE;
			OnBnClickedRadioObjNo();
		}
	}
	else if (outputFormat == JBXL_3D_FORMAT_FBX) {
		if (!degeneracy) {
			outputFbxButton->SetCheck(1);
			OnBnClickedRadioFbx();
		}
		else {
			outputFNOButton->SetCheck(1);
			degeneracy = TRUE;
			OnBnClickedRadioFbxNo();
		}
	}
	else {
		outputStlButton->SetCheck(1);
		OnBnClickedRadioStl();
	}

	outputUnityButton->SetCheck(0);
	outputUEButton->SetCheck(0);
	if (outputEngine == JBXL_3D_ENGINE_UNITY) {
		outputUnityButton->SetCheck(1);
	}
	else if (outputEngine == JBXL_3D_ENGINE_UE) {
		outputUEButton->SetCheck(1);
	}

	if (procJoints) procJointsButton->SetCheck(1);
	else            procJointsButton->SetCheck(0);

	return TRUE;
}


void COutFormatDLG::OnOK()
{
	degeneracy = FALSE;
	outputFormat = JBXL_3D_FORMAT_NONE;

	if      (outputDaeButton->GetCheck()) outputFormat = JBXL_3D_FORMAT_DAE;
	else if (outputObjButton->GetCheck()) outputFormat = JBXL_3D_FORMAT_OBJ;
	else if (outputFbxButton->GetCheck()) outputFormat = JBXL_3D_FORMAT_FBX;
	else if (outputStlButton->GetCheck()) outputFormat = JBXL_3D_FORMAT_STL_A;
	else if (outputONOButton->GetCheck()) {
		outputFormat = JBXL_3D_FORMAT_OBJ;
		degeneracy = TRUE;
	}
	else if (outputFNOButton->GetCheck()) {
		outputFormat = JBXL_3D_FORMAT_FBX;
		degeneracy = TRUE;
	}

	outputEngine = JBXL_3D_ENGINE_NONE;
	if (outputUnityButton->GetCheck())   outputEngine = JBXL_3D_ENGINE_UNITY;
	else if (outputUEButton->GetCheck()) outputEngine = JBXL_3D_ENGINE_UE;

	if (procJointsButton->GetCheck()) procJoints = TRUE;
	else                              procJoints = FALSE;

	CDialogEx::OnOK();
}

BEGIN_MESSAGE_MAP(COutFormatDLG, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_DAE, &COutFormatDLG::OnBnClickedRadioDae)
	ON_BN_CLICKED(IDC_RADIO_OBJ, &COutFormatDLG::OnBnClickedRadioObj)
	ON_BN_CLICKED(IDC_RADIO_OBJ_NO, &COutFormatDLG::OnBnClickedRadioObjNo)
	ON_BN_CLICKED(IDC_RADIO_FBX, &COutFormatDLG::OnBnClickedRadioFbx)
	ON_BN_CLICKED(IDC_RADIO_FBX_NO, &COutFormatDLG::OnBnClickedRadioFbxNo)
	ON_BN_CLICKED(IDC_RADIO_STL, &COutFormatDLG::OnBnClickedRadioStl)
	//ON_BN_CLICKED(IDC_CHECK_JOINTS, &COutFormatDLG::OnBnClickedCheckJoints)
END_MESSAGE_MAP()


void COutFormatDLG::OnBnClickedRadioDae()
{
	outputUnityButton->EnableWindow(TRUE);
	outputUnityButton->SetCheck(1);
	outputUEButton->EnableWindow(FALSE);
	outputUEButton->SetCheck(0);
	outputEngine = JBXL_3D_ENGINE_UNITY;
	procJointsButton->EnableWindow(TRUE);
}


void COutFormatDLG::OnBnClickedRadioObj()
{
	outputUnityButton->EnableWindow(TRUE);
	outputUEButton->EnableWindow(TRUE);
	if (outputEngine == JBXL_3D_ENGINE_NONE) {
		outputEngine = JBXL_3D_ENGINE_UNITY;
		outputUnityButton->SetCheck(1);
		outputUEButton->SetCheck(0);
	}
	procJointsButton->EnableWindow(FALSE);
}


void COutFormatDLG::OnBnClickedRadioObjNo()   // with No Offset for UE only
{
	outputEngine = JBXL_3D_ENGINE_UE;
	outputUnityButton->EnableWindow(FALSE);
	outputUnityButton->SetCheck(0);
	outputUEButton->EnableWindow(TRUE);
	outputUEButton->SetCheck(1);
	procJointsButton->EnableWindow(FALSE);
}


void COutFormatDLG::OnBnClickedRadioFbx()
{
	outputUnityButton->EnableWindow(TRUE);
	outputUEButton->EnableWindow(TRUE);
	if (outputEngine == JBXL_3D_ENGINE_NONE) {
		outputEngine = JBXL_3D_ENGINE_UNITY;
		outputUnityButton->SetCheck(1);
		outputUEButton->SetCheck(0);
		procJointsButton->EnableWindow(TRUE);
	}
}


void COutFormatDLG::OnBnClickedRadioFbxNo()		// with No Offset for UE only
{
	outputEngine = JBXL_3D_ENGINE_UE;
	outputUnityButton->EnableWindow(FALSE);
	outputUnityButton->SetCheck(0);
	outputUEButton->EnableWindow(TRUE);
	outputUEButton->SetCheck(1);
	procJointsButton->EnableWindow(TRUE);
}


void COutFormatDLG::OnBnClickedRadioStl()
{
	outputEngine = JBXL_3D_ENGINE_NONE;
	outputUnityButton->EnableWindow(FALSE);
	outputUnityButton->SetCheck(0);
	outputUEButton->EnableWindow(FALSE);
	outputUEButton->SetCheck(0);
	procJointsButton->EnableWindow(FALSE);
}

/*
void COutFormatDLG::OnBnClickedCheckJoints()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}
*/
