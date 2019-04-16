#pragma once
#include "..\OpenGLBasic\Patch.h"
#include "Frame.h"
#include "Vector2d.h"
class CDetailPatch
{
public:
	CDetailPatch(void);
	~CDetailPatch(void);
private:
	CPatch *m_pPatchI;
	CPatch *m_pPatchO;
	CFrame m_Frame;
	CVertex *m_pCorner1;
	CVertex *m_pCorner2;
	CVertex *m_pCorner3;
	CVertex *m_pCorner4;
	CBoundary *m_pBoundary;
	double m_dXYRatio;
	CVector2d * m_pParaCoord;
	double** m_ppLambda;
	int * m_pValency;
	vector<CVector3d*> m_vecDetails;
public:
	void SetIPatch(CPatch* pPatch);
	void SetOPatch(CPatch* pPatch);
	void SetFrame(CVector3d& x, CVector3d& y, CVector3d& z);
	void DetermineCorner(CVector3d& c1,CVector3d& c2, CVector3d& c3, CVector3d& c4);
	void ConvexMapping(void);
private:
	void BoundaryMapping(void);
	void InternalMapping(void);
public:
	double Compare(CDetailPatch* pDPatch);
	void FillUnknownFromSample(CDetailPatch* pDPatch);
	CFrame& GetFrame(void);
	CVector3d DetailFromParaCoord(CVector2d& coord);
	void Display(void);
	int GetEdgeNum(void);
	CEdge* GetEdgeByIndex(int index);
	double GetXYRatio(void);
	CVector2d& GetParaCoord(int index);
	bool IsFaceInside(CFace* pFace);
	void SetXYRatio(double ratio);
private:
	bool m_bDisplayStyle;
public:
	void SwitchDisplayStyle(void);
	void ClearFlag(void);
	void AddDetail(CVector3d* pDetail);

	void ResetIndex(void)
	{
		int i,count=0;
		CVertex *pV;

		m_pBoundary->SetFlag();

		for (i=0;i<m_pPatchO->GetVertexNum();i++)
		{
			pV=m_pPatchO->GetVertexByIndex(i);
			if(pV->IsBoundary())
			{
				continue;
			}
			pV->SetIndex(count++);
		}
		for (i=0;i<m_pBoundary->GetVertexNum();i++)
		{
			pV=m_pBoundary->GetVertexByIndex(i);
			pV->SetIndex(count++);
		}

	}
	void ImportDetails(vector<CVector3d*>& vDetails);
	void ApplyDetail(vector<CVector3d*>& vDetails);
};
