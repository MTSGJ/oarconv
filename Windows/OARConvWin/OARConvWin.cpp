// OARConvWin.cpp : アプリケーションのクラス動作を定義します。
//

#include  "stdafx.h"

#include  "OARConvWin.h"
#include  "MainFrm.h"

#include  "Tools.h"
#include  "Dx9.h"
#include  "Graph.h"
#include  "WinTools.h"

#include  "BREPView.h"
#include  "BREPDoc.h"
#include  "BREPFrame.h"
#include  "BREP_MFCLib.h"

#include  "ShowOARInfoDLG.h"
#include  "ShowOBJInfoDLG.h"
#include  "SetParamDLG.h"
#include  "OutFormatDLG.h"
#include  "ObjectsListDLG.h"

#include  "tar32api.h"

#ifdef WIN64
#pragma comment(lib, "tar64.lib")
#else
#pragma comment(lib, "tar32.lib")
#endif


using namespace jbxl;
using namespace jbxwl;


// COARConvWinApp
BEGIN_MESSAGE_MAP(COARConvWinApp, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, &COARConvWinApp::OnAppAbout)
    // 標準のファイル基本ドキュメント コマンド
    //ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    //ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
    // 標準の印刷セットアップ コマンド
    //ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
    //ON_COMMAND(ID_FILE_OPEN, OnFileOpenBrep)
    //ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
    //ON_COMMAND(ID_APP_EXIT, OnAppExit)
    ON_COMMAND(ID_FILE_OPEN, &COARConvWinApp::OnFileOpen)
    ON_COMMAND(ID_FOLDER_OPEN, &COARConvWinApp::OnFolderOpen)
    ON_COMMAND(ID_CONVERT_DATA, &COARConvWinApp::OnConvertData)
    ON_COMMAND(ID_FILE_QUICK, &COARConvWinApp::OnFileOpenQuick)
    ON_COMMAND(ID_FOLDER_QUICK, &COARConvWinApp::OnFolderOpenQuick)
    ON_COMMAND(ID_LOG_WINDOW, &COARConvWinApp::OnLogWindow)
    ON_COMMAND(ID_OUTPUT_FORMAT, &COARConvWinApp::OnOutFormatDialog)
    ON_COMMAND(ID_SETTING_DIALOG, &COARConvWinApp::OnSettingDialog)
    ON_COMMAND(ID_OBJ_LIST, &COARConvWinApp::OnObjectsList)
END_MESSAGE_MAP()


// COARConvWinApp コンストラクション
COARConvWinApp::COARConvWinApp()
{
    pDocTemplLOG = NULL;
    pDocTemplBREP = NULL;

    pMainFrame = NULL;
    pLogFrame = NULL;
    pLogDoc = NULL;
    aboutBox = NULL;
    objListBox = NULL;

    memset(&windowSize, -1, sizeof(RECT));
    appParam.init();

    char* prgfld = GetProgramFolderA();
    homeFolder = make_Buffer_bystr(prgfld);
    ::free(prgfld);

    assetsFolder = make_Buffer(64);
    Buffer pathAST = make_Buffer_bystr(OART_DEFAULT_AST_DIR);
    Buffer* dirs = awk_Buffer_dim(pathAST, '|');
    if (dirs != NULL) {
        int num = dirs->state;
        for (int i = 0; i < num; i++) {
            if (dirs[i].buf[strlen((char*)dirs[i].buf) - 1] != '\\') cat_s2Buffer("\\", &dirs[i]);
            if (i!=0) cat_s2Buffer("|", &assetsFolder);
            cat_Buffer(&homeFolder, &assetsFolder);
            cat_Buffer(&dirs[i],    &assetsFolder);
            free_Buffer(&dirs[i]);
        }
    }
    free_Buffer(&pathAST);

    comDecomp = dup_Buffer(homeFolder);
    cat_s2Buffer(OART_JP2_DECOMP_COM, &comDecomp);

    hasData = false;
    isConverting = false;
}


//
COARConvWinApp::~COARConvWinApp()
{
    DEBUG_INFO("DESTRUCTOR: COARConvWinApp START\n");

    free_Buffer(&homeFolder);
    free_Buffer(&assetsFolder);
    free_Buffer(&comDecomp);

    appParam.saveConfigFile();
    appParam.free();
    oarTool.free();

    deleteNull(objListBox);
    deleteNull(aboutBox);
    Dx9ReleaseInterface();

    DEBUG_INFO("DESTRUCTOR: COARConvWinApp END\n");
}


//
COARConvWinApp theApp;


