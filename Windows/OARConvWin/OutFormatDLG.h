#pragma once


// COutFormatDLG ダイアログ

class COutFormatDLG : public CDialogEx
{
	DECLARE_DYNAMIC(COutFormatDLG)

public:
	COutFormatDLG(CParameterSet* param, CWnd* pParent=NULL);   // 標準コンストラクター
	virtual ~COutFormatDLG();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_OUTPUT };

//
protected:
	int      outputFormat;
	int      outputEngine;
//
protected:
	CButton* outputDaeButton;
	CButton* outputObjButton;
	CButton* outputStlButton;

	CButton* outputUnityButton;
	CButton* outputUEButton;

public:
	void     getParameters(CParameterSet* param);
//
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedRadioObj();
	afx_msg void OnBnClickedRadioDae();
	afx_msg void OnBnClickedRadioStl();
};
