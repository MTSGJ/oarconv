// BREPDoc.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "BREPDoc.h"
#include "STL.h"
#include "TriBrep.h"
#include "ProgressBarDLG.h"
#include "WinTools.h"
#include "MessageBoxDLG.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace jbxl;
using namespace jbxwl;



/////////////////////////////////////////////////////////////////////////////
// CBREPDoc

IMPLEMENT_DYNCREATE(CBREPDoc, CExDocument)

CBREPDoc::CBREPDoc()
{
	Solid = new BREP_SOLID();

	ChangeData = false;
	CallSave   = false;

	DataNum = -1;
	WinApp  = NULL;
}



CBREPDoc::~CBREPDoc()
{
//	if (ChangeData && !CallSave) SaveFile(false);

	CString  tname = Title + _T(" deleting data");
	CProgressBarDLG* counter = new CProgressBarDLG(IDD_PROGBAR, (LPCTSTR)tname);
	Solid->counter = counter;
	counter->Start(100);
	//DEBUG_ERR("BREP_FACET  = %d", Solid->facetno);
	//DEBUG_ERR("BREP_VERTEX = %d", Solid->vertexno);

	delete Solid;		// Shellと verticesのdeleteでカウンター表示

	if (counter!=NULL) {
		counter->PutFill();
		delete counter;
	}
}



BEGIN_MESSAGE_MAP(CBREPDoc, CExDocument)
	//{{AFX_MSG_MAP(CBREPDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CBREPDoc 診断

#ifdef _DEBUG
void CBREPDoc::AssertValid() const
{
	CExDocument::AssertValid();
}


void CBREPDoc::Dump(CDumpContext& dc) const
{
	CExDocument::Dump(dc);
}
#endif //_DEBUG





/////////////////////////////////////////////////////////////////////////////
// CBREPDoc シリアライズ

void CBREPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: この位置に保存用のコードを追加してください
	}
	else
	{
		// TODO: この位置に読み込み用のコードを追加してください
	}
}




/////////////////////////////////////////////////////////////////////////////
// CBREPDoc コマンド

//
// ExecDocFrmView()でファイル名が NULLの場合は，こちらが呼び出される．
//
BOOL CBREPDoc::OnNewDocument()
{
	hasReadData = FALSE;
	hasViewData = FALSE;

//	if (!CExDocument::OnNewDocument()) return FALSE;

	pFrame->GetMDIFrame()->RedrawWindow();

	if (Solid==NULL) {
		MessageBox(pFrame->m_hWnd, _T("CBREPDoc::OnNewDocument: Solid is NULL"), _T("Error"), MB_OK);
		pFrame->doneErrorMessage = TRUE;
		return FALSE;
	}

	hasReadData = TRUE;
	hasViewData = TRUE;

	return TRUE;
}



//
// ExecDocFrmView()でファイル名が NULLでない場合は，こちらが呼び出される．
//
BOOL CBREPDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	hasReadData = FALSE;
	hasViewData = FALSE;

//	if (!CExDocument::OnOpenDocument(lpszPathName)) return FALSE;
	if (lpszPathName==NULL) return  FALSE;

	pFrame->GetMDIFrame()->RedrawWindow();

	long int fno, vfno;
	char* mbstr = ts2mbs(lpszPathName);
	STLData* stldata = ReadSTLFile(mbstr, &fno);
	::free(mbstr);
	if (stldata==NULL) return FALSE;
	hasReadData = TRUE;

	SetVertexTolerance(1.0e-8f);
	SetAbsVertexTolerance(1.0e-6f);

	Solid->counter = new CProgressBarDLG(IDD_PROGBAR, _T("Reading Data")); 
	Solid->counter->Start(30);				// 全目盛り30でスタート
	Solid->counter->MakeChildCounter(29);	// 目盛り29で チャイルドカウンタを作成

	vfno = CreateTriSolidFromSTL(Solid, stldata, fno, true);
	freeSTL(stldata);
	//
	DEBUG_INFO("Facet No.= %d/%d", vfno, fno);
	Solid->counter->DeleteChildCounter();

	if (vfno<=0) {
		if (vfno==-1) {
			MessageBox(pFrame->m_hWnd, _T("CBREPDoc::OnOpenDocument: Solid is NULL"), _T("Error"), MB_OK);
		}
		else if (vfno==-3) {
			MessageBox(pFrame->m_hWnd, _T("CBREPDoc::OnOpenDocument: Canceled"), _T("Cancel"), MB_OK);
			pFrame->cancelOperation = TRUE;
		}
		pFrame->doneErrorMessage = TRUE;
		Solid->counter->Stop();
		delete ((CProgressBarDLG*)(Solid->counter));
		Solid->counter = NULL;
		return FALSE;
	}
	Solid->facetno = vfno;

	//sntprintf(message, LMESG, _T("Number of Data  : %7d\nValid Data      : %7d\nInvalid Data    : %7d\nShortage  Edges : %7d\nDuplicate Edges : %7d\n"),
	//				 fno, vfno, fno-vfno, Solid->shortage_wings.size(), Solid->surplus_contours.size());
	//MessageBox(pFrame->m_hWnd, message, _T("BREP_SOLID Data Report"), MB_ICONQUESTION);

	Solid->counter->MakeChildCounter(1); // 目盛り1を BREPViewへ．deleteはBREPViewが行う． 
	hasViewData = TRUE;

	return TRUE;
}



