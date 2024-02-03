// BREPFrm.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "BREPFrame.h"
#include "BREPView.h"
#include "BREPDoc.h"
#include "resource.h"
#include ".\brepframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace jbxl;
using namespace jbxwl;


/////////////////////////////////////////////////////////////////////////////
// CBREPFrm

IMPLEMENT_DYNCREATE(CBREPFrame, CExFrame)

BEGIN_MESSAGE_MAP(CBREPFrame, CExFrame)
	//{{AFX_MSG_MAP(CBREPFrame)
	ON_COMMAND(ID_WIRED, OnWired)
	ON_COMMAND(ID_SOLID, OnSolid)
	ON_WM_CREATE()
	ON_COMMAND(ID_ROTATION, OnRotation)
	ON_COMMAND(ID_REPAIR_NX,  OnRepairNext)
	ON_COMMAND(ID_REPAIR_NR,  OnRepairNear)
	//ON_COMMAND(ID_REPAIR_C,  OnRepairCancel)
	ON_COMMAND(ID_SAVE_D, OnSaveD)
	ON_COMMAND(ID_SAVE_S, OnSaveS)
	ON_COMMAND(ID_INFO, OnSolidInfo)
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CBREPFrame クラスの構築/消滅

CBREPFrame::CBREPFrame()
{
	// TODO: メンバ初期化コードをこの位置に追加してください。
}


CBREPFrame::~CBREPFrame()
{
}


BOOL CBREPFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~(FWS_PREFIXTITLE | FWS_ADDTOTITLE); 
	
	if( !CExFrame::PreCreateWindow(cs) ) return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CBREPFrame クラスの診断

#ifdef _DEBUG
void CBREPFrame::AssertValid() const
{
	CExFrame::AssertValid();
}


void CBREPFrame::Dump(CDumpContext& dc) const
{
	CExFrame::Dump(dc);
}

#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CBREPFrame クラスのメッセージハンドラ
//

int CBREPFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CExFrame::OnCreate(lpCreateStruct) == -1) return -1;

	// ツールバーの作成
	toolBar = new CExToolBar();
	if (!toolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!toolBar->LoadToolBar(IDR_BREP))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // 作成に失敗
	}

	// TODO: ツール バーをドッキング可能にしない場合は以下の３行を削除してください。
	toolBar->EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(toolBar);
	
	return 0;
}


////////////////////////////////////////////////////////

void CBREPFrame::OnWired() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if (!isNull(pView)) ((CBREPView*)pView)->SwitchWired();
}


void CBREPFrame::OnSolid() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if (pView!=NULL) ((CBREPView*)pView)->SwitchSolid();
}


void CBREPFrame::OnRotation() 
{
	if (pView!=NULL) ((CBREPView*)pView)->SwitchRotation();
}


void CBREPFrame::OnRepairNext() 
{
	bool  start_r = false;
	CBREPView* pviw = (CBREPView*)pView;
	CBREPDoc*  pdoc = (CBREPDoc*)pDoc;

	if (pviw!=NULL) {
		if (pviw->rotation) {
			start_r = true;
			pviw->SwitchRotation();
		}

		if (pDoc!=NULL) {
			pdoc->SolidRepair(1);
			if (pdoc->hasViewData) {
				pviw->ReleaseVB();
				pviw->facetno = (unsigned int)(pviw->Solid->contours.size());
				pviw->PrepareVB();
				pviw->SetState();
				pviw->hasViewData = TRUE;
			}
		}
		if (start_r) pviw->SwitchRotation();
	}
}


void CBREPFrame::OnRepairNear() 
{
	bool  start_r = false;
	CBREPView* pviw = (CBREPView*)pView;
	CBREPDoc*  pdoc = (CBREPDoc*)pDoc;

	if (pviw!=NULL) {
		if (pviw->rotation) {
			start_r = true;
			pviw->SwitchRotation();
		}

		if (pDoc!=NULL) {
			pdoc->SolidRepair(2);
			if (pdoc->hasViewData) {
				pviw->ReleaseVB();
				pviw->facetno = (unsigned int)(pviw->Solid->contours.size());
				pviw->PrepareVB();
				pviw->SetState();
				pviw->hasViewData = TRUE;
			}
		}
		if (start_r) pviw->SwitchRotation();
	}
}


//void CBREPFrame::OnRepairCancel() 
//{
//}


void CBREPFrame::OnSaveD() 
{
	((CBREPDoc*)pDoc)->SaveFile(true);	
}


void CBREPFrame::OnSaveS() 
{
	((CBREPDoc*)pDoc)->SaveFile(false);
}


// Solid の状態を表示する．
void CBREPFrame::OnSolidInfo()
{
	((CBREPDoc*)pDoc)->ShowSolidInfo();
}


void CBREPFrame::OnClose()
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	//if (((CBREPDoc*)pDoc)->ChangeData) ((CBREPDoc*)pDoc)->SaveFile(false);
	CExFrame::OnClose();
}


void CBREPFrame::OnDestroy()
{
//	if (((CBREPDoc*)pDoc)->ChangeData) ((CBREPDoc*)pDoc)->SaveFile(false);
	CExFrame::OnDestroy();

	// TODO : ここにメッセージ ハンドラ コードを追加します。
}

