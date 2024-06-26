// ObjectsListDLG.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OARConvWin.h"
#include "ObjectsListDLG.h"
#include "afxdialogex.h"


// CObjectsListDLG ダイアログ

IMPLEMENT_DYNAMIC(CObjectsListDLG, CDialogEx)


CObjectsListDLG::CObjectsListDLG(tList* olst, COARConvWinApp* app, CWnd* pParent /*=NULL*/)
    : CDialogEx(CObjectsListDLG::IDD, pParent)
{
    dialogID  = CObjectsListDLG::IDD;
    pWnd      = pParent;

    objNum    = 0;
    objList   = olst;
    winApp    = app;
    //
    slctNum   = 0;
    slctItems = NULL;
    findPos   = 0;

    listLBox  = NULL;
    findEBox  = NULL;
    findBBox  = NULL;
    convBBox  = NULL;

//    Create(dialogID, pWnd);
}


CObjectsListDLG::~CObjectsListDLG()
{
    DEBUG_INFO("DESTRUCTOR: CObjectsListDLG START\n");

    Destroy();

    DEBUG_INFO("DESTRUCTOR: CObjectsListDLG END\n");
}


void  CObjectsListDLG::Destroy()
{
    if (slctItems!=NULL) ::free(slctItems);
    if (winApp!=NULL) {
        winApp->objListBox = NULL;
        winApp->updateMenuBar();
    }
    DestroyWindow();
}


void  CObjectsListDLG::Display()
{
    if (pWnd!=NULL) {
        Create(dialogID, pWnd);
        //
        RECT rect;
        pWnd->GetWindowRect(&rect);
        int sx = (rect.left+rect.right)/2;
        int sy = (rect.top+rect.bottom)/2;
        this->GetWindowRect(&rect);
        sx -= (rect.right-rect.left)/2;
        sy -= (rect.bottom-rect.top)/2;
        this->SetWindowPos(NULL, sx, sy, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
    }
    ShowWindow(SW_SHOW);
}


void CObjectsListDLG::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CObjectsListDLG, CDialogEx)
    ON_BN_CLICKED(IDC_OBJLIST_CONV,  &CObjectsListDLG::OnBnClickedObjlistConv)
    ON_BN_CLICKED(IDC_OBJLIST_CLOSE, &CObjectsListDLG::OnBnClickedObjlistClose)
    ON_LBN_DBLCLK(IDC_LIST_OBJECTS,  &CObjectsListDLG::OnLbnDblclkListObjects)
    ON_BN_CLICKED(IDC_OBJLIST_FIND,  &CObjectsListDLG::OnBnClickedObjlistFind)
    ON_BN_CLICKED(IDC_OBJLIST_CLEAR, &CObjectsListDLG::OnBnClickedObjlistClear)
    ON_BN_CLICKED(IDC_OBJLIST_PREVIEW, &CObjectsListDLG::OnBnClickedObjlistPreview)
    ON_EN_CHANGE(IDC_EDIT_FNDSTR, &CObjectsListDLG::OnEnChangeEditFndstr)
    ON_LBN_SELCHANGE(IDC_LIST_OBJECTS, &CObjectsListDLG::OnLbnSelchangeListObjects)
END_MESSAGE_MAP()



////////////////////////////////////////////////////////////////
// CObjectsListDLG メッセージ ハンドラー

BOOL  CObjectsListDLG::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    findEBox = (CEdit*)GetDlgItem(IDC_EDIT_FNDSTR);
    listLBox = (CListBox*)GetDlgItem(IDC_LIST_OBJECTS);
    findBBox = (CButton*)GetDlgItem(IDC_OBJLIST_FIND);
    convBBox = (CButton*)GetDlgItem(IDC_OBJLIST_CONV);

    objNum = 0;
    tList* lp = objList;
    while(lp!=NULL) {
        listLBox->AddString(get_file_name_t(mbs2ts((char*)lp->ldat.val.buf)));
        objNum++;
        lp = lp->next;
    }

    convBBox->GetFocus();
    return TRUE;
}


