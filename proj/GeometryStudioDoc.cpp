// GeometryStudioDoc.cpp : implementation of the CGeometryStudioDoc class
//

#include "stdafx.h"
#include "GeometryStudio.h"

#include "GeometryStudioDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGeometryStudioDoc

IMPLEMENT_DYNCREATE(CGeometryStudioDoc, CDocument)

BEGIN_MESSAGE_MAP(CGeometryStudioDoc, CDocument)
END_MESSAGE_MAP()


// CGeometryStudioDoc construction/destruction

CGeometryStudioDoc::CGeometryStudioDoc()
{
	// TODO: add one-time construction code here

}

CGeometryStudioDoc::~CGeometryStudioDoc()
{
}

BOOL CGeometryStudioDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CGeometryStudioDoc serialization

void CGeometryStudioDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CGeometryStudioDoc diagnostics

#ifdef _DEBUG
void CGeometryStudioDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGeometryStudioDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGeometryStudioDoc commands
