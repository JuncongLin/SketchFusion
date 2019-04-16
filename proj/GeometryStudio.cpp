// GeometryStudio.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GeometryStudio.h"
#include "MainFrm.h"
#include "StartView.h"

#include "ChildFrm.h"
#include "GeometryStudioDoc.h"
#include "GeometryStudioView.h"
#include "CutView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGeometryStudioApp

BEGIN_MESSAGE_MAP(CGeometryStudioApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CGeometryStudioApp construction

CGeometryStudioApp::CGeometryStudioApp() :
	CBCGPWorkspace (TRUE /* m_bResourceSmartUpdate */)
{
	m_pStartDocTemplate=NULL;
	m_pDocTemplateScene=NULL;
	m_pDocTemplateCut=NULL;

}


// The one and only CGeometryStudioApp object

CGeometryStudioApp theApp;

// CGeometryStudioApp initialization

BOOL CGeometryStudioApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("BCGP AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	SetRegistryBase (_T("Settings"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitContextMenuManager();
	InitKeyboardManager();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views

	m_pDocTemplateScene = new CMultiDocTemplate(
		IDR_GeometryStudioTYPE,
		RUNTIME_CLASS(CGeometryStudioDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CGeometryStudioView));
	AddDocTemplate (m_pDocTemplateScene);

	m_pDocTemplateCut = new CMultiDocTemplate(
		IDR_CUT,
		RUNTIME_CLASS(CGeometryStudioDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CCutView));
	AddDocTemplate (m_pDocTemplateCut);

	m_pStartDocTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGeometryStudioDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CStartView));
	AddDocTemplate (m_pStartDocTemplate);


	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd

	// Parse command line for standard shell commands, DDE, file open

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{
		if (!pMainFrame->LoadMDIState (GetRegSectionPath ()))
		{
			m_pStartDocTemplate->OpenDocumentFile (NULL);
		}
	}
	else
	{
		// Dispatch commands specified on the command line
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}

	m_pMainWnd->DragAcceptFiles();

	// The main window has been initialized, so show and update it.
	if (!ReloadWindowPlacement (pMainFrame))
	{
		pMainFrame->ShowWindow(m_nCmdShow);
		pMainFrame->UpdateWindow();
	}

	return TRUE;
}

// CGeometryStudioApp message handlers

int CGeometryStudioApp::ExitInstance() 
{
	BCGCBProCleanUp();

	return CWinApp::ExitInstance();
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };
	CBCGPURLLinkButton m_btnURL;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMPANY_URL, m_btnURL);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CGeometryStudioApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CGeometryStudioApp message handlers


void CGeometryStudioApp::PreLoadState ()
{

	GetContextMenuManager()->AddMenu (_T("My menu"), IDR_CONTEXT_MENU);

	// TODO: add another context menus here
}