void  CObjectsListDLG::OnBnClickedObjlistConv()
{
    listLBox = (CListBox*)GetDlgItem(IDC_LIST_OBJECTS);
    convBBox = (CButton*)GetDlgItem(IDC_OBJLIST_CONV);

    int* tmp = (int*)malloc(sizeof(int)*objNum);
    if (tmp==NULL) return;

    memset(tmp, 0, sizeof(int)*objNum);
    slctNum = listLBox->GetSelItems(objNum, tmp);
    if (slctNum>0) {
        size_t len = sizeof(int)*slctNum;
        slctItems = (int*)malloc(len);
        if (slctItems==NULL) {
            ::free(tmp);
            return;
        }
        memcpy(slctItems, tmp, len);
    }
    ::free(tmp);

    PrintSelectedObjects();   // Log
    winApp->convertSelectedData(slctItems, slctNum);
    //
    slctNum = 0;
    ::freeNull(slctItems);

    convBBox->GetFocus();
}


void CObjectsListDLG::OnBnClickedObjlistPreview()
{
    listLBox = (CListBox*)GetDlgItem(IDC_LIST_OBJECTS);
    convBBox = (CButton*) GetDlgItem(IDC_OBJLIST_CONV);

    int* tmp = (int*)malloc(sizeof(int)*objNum);
    if (tmp==NULL) return;

    memset(tmp, 0, sizeof(int)*objNum);
    slctNum = listLBox->GetSelItems(objNum, tmp);
    if (slctNum>0) {
        size_t len = sizeof(int)*slctNum;
        slctItems = (int*)malloc(len);
		if (slctItems == NULL) {
			::free(tmp);
			return;
        }
        memcpy(slctItems, tmp, len);
    }
    ::free(tmp);

    if (slctNum>5) slctNum = 5;
    for (int i=0; i<slctNum; i++) {
        OpenPreviewWindow(slctItems[i]);
    }
    //
    slctNum = 0;
    ::freeNull(slctItems);

    convBBox->GetFocus();
}


// ダブルクリック
void  CObjectsListDLG::OnLbnDblclkListObjects()
{
    listLBox = (CListBox*)GetDlgItem(IDC_LIST_OBJECTS);
    convBBox = (CButton*)GetDlgItem(IDC_OBJLIST_CONV);

    int index = listLBox->GetAnchorIndex();
    OpenOBJInfoDLG(index);

    convBBox->GetFocus();
}


void CObjectsListDLG::OnBnClickedObjlistFind()
{    
    TCHAR buf[LNAME];

    findBBox = (CButton*)GetDlgItem(IDC_OBJLIST_FIND);
    listLBox = (CListBox*)GetDlgItem(IDC_LIST_OBJECTS);
    findEBox = (CEdit*)GetDlgItem(IDC_EDIT_FNDSTR);
    findEBox->GetWindowText(buf, LNAME);
    findStr = buf;

    int indxPos = listLBox->GetAnchorIndex();
    if (indxPos<0 || indxPos>=objNum) indxPos = 0;
    findPos = indxPos + 1;    // 次の行以降を検索
    if (findPos<0) findPos = 0;
    if (findPos>=objNum) findPos = objNum - 1;

    findPos = SearchString(findPos, (LPCTSTR)findStr);
    if (findPos<0 || findPos>=objNum) {
        findPos = indxPos;
        listLBox->SetAnchorIndex(indxPos);
    }
    else {
        listLBox->SetSel(indxPos, FALSE);
        listLBox->SetSel(findPos + 2);        // 少し下にスクロールして選択行を上に上げる
        listLBox->SetSel(findPos + 2, FALSE);
        listLBox->SetSel(findPos);
    }

    findBBox->GetFocus();
}


void CObjectsListDLG::OnBnClickedObjlistClear()
{
    findBBox = (CButton*)GetDlgItem(IDC_OBJLIST_FIND);    
    findEBox = (CEdit*)GetDlgItem(IDC_EDIT_FNDSTR);
    findEBox->SetWindowText(_T(""));

    listLBox = (CListBox*)GetDlgItem(IDC_LIST_OBJECTS);
    int indxPos = listLBox->GetAnchorIndex();
    listLBox->SetSel(indxPos, FALSE);

    findPos  = 0;
    findStr  = _T("");

    findBBox->GetFocus();
}


