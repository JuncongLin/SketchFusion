// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GeometryStudio.h"

#include "MainFrm.h"
#include ".\mainfrm.h"
#include "ChildFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CBCGPMDIFrameWnd)
const int  iMaxUserToolbars		= 10;
const UINT uiFirstUserToolBarId	= AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId	= uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_COMMAND(ID_VIEW_MANIPULATE, OnViewManipulate)
	ON_COMMAND(ID_VIEW_STANDARD, OnViewStandard)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
	ON_COMMAND_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnToolsViewUserToolbar)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnUpdateToolsViewUserToolbar)
	ON_REGISTERED_MESSAGE(BCGM_CUSTOMIZEHELP, OnHelpCustomizeToolbars)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_COMMAND(ID_VIEW_WORKSPACE2, OnViewWorkspace2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE2, OnUpdateViewWorkspace2)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_VS2005, OnAppLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_VS2005, OnUpdateAppLook)
	ON_COMMAND(ID_MDI_MOVE_TO_NEXT_GROUP, OnMdiMoveToNextGroup)
	ON_COMMAND(ID_MDI_MOVE_TO_PREV_GROUP, OnMdiMoveToPrevGroup)
	ON_COMMAND(ID_MDI_NEW_HORZ_TAB_GROUP, OnMdiNewHorzTabGroup)
	ON_COMMAND(ID_MDI_NEW_VERT_GROUP, OnMdiNewVertGroup)
	ON_COMMAND(ID_MDI_CANCEL, OnMdiCancel)

	ON_COMMAND(ID_VIEW_TOOLBOX, OnViewToolbox)
	ON_COMMAND(ID_VIEW_VIDEO, OnViewVideo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VIDEO, OnUpdateViewVideo)
	ON_COMMAND(ID_VIEW_POVRAY, OnViewPovray)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POVRAY, OnUpdateViewPovray)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_VS2005);
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	OnAppLook (m_nAppLook);

	// VISUAL_MANAGER

	if (CBCGPToolBar::GetUserImages () == NULL)
	{
		// Load toolbar user images:
		if (!m_UserImages.Load (_T(".\\UserImages.bmp")))
		{
			TRACE(_T("Failed to load user images\n"));
			ASSERT (FALSE);
		}
		else
		{
			CBCGPToolBar::SetUserImages (&m_UserImages);
		}
	}

	CBCGPToolBar::EnableQuickCustomization ();

	// TODO: Define your own basic commands. Be sure, that each pulldown 
	// menu have at least one basic command.

	CList<UINT, UINT>	lstBasicCommands;

	lstBasicCommands.AddTail (ID_FILE_OPEN);
	lstBasicCommands.AddTail (ID_FILE_SAVE);
	lstBasicCommands.AddTail (ID_FILE_PRINT);
	lstBasicCommands.AddTail (ID_APP_EXIT);
	lstBasicCommands.AddTail (ID_EDIT_CUT);
	lstBasicCommands.AddTail (ID_EDIT_PASTE);
	lstBasicCommands.AddTail (ID_EDIT_UNDO);
	lstBasicCommands.AddTail (ID_RECORD_NEXT);
	lstBasicCommands.AddTail (ID_RECORD_LAST);
	lstBasicCommands.AddTail (ID_APP_ABOUT);
	lstBasicCommands.AddTail (ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail (ID_VIEW_CUSTOMIZE);
	lstBasicCommands.AddTail (ID_WINDOW_TILE_HORZ);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2000);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_XP);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2003);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_VS2005);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_WIN_XP);

	CBCGPToolBar::SetBasicCommands (lstBasicCommands);

	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	// Detect color depth. 256 color toolbars can be used in the
	// high or true color modes only (bits per pixel is > 8):
	CClientDC dc (this);
	BOOL bIsHighColor = dc.GetDeviceCaps (BITSPIXEL) > 8;

	UINT uiToolbarHotID,uiToolbarColdID;

	uiToolbarHotID = bIsHighColor ? IDB_TOOLBAR256 : 0;
	uiToolbarColdID = bIsHighColor ? IDB_TOOLBARCOLD256 : 0;
	if (!m_wndToolBarStandard.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBarStandard.LoadToolBar(IDR_MAINFRAME, uiToolbarColdID, 0, FALSE, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	m_wndToolBarStandard.SetWindowText (strMainToolbarTitle);

	uiToolbarHotID = bIsHighColor ? IDB_MANIPULATE256 : 0;
	uiToolbarColdID = bIsHighColor ? IDB_MANIPULATECOLD256 : 0;

	if (!m_wndToolBarManipulate.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBarManipulate.LoadToolBar(IDR_MANIPULATE, uiToolbarColdID, 0, FALSE, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	CString strMainpulateToolbarTitle;
	strMainpulateToolbarTitle.LoadString (IDS_MANIPULATE_TOOLBAR);
	m_wndToolBarManipulate.SetWindowText (strMainpulateToolbarTitle);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_Progress.Create(WS_CHILD | WS_VISIBLE , CRect(2,2,450,18), &m_wndStatusBar, IDC_PROGRESS ))
	{
		TRACE0("Failed to create progress control \n");
		return -1;      // fail to create
	}
	m_Progress.SetRange(0,100);
	m_Progress.SetPos(0);


	CImageList imagesWorkspace;
	imagesWorkspace.Create (IDB_WORKSPACE, 16, 0, RGB (255, 0, 255));

	if (!m_wndWorkSpace.Create (_T("View  1"), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar\n");
		return FALSE;      // fail to create
	}

	m_wndWorkSpace.SetIcon (imagesWorkspace.ExtractIcon (0), FALSE);

	if (!m_wndWorkSpace2.Create (_T("View 2"), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE2,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar 2\n");
		return FALSE;      // fail to create
	}

	m_wndWorkSpace2.SetIcon (imagesWorkspace.ExtractIcon (1), FALSE);


	if (!m_wndOutput.Create (_T("Output"), this, CSize (150, 150),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}

	CToolBoxBarEx* pToolBox = new CToolBoxBarEx;

	if (!pToolBox->Create (_T("ToolBox"), this, CRect (0, 0, 200, 200),
		TRUE, 
		ID_TOOLBOX,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create ToolBox Bar\n");
		return FALSE;		// fail to create
	}

	m_pWndToolBoxBar = pToolBox;

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarStandard.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarManipulate.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace2.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);


	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(m_pWndToolBoxBar);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBarStandard);
	DockControlBar(&m_wndToolBarManipulate);
	DockControlBar(&m_wndWorkSpace);
	m_wndWorkSpace2.AttachToTabWnd (&m_wndWorkSpace, DM_STANDARD, FALSE, NULL);
	////	m_pWndToolBoxBar->AttachToTabWnd(&m_wndWorkSpace2,DM_STANDARD,FALSE,NULL);
	DockControlBar(&m_wndOutput);


	m_wndToolBarStandard.EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));

	// Allow user-defined toolbars operations:
	InitUserToobars (NULL,
		uiFirstUserToolBarId,
		uiLastUserToolBarId);

	// Enable windows manager:
	EnableWindowsDialog (ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

	CString str;
	str.Format("Welcome to Geometry Studio! ^0^");
	m_wndOutput.FillOperationInfo(str);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
		TRUE /* Automatic menus scaning */,
		BCGCUSTOMIZE_MENU_SHADOWS | BCGCUSTOMIZE_TEXT_LABELS | 
		BCGCUSTOMIZE_LOOK_2000 | BCGCUSTOMIZE_MENU_ANIMATIONS | BCGCUSTOMIZE_CONTEXT_HELP);
	pDlgCust->EnableUserDefinedToolbars ();
	pDlgCust->Create ();
}

void CMainFrame::OnToolsViewUserToolbar (UINT uiId)
{
	CBCGPToolBar* pUserToolBar = GetUserBarByIndex (uiId - ID_VIEW_USER_TOOLBAR1);
	if (pUserToolBar == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	ShowControlBar (pUserToolBar, !(pUserToolBar->IsVisible ()), FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI)
{
	CBCGPToolBar* pUserToolBar = GetUserBarByIndex (pCmdUI->m_nID - ID_VIEW_USER_TOOLBAR1);
	if (pUserToolBar == NULL)
	{
		pCmdUI->Enable (FALSE);
		return;
	}

	pCmdUI->Enable ();
	pCmdUI->SetCheck (pUserToolBar->IsVisible ());
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM /*wp*/,LPARAM)
{
	// TODO: reset toolbar with id = (UINT) wp to its initial state:
	//
	// UINT uiToolBarId = (UINT) wp;
	// if (uiToolBarId == IDR_MAINFRAME)
	// {
	//		do something with m_wndToolBar
	// }

	return 0;
}

LRESULT CMainFrame::OnHelpCustomizeToolbars(WPARAM wp, LPARAM lp)
{
	int iPageNum = (int) wp;

	CBCGPToolbarCustomize* pDlg = (CBCGPToolbarCustomize*) lp;
	ASSERT_VALID (pDlg);

	// TODO: show help about page number iPageNum

	return 0;
}

void CMainFrame::OnViewWorkspace() 
{
	BOOL bAutoHideMode = m_wndWorkSpace.IsAutoHideMode ();
	BOOL bTabbed = m_wndWorkSpace.IsTabbed ();
	ShowControlBar (&m_wndWorkSpace,
					!(m_wndWorkSpace.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndWorkSpace.IsVisible ());
}

void CMainFrame::OnViewWorkspace2() 
{
	BOOL bAutoHideMode = m_wndWorkSpace2.IsAutoHideMode ();
	BOOL bTabbed = m_wndWorkSpace2.IsTabbed ();
	ShowControlBar (&m_wndWorkSpace2,
					!(m_wndWorkSpace2.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewWorkspace2(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndWorkSpace2.IsVisible ());
}


void CMainFrame::OnViewOutput() 
{
	ShowControlBar (&m_wndOutput,
					!(m_wndOutput.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndOutput.IsVisible ());
}

void CMainFrame::OnWindowManager() 
{
	ShowWindowsDialog ();
}

void CMainFrame::OnAppLook(UINT id)
{
	CBCGPDockManager::SetDockMode (DT_SMART);

	m_nAppLook = id;

	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D;

	CBCGPMDITabParams mdiTabParams;
	mdiTabParams.m_bTabIcons = TRUE;

	switch (m_nAppLook)
	{
	case ID_VIEW_APPLOOK_2000:
		// enable Office 2000 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager));
		break;

	case ID_VIEW_APPLOOK_XP:
		// enable Office XP look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		// enable Windows XP look (in other OS Office XP look will be used):
		CBCGPWinXPVisualManager::m_b3DTabsXPTheme = TRUE;
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
		break;

	case ID_VIEW_APPLOOK_2003:
		// enable Office 2003 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));

		mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005;
		mdiTabParams.m_bAutoColor = TRUE;
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2005:
		// enable VS.NET 2005 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));
		CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D_ROUNDED;
		CBCGPVisualManager::GetInstance ();

		mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005;
		mdiTabParams.m_bDocumentMenu = TRUE;
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	default:
		ASSERT (FALSE);
	}

	EnableMDITabbedGroups (TRUE, mdiTabParams);

	CBCGPDockManager* pDockManager = GetDockManager ();
	if (pDockManager != NULL)
	{
		ASSERT_VALID (pDockManager);
		pDockManager->AdjustBarFrames ();
	}

	CBCGPTabbedControlBar::ResetTabs ();

	RecalcLayout ();
	RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

	theApp.WriteInt (_T("ApplicationLook"), m_nAppLook);
}

void CMainFrame::OnUpdateAppLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio (m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop)
{
	CMenu menu;
	VERIFY(menu.LoadMenu (bDrop ? IDR_POPUP_DROP_MDITABS : IDR_POPUP_MDITABS));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if ((dwAllowedItems & BCGP_MDI_CREATE_HORZ_GROUP) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_NEW_HORZ_TAB_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CREATE_VERT_GROUP) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_NEW_VERT_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_NEXT) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_MOVE_TO_NEXT_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_PREV) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_MOVE_TO_PREV_GROUP, MF_BYCOMMAND);
	}

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->SetAutoDestroy (FALSE);
	pPopupMenu->Create (this, point.x, point.y, pPopup->GetSafeHmenu ());

	return TRUE;
}

void CMainFrame::OnMdiMoveToNextGroup() 
{
	MDITabMoveToNextGroup ();
}

void CMainFrame::OnMdiMoveToPrevGroup() 
{
	MDITabMoveToNextGroup (FALSE);
}

void CMainFrame::OnMdiNewHorzTabGroup() 
{
	MDITabNewGroup (FALSE);
}

void CMainFrame::OnMdiNewVertGroup() 
{
	MDITabNewGroup ();
}

void CMainFrame::OnMdiCancel() 
{
	// TODO: Add your command handler code here
	
}



CBCGPDockingControlBar* CMainFrame::GetToolBoxBar(void)
{
	return m_pWndToolBoxBar;
}

void CMainFrame::OnViewStandard()
{
	ShowControlBar (&m_wndToolBarStandard,
		!m_wndToolBarStandard.IsVisible (),
		FALSE, FALSE);
}

void CMainFrame::OnViewManipulate()
{
	ShowControlBar (&m_wndToolBarManipulate,
		!m_wndToolBarManipulate.IsVisible (),
		FALSE, FALSE);
}

void CMainFrame::OnViewToolbox()
{
	// demonstartes how to make bar active in autohide mode and inactive when non-tabbed
	BOOL bAutoHideMode = m_pWndToolBoxBar->IsAutoHideMode ();
	BOOL bTabbed = m_pWndToolBoxBar->IsTabbed ();

	ShowControlBar (m_pWndToolBoxBar,
		!m_pWndToolBoxBar->IsVisible (),
		FALSE, bAutoHideMode || !bTabbed);
	RecalcLayout ();
}

void CMainFrame::OnViewVideo()
{
	if(m_wndVideo.GetSafeHwnd()==0)
	{
		m_wndVideo.Create();
		m_wndVideo.ShowWindow(SW_SHOW);
	}
	else
	{
		m_wndVideo.DestroyWindow();
	}
}


void CMainFrame::OnUpdateViewVideo(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_wndVideo.GetSafeHwnd()!=0);
}

void CMainFrame::OnViewPovray()
{
	CChildFrame *pChildFrame=(CChildFrame*)this->GetActiveFrame();
	CView   *pView  = pChildFrame->GetActiveView();   
	if(pView->IsKindOf(RUNTIME_CLASS(CGeometryStudioView)))
	{
		if(m_wndPOVRay.GetSafeHwnd()==0)
		{
			m_wndPOVRay.SetView((CGeometryStudioView*)pView);
			m_wndPOVRay.Create();
			m_wndPOVRay.ShowWindow(SW_SHOW);
		}
		else
		{
			m_wndPOVRay.DestroyWindow();

		}
	}
}

void CMainFrame::OnUpdateViewPovray(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_wndPOVRay.GetSafeHwnd()!=0);
}
