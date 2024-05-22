#pragma once


// CShowOBJInfoDLG ダイアログ

class CShowOBJInfoDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CShowOBJInfoDLG)

public:
	CString objectName;
	CString coordinate;
	CString other;
	Vector<double> coordVec;
	int    vertexNnum;
	int	   jointNum;

	CStatic* nameSBox;
	CStatic* coordSBox;
	CStatic* vertexSBox;
	CStatic* jointSBox;
	CStatic* otherSBox;

public:
	CShowOBJInfoDLG(char* objname, OARTool oar, CWnd* pParent=NULL);   // 標準コンストラクター
	virtual ~CShowOBJInfoDLG();

	virtual BOOL  OnInitDialog();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_OBJINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
