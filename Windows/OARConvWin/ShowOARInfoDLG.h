#pragma once


// CShowInfoDLG ダイアログ

class CShowOARInfoDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CShowOARInfoDLG)

public:
	CString regionName;
	int	   majorVersion;
	int	   minorVersion;
	int    rgSize;
	int	   objNum;

	CStatic* nameSBox;
	CStatic* verSBox;
	CStatic* sizeSBox;
	CStatic* numSBox;

public:
	CShowOARInfoDLG(OARTool oar, CWnd* pParent=NULL);   // 標準コンストラクター
	virtual ~CShowOARInfoDLG();

	virtual BOOL  OnInitDialog();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_OARINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
