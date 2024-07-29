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

	BOOL     outputTerrain;
	BOOL     debugMode;
	
	float    xShift;
	float    yShift;
	float    zShift;

	CString  prefixOAR;
	CString  prefixDAE;
	CString  prefixOBJ;
	CString  prefixGLTF;
	CString  prefixGLB;
	CString  prefixFBX;
	CString  prefixSTL;

//
protected:
	CEdit*	 startNumEBox;
	CEdit*   stopNumEBox;
	CEdit*   terrainScaleEBox;

	//CButton* outputDaeButton;
	//CButton* outputObjButton;
	//CButton* outputStlButton;
	CButton* outputTerrainButton;
	CButton* debugModeButton;

	CEdit*   xShiftEBox;
	CEdit*   yShiftEBox;
	CEdit*   zShiftEBox;

	CEdit*   prefixOarEBox;
	CEdit*   prefixDaeEBox;
	CEdit*   prefixObjEBox;
	CEdit*   prefixGltfEBox;
	CEdit*   prefixGlbEBox;
	CEdit*   prefixFbxEBox;
	CEdit*   prefixStlEBox;

public:
	void     getParameters(CParameterSet* param);
//
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};
