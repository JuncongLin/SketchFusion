// ParameterizationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GeometryStudio.h"
#include "ParameterizationDialog.h"
#include ".\parameterizationdialog.h"


// CParameterizationDialog dialog

IMPLEMENT_DYNAMIC(CParameterizationDialog, CDialog)
CParameterizationDialog::CParameterizationDialog(CDetailPatch *pPatch)
	: CDialog(CParameterizationDialog::IDD, NULL)
{
	m_pPatch=pPatch;
}

CParameterizationDialog::~CParameterizationDialog()
{
}

void CParameterizationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PARAMETERIZATION, m_CtrlParameterization);
}


BEGIN_MESSAGE_MAP(CParameterizationDialog, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CParameterizationDialog message handlers

void CParameterizationDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	m_CtrlParameterization.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CBrush BrushBack(RGB(255,255,255));
	dc.FillRect(&rect,&BrushBack);

	CEdge *pE;
	CVertex *pV1,*pV2;
	CFace *pF1,*pF2;
	int w,h,l,sx,sy;
	float r;
	CVector2d ParaCoord1, ParaCoord2;
	POINT ScrCoord[2],LT,LB,RT,RB;
	int i;
	r=m_pPatch->GetXYRatio();
	w=rect.Width();
	h=rect.Height();
	l=(w*r>h)?(h/r):w;
	l*=0.9;
	sx=(w-l)/2;
	sy=(h-l*r)/2;
	CPen* pOldPen;
	CPen PenInner(PS_SOLID,1.0,RGB(255,0,0));
	CPen PenOutter(PS_SOLID,1.0,RGB(0,0,255));
	CPen PenBound(PS_SOLID,1.0,RGB(0,255,0));
	pOldPen=dc.SelectObject(&PenInner);
	for(i=0;i<m_pPatch->GetEdgeNum();i++)
	{
		pE=m_pPatch->GetEdgeByIndex(i);
		pV1=pE->GetHe1()->GetVertex();
		pV2=pE->GetHe2()->GetVertex();
		ParaCoord1=m_pPatch->GetParaCoord(pV1->GetIndex());
		ParaCoord2=m_pPatch->GetParaCoord(pV2->GetIndex());

		pF1=pE->GetHe1()->GetFace();
		pF2=pE->GetHe2()->GetFace();

		//		if(pV1->GetParaDomain()==2&&pV2->GetParaDomain()==2)
		//			pOldPen=dc.SelectObject(&PenInner);
		//		else
		//			pOldPen=dc.SelectObject(&PenOutter);

		if(m_pPatch->IsFaceInside(pF1)&&m_pPatch->IsFaceInside(pF2))
			pOldPen=dc.SelectObject(&PenInner);
		else if(m_pPatch->IsFaceInside(pF1)||m_pPatch->IsFaceInside(pF2))
			pOldPen=dc.SelectObject(&PenBound);
		else
			pOldPen=dc.SelectObject(&PenOutter);

		ScrCoord[0].x=rect.left+sx+ParaCoord1[0]*l;
		ScrCoord[0].y=rect.bottom-sy-ParaCoord1[1]*l;

		ScrCoord[1].x=rect.left+sx+ParaCoord2[0]*l;
		ScrCoord[1].y=rect.bottom-sy-ParaCoord2[1]*l;

		dc.MoveTo(ScrCoord[0]);
		dc.LineTo(ScrCoord[1]);
		dc.SelectObject(pOldPen);
	}
	LT.x=rect.left+sx;
	LT.y=rect.top+sy;

	LB.x=rect.left+sx;
	LB.y=rect.bottom-sy;

	RT.x=rect.right-sx;
	RT.y=rect.top+sy;

	RB.x=rect.left-sx;
	RB.y=rect.bottom-sy;
	dc.SetPixelV(LT,RGB(255,0,0));
	dc.SetPixelV(LB,RGB(0,255,0));
	dc.SetPixelV(RT,RGB(0,0,255));
	dc.SetPixelV(RB,RGB(255,255,0));

}
