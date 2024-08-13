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
	outputEngine  = param->outputEngine;
	outputFormat  = param->outputFormat;
	noOffset      = param->noOffset;
	procJoints    = param->procJoints;

	outputDaeButton   = NULL;
	outputObjButton   = NULL;
	outputGltfButton  = NULL;
	outputGlbButton   = NULL;
	outputFbxButton   = NULL;
	outputStlButton   = NULL;

	outputUnityButton = NULL;
	outputUEButton    = NULL;

	procJointsButton  = NULL;
	noOffsetButton    = NULL;
}


COutFormatDLG::~COutFormatDLG()
{
}


void  COutFormatDLG::getParameters(CParameterSet* param)
{
	param->outputEngine = outputEngine;
	param->outputFormat = outputFormat;
	param->noOffset     = noOffset;
	param->procJoints   = procJoints;

	if (outputFormat == JBXL_3D_FORMAT_DAE) {
		param->prefixOUT = param->prefixDAE;
	}
	else if (outputFormat == JBXL_3D_FORMAT_OBJ) {
		param->prefixOUT = param->prefixOBJ;
	}
	else if (outputFormat == JBXL_3D_FORMAT_GLTF) {
		param->prefixOUT = param->prefixGLTF;
	}
	else if (outputFormat == JBXL_3D_FORMAT_GLB) {
		param->prefixOUT = param->prefixGLB;
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
	outputGltfButton  = (CButton*)GetDlgItem(IDC_RADIO_GLTF);
	outputGlbButton   = (CButton*)GetDlgItem(IDC_RADIO_GLB);
	outputFbxButton   = (CButton*)GetDlgItem(IDC_RADIO_FBX);
	outputStlButton   = (CButton*)GetDlgItem(IDC_RADIO_STL);
	outputUnityButton = (CButton*)GetDlgItem(IDC_RADIO_UNITY);
	outputUEButton    = (CButton*)GetDlgItem(IDC_RADIO_UE);
	procJointsButton  = (CButton*)GetDlgItem(IDC_CHECK_JOINTS);
	noOffsetButton    = (CButton*)GetDlgItem(IDC_CHECK_OFFSET);

	outputDaeButton->SetCheck(0);
	outputObjButton->SetCheck(0);
	outputGltfButton->SetCheck(0);
	outputGlbButton->SetCheck(0);
	outputFbxButton->SetCheck(0);
	outputStlButton->SetCheck(0);

	// Not Implement yet
	outputFbxButton->EnableWindow(FALSE);

	if (outputFormat == JBXL_3D_FORMAT_DAE) {
		outputDaeButton->SetCheck(1);
		OnBnClickedRadioDae();
	}
	else if (outputFormat == JBXL_3D_FORMAT_OBJ) {
        outputObjButton->SetCheck(1);
        OnBnClickedRadioObj();
	}
	else if (outputFormat == JBXL_3D_FORMAT_GLTF) {
		outputGltfButton->SetCheck(1);
		OnBnClickedRadioGltf();
	}
	else if (outputFormat == JBXL_3D_FORMAT_GLB) {
		outputGlbButton->SetCheck(1);
		OnBnClickedRadioGlb();
	}
	else if (outputFormat == JBXL_3D_FORMAT_FBX) {
		outputFbxButton->SetCheck(1);
		OnBnClickedRadioFbx();
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

	if (noOffset) noOffsetButton->SetCheck(1);
	else          noOffsetButton->SetCheck(0);

	return TRUE;
}


void COutFormatDLG::OnOK()
{
	noOffset = FALSE;
	outputFormat = JBXL_3D_FORMAT_NONE;

	if      (outputDaeButton->GetCheck()) outputFormat = JBXL_3D_FORMAT_DAE;
	else if (outputObjButton->GetCheck()) outputFormat = JBXL_3D_FORMAT_OBJ;
	else if (outputGltfButton->GetCheck())outputFormat = JBXL_3D_FORMAT_GLTF;
    else if (outputGlbButton->GetCheck()) outputFormat = JBXL_3D_FORMAT_GLB;
	else if (outputFbxButton->GetCheck()) outputFormat = JBXL_3D_FORMAT_FBX;
	else if (outputStlButton->GetCheck()) outputFormat = JBXL_3D_FORMAT_STL_A;

	outputEngine = JBXL_3D_ENGINE_NONE;
	if (outputUnityButton->GetCheck())   outputEngine = JBXL_3D_ENGINE_UNITY;
	else if (outputUEButton->GetCheck()) outputEngine = JBXL_3D_ENGINE_UE;

	if (procJointsButton->GetCheck()) procJoints = TRUE;
	else                              procJoints = FALSE;
	if (noOffsetButton->GetCheck())   noOffset = TRUE;
	else                              noOffset = FALSE;

	CDialogEx::OnOK();
}

BEGIN_MESSAGE_MAP(COutFormatDLG, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_DAE, &COutFormatDLG::OnBnClickedRadioDae)
	ON_BN_CLICKED(IDC_RADIO_OBJ, &COutFormatDLG::OnBnClickedRadioObj)
    ON_BN_CLICKED(IDC_RADIO_GLTF,&COutFormatDLG::OnBnClickedRadioGltf)
	ON_BN_CLICKED(IDC_RADIO_GLB, &COutFormatDLG::OnBnClickedRadioGlb)
	ON_BN_CLICKED(IDC_RADIO_FBX, &COutFormatDLG::OnBnClickedRadioFbx)
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
	noOffsetButton->EnableWindow(TRUE);
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
	noOffsetButton->EnableWindow(TRUE);
}


void COutFormatDLG::OnBnClickedRadioGltf()
{
	outputUnityButton->EnableWindow(TRUE);
	outputUEButton->EnableWindow(TRUE);
	if (outputEngine == JBXL_3D_ENGINE_NONE) {
		outputEngine = JBXL_3D_ENGINE_UNITY;
		outputUnityButton->SetCheck(1);
		outputUEButton->SetCheck(0);
	}
	procJointsButton->EnableWindow(TRUE);
	noOffsetButton->EnableWindow(TRUE);
}


void COutFormatDLG::OnBnClickedRadioGlb()
{
	outputUnityButton->EnableWindow(TRUE);
	outputUEButton->EnableWindow(TRUE);
	if (outputEngine == JBXL_3D_ENGINE_NONE) {
		outputEngine = JBXL_3D_ENGINE_UNITY;
		outputUnityButton->SetCheck(1);
		outputUEButton->SetCheck(0);
	}
	procJointsButton->EnableWindow(TRUE);
	noOffsetButton->EnableWindow(TRUE);
}


void COutFormatDLG::OnBnClickedRadioFbx()
{
	outputUnityButton->EnableWindow(TRUE);
	outputUEButton->EnableWindow(TRUE);
	if (outputEngine == JBXL_3D_ENGINE_NONE) {
		outputEngine = JBXL_3D_ENGINE_UNITY;
		outputUnityButton->SetCheck(1);
		outputUEButton->SetCheck(0);
	}
	procJointsButton->EnableWindow(TRUE);
	noOffsetButton->EnableWindow(TRUE);
}


void COutFormatDLG::OnBnClickedRadioStl()
{
	outputEngine = JBXL_3D_ENGINE_NONE;
	outputUnityButton->EnableWindow(FALSE);
	outputUnityButton->SetCheck(0);
	outputUEButton->EnableWindow(FALSE);
	outputUEButton->SetCheck(0);
	procJointsButton->EnableWindow(FALSE);
	noOffsetButton->EnableWindow(FALSE);
}

