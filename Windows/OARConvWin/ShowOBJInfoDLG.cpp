// ShowOBJInformationDLG.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OARConvWin.h"
#include "ShowOBJInfoDLG.h"


// CShowInfoDLG ダイアログ

IMPLEMENT_DYNAMIC(CShowOBJInfoDLG, CDialogEx)


CShowOBJInfoDLG::CShowOBJInfoDLG(OARTool oar, CWnd* pParent)
	: CDialogEx(CShowOBJInfoDLG::IDD, pParent)
{
	/*
	objectName;
	coordinate;
	other;
	coordVec;
	vertexNnum;
	jointNum;
	*/

	nameSBox   = NULL;
	coordSBox  = NULL;
	vertexSBox = NULL;
	jointSBox  = NULL;
	otherSBox  = NULL;
}



CShowOBJInfoDLG::~CShowOBJInfoDLG()
{
}



void CShowOBJInfoDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CShowOBJInfoDLG, CDialogEx)
END_MESSAGE_MAP()



////////////////////////////////////////////////////////////////////////
// CShowInfoDLG メッセージ ハンドラー

BOOL   CShowOBJInfoDLG::OnInitDialog()
{
	//TCHAR buf[LNAME];

	nameSBox   = (CStatic*)GetDlgItem(IDC_STATIC_OBJNAME);
	coordSBox  = (CStatic*)GetDlgItem(IDC_STATIC_CNTCRD);
	vertexSBox = (CStatic*)GetDlgItem(IDC_STATIC_VRTXNUM);
	jointSBox  = (CStatic*)GetDlgItem(IDC_STATIC_JNTNUM);
	otherSBox  = (CStatic*)GetDlgItem(IDC_STATIC_OTHER);

	/*
	sntprintf(buf, LNAME, _T("%s"), regionName.GetString());
	nameSBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T("%d.%d"), majorVersion, minorVersion);
	verSBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T("%dx%d"), rgSize, rgSize);
	sizeSBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T("%d"), objNum);
	numSBox->SetWindowText(buf);
	*/
	return TRUE;
}