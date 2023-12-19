#pragma once


// CSetParamDLG ダイアログ

class CSetParamDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CSetParamDLG)

public:
	CSetParamDLG(CParameterSet* param, CWnd* pParent=NULL);   // 標準コンストラクター
	virtual ~CSetParamDLG();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_OARSET };

//
protected:
	int      startNum;
	int      stopNum;
	float    terrainScale;

	BOOL     outputDae;
	BOOL     outputTerrain;
	BOOL     debugMode;
	
	float    xShift;
	float    yShift;
	float    zShift;

	CString  prefixOAR;
	CString  prefixDAE;
	CString  prefixSTL;

//
protected:
	CEdit*	 startNumEBox;
	CEdit*   stopNumEBox;
	CEdit*   terrainScaleEBox;

	CButton* outputDaeButton;
	CButton* outputStlButton;
	CButton* outputTerrainButton;
	CButton* debugModeButton;

	CEdit*   xShiftEBox;
	CEdit*   yShiftEBox;
	CEdit*   zShiftEBox;

	CEdit*   prefixOarEBox;
	CEdit*   prefixDaeEBox;
	CEdit*   prefixStlEBox;

public:
	void     getParameters(CParameterSet* param);
//
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
