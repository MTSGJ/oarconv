// BREPView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "DX9.h"
#include "TriBrep.h"
#include "BREPView.h"
#include "BREPDoc.h"
#include "ProgressBarDLG.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace jbxl;
using namespace jbxwl;




extern UINT TimerID;



/////////////////////////////////////////////////////////////////////////////
// CBREPView

IMPLEMENT_DYNCREATE(CBREPView, CExView)

CBREPView::CBREPView()
{
	solided    = true;
	wired	   = false;
	cnstXYRate = true;
//	rotation = false;
}


CBREPView::~CBREPView()
{
}



BEGIN_MESSAGE_MAP(CBREPView, CDxVTXBaseView)
	//{{AFX_MSG_MAP(CBREPView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// CBREPView 描画
/*
void CBREPView::OnDraw(CDC* pDC)
{
//	ExecWindowReSize();
//	ExecRender();
}
*/



/////////////////////////////////////////////////////////////////////////////
// CBREPView 診断

#ifdef _DEBUG
void CBREPView::AssertValid() const
{
	CDxVTXBaseView::AssertValid();
}

void CBREPView::Dump(CDumpContext& dc) const
{
	CDxVTXBaseView::Dump(dc);
}
#endif //_DEBUG




/////////////////////////////////////////////////////////////////////////////
// CBREPView メッセージ ハンドラ

void  CBREPView::OnInitialUpdate() 
{
	Solid = ((CBREPDoc*)pDoc)->Solid;
	if (Solid==NULL) return;
	facetno = Solid->facetno;
	if (facetno==0) return;

	center = D3DXVECTOR3((float)((Solid->rbound.xmax + Solid->rbound.xmin)/2.),
						 (float)((Solid->rbound.ymax + Solid->rbound.ymin)/2.),
						 (float)((Solid->rbound.zmax + Solid->rbound.zmin)/2.));

	xsize = Solid->rbound.xmax - Solid->rbound.xmin;
	ysize = Solid->rbound.ymax - Solid->rbound.ymin;
	zsize = Solid->rbound.zmax - Solid->rbound.zmin;
	msize = Max(xsize, ysize);
	msize = Max(msize, zsize);
	sizeFac = Min(sizeFac, 10./msize);
	
	//DEBUG_Info("msize = %f", msize);
	//DEBUG_Info("%f %f %f", Solid->rbound.xmax, Solid->rbound.ymax, Solid->rbound.zmax);
	//DEBUG_Info("%f %f %f", Solid->rbound.xmin, Solid->rbound.ymin, Solid->rbound.zmin);
	//DEBUG_Info("%f %f %f", center.x, center.y, center.z);
	//DEBUG_Info("%f %f %f", xsize, ysize, zsize);

	int dxsize = (int)msize;
	if (dxsize<MSG_DEFAULT_WINDOW_SIZE) dxsize = MSG_DEFAULT_WINDOW_SIZE;

	BOOL rslt = InitDevice(dxsize, dxsize);
	if (!rslt) {
		MessageBox(_T("BREPview::OnInitialUpdate: Failed to get DirextX device"));
		pFrame->doneErrorMessage = TRUE;
		return;
	}

	if (!PrepareVB()) return;
	SetState();

	SetWindowSize(300, 300);
	hasViewData = TRUE;
	return;
}



void    CBREPView::SetState()
{
	D3DLIGHT9    m_light;
	D3DXVECTOR3  mVecDir;

	// Z バッファ有効化
	lpD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// アルファブレンディング無効化
	lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// ポリゴンの両面を描画?
	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// フィルタ設定
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	// レンダリング時の色の計算方法の設定
	lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	// ライト
	lpD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
	m_light.Type = D3DLIGHT_DIRECTIONAL;
	m_light.Diffuse.r = 1.0f;
	m_light.Diffuse.g = 1.0f;
	m_light.Diffuse.b = 1.0f;

	// ライトの方向
//	mVecDir = D3DXVECTOR3(1.0, -1.0, -2.0);
//	mVecDir = D3DXVECTOR3(1.0, 2.0, 1.0);
	mVecDir = D3DXVECTOR3(1.0, 1.5, 1.0);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_light.Direction, &mVecDir);
	lpD3DDevice->SetLight(0, &m_light);
	lpD3DDevice->LightEnable(0, TRUE);
	
	// プロジェクション行列を設定: 視界 60 度，近くの限界 0.1，遠くの限界 10*msize*sizeFac
	D3DXMatrixPerspectiveFovLH(&matProj, (float)(PI/3.0f), 1.0f, 0.1f, (float)(10*msize*sizeFac));
	lpD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	// カメラの制御:　カメラの位置，物体の位置，カメラの上方向の傾き()
