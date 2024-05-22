// ShowInformationDLG.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OARConvWin.h"
#include "ShowOARInfoDLG.h"


// CShowOARInfoDLG ダイアログ

IMPLEMENT_DYNAMIC(CShowOARInfoDLG, CDialogEx)


CShowOARInfoDLG::CShowOARInfoDLG(OARTool oar, CWnd* pParent)
	: CDialogEx(CShowOARInfoDLG::IDD, pParent)
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



CShowOARInfoDLG::~CShowOARInfoDLG()
{
}



void CShowOARInfoDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CShowOARInfoDLG, CDialogEx)
END_MESSAGE_MAP()



////////////////////////////////////////////////////////////////////////
// CShowOARInfoDLG メッセージ ハンドラー

BOOL   CShowOARInfoDLG::OnInitDialog()
{
	nameSBox = (CStatic*)GetDlgItem(IDC_STATIC_RGNAME);
	verSBox	 = (CStatic*)GetDlgItem(IDC_STATIC_OARVER);
	sizeSBox = (CStatic*)GetDlgItem(IDC_STATIC_RGSIZE);
	numSBox	 = (CStatic*)GetDlgItem(IDC_STATIC_OBJNUM);

	TCHAR buf[LNAME];
	sntprintf(buf, LNAME, _T("%s"), regionName.GetString());
	nameSBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T("%d.%d"), majorVersion, minorVersion);
	verSBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T("%dx%d"), rgSize, rgSize);
	sizeSBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T("%d"), objNum);
	numSBox->SetWindowText(buf);

	return TRUE;
}