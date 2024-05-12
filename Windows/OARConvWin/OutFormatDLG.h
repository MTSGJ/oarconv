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
	BOOL     degeneracy;		// No Offset
	BOOL	 procJoints;
//
protected:
	CButton* outputDaeButton;
	CButton* outputObjButton;
	CButton* outputONOButton;	// OBJ with No Offset
	CButton* outputFbxButton;
	CButton* outputFNOButton;	// FBX with No Offset
	CButton* outputStlButton;

	CButton* outputUnityButton;
	CButton* outputUEButton;

	CButton* procJointsButton;

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
	afx_msg void OnBnClickedRadioObjNo();
	afx_msg void OnBnClickedRadioFbx();
	afx_msg void OnBnClickedRadioFbxNo();
	//afx_msg void OnBnClickedCheckJoints();
};