//	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0,0,1.5*msize*sizeFac), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0));
//	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0,-1.5f*msize*sizeFac,0), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,0,-1));
//	lpD3DDevice->SetTransform(D3DTS_VIEW, &matView);
	if (mWheelAc==0.0) mWheelAc = -1.5*msize*sizeFac;
	ExMouseWheel();

	// 頂点フォーマット設定
	lpD3DDevice->SetFVF(FVF_SBREP_VERTEX);
	
	D3DXMatrixTranslation(&matTrans, -(float)(center.x*sizeFac), -(float)(center.y*sizeFac), (float)(center.z*sizeFac));
	lpD3DDevice->SetTransform(D3DTS_WORLD, &matTrans);
}



// 
void   CBREPView::ExMouseWheel()
{
	if (Xabs(mWheelAc)<0.5) mWheelAc = Sign(mWheelAc)*0.5; // 0.5 以下には接近しない

	// カメラの制御:　カメラの位置，物体の位置，カメラの上方向の傾き()
	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0,(float)mWheelAc,0), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,0,-1));
	lpD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}



void    CBREPView::ExecRender()
{
	HRESULT hr;

	// 画面のクリア
//	hr = lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,96), 1.0f, 0);
//	hr = lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(10,10,120), 1.0f, 0);
	hr = lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(30,30,200), 1.0f, 0);
	if (FAILED(hr)) { DEBUG_MODE PRINT_MESG("CBREPView::ExecRender: Failed to clear window\n"); return;}


	// シーンの描画開始
	hr = lpD3DDevice->BeginScene();	
	if (FAILED(hr)) { DEBUG_MODE PRINT_MESG("CBREPView::ExecRender: Failed to start scene\n"); return;}
	
	if (solided) {
		hr = lpD3DDevice->SetStreamSource(0, vb, 0, sizeof(SBREP_VERTEX));
		if (FAILED(hr)) { DEBUG_MODE PRINT_MESG("CBREPView::ExecRender: Failed to setup stream: BREP_SOLID\n"); lpD3DDevice->EndScene(); return;}
		hr = Dx9DrawPrimitive(lpD3DDevice, D3DPT_TRIANGLELIST, 3, facetno);
		if (FAILED(hr)) { DEBUG_MODE PRINT_MESG("CBREPView::ExecRender: Failed to draw primitive: BREP_SOLID\n"); lpD3DDevice->EndScene(); return;}
	}

	if (wired) {
		hr = lpD3DDevice->SetStreamSource(0, lb, 0, sizeof(SBREP_VERTEX));
		if (FAILED(hr)) { DEBUG_MODE PRINT_MESG("CBREPView::ExecRender: Failed to setup stream: WIRE\n"); lpD3DDevice->EndScene(); return;}
		hr = Dx9DrawPrimitive(lpD3DDevice, D3DPT_LINELIST, 2, facetno*3);
		if (FAILED(hr)) { DEBUG_MODE PRINT_MESG("CBREPView::ExecRender: Failed to draw primitive: WIRE\n"); lpD3DDevice->EndScene(); return;}
	}

	hr = lpD3DDevice->EndScene();	// シーンの描画終了
	if (FAILED(hr)) { DEBUG_MODE PRINT_MESG("CBREPView::ExecRender: Failed to stop scene\n"); return;}
	// シーンの描画終了

	// 画面表示
	hr = lpD3DDevice->Present(NULL, NULL, NULL, NULL);
	if (FAILED(hr)) {	// デバイスチェック
//		DEBUG_MODE PRINT_MESG("CBREPView::ExecRender: デバイスリセット中!!"); 
		hr = ResetDx9Device(lpD3DDevice, &d3dParam, this);
		if (FAILED(hr)) {
			MessageBox(_T("CBREPView::ExecRender: Failed to repair DirextX device!!\n"));
			GetParentFrame()->DestroyWindow();
		}
	}
}



