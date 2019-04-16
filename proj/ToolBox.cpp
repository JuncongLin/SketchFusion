// ToolBox.cpp: implementation of the CResourceViewBar class.
//

#include "stdafx.h"
#include "GeometryStudio.h"
#include "MainFrm.h"
#include "ToolBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBoxBar

CToolBoxBar::CToolBoxBar()
{
	CBCGPOutlookWnd::EnableAnimation ();
}

CToolBoxBar::~CToolBoxBar()
{
}

BEGIN_MESSAGE_MAP(CToolBoxBar, CBCGPToolBox)
	//{{AFX_MSG_MAP(CToolBoxBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CToolBoxBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPToolBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	AddToolsPage (_T("Windows Forms"), IDB_TOOLS_PAGE, 16, 
		_T("Pointer\nLabel\nButton\nTextBox\nSketch"));
//	AddToolsPage (_T("Components"),	IDB_TOOLS_PAGE2, 16, _T("Pointer\nDataSet\nOleDbDataAdapter\nOleDbConnection"));
//	AddToolsPage (_T("Data"),	IDB_TOOLS_PAGE3, 16, _T("Pointer\nFileSystemWatch\nEventLog\nDyrectoryEntry"));

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CToolBoxBarEx

CToolBoxBarEx::CToolBoxBarEx()
{
}

CToolBoxBarEx::~CToolBoxBarEx()
{
}

BEGIN_MESSAGE_MAP(CToolBoxBarEx, CBCGPToolBoxEx)
	//{{AFX_MSG_MAP(CToolBoxBarEx)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CToolBoxBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPToolBoxEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	AddToolsPage (_T("Windows Forms"), IDB_TOOLS_PAGE, 16, 
		_T("Pointer\nLabel\nButton\nTextBox\nSketch"));
//	AddToolsPage (_T("Components"),	IDB_TOOLS_PAGE2, 16, _T("Pointer\nDataSet\nOleDbDataAdapter\nOleDbConnection"));
//	AddToolsPage (_T("Data"),	IDB_TOOLS_PAGE3, 16, _T("Pointer\nFileSystemWatch\nEventLog\nDyrectoryEntry"));

	return 0;
}