void CObjectsListDLG::OnBnClickedObjlistClose()
{
    DEBUG_INFO("CObjectsListDLG OnBnClickedObjlistClose START\n");
    
    Destroy();

    DEBUG_INFO("CObjectsListDLG OnBnClickedObjlistClose END\n");
}


// Close イベント
void CObjectsListDLG::OnCancel()
{
    DEBUG_INFO("CObjectsListDLG OnCancel START\n");

    Destroy();

    DEBUG_INFO("CObjectsListDLG OnCancel END\n");
}


void CObjectsListDLG::OnOK()
{
    //CDialogEx::OnOK();    // 何もしない
}



/////////////////////////////////////////////////////////////////////////////////
// Tool
//
void  CObjectsListDLG::PrintSelectedObjects()
{
    if (slctNum==0 || slctItems==NULL || winApp==NULL) return;

    tList* lp = winApp->oarTool.GetObjectsList();

    int num = 0;
    int cnt = 0;
    while (lp!=NULL) {
        if (num==slctItems[cnt]) {
            char* fname = get_file_name((char*)lp->ldat.val.buf);
            PRINT_MESG("Objects List: selected [%d]: %s\n", num+1, fname);
            cnt++;
            if (cnt==slctNum) break;
        }
        num++;
        lp = lp->next;
    }
    return;
}


int  CObjectsListDLG::SearchString(int stn, LPCTSTR str)
{
    CString buf;

    int   index = -1;
    int   objnum = listLBox->GetCount();    // objNum に等しいはず
    char* fnd = ts2mbs(str);

    for (int i=0; i<objNum; i++, stn++) {
        int num = stn % objnum;
        listLBox->GetText(num, buf);
        char* name = ts2mbs(buf);
        if (strstrcase(name, fnd)!=NULL) {
            PRINT_MESG("SearchString: found [%d]: %s\n", num+1, name);
            index = num;
            ::free(name);
            break;
        }
        ::free(name);
    }
    ::free(fnd);

    return index;
}


void  CObjectsListDLG::OpenPreviewWindow(int idx)
{
    int   num = 0;
    char* obj = NULL;

    tList* lp = objList;
    while (lp!=NULL) {
        if (num==idx) {
            obj = (char*)lp->ldat.val.buf;
            break;
        }
        num++;
        lp = lp->next;
    }
    char* fname = get_file_name(obj);
    if (obj!=NULL) PRINT_MESG("Objects List: selected [%d]: %s\n", num + 1, fname);
    //
    BrepSolidList* slist = (BrepSolidList*)winApp->oarTool.generateSolidData(JBXL_3D_FORMAT_STL, obj);
    if (slist == NULL) {
        return;
    }

    CProgressBarDLG* counter = new CProgressBarDLG(_T("Create Preview Window"), FALSE);
    BREP_SOLID* solid = slist->getMerge(counter);

    freeBrepSolidList(slist);
    if (counter!=NULL) {
        counter->PutFill();
        delete counter;
    }
    if (solid != NULL) {
        winApp->solidOpenBrep(solid, mbs2ts(fname), num + 1);    // solid は呼び出された関数が解放する
    }
    return;
}


void  CObjectsListDLG::OpenOBJInfoDLG(int idx)
{
	int   num = 0;
	char* obj = NULL;

	tList* lp = objList;
	while (lp != NULL) {
		if (num == idx) {
			obj = (char*)lp->ldat.val.buf;
			break;
		}
		num++;
		lp = lp->next;
	}
	char* fname = get_file_name(obj);
	if (obj != NULL) PRINT_MESG("Objects List: selected [%d]: %s\n", num + 1, fname);
	//
	winApp->showOBJInfoDLG(obj);

	return;
}


void CObjectsListDLG::OnEnChangeEditFndstr()
{
    findBBox->GetFocus();
}


void CObjectsListDLG::OnLbnSelchangeListObjects()
{
    convBBox->GetFocus();
}