void  CBREPDoc::SolidRepair(int method)
{
	CProgressBarDLG* counter = new CProgressBarDLG(IDD_PROGBAR, _T("Correcting"), FALSE, pFrame);
	Solid->counter = counter;
	counter->Start(105);

	// false と true. どちらを先に実行するか？
	counter->MakeChildCounter(40);
	ContoursRepair(method, true); 
	counter->DeleteChildCounter();

	counter->MakeChildCounter(60);
	ContoursRepair(method, false);
	counter->DeleteChildCounter();


	CreateSurplusContoursList(Solid);
	CreateShortageWingsList(Solid);

	if (!Solid->shortage_wings.empty() || !Solid->surplus_contours.empty()){
		ShowSolidInfo();
//		DEBUG_Error("BREPDoc: 補填不能データ %d 個", Solid->shortage_wings.size());
	}

	if (Solid->contours.empty()) hasViewData = FALSE;
	Solid->CloseData();

	counter->MakeChildCounter(5);	// BREPView用 deleteはBREPViewが行う． 
	
	ChangeData = true;
	CallSave   = false;
	return;
}



void  CBREPDoc::ContoursRepair(int method, bool mode)
{ 
	
	CProgressBarDLG* counter = NULL;
	if (Solid->counter!=NULL) counter = (CProgressBarDLG*)(Solid->counter->GetUsableCounter());
	if (counter!=NULL) counter->SetMax(200);

	// 重複Contourの削除
	CreateSurplusContoursList(Solid); 
	counter->MakeChildCounter(10);
	DeleteSurplusContours(Solid);
	counter->DeleteChildCounter();

	// 直線に並んだ Edgeの削除
	CreateShortageWingsList(Solid);
	counter->MakeChildCounter(10);
	DeleteStraightEdges(Solid);
	counter->DeleteChildCounter();

	// 不足Contourの充填
	CreateShortageWingsList(Solid);
	counter->MakeChildCounter(140);
	FillShortageWings(Solid, method, mode);		// 充填
	counter->DeleteChildCounter();
	counter->MakeChildCounter(10);
	DeleteShortageWings(Solid);					// 孤立Edge(Wing)の削除
	counter->DeleteChildCounter();

	CreateShortageWingsList(Solid);				// Listの作り直し
	counter->MakeChildCounter(30);
 	FillShortageWings(Solid, method, mode);		// 念のためもう一回充当
	counter->DeleteChildCounter();

	return;
}



void CBREPDoc::SaveFile(bool mode) 
{
	if (WinApp!=NULL && DataNum>=0) {
		if (mode) WinApp->convertOneData(DataNum, TRUE);
		else      WinApp->convertOneData(DataNum, FALSE);
	}

/*
	TCHAR buf[LMESG];
	CString fname;

	CallSave = true;

	if (pFrame==NULL) return;	// フレームは既に削除されている．
	if (mode) sntprintf(buf, LMESG, _T("%s (ASCII)  Specify the file to svae"), Title);
	else      sntprintf(buf, LMESG, _T("%s (Binary) Specify the file to svae"), Title);
	
	fname = EasyGetSaveFileName(buf, _T(""), pFrame->m_hWnd);
	if (fname.IsEmpty()) return;

	int ret = MessageBox(pFrame->m_hWnd, (LPCTSTR)fname, _T("Confirmation"), MB_OKCANCEL);
	if (ret!=IDOK) return;
	
	char* mbstr = ts2mbs(fname);
	ret = WriteSTLFile(mbstr, Solid, mode);	// で保存
	::free(mbstr);
	
	if (ret<=0) { 
		CString fn  = get_file_name_t(fname);	// ファイル名部分
		sntprintf(buf, LMESG, _T("File open error．%s "), fn);
		MessageBox(pFrame->m_hWnd, buf, _T("Error"), MB_OK);
	}
	else {
		ChangeData = false;
	}
	*/
}



// Solid の状態を表示する．
void CBREPDoc::ShowSolidInfo()
{
	//BREP_SOLID* Solid = ((CBREPDoc*)pDoc)->Solid;
	TCHAR message[LMESG];

	sntprintf(message, LMESG, _T("Shortage Edges: %zd, Duplicate Edges: %zd"), 
			Solid->shortage_wings.size(), Solid->surplus_contours.size());
	//MessageBox(message, "CBREPDoc::データレポート", MB_OK);
	MessageBox(pFrame->m_hWnd, message, _T("BREP_SOLID Data Report"), MB_ICONQUESTION);
}



