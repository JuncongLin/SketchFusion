#pragma once

#include "..\OpenGLBasic\Boundary.h"
#include "..\OpenGLBasic\Patch.h"
#include <afxtempl.h>

class CWrinkle
{
public:
	CWrinkle(void);
	~CWrinkle(void);
	CPatch* GlobalMinimumBendingBridgeTriangulation(void);
	void BuildEdgeValidityMatrix(void);
private:
	int m_levelOfLocalConvexity;
	int m_curveOnePntNum;
	int m_curveTwoPntNum;
	CBoundary *m_pCurveOne;
	CBoundary *m_pCurveTwo;
	bool **m_validEdgeFlags;
	CArray<int,int> m_pList,m_qList;

public:
	double HausdorffDistance(void);
	bool EdgeValidityCheck(int pIndex, int qIndex);
	bool ConvexEdgeDetection(int pntNum, CVector3d* pPt);
	double ComputeEdgeBendingEnergy(int pIndex, int qIndex, bool prev_pOrq, bool next_pOrq);
	CPatch* MeshGenerationByShortestPath(void);
	void ImportCurvePair(CBoundary* pBound1, CBoundary* pBound2);
	void Initialize(void);
	CPatch* GlobalMaximumTriRegularityBridgeTriangulation(void);
	double ComputeTrglRegularity(int pIndex, int qIndex, bool next_pOrq);
};
	