// 頂点バッファの準備
BOOL   CBREPView::PrepareVB()
{
	lpD3DDevice->CreateVertexBuffer(sizeof(SBREP_VERTEX)*facetno*3, 0, FVF_SBREP_VERTEX, D3DPOOL_MANAGED, &vb, NULL);
	lpD3DDevice->CreateVertexBuffer(sizeof(SBREP_VERTEX)*facetno*6, 0, FVF_SBREP_VERTEX, D3DPOOL_MANAGED, &lb, NULL);
	if (vb==NULL || lb==NULL) {			// メモリ確保失敗
		if (!isNull(Solid->counter)) {		// カウンタが存在すればカウンタ自体を削除する．
			delete (CProgressBarDLG*)(Solid->counter);
			Solid->counter = NULL;
		}
		ReleaseVB();
		MessageBox(_T("CBREPView::PrepareVB: Failed to secure the vertex buffer"), _T("Error"));
 		return  FALSE;
	}

	SBREP_VERTEX *v, *lv;
	vb->Lock(0, 0, (void **)&v,  0);
	lb->Lock(0, 0, (void **)&lv, 0);

	int intvl = 1;
	BREP_CONTOUR_LIST contours = Solid->contours;
	CProgressBarDLG*  counter  = NULL;
	if (Solid->counter!=NULL) {
		counter = (CProgressBarDLG*)(Solid->counter->GetUsableCounter());
		if (counter!=NULL) {
			counter->SetTitle("Processing of the Vertex Buffer");
			intvl = Max(1, (int)(contours.size())/10);
			counter->SetMax(10);
		}
	}

	int cnt=0, lct=0;
	BREP_CONTOUR_LIST::iterator contour;
	for (contour=contours.begin(); contour!=contours.end(); contour++){
		BREP_WING* first = (*contour)->wing;
		if (first!=NULL) {
			BREP_WING* next = first;
			do {
				BREP_WING* wing = next;
				next = wing->next;

				// Facet
				v[cnt].x  =  (float)(wing->vertex->point.x*sizeFac);
				v[cnt].y  =  (float)(wing->vertex->point.y*sizeFac);
				v[cnt].z  = -(float)(wing->vertex->point.z*sizeFac);
				v[cnt].nx =  (float)((*contour)->normal.x);
				v[cnt].ny =  (float)((*contour)->normal.y);
				v[cnt].nz = -(float)((*contour)->normal.z);
//				v[cnt].nx =  (float)(wing->vertex->normal.x);
//				v[cnt].ny =  (float)(wing->vertex->normal.y);
//				v[cnt].nz = -(float)(wing->vertex->normal.z);

				DupEdgeNumber((BREP_CONTOUR*)(*contour));
				v[cnt].color = D3DCOLOR_RGBA(200,200,200,200);
				if (((BREP_CONTOUR*)(*contour))->dup_edge==3) v[cnt].color = D3DCOLOR_RGBA(200,0,0,200);
				if (((BREP_CONTOUR*)(*contour))->collision)   v[cnt].color = D3DCOLOR_RGBA(200,200,0,200);
//				if (!((BREP_EDGE*)(wing->edge))->complete)    v[cnt].color = D3DCOLOR_RGBA(0,200,0,200);

				// Line
				lv[2*cnt].x   =  (float)(wing->vertex->point.x*sizeFac);
				lv[2*cnt].y   =  (float)(wing->vertex->point.y*sizeFac);
				lv[2*cnt].z   = -(float)(wing->vertex->point.z*sizeFac);
				lv[2*cnt+1].x =  (float)(next->vertex->point.x*sizeFac);
				lv[2*cnt+1].y =  (float)(next->vertex->point.y*sizeFac);
				lv[2*cnt+1].z = -(float)(next->vertex->point.z*sizeFac);

				lv[2*cnt].color   = D3DCOLOR_RGBA(32,32,32,127);
				lv[2*cnt+1].color = D3DCOLOR_RGBA(32,32,32,127);
				cnt++;
			} while(next!=first);
		}

		if (counter!=NULL) {
			if (lct%intvl==0) counter->StepIt();
		}

		lct++;
	}

	//
	if (Solid->counter!=NULL) {		// カウンタが存在すればカウンタ自体を削除する．
		delete (CProgressBarDLG*)(Solid->counter);
		Solid->counter = NULL;
	}
 
	vb->Unlock();
	lb->Unlock();

	return TRUE;
}



//
//  (angleX, angleY -> angle?, angle?)
// 
void   CBREPView::ExRotationAngle()
{
	angleZ =  angleX;
	angleX = -angleY;
	angleY =  0.0f;
}



void   CBREPView::ClearObject()
{
	hasViewData = FALSE;
} 



void   CBREPView::InitObject()
{
	SetState();
	hasViewData = TRUE;
}



void CBREPView::SwitchWired() 
{	
	if (wired) wired = false;
	else       wired = true;
}



void CBREPView::SwitchSolid() 
{
	if (solided) solided = false;
	else         solided = true;
}


