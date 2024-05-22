#pragma once


// CShowOBJInfoDLG ダイアログ

class CShowOBJInfoDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CShowOBJInfoDLG)

public:
	PrimBaseShape shape;
	Vector<double> shift;

	CString fileName;
	CString objectName;
	CString objectKind;
	CString other;

	CStatic* fileSBox;
	CStatic* nameSBox;
	CStatic* kindSBox;
	CStatic* coordSBox;
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
public:
	afx_msg void OnStnClickedStaticObjkind();
};
