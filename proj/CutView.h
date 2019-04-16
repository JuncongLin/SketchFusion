#pragma once


// CCutView view

class CCutView : public CView
{
	DECLARE_DYNCREATE(CCutView)

protected:
	CCutView();           // protected constructor used by dynamic creation
	virtual ~CCutView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


