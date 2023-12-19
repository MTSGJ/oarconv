// ShowInformationDLG.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OARConvWin.h"
#include "ShowInfoDLG.h"


// CShowInfoDLG ダイアログ

IMPLEMENT_DYNAMIC(CShowInfoDLG, CDialogEx)


CShowInfoDLG::CShowInfoDLG(OARTool oar, CWnd* pParent)
	: CDialogEx(CShowInfoDLG::IDD, pParent)
{
	regionName   = (char*)oar.regionName.buf;
	majorVersion = oar.majorVersion;
	minorVersion = oar.minorVersion;
	rgSize = oar.xsize;
	objNum = oar.objectsNum;

	nameSBox = NULL;
	verSBox  = NULL;
	sizeSBox = NULL;
	numSBox  = NULL;
}



CShowInfoDLG::~CShowInfoDLG()
{
}



void CShowInfoDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CShowInfoDLG, CDialogEx)
END_MESSAGE_MAP()



////////////////////////////////////////////////////////////////////////
// CShowInfoDLG メッセージ ハンドラー

BOOL   CShowInfoDLG::OnInitDialog()
{
	TCHAR buf[LNAME];

	nameSBox = (CStatic*)GetDlgItem(IDC_STATIC_RGNAME);
	verSBox	 = (CStatic*)GetDlgItem(IDC_STATIC_OARVER);
	sizeSBox = (CStatic*)GetDlgItem(IDC_STATIC_RGSIZE);
	numSBox	 = (CStatic*)GetDlgItem(IDC_STATIC_OBJNUM);

	sntprintf(buf, LNAME, _T("%s"), regionName);
	nameSBox->SetWindowText(buf);

	sntprintf(buf, LNAME, _T("%d.%d"), majorVersion, minorVersion);
	verSBox->SetWindowText(buf);

	sntprintf(buf, LNAME, _T("%dx%d"), rgSize, rgSize);
	sizeSBox->SetWindowText(buf);

	sntprintf(buf, LNAME, _T("%d"), objNum);
	numSBox->SetWindowText(buf);

	return TRUE;
}