// COARConvWinApp 初期化
BOOL  COARConvWinApp::InitInstance()
{
    CWinApp::InitInstance();

    AfxEnableControlContainer();
    Enable3dControlsStatic();    // MFC と静的にリンクしている場合にはここを呼び出してください．

    SetRegistryKey(_T("OARConvWin by NSL"));
    LoadStdProfileSettings(4);   // 標準の INI ファイルのオプションをロードします (MRU を含む)

    ///////////////////////////////////////////////////////////////////////////////
    //
    CMultiDocTemplate* pDocTemplate;

    // ログ ウィンドウ
    pDocTemplate = new CMultiDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CLogWndDoc),
        RUNTIME_CLASS(CLogWndFrame),
        RUNTIME_CLASS(CLogWndView));
    if (!pDocTemplate) return FALSE;
    AddDocTemplate(pDocTemplate);
    pDocTemplLOG = pDocTemplate;

    // BREP ウィンドウ
    pDocTemplate = new CMultiDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CBREPDoc),
        RUNTIME_CLASS(CBREPFrame),
        RUNTIME_CLASS(CBREPView));

    if (!pDocTemplate) return FALSE;
    AddDocTemplate(pDocTemplate);
    pDocTemplBREP = pDocTemplate;

    ///////////////////////////////////////////////////////////////////////////////
    // メイン MDI フレーム ウィンドウ
    pMainFrame = new CMainFrame;
    pMainFrame->pApp = this;
    if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME)) return FALSE;
    m_pMainWnd = pMainFrame;
    m_pMainWnd->DragAcceptFiles(TRUE);

    ///////////////////////////////////////////////////////////////////////////////
    // Locale
    setSystemLocale();

    ///////////////////////////////////////////////////////////////////////////////
    // ウィンドウサイズ
    int wx = OARCONV_WND_XSIZE;
    int wy = OARCONV_WND_YSIZE;
    int rx = GetSystemMetrics(SM_CXSCREEN);         // X方向の解像度
    int ry = GetSystemMetrics(SM_CYSCREEN) - 14;    // Y方向の解像度（-ツールバー）

    // Default Size
    windowSize.left = rx / 2 - wx / 2;
    windowSize.right = wx;
    windowSize.top = ry / 2 - wy / 2;
    windowSize.bottom = wy;

    // read  windowSize
    appParam.readWindowSize(&windowSize);

    int sx, sy;
    if (windowSize.left <= rx) {
        sx = windowSize.left;
        wx = windowSize.right;
        if (wx <= 0 || wx >= rx) wx = OARCONV_WND_XSIZE;
    }
    else {
        sx = rx / 2 - OARCONV_WND_XSIZE / 2;
    }
    if (windowSize.top <= ry) {
        sy = windowSize.top;
        wy = windowSize.bottom;
        if (wy <= 0 || wy >= ry) wy = OARCONV_WND_YSIZE;
    }
    else {
        sy = ry / 2 - OARCONV_WND_YSIZE / 2;
    }
    pMainFrame->SetWindowPos(NULL, sx, sy, wx, wy, 0);

    ///////////////////////////////////////////////////////////////////////////////
    // メイン ウィンドウの表示と更新
    pMainFrame->ShowWindow(SW_SHOW);
    pMainFrame->UpdateWindow();

    ///////////////////////////////////////////////////////////////////////////////
    // DirectX9 の初期化
    BOOL rslt = Dx9CreateInterface(this);
    if (!rslt) {
        MessageBoxDLG(IDS_STR_ERROR, IDS_STR_FAIL_GET_DX9DEV, MB_OK, m_pMainWnd); // "DirectXの初期化に失敗しました"
        return FALSE;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // 初期化
    appParam.readConfigFile();
    DebugMode = appParam.debugMode;

    oarTool.SetEngine(appParam.outputEngine);
    oarTool.SetDataFormat(appParam.outputFormat);
    oarTool.SetNoOffset(appParam.noOffset);
    oarTool.SetProcJoints(appParam.procJoints);
    //
    updateMenuBar();
    updateStatusBar(_T(""), _T(""));

    // この後，メッセージループに入る．
    return TRUE;
}


// CExTextFrame のディストラクト時のコールバック関数
void  COARConvWinApp::FrameDestructor(CExTextFrame* frm)
{
    // ログ ウィンドウ
    if (frm == pLogFrame) {
        DEBUG_INFO("COARConvWinApp::FrameDestructor(): STOP LOG FRAME");
        pLogFrame = NULL;
    }

    updateMenuBar();
    return;
}


// CExTextView のディストラクト時のコールバック関数
void  COARConvWinApp::ViewDestructor(CExTextView* view)
{
    // ログ ウィンドウ
    if (view == pLogFrame->pView) {
        DEBUG_INFO("COARConvWinApp::ViewDestructor(): STOP LOG VIEW");
        ClearGlobalTextDocument();
        pLogDoc = NULL;
    }

    updateMenuBar();
    return;
}


