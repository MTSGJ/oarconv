#if !defined(AFX_BREPDOC_H__28E49D6C_8E63_42CF_BEF6_460475B91CBA__INCLUDED_)
#define AFX_BREPDOC_H__28E49D6C_8E63_42CF_BEF6_460475B91CBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
CBREPDoc クラス

	CExDocumentクラスからの継承クラス
	STLファイルからデータを読み込んで，BREPの SOLODを作り出す．

*/

#include  "ExClass.h"
#include  "BrepLib.h"

#include  "OARConvWin.h"


using namespace jbxl;
using namespace jbxwl;


/////////////////////////////////////////////////////////////////////////////
// CBREPDoc ドキュメント

class CBREPDoc : public CExDocument
{
protected:
	CBREPDoc();           // 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CBREPDoc)

// アトリビュート
public:
	BREP_SOLID* Solid;
	
	bool	 ChangeData;	// Dataが変更されたか？ 保存された場合は falseに戻る．
	bool	 CallSave;		// 保存関数が呼び出されたか？ 実際に保存されたかどうかは別．

// オペレーション
public:
	void	 SolidRepair(int method);
	void	 ContoursRepair(int method, bool mode);
	void	 SaveFile(bool mode);
	void	 ShowSolidInfo();

public:
	int  DataNum;
	COARConvWinApp* WinApp;

public:
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CBREPDoc)
	public:
	virtual void Serialize(CArchive& ar);   // ドキュメント I/O に対してオーバーライドされます。
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL
	
// インプリメンテーション
public:
	virtual ~CBREPDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// メッセージ マップ関数の生成
protected:
	//{{AFX_MSG(CBREPDoc)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_BREPDOC_H__28E49D6C_8E63_42CF_BEF6_460475B91CBA__INCLUDED_)
