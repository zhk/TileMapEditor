
// TileMapEditor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "TileMapEditor.h"
#include "MainFrm.h"

#include "TileMapEditorDoc.h"
#include "TileMapEditorView.h"

#include "ResourceManager.h"
#include "ToolManager.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif




//-------------------------------------------------------------------------
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CTileMapEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}




//-------------------------------------------------------------------------
// CTileMapEditorApp

BEGIN_MESSAGE_MAP(CTileMapEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CTileMapEditorApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CTileMapEditorApp 构造

CTileMapEditorApp::CTileMapEditorApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CTileMapEditorApp 对象

CTileMapEditorApp theApp;


// CTileMapEditorApp 初始化

BOOL CTileMapEditorApp::InitInstance()
{
	setlocale(LC_CTYPE, "");

	char buffer[MAX_PATH];
	::GetModuleFileName(0, buffer, MAX_PATH);
	std::string strModulePath = buffer;
	strModulePath.erase(strModulePath.find_last_of('\\') + 1);
	SetCurrentDirectory(strModulePath.c_str());


	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

#if _DEBUG
	Mem_New Cactus::LogManager(Cactus::eLogLevelDebug, "TileMapEditor.log", Cactus::FileAppender::eMBCS);
#else
	Mem_New Cactus::LogManager(Cactus::eLogLevelInfo, "TileMapEditor.log", Cactus::FileAppender::eMBCS);
#endif

	new ResourceManager;
	std::string dataPath = strModulePath + "Data\\";
	if( !ResourceManager::getSingleton().Load(dataPath.c_str()) )
	{
		AfxMessageBox("加载资源失败！", MB_OK | MB_ICONHAND);
		return FALSE;
	}
	new ToolManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("ZHKSoft"));
	LoadStdProfileSettings(0);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTileMapEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CTileMapEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}


// CTileMapEditorApp 自定义加载/保存方法

void CTileMapEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);

	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);

	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);

	GetContextMenuManager()->AddMenu("地层菜单", IDR_MENU_LAYER_OP);
	GetContextMenuManager()->AddMenu("地层组菜单", IDR_MENU_MAPLAYER_OP);
	GetContextMenuManager()->AddMenu("画刷菜单", IDR_MENU_MAPBRUSH_OP);
	GetContextMenuManager()->AddMenu("资源菜单", IDR_MENU_POPUP_RESVIEW);
	GetContextMenuManager()->AddMenu("游戏对象组菜单", IDR_MENU_GAMEOBJECTGROUP);
	GetContextMenuManager()->AddMenu("游戏对象菜单", IDR_MENU_POPUP_GAMEOBJECT_ADD);
	GetContextMenuManager()->AddMenu("Tile资源组菜单", IDR_MENU_RESOURCE_TILE);
}

void CTileMapEditorApp::LoadCustomState()
{
}

void CTileMapEditorApp::SaveCustomState()
{
}

// CTileMapEditorApp 消息处理程序




int CTileMapEditorApp::ExitInstance()
{
	if( ToolManager::getSingletonPtr() )
		delete ToolManager::getSingletonPtr();

	if( ResourceManager::getSingletonPtr() )
		delete ResourceManager::getSingletonPtr();

	if( Cactus::LogManager::getSingletonPtr() )
	{
		Cactus::LogManager::getSingleton().Flush();
		Mem_Delete Cactus::LogManager::getSingletonPtr();
	}

	return CWinAppEx::ExitInstance();
}
