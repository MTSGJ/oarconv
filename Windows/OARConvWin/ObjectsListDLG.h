#pragma once


// CObjectsListDLG ダイアログ

class CObjectsListDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CObjectsListDLG)

public:
	CObjectsListDLG(tList* olst, COARConvWinApp* app, CWnd* pParent=NULL);
	virtual ~CObjectsListDLG();

public:
	CWnd*		pWnd;
	UINT		dialogID;

public:
	int         objNum;
	tList*      objList;
	//
	int         slctNum;
	int*        slctItems;
	//
	int         findPos;
	CString     findStr;

	CListBox*	listLBox;
	CEdit*      findEBox;
	CButton*	findBBox;
	CButton*    convBBox;

	COARConvWinApp* winApp;

public:
	void   Destroy(void);
	void   Display(void);

	void   PrintSelectedObjects(void);
	int    SearchString(int stn, LPCTSTR str);
	void   OpenPreviewWindow(int index);
	void   OpenOBJInfoDLG(int index);

//
// ダイアログ データ
	enum { IDD = IDD_DIALOG_OBJLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedObjlistClose();
	afx_msg void OnBnClickedObjlistConv();
	virtual void OnCancel();
	afx_msg void OnLbnDblclkListObjects();
	afx_msg void OnBnClickedObjlistFind();
	afx_msg void OnBnClickedObjlistClear();
	virtual void OnOK();
	afx_msg void OnBnClickedObjlistPreview();
	afx_msg void OnEnChangeEditFndstr();
	afx_msg void OnLbnSelchangeListObjects();
};
