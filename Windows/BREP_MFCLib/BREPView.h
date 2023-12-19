#if !defined(AFX_BREPVIEW_H__C87FACC3_6850_4C09_B5A3_2DB11EA8BE22__INCLUDED_)
#define AFX_BREPVIEW_H__C87FACC3_6850_4C09_B5A3_2DB11EA8BE22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



/**
CBREPView クラス

	CDxVTXBaseViewクラスからの継承クラス
	CBREPDoc から BREP_SOLID データを貰って，DirectXの機能を使って 3D表示を行う．

*/


#include  "DxBaseClass.h"
#include  "Brep.h"
#include  "TriBrep.h"



using namespace jbxl;
using namespace jbxwl;




typedef struct 
{
	float     x,  y,  z;	// D3DXVECTOR3	頂点座標
	float    nx, ny, nz;	// D3DXVECTOR3	法線ベクトル
	D3DCOLOR color;			// DWORD		ディフューズ色
//	float    tu, tv;		// D3DXVECTOR2	テクスチャ座標
} SBREP_VERTEX;


//#define	 FVF_SBREP_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//#define	 FVF_SBREP_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	 FVF_SBREP_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE)



/////////////////////////////////////////////////////////////////////////////
// CBREPView ビュー

class CBREPView : public CDxVTXBaseView  //CDirectXView
{
protected:
	CBREPView();           // 動的生成に使用されるプロテクト コンストラクタ
	DECLARE_DYNCREATE(CBREPView)

// アトリビュート
public:
	BREP_SOLID*	 Solid;
	unsigned int facetno;

	double  xsize;
	double  ysize;
	double  zsize;
	double  msize;

	bool	solided;
	bool	wired;
	
// オペレーション
public:
	virtual void    SetState();
	virtual BOOL	PrepareVB();

	virtual void	ExecRender();
	virtual void	ClearObject();
	virtual void	InitObject();

	virtual void	ExRotationAngle();
	virtual void	ExMouseWheel();

	void    SwitchSolid();
	void	SwitchWired();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CBREPView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC) {}    
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CBREPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CBREPView)
	afx_msg void OnBrepMenu();
	afx_msg void OnUpdateBrepMenu(CCmdUI* pCmdUI);
	afx_msg void OnViewStatusBar();
	afx_msg void OnUpdateViewStatusBar(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_BREPVIEW_H__C87FACC3_6850_4C09_B5A3_2DB11EA8BE22__INCLUDED_)
