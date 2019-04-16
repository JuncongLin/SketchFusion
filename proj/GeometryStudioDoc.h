// GeometryStudioDoc.h : interface of the CGeometryStudioDoc class
//


#pragma once

class CGeometryStudioDoc : public CDocument
{
protected: // create from serialization only
	CGeometryStudioDoc();
	DECLARE_DYNCREATE(CGeometryStudioDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CGeometryStudioDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


