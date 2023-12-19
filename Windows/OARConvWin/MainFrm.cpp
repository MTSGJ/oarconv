// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"

#include "OARConvWin.h"
#include "MainFrm.h"
#include "WinTools.h"


using namespace jbxl;
using namespace jbxwl;



// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
//	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_WM_DROPFILES()
//	ON_COMMAND(ID_APP_EXIT, OnAppExit)
    ON_WM_INITMENU()
	ON_WM_SIZE()
	ON_WM_MOVE()
END_MESSAGE_MAP()


static UINT indicators[] =
{
	IDS_STR_STATUSBAR_INIT
	//ID_SEPARATOR,           // ステータス ライン インジケータ
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};





/////////////////////////////////////////////////////////////////////////////
// CMainFrame コンストラクション/デストラクション
CMainFrame::CMainFrame()
{
	pApp = NULL;
	memset(&windowSize, 0, sizeof(RECT));

	// メニューの有効，無効の切り替えを可能にする
	m_bAutoMenuEnable = FALSE;
}



CMainFrame::~CMainFrame()
{
	DEBUG_INFO("DESTRUCTOR: CMainFrame: START\n");

	if (pApp!=NULL) {
		if (pApp->pMainFrame!=NULL) {
			pApp->appParam.saveWindowSize(pApp->pMainFrame->windowSize);
			pApp->pMainFrame = NULL;
		}
	}
	DEBUG_INFO("DESTRUCTOR: CMainFrame: END\n");
}



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;

	// ステータスバー
	if (!m_wndStatusBar.Create(this)) return -1;
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	
	return 0;
}



BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CMDIFrameWnd::PreCreateWindow(cs)) return FALSE;

	cs.style |= WS_CLIPCHILDREN;
	cs.style &= ~(FWS_PREFIXTITLE | FWS_ADDTOTITLE); 
	cs.lpszName = _T(OARCONV_VERSION);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// ステータスバーへの設定

void  CMainFrame::SetStausBarText(CString mesg)
{
	m_wndStatusBar.SetPaneText(0, mesg);
	return;
}




/////////////////////////////////////////////////////////////////////////////
// CMainFrame 診断

#ifdef _DEBUG

void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}


void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG




/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	int size = DragQueryFile(hDropInfo, 0, 0, 0) + 1;
	if (size<=0) return;

	TCHAR* buf = new TCHAR[size];
	DragQueryFile(hDropInfo, 0, buf, size);
	CString fname = buf;
	
	bool ret = theApp.fileOpen(fname);
	if (ret) theApp.showOARInfoDLG();

	delete[] buf;
}



void  CMainFrame::OnInitMenu(CMenu* pMenu)
{
	if (pApp!=NULL) pApp->updateMenuBar(pMenu);

	CMDIFrameWnd::OnInitMenu(pMenu);
}



void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIFrameWnd::OnSize(nType, cx, cy);

	GetWindowRect(&windowSize);
}



void CMainFrame::OnMove(int x, int y)
{
	CMDIFrameWnd::OnMove(x, y);

	GetWindowRect(&windowSize);
}