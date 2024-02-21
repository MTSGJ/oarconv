#if !defined(AFX_BREPFRM_H__7C9C023C_51DB_4BF5_8C9C_7929AA8155F9__INCLUDED_)
#define AFX_BREPFRM_H__7C9C023C_51DB_4BF5_8C9C_7929AA8155F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
CBREPFrame クラス

	CExFrameクラスの継承クラス
	CBREPView とCBREODoc を繋ぐためのフレームテンプレートクラス．処理も中継する．

*/

#include  "ExClass.h"


using namespace jbxl;
using namespace jbxwl;


/////////////////////////////////////////////////////////////////////////////
// CBREPFrame フレーム

class CBREPFrame : public CExFrame
{
	DECLARE_DYNCREATE(CBREPFrame)
public:
	CBREPFrame();

// アトリビュート
public:

// オペレーション
public:
//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CBREPFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CBREPFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成したメッセージ マップ関数
protected:
	//{{AFX_MSG(CBREPFrame)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRotation();
	afx_msg void OnWired();
	afx_msg void OnSolid();
	afx_msg void OnSolidInfo();
	//afx_msg void OnSaveD();
	//afx_msg void OnSaveS();
	//afx_msg void OnRepairNext();
	//afx_msg void OnRepairNear();
//	afx_msg void OnRepairCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	// Solid の状態を表示する．
	afx_msg void OnDestroy();
	afx_msg void OnClose();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_BREPFRM_H__7C9C023C_51DB_4BF5_8C9C_7929AA8155F9__INCLUDED_)
