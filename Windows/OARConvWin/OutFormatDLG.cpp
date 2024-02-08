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

	outputDaeButton   = NULL;
	outputObjButton   = NULL;
	outputStlButton   = NULL;

	outputUnityButton = NULL;
	outputUEButton    = NULL;
}


COutFormatDLG::~COutFormatDLG()
{
}


void  COutFormatDLG::getParameters(CParameterSet* param)
{
	param->outputEngine = outputEngine;
	param->outputFormat = outputFormat;

	if (outputFormat == JBXL_3D_FORMAT_DAE) {
		param->prefixOUT = param->prefixDAE;
	}
	else if (outputFormat == JBXL_3D_FORMAT_OBJ) {
		param->prefixOUT = param->prefixOBJ;
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
	outputStlButton   = (CButton*)GetDlgItem(IDC_RADIO_STL);
	outputUnityButton = (CButton*)GetDlgItem(IDC_RADIO_UNITY);
	outputUEButton    = (CButton*)GetDlgItem(IDC_RADIO_UE);

	if (outputFormat == JBXL_3D_FORMAT_DAE) {
		outputDaeButton->SetCheck(1);
		outputObjButton->SetCheck(0);
		outputStlButton->SetCheck(0);
		OnBnClickedRadioDae();
	}
	else if (outputFormat == JBXL_3D_FORMAT_OBJ) {
		outputDaeButton->SetCheck(0);
		outputObjButton->SetCheck(1);
		outputStlButton->SetCheck(0);
		OnBnClickedRadioObj();
		
	}
	else {
		outputDaeButton->SetCheck(0);
		outputObjButton->SetCheck(0);
		outputStlButton->SetCheck(1);
		OnBnClickedRadioStl();
	}

	if (outputEngine == JBXL_3D_ENGINE_UNITY) {
		outputUnityButton->SetCheck(1);
		outputUEButton->SetCheck(0);
	}
	else if (outputEngine == JBXL_3D_ENGINE_UE) {
		outputUnityButton->SetCheck(0);
		outputUEButton->SetCheck(1);
	}
	else {
		outputUnityButton->SetCheck(0);
		outputUEButton->SetCheck(0);
	}

	return TRUE;
}


void COutFormatDLG::OnOK()
{
	if (outputDaeButton->GetCheck())   outputFormat = JBXL_3D_FORMAT_DAE;
	if (outputObjButton->GetCheck())   outputFormat = JBXL_3D_FORMAT_OBJ;
	if (outputStlButton->GetCheck())   outputFormat = JBXL_3D_FORMAT_STL_A;

	outputEngine = JBXL_3D_ENGINE_NONE;
	if (outputUnityButton->GetCheck()) outputEngine = JBXL_3D_ENGINE_UNITY;
	if (outputUEButton->GetCheck())    outputEngine = JBXL_3D_ENGINE_UE;

	CDialogEx::OnOK();
}

BEGIN_MESSAGE_MAP(COutFormatDLG, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_OBJ, &COutFormatDLG::OnBnClickedRadioObj)
	ON_BN_CLICKED(IDC_RADIO_DAE, &COutFormatDLG::OnBnClickedRadioDae)
	ON_BN_CLICKED(IDC_RADIO_STL, &COutFormatDLG::OnBnClickedRadioStl)
END_MESSAGE_MAP()


void COutFormatDLG::OnBnClickedRadioDae()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	outputUnityButton->EnableWindow(TRUE);
	outputUnityButton->SetCheck(1);
	outputUEButton->EnableWindow(FALSE);
	outputUEButton->SetCheck(0);
	outputEngine = JBXL_3D_ENGINE_UNITY;
}


void COutFormatDLG::OnBnClickedRadioObj()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	outputUnityButton->EnableWindow(TRUE);
	outputUEButton->EnableWindow(TRUE);
	if (outputEngine == JBXL_3D_ENGINE_NONE) {
		outputEngine = JBXL_3D_ENGINE_UNITY;
		outputUnityButton->SetCheck(1);
	}
}


void COutFormatDLG::OnBnClickedRadioStl()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	outputUnityButton->EnableWindow(FALSE);
	outputUnityButton->SetCheck(0);
	outputUEButton->EnableWindow(FALSE);
	outputUEButton->SetCheck(0);
	outputEngine = JBXL_3D_ENGINE_NONE;
}