/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ
//
// pfrm->pView->TimerStart() の値の影響でウィンドウが多数表示される場合，
// DoModal() が表示されない現象が発生するので，その対応処理.
void  COARConvWinApp::OnAppAbout()
{
    if (aboutBox == NULL) aboutBox = new CMessageBoxDLG(IDD_ABOUTBOX, NULL, m_pMainWnd);
    if (aboutBox != NULL) aboutBox->Display();
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
// COARConvWinApp メッセージ ハンドラ
//

//
void  COARConvWinApp::OnFileOpen()
{
    CString fname = EasyGetOpenFileName(_T("Open OAR File"), m_pMainWnd->m_hWnd);
    /*
    CFileDialog fileDialog(true);
    if (fileDialog.DoModal() == IDOK) {
        fname = fileDialog.GetPathName();
    }
    int err = CommDlgExtendedError();
    */

    if (fname.IsEmpty()) return;
    bool ret = fileOpen(fname);
    if (ret) showOARInfoDLG();

    return;
}


//
void  COARConvWinApp::OnFolderOpen()
{
    CString crntd = getOARFolder();
    CString foldr = EasyGetOpenFolderName(crntd, _T("Open OAR Folder"), m_pMainWnd->m_hWnd);
    if (foldr.IsEmpty()) return;

    bool ret = folderOpenOAR(foldr);
    if (ret) showOARInfoDLG();

    return;
}


//
void  COARConvWinApp::OnConvertData()
{
    convertAllData();
    return;
}


//
void  COARConvWinApp::OnFileOpenQuick()
{
    CString fname = EasyGetOpenFileName(_T("Open OAR File"), m_pMainWnd->m_hWnd);
    if (fname.IsEmpty()) return;

    bool ret = fileOpen(fname);
    if (!ret) return;

    convertAllData();
    return;
}


//
void  COARConvWinApp::OnFolderOpenQuick()
{
    CString crntd = getOARFolder();
    CString foldr = EasyGetOpenFolderName(crntd, _T("Open OAR Folder"), m_pMainWnd->m_hWnd);
    if (foldr.IsEmpty()) return;

    bool ret = folderOpenOAR(foldr);
    if (!ret) return;

    convertAllData();
    return;
}


//
void  COARConvWinApp::OnObjectsList()
{
    if (objListBox == NULL) {
        tList* objlist = oarTool.GetObjectsList();
        if (objlist != NULL) {
            objListBox = new CObjectsListDLG(objlist, this, m_pMainWnd);
            if (objListBox == NULL) return;
            objListBox->Display();
        }
    }

    updateMenuBar();
    return;
}


//
void  COARConvWinApp::OnLogWindow()
{
    if (pLogFrame == NULL) {
        pLogFrame = ExecLogWnd(pDocTemplLOG, _T("Log Window"), this);
        pLogDoc = pLogFrame->pDoc;
        SetGlobalTextDocument(pLogDoc);
    }

    updateMenuBar();
    return;
}


void  COARConvWinApp::OnOutFormatDialog()
{
    COutFormatDLG* setdlg = new COutFormatDLG(&appParam, m_pMainWnd);
    if (setdlg == NULL) return;
    setdlg->DoModal();
    setdlg->getParameters(&appParam);
    delete (setdlg);

    CString path = get_file_path_t(appParam.oarFolder);
    CString outf = path + appParam.prefixOUT + getOARName();

    // No Offset の設定
    bool no_offset_flg = false;
    if (appParam.noOffset) {
        if (appParam.outputFormat == JBXL_3D_FORMAT_DAE) {
            no_offset_flg = true;
        }
        else if (appParam.outputFormat == JBXL_3D_FORMAT_OBJ) {
            no_offset_flg = true;
        }
        else if (appParam.outputFormat == JBXL_3D_FORMAT_GLTF) {
            no_offset_flg = true;
        }
        else if (appParam.outputFormat == JBXL_3D_FORMAT_GLB) {
            no_offset_flg = true;
        }
        else if (appParam.outputFormat == JBXL_3D_FORMAT_FBX) {
            no_offset_flg = true;
        }
    }
    if (no_offset_flg) outf += OART_DEFAULT_NOFST_DIR;
    appParam.outFolder = outf;

    char* op = ts2mbs(outf);
    oarTool.ChangePathInfo(NULL, op, NULL);
    ::free(op);
    //
    oarTool.SetEngine(appParam.outputEngine);
    oarTool.SetDataFormat(appParam.outputFormat);
    oarTool.SetNoOffset(appParam.noOffset);
    oarTool.SetProcJoints(appParam.procJoints);
    appParam.saveConfigFile();

    if (hasData) updateStatusBar(getOARFolder(), getOutFolder());  // appParam.outFolder
    else         updateStatusBar(_T(""), _T(""));

    return;
}


void  COARConvWinApp::OnSettingDialog()
{
    CSetParamDLG* setdlg = new CSetParamDLG(&appParam, m_pMainWnd);
    if (setdlg == NULL) return;
    setdlg->DoModal();
    setdlg->getParameters(&appParam);
    delete (setdlg);

    CString path = get_file_path_t(appParam.oarFolder);
    CString outf = path + appParam.prefixOUT + getOARName();

    // No Offset の設定
    bool no_offset_flg = false;
    if (appParam.noOffset) {
        if (appParam.outputFormat == JBXL_3D_FORMAT_DAE) {
            no_offset_flg = true;
        }
        else if (appParam.outputFormat == JBXL_3D_FORMAT_OBJ) {
            no_offset_flg = true;
        }
        else if (appParam.outputFormat == JBXL_3D_FORMAT_GLTF) {
            no_offset_flg = true;
        }
        else if (appParam.outputFormat == JBXL_3D_FORMAT_GLB) {
            no_offset_flg = true;
        }
        else if (appParam.outputFormat == JBXL_3D_FORMAT_FBX) {
            no_offset_flg = true;
        }
    }
    if (no_offset_flg) outf += OART_DEFAULT_NOFST_DIR;
    appParam.outFolder = outf;

    char* op = ts2mbs(outf);
    oarTool.ChangePathInfo(NULL, op, NULL);
    ::free(op);

    DebugMode = appParam.debugMode;
    appParam.saveConfigFile();

    if (hasData) updateStatusBar(getOARFolder(), getOutFolder());  // appParam.outFolder
    else         updateStatusBar(_T(""), _T(""));

    return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//

bool  COARConvWinApp::fileOpen(CString fname)
{
    bool ret;

    CString fn = get_file_name_t((LPCTSTR)fname);
    if (!fn.Compare(_T("archive.xml"))) {
        CString path = get_file_path_t(fname);
        ret = folderOpenOAR(path);
    }
    else {
        ret = fileOpenOAR(fname);
    }
    return ret;
}


Buffer  COARConvWinApp::extractOARfile(CString fname, int* filenum)
{
    char* fn = ts2mbs((LPCTSTR)fname);          // 要 free
    Buffer dec = init_Buffer();
    //
    PRINT_MESG("COARConvWinApp::extractOARfile: Wait a minute. Extracting OAR file... (%s)\n", fn);
    DisPatcher();
    Buffer enc = read_Buffer_file(fn);
    if (!is_gz_data(enc)) {
        PRINT_MESG("COARConvWinApp::extractOARfile: %s is not OAR file!!\n", fn);
        ::free(fn);
        free_Buffer(&enc);
        return dec;
    }
    ::free(fn);
    //
    //CMessageBoxDLG* mbox = MessageBoxDLG(IDS_STR_INFO, IDS_STR_EXTRACT_GZIP, m_pMainWnd);
    //if (mbox != NULL) mbox->Display();
    dec = gz_decode_data(enc);
    free_Buffer(&enc);
    //if (mbox != NULL) delete mbox;

    // count file num
    Tar_Header  tar_header;
    long unsigned int size = 0;
    long unsigned int datalen = (long unsigned int)(dec.vldsz - 1024);
    *filenum = 0;
    while (size < datalen) {
        memcpy(&tar_header, (char*)&dec.buf[size], sizeof(Tar_Header));
        size += sizeof(Tar_Header);
        long unsigned int len = (long unsigned int)strtol(tar_header.size, NULL, 8);
        if (len%512>0) len = (len/512 + 1)*512;
        size += len;
        if (*tar_header.typeflag!='L' && *tar_header.typeflag!='K') (*filenum)++;
    }
    return dec;
}


//
// OARファイルをオープンする．
//
bool  COARConvWinApp::fileOpenOAR(CString fname)
{
    hasData = false;
    updateMenuBar();
    updateStatusBar(_T(""), _T(""));
/*
    // Check
    char* fc = ts2mbs((LPCTSTR)fname);        // 要 free
    int ret = TarCheckArchive(fc, 0);
    if (!ret) {
        PRINT_MESG("COARConvWinApp::fileOpenOAR: ERROR: Tar32/64 Check Error = 0x%04x [%s]\n", ret, fc);
        MessageBoxDLG(IDS_STR_ERROR, IDS_STR_ERR_OPEN_FILE, MB_OK, pMainFrame);
        ::free(fc);
        return false;
    }
    ::free(fc);

    CString path = get_file_path_t(fname);    // パス名
    CString file = get_file_name_t(fname);
    file = cut_file_extension_t(file);        // 拡張子なしファイル名

    CString oarf = path + appParam.prefixOAR + file;
    if (file_exist_t((LPCTSTR)oarf)) tunlink((LPCTSTR)oarf);

    // Extract
    CString mode = _T(" -zxvf \"") + fname + _T("\" -o \"") + oarf + _T("\"");
    char* md = ts2mbs((LPCTSTR)mode);
    ret = Tar(pMainFrame->m_hWnd, md, NULL, 0);
    if (ret) {
        PRINT_MESG("COARConvWinApp::fileOpenOAR: ERROR: Tar32/64 Extract Error = 0x%04x [%s]\n", ret, md);
        MessageBoxDLG(IDS_STR_ERROR, IDS_STR_ERR_EXTRACT, MB_OK, pMainFrame);
        ::free(md);
        return false;
    }
    ::free(md);
*/

    int filenum;
    Buffer dec = extractOARfile(fname, &filenum);
    if (dec.vldsz<=0) {
        MessageBoxDLG(IDS_STR_ERROR, IDS_STR_NOT_OAR, MB_OK, pMainFrame);
        return false;
    }

    CProgressBarDLG* progress = new CProgressBarDLG(IDD_PROGBAR, _T(""), TRUE);
    if (progress != NULL) {
        progress->SetTitle("Saving files from OAR file...");
        progress->Start(filenum);
        SetGlobalCounter(progress);
    }
    
    CString path = get_file_path_t(fname);      // パス名
    CString file = get_file_name_t(fname);
    file = cut_file_extension_t(file);          // 拡張子なしファイル名
    CString oarpath = path + appParam.prefixOAR + file;

    if (file_exist_t((LPCTSTR)oarpath)) tunlink((LPCTSTR)oarpath);
    setupParameters(path, file, oarpath);

    char* fn = ts2mbs((LPCTSTR)oarpath);        // 要 free
    Buffer prefix = make_Buffer_bystr(fn);
    ::free(fn);
    
    oarTool.ExtractTar(dec, prefix, 0750);
    free_Buffer(&dec);
    free_Buffer(&prefix);

    if (progress != NULL) {
        progress->PutFill();
        //DisPatcher();
        delete progress;
        ClearGlobalCounter();
    }

    bool chk = oarTool.GetDataInfo();
    if (!chk) {
        MessageBoxDLG(IDS_STR_ERROR, IDS_STR_NOT_OAR, MB_OK, pMainFrame);
        return false;
    }

    hasData = true;
    updateMenuBar();
    updateStatusBar(getOARFolder(), getOutFolder());
    //
    //PRINT_MESG("COARConvWinApp::fileOpenOAR: File is opened %s\n", (char*)oarfile.buf);
    //free_Buffer(&oarfile);

    return chk;
}


//
// OARフォルダをオープンする．
//
bool  COARConvWinApp::folderOpenOAR(CString folder)
{
    hasData = false;
    updateMenuBar();
    updateStatusBar(_T(""), _T(""));

    CString oarf = folder;
    if (oarf.Right(1) == _T("\\")) oarf = oarf.Left(oarf.GetLength() - 1);

    int len = appParam.prefixOAR.GetLength();

    CString path = get_file_path_t(oarf);
    CString dirn = get_file_name_t(oarf);
    CString topd = dirn.Left(len);
    if (!topd.Compare(appParam.prefixOAR)) {
        dirn = dirn.Right(dirn.GetLength() - len);
    }
    //
    setupParameters(path, dirn, oarf);

    bool chk = oarTool.GetDataInfo();
    if (!chk) {
        MessageBoxDLG(IDS_STR_ERROR, IDS_STR_NOT_OAR, MB_OK, pMainFrame);
        return false;
    }

    hasData = true;
    updateMenuBar();
    updateStatusBar(getOARFolder(), getOutFolder());
    //
    char* fn = ts2mbs(folder);
    PRINT_MESG("COARConvWinApp::folderOpenOAR: Folder is opened %s\n", fn);
    free(fn);

    return chk;
}


void  COARConvWinApp::setupParameters(CString path, CString file, CString oarf)
{
    CString outf = path + appParam.prefixOUT + file;

    // No Offset の設定
    bool no_offset_flg = false;
    if (appParam.noOffset) {
        if (appParam.outputFormat == JBXL_3D_FORMAT_DAE) {
            no_offset_flg = true;
        }
        else if (appParam.outputFormat == JBXL_3D_FORMAT_OBJ) {
            no_offset_flg = true;
        }
        else if (appParam.outputFormat == JBXL_3D_FORMAT_GLTF) {
            no_offset_flg = true;
        }
        else if (appParam.outputFormat == JBXL_3D_FORMAT_GLB) {
            no_offset_flg = true;
        }
        else if (appParam.outputFormat == JBXL_3D_FORMAT_FBX) {
            no_offset_flg = true;
        }
    }
    if (no_offset_flg) outf += OART_DEFAULT_NOFST_DIR;
    appParam.outFolder = outf;
    
    char* op = ts2mbs(outf);
    oarTool.SetOutPath(op);
    ::free(op);
    setOARName((LPCTSTR)file);
    setBaseFolder((LPCTSTR)path);
    setOARFolder((LPCTSTR)oarf);
    setOutFolder((LPCTSTR)outf);
    appParam.saveConfigFile();

    ////////////////////////////////////////////////////////////////////
    char* oardir = ts2mbs(getOARFolder());
    char* outdir = ts2mbs(getOutFolder());
    oarTool.free();
    oarTool.init();
    oarTool.SetEngine(appParam.outputEngine);
    oarTool.SetDataFormat(appParam.outputFormat);
    oarTool.SetNoOffset(appParam.noOffset);
    oarTool.SetProcJoints(appParam.procJoints);
    oarTool.SetPathInfo(oardir, outdir, (char*)assetsFolder.buf);
    ::free(oardir);
    ::free(outdir);

    return;
}


//
void  COARConvWinApp::convertAllData()
{
    isConverting = true;
    updateMenuBar();

    oarTool.MakeOutputFolder();

    int num = _convertAllData();
    if (num >= 0) {
        CString format;
        CString mesg;
        format.LoadString(IDS_STR_CONVERT_NUM);
        mesg.Format(format, num);
        MessageBoxDLG(IDS_STR_CNFRM, mesg, MB_OK, m_pMainWnd);
    }
    else {
        MessageBoxDLG(IDS_STR_CANCEL, IDS_STR_CANCEL_TRNS_DATA, MB_OK, m_pMainWnd);
    }

    isConverting = false;
    updateMenuBar();

    return;
}



/////////////////////////////////////////////////////////////////////////////////////////
// Convert
//

//
int   COARConvWinApp::_convertAllData()
{
    int strtnum = appParam.startNum;
    int stopnum = appParam.stopNum;

    // 処理データ数の推測（Terrainは除く）
    int prognum = oarTool.objectsNum;
    if (stopnum > 0) prognum = Min(prognum, stopnum);
    if (strtnum < 1) strtnum = 1;
    prognum -= strtnum + 1;
    //
    oarTool.SetTerrainShift(appParam.xShift, appParam.yShift, appParam.zShift);

    //////////////////////////////////////////////////////////////////////////////
    // Convert
    int num = 0;
    if (appParam.outputTerrain) {
        CMessageBoxDLG* mbox = MessageBoxDLG(IDS_STR_INFO, IDS_STR_CONV_TERRAIN, m_pMainWnd);
        if (mbox != NULL) mbox->Display();
        //
        oarTool.ReadTerrainData();
        oarTool.SetTerrainTextureScale(appParam.terrainScale);
        num = oarTool.GenerateTerrainDataFile();
        //
        if (mbox != NULL) delete mbox;
    }
    //
    if (stopnum != 0) {
        CProgressBarDLG* progress = new CProgressBarDLG(IDD_PROGBAR, _T(""), TRUE);
        if (progress != NULL) {
            if      (appParam.outputFormat==JBXL_3D_FORMAT_DAE)  progress->SetTitle("Convert to DAE Files");
            else if (appParam.outputFormat==JBXL_3D_FORMAT_OBJ)  progress->SetTitle("Convert to OBJ Files");
            else if (appParam.outputFormat==JBXL_3D_FORMAT_GLTF) progress->SetTitle("Convert to GLTF Files");
            else if (appParam.outputFormat==JBXL_3D_FORMAT_GLB)  progress->SetTitle("Convert to GLB Files");
            else if (appParam.outputFormat==JBXL_3D_FORMAT_FBX)  progress->SetTitle("Convert to FBX Files");
            else                                                 progress->SetTitle("Convert to STL Files");
            progress->Start(prognum);
            SetGlobalCounter(progress);
        }
        //
        num = oarTool.GenerateObjectFromDataIndex(strtnum, stopnum, false, (char*)comDecomp.buf);
        if (appParam.outputFormat==JBXL_3D_FORMAT_GLB) oarTool.RmTextureFolder();
        //
        if (progress != NULL) {
            progress->PutFill();
            delete progress;
            ClearGlobalCounter();
        }
    }

    return num;
}


void  COARConvWinApp::convertSelectedData(int* selectedObjs, int selectedNums)
{
    isConverting = true;
    updateMenuBar();
    oarTool.MakeOutputFolder();

    //
    int num = _convertSelectedData(selectedObjs, selectedNums);
    if (num >= 0) {
        CString format;
        CString mesg;
        format.LoadString(IDS_STR_CONVERT_NUM);
        mesg.Format(format, num);
        MessageBoxDLG(IDS_STR_CNFRM, mesg, MB_OK, m_pMainWnd);
    }
    else {
        MessageBoxDLG(IDS_STR_CANCEL, IDS_STR_CANCEL_TRNS_DATA, MB_OK, m_pMainWnd);
    }

    isConverting = false;
    updateMenuBar();

    return;
}


//
int   COARConvWinApp::_convertSelectedData(int* selectedObjs, int objNums)
{
    if (selectedObjs == NULL) return 0;

    DebugMode = appParam.debugMode;
    int prognum = objNums;
    oarTool.SetTerrainShift(appParam.xShift, appParam.yShift, appParam.zShift);

    //////////////////////////////////////////////////////////////////////////////
    // Convert
    int num = 0;
    CProgressBarDLG* progress = new CProgressBarDLG(_T(""), TRUE);
    if (progress != NULL) {
        if (appParam.outputFormat == JBXL_3D_FORMAT_DAE)      progress->SetTitle("Convert to DAE Files");
        else if (appParam.outputFormat == JBXL_3D_FORMAT_OBJ) progress->SetTitle("Convert to OBJ Files");
        else                                                  progress->SetTitle("Convert to STL Files");
        progress->Start(prognum);
        SetGlobalCounter(progress);
    }
    //
    num = oarTool.GenerateObjectFromDataList(selectedObjs, objNums, false, (char*)comDecomp.buf);
    if (appParam.outputFormat==JBXL_3D_FORMAT_GLB) oarTool.RmTextureFolder();
    //
    if (progress != NULL) {
        progress->PutFill();
        delete progress;
        ClearGlobalCounter();
    }

    return num;
}


void  COARConvWinApp::convertOneData(int index)
{
    isConverting = true;
    updateMenuBar();
    oarTool.MakeOutputFolder();
    //
    int num = _convertOneData(index);
    if (num >= 0) {
        CString strformat;
        CString mesg;
        strformat.LoadString(IDS_STR_CONVERT_NUM);
        mesg.Format(strformat, num);
        MessageBoxDLG(IDS_STR_CNFRM, mesg, MB_OK, m_pMainWnd);
    }
    else {
        MessageBoxDLG(IDS_STR_CANCEL, IDS_STR_CANCEL_TRNS_DATA, MB_OK, m_pMainWnd);
    }

    isConverting = false;
    updateMenuBar();

    return;
}


//
int   COARConvWinApp::_convertOneData(int index)
{
    DebugMode = appParam.debugMode;
    oarTool.SetTerrainShift(appParam.xShift, appParam.yShift, appParam.zShift);

    //////////////////////////////////////////////////////////////////////////////
    // Convert
    int num = oarTool.GenerateObjectFromDataIndex(index, index, false, (char*)comDecomp.buf);
    if (appParam.outputFormat==JBXL_3D_FORMAT_GLB) oarTool.RmTextureFolder();

    return num;
}



/////////////////////////////////////////////////////////////////////////////////////////
//

void  COARConvWinApp::solidOpenBrep(BREP_SOLID* solid, LPCTSTR title, int num)
{
    if (solid == NULL) return;

    CExFrame* pfrm = CreateDocFrmView(pDocTemplBREP, this);
    if (pfrm != NULL) {
        ((CBREPDoc*)(pfrm->pDoc))->Solid = solid;
        ((CBREPDoc*)(pfrm->pDoc))->DataNum = num;
        ((CBREPDoc*)(pfrm->pDoc))->WinApp = this;
        int ret = ExecDocFrmView(pfrm);
        if (pfrm->pDoc->pView != NULL) pfrm->pDoc->pView->SetTitle(title);
        //
        if (ret == 0) pfrm->pView->TimerStart();
        else ExecDocFrmViewError(m_pMainWnd->m_hWnd, ret);
    }

    return;
}


void  COARConvWinApp::showOARInfoDLG()
{
    CShowOARInfoDLG* shwdlg = new CShowOARInfoDLG(oarTool, m_pMainWnd);
    if (shwdlg == NULL) return;
    shwdlg->DoModal();
    delete (shwdlg);

    return;
}


void  COARConvWinApp::showOBJInfoDLG(char* obj)
{
    CShowOBJInfoDLG* shwdlg = new CShowOBJInfoDLG(obj, oarTool, m_pMainWnd);
    if (shwdlg == NULL) return;
    shwdlg->DoModal();
    delete (shwdlg);

    return;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tools
//

void  COARConvWinApp::updateMenuBar(CMenu* menu)
{
    if (menu == NULL) {
        if (pMainFrame == NULL) return;
        menu = pMainFrame->GetMenu();
        if (menu == NULL) return;
    }

    // default
    menu->EnableMenuItem(ID_FILE_OPEN, MF_BYCOMMAND | MF_ENABLED);
    menu->EnableMenuItem(ID_FOLDER_OPEN, MF_BYCOMMAND | MF_ENABLED);
    menu->EnableMenuItem(ID_FILE_QUICK, MF_BYCOMMAND | MF_ENABLED);
    menu->EnableMenuItem(ID_FOLDER_QUICK, MF_BYCOMMAND | MF_ENABLED);
    menu->EnableMenuItem(ID_CONVERT_DATA, MF_BYCOMMAND | MF_ENABLED);
    menu->EnableMenuItem(ID_OBJ_LIST, MF_BYCOMMAND | MF_ENABLED);

    //
    if (isConverting) {
        menu->EnableMenuItem(ID_FILE_OPEN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        menu->EnableMenuItem(ID_FOLDER_OPEN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        menu->EnableMenuItem(ID_FILE_QUICK, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        menu->EnableMenuItem(ID_FOLDER_QUICK, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        menu->EnableMenuItem(ID_CONVERT_DATA, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        menu->EnableMenuItem(ID_OBJ_LIST, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    }
    else {
        if (!hasData) {
            menu->EnableMenuItem(ID_CONVERT_DATA, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
            menu->EnableMenuItem(ID_OBJ_LIST, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        }
    }

    if (objListBox != NULL) {
        menu->EnableMenuItem(ID_FILE_OPEN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        menu->EnableMenuItem(ID_FOLDER_OPEN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        menu->EnableMenuItem(ID_FILE_QUICK, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        menu->EnableMenuItem(ID_FOLDER_QUICK, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        menu->EnableMenuItem(ID_CONVERT_DATA, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        menu->EnableMenuItem(ID_OBJ_LIST, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    }

    return;
}


void  COARConvWinApp::updateStatusBar(CString oar_path, CString out_path)
{
    if (pMainFrame == NULL) return;

    bool no_offset_flg = false;
    CString prefix;
    if (appParam.outputFormat == JBXL_3D_FORMAT_DAE) {
        prefix = _T(" DAE | ");
        if (appParam.procJoints) prefix += _T("JOINTS | ");
        if (appParam.noOffset) {
            no_offset_flg = true;
            prefix += _T("NO_OFFSET | ");
        }
    }
    else if (appParam.outputFormat == JBXL_3D_FORMAT_OBJ) {
        prefix = _T(" OBJ | ");
        if (appParam.noOffset) {
            no_offset_flg = true;
            prefix += _T("NO_OFFSET | ");
        }
    }
    else if (appParam.outputFormat == JBXL_3D_FORMAT_GLTF) {
        prefix = _T(" GLTF | ");
        if (appParam.procJoints) prefix += _T("JOINTS | ");
        if (appParam.noOffset) {
            no_offset_flg = true;
            prefix += _T("NO_OFFSET | ");
        }
    }
    else if (appParam.outputFormat == JBXL_3D_FORMAT_GLB) {
        prefix = _T(" GLB | ");
        if (appParam.procJoints) prefix += _T("JOINTS | ");
        if (appParam.noOffset) {
            no_offset_flg = true;
            prefix += _T("NO_OFFSET | ");
        }
    }
    else if (appParam.outputFormat == JBXL_3D_FORMAT_FBX) {
        prefix = _T(" FBX | ");
        if (appParam.procJoints) prefix += _T("JOINTS | ");
        if (appParam.noOffset) {
            no_offset_flg = true;
            prefix += _T("NO_OFFSET | ");
        }
    }
    // STL は元々 OFFSETなし．
    else if (appParam.outputFormat == JBXL_3D_FORMAT_STL_A) prefix = _T(" STL | ");
    else if (appParam.outputFormat == JBXL_3D_FORMAT_STL_B) prefix = _T(" STL | ");
    else                                                    prefix = _T(" NONE | ");

    if (appParam.outputEngine == JBXL_3D_ENGINE_UNITY)   prefix += _T("UNITY | ");
    else if (appParam.outputEngine == JBXL_3D_ENGINE_UE) prefix += _T("UE | ");

    //CString mesg = prefix + _T("OAR-Path: ") + oar_path + " | " + _T("OUT-Path: ") + out_path;
    CString mesg = prefix + _T("OUT: ") + out_path;
    pMainFrame->SetStausBarText(mesg);

    return;
}
