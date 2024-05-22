// ShowOBJInformationDLG.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OARConvWin.h"
#include "ShowOBJInfoDLG.h"


// CShowInfoDLG ダイアログ

IMPLEMENT_DYNAMIC(CShowOBJInfoDLG, CDialogEx)


CShowOBJInfoDLG::CShowOBJInfoDLG(char* obj, OARTool oar, CWnd* pParent)
	: CDialogEx(CShowOBJInfoDLG::IDD, pParent)
{
	shape = oar.getAbstractObject(obj);
	fileName = get_file_name(obj);
	objectName = shape.ObjectName.buf;

	objectKind = "None";
	if (shape.PCode == PRIM_PCODE_PRIM) {
		PrimMeshParam param;
		param.GetParamFromBaseShape(shape);
		if ((param.sculptType & 0x07) == SCULPT_TYPE_MESH) {
			objectKind = "Mesh";
		}
		else if (param.sculptEntry) {
			objectKind = "Sculpted Prim";
		}
		else {
			objectKind = "Prim";
		}
		param.free();
		//
		if (strstr((const char*)shape.ObjFlags.buf, OART_FLAGS_PHANTOM) != NULL) {
			objectKind += " + Phantom";
		}
	}
	else if (shape.PCode == PRIM_PCODE_AVATAR) objectKind = "Avatar";
	else if (shape.PCode == PRIM_PCODE_GRASS) objectKind = "Grass";
	else if (shape.PCode == PRIM_PCODE_NEWTREE) objectKind = "Tree";
	else if (shape.PCode == PRIM_PCODE_PARTICLE) objectKind = "Particle";
 
	TCHAR buf[LNAME];
	shift = shape.affineTrans.shift;
	sntprintf(buf, LNAME, _T("%g, %g, %g"), shift.x, shift.y, shift.z);
	coordinate = buf;

	other = _T("");

	fileSBox  = NULL;
	nameSBox  = NULL;
	kindSBox  = NULL;
	coordSBox = NULL;
	otherSBox = NULL;
}



CShowOBJInfoDLG::~CShowOBJInfoDLG()
{
	shape.free();
}



void CShowOBJInfoDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CShowOBJInfoDLG, CDialogEx)
	ON_BN_CLICKED(IDOK_CRDCOPY, &CShowOBJInfoDLG::OnBnClickedCrdcopy)
	ON_BN_CLICKED(IDOK, &CShowOBJInfoDLG::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()



////////////////////////////////////////////////////////////////////////
// CShowInfoDLG メッセージ ハンドラー

BOOL   CShowOBJInfoDLG::OnInitDialog()
{
	TCHAR buf[LNAME];

	fileSBox  = (CStatic*)GetDlgItem(IDC_STATIC_FLNAME);
	nameSBox  = (CStatic*)GetDlgItem(IDC_STATIC_OBJNAME);
	kindSBox  = (CStatic*)GetDlgItem(IDC_STATIC_OBJKIND);
	coordSBox = (CStatic*)GetDlgItem(IDC_STATIC_CNTCRD);
	otherSBox = (CStatic*)GetDlgItem(IDC_STATIC_OTHER);

	sntprintf(buf, LNAME, _T(" %s "), fileName.GetString());
	fileSBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T(" %s"), objectName.GetString());
	nameSBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T(" %s"), objectKind.GetString());
	kindSBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T(" %g, %g, %g"), shift.x, shift.y, shift.z);
	coordSBox->SetWindowText(buf);
	sntprintf(buf, LNAME, _T(" %s"), other.GetString());

	return TRUE;
}


void CShowOBJInfoDLG::OnBnClickedCrdcopy()
{
	SaveToClipboard_byStr(coordinate);
}


void CShowOBJInfoDLG::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void CShowOBJInfoDLG::OnClose()
{
	CDialogEx::OnClose();
	CDialogEx::OnOK();
}
