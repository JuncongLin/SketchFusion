// CutView.cpp : implementation file
//

#include "stdafx.h"
#include "GeometryStudio.h"
#include "CutView.h"


// CCutView

IMPLEMENT_DYNCREATE(CCutView, CView)

CCutView::CCutView()
{
}

CCutView::~CCutView()
{
}

BEGIN_MESSAGE_MAP(CCutView, CView)
END_MESSAGE_MAP()


// CCutView drawing

void CCutView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CCutView diagnostics

#ifdef _DEBUG
void CCutView::AssertValid() const
{
	CView::AssertValid();
}

void CCutView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CCutView message handlers
