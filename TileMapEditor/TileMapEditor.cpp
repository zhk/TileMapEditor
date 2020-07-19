
// TileMapEditor.cpp : ����Ӧ�ó��������Ϊ��
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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CTileMapEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}




//-------------------------------------------------------------------------
// CTileMapEditorApp

BEGIN_MESSAGE_MAP(CTileMapEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CTileMapEditorApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CTileMapEditorApp ����

CTileMapEditorApp::CTileMapEditorApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CTileMapEditorApp ����

CTileMapEditorApp theApp;


// CTileMapEditorApp ��ʼ��

BOOL CTileMapEditorApp::InitInstance()
{
	setlocale(LC_CTYPE, "");

	char buffer[MAX_PATH];
	::GetModuleFileName(0, buffer, MAX_PATH);
	std::string strModulePath = buffer;
	strModulePath.erase(strModulePath.find_last_of('\\') + 1);
	SetCurrentDirectory(strModulePath.c_str());


	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
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
		AfxMessageBox("������Դʧ�ܣ�", MB_OK | MB_ICONHAND);
		return FALSE;
	}
	new ToolManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("ZHKSoft"));
	LoadStdProfileSettings(0);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTileMapEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CTileMapEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	// ������/��
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}


// CTileMapEditorApp �Զ������/���淽��

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

	GetContextMenuManager()->AddMenu("�ز�˵�", IDR_MENU_LAYER_OP);
	GetContextMenuManager()->AddMenu("�ز���˵�", IDR_MENU_MAPLAYER_OP);
	GetContextMenuManager()->AddMenu("��ˢ�˵�", IDR_MENU_MAPBRUSH_OP);
	GetContextMenuManager()->AddMenu("��Դ�˵�", IDR_MENU_POPUP_RESVIEW);
	GetContextMenuManager()->AddMenu("��Ϸ������˵�", IDR_MENU_GAMEOBJECTGROUP);
	GetContextMenuManager()->AddMenu("��Ϸ����˵�", IDR_MENU_POPUP_GAMEOBJECT_ADD);
	GetContextMenuManager()->AddMenu("Tile��Դ��˵�", IDR_MENU_RESOURCE_TILE);
}

void CTileMapEditorApp::LoadCustomState()
{
}

void CTileMapEditorApp::SaveCustomState()
{
}

// CTileMapEditorApp ��Ϣ�������




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
