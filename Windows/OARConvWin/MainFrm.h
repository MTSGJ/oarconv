// MainFrm.h : CMainFrame クラスのインターフェイス
//
#pragma once

#pragma warning(disable:4995)


#include "ExToolBar.h"

class COARConvWinApp;

using namespace jbxwl;



class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

//
protected:  // コントロール バー用メンバ
	CStatusBar  m_wndStatusBar;
	CExToolBar  m_wndToolBar;

public:
	void  SetStausBarText(CString mesg);

	RECT  windowSize;
	COARConvWinApp*  pApp;

//
// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//
// 生成された、メッセージ割り当て関数
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnInitMenu(CMenu* pMenu);
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnFileSaveAs();
	afx_msg void OnDropFiles(HDROP hDropInfo);
//	afx_msg void OnAppExit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
};


