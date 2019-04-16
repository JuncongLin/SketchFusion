// GeometryStudio.h : main header file for the GeometryStudio application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CGeometryStudioApp:
// See GeometryStudio.cpp for the implementation of this class
//

class CGeometryStudioApp : public CWinApp,
							public CBCGPWorkspace
{
public:
	CGeometryStudioApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CMultiDocTemplate*		m_pStartDocTemplate;
	CMultiDocTemplate*		m_pDocTemplateScene;
	CMultiDocTemplate*		m_pDocTemplateCut;

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGeometryStudioApp theApp;