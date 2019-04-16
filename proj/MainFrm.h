// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "WorkSpaceBar.h"
#include "WorkSpaceBar2.h"
#include "OutputBar.h"
#include "ToolBox.h"
#include "VideoDialog.h"
#include "POVSettingDialog.h"

class CMainFrame : public CBCGPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CProgressCtrl	m_Progress;
	CBCGPMenuBar		m_wndMenuBar;
	CBCGPStatusBar  m_wndStatusBar;
	CBCGPToolBar    m_wndToolBarStandard;
	CBCGPToolBar    m_wndToolBarManipulate;
	CWorkSpaceBar	m_wndWorkSpace;
	CWorkSpaceBar2	m_wndWorkSpace2;
	COutputBar	m_wndOutput;
	CVideoDialog m_wndVideo;
	CPOVSettingDialog m_wndPOVRay;

	CBCGPDockingControlBar*	m_pWndToolBoxBar;

	CBCGPToolBarImages	m_UserImages;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	void OnToolsViewUserToolbar (UINT id);
	void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
	afx_msg LRESULT OnHelpCustomizeToolbars(WPARAM wp, LPARAM lp);
	afx_msg void OnViewWorkspace();
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnViewWorkspace2();
	afx_msg void OnUpdateViewWorkspace2(CCmdUI* pCmdUI);
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnWindowManager();
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
	afx_msg void OnMdiMoveToNextGroup();
	afx_msg void OnMdiMoveToPrevGroup();
	afx_msg void OnMdiNewHorzTabGroup();
	afx_msg void OnMdiNewVertGroup();
	afx_msg void OnMdiCancel();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop);

	UINT	m_nAppLook;
public:
	CBCGPDockingControlBar* GetToolBoxBar(void);
	afx_msg void OnViewStandard();
	afx_msg void OnViewManipulate();
	afx_msg void OnViewToolbox();
private:
public:
	afx_msg void OnViewVideo();
	afx_msg void OnUpdateViewVideo(CCmdUI *pCmdUI);
	afx_msg void OnViewPovray();
	afx_msg void OnUpdateViewPovray(CCmdUI *pCmdUI);
};


