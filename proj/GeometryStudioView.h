// GeometryStudioView.h : interface of the CGeometryStudioView class
//


#pragma once

#include "..\openglbasic\easyopengl.h"
#include "..\openglbasic\arcball\camera.h"
#include "..\OpenGLBasic\Solid.h"
#include "global.h"
#include "Stroke.h"
#include "..\OpenGLBasic\Boundary.h"
#include "..\OpenGLBasic\Patch.h"
#include "TransitionPrimitive.h"
#include "ParticleSamplingSys.h"
#include "Plane.h"
#include "GeodesicFan.h"
#include "FSNFileManager.h"
#include "SIRFileManager.h"
#include "RBFSurface.h"
#include "DetailPatch.h"

class CGeometryStudioDoc;
typedef enum{VIS_TMC,VIS_PS}VIS_STYLE;
class CGeometryStudioView : public CView
{
protected: // create from serialization only
	CGeometryStudioView();
	DECLARE_DYNCREATE(CGeometryStudioView)

// Attributes
public:
	CGeometryStudioDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGeometryStudioView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	CEasyOpenGL m_OpenGLManager;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	//MarchingCubes mc;
	CRBFSurface Solver;

	int criticalPts[2];
	vector<int> UKVs,KVs;
	int KVIndex;
	CDetailPatch *m_pDPatch;

	VIS_STYLE m_vsMode;

	CSolid* m_pModel;
	MouseOp m_moType;
	CStroke* m_pStroke;
	CStroke* m_pDepthStroke;
	CStroke* m_pSilhouetteStroke;
	int m_iFaceIndex;

	int m_iPTIndex;

	CFace *m_pWFace;
	CFace *m_pWNFace1;
	CFace *m_pWNFace2;
	CFace *m_pWNFace3;

	CPatch *m_pWPatch;
	CVertex* m_pVertex;
	CGeodesicFan* m_pGeoFan;

	CVector3d cn1;
	CVector3d cn2;
	CVector3d cn3;
	CVector3d cn4;


	vector<CVector3d*> m_vBaseCoords;
	vector<CVector3d*> m_vRawCoords;
	vector<CVector3d*> m_vReconCoords;
	vector<CVector3d*> m_vDetails;
	vector<CVertex*> m_vecTriVertices;

	vector<CPoint3*> m_vecCtrlPolygon;

	CFSNFileManager m_FSNManager;
	CSIRFileManager m_SIRManager;


	CBoundary *m_pBoundary;


	vector<CPoint> m_vecStrokePoints2D;
	CPoint m_PrevMousePos;
	int m_iStrokeIn;
	int m_iStrokeOut;
	CPatch* m_pPatch;
	CEdge* m_pEdge;
	double *m_VerticesProp;
	//vector<CSolid*> m_vecComponents;
	vector<CSolid*> m_vecSelectedComponents;
	vector<CEdge*> m_vecEdges;
	CFrame* m_pFrame;
	float* m_pDepthMap;
	bool* m_pVerticesVis;
	CArcball m_Arcball;
	double m_dMatrix[16];
	double m_dScale;
	int m_iPrimitiveType;
	int m_iPrimitiveLocatingStyle;
	SilhoueteStrokeType m_sstType;
public:
	afx_msg void OnFileOpen();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnStrokeCut();
	afx_msg void OnToolRotate();
	afx_msg void OnStrokeSelect();
private:
	bool m_bAutoLocate;
	OperationMode m_omMode;
	SceneMode m_smMode;
	ObjectDisplayMode m_odmMode;
public:
	afx_msg void OnExportObjfile();
	afx_msg void OnSceneBackground();
	afx_msg void OnSceneAxis();
	afx_msg void OnSceneTprimitive();
	afx_msg void OnUpdateSceneBackground(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSceneAxis(CCmdUI *pCmdUI);
	afx_msg void OnObjectComposed();
	afx_msg void OnUpdateObjectComposed(CCmdUI *pCmdUI);
	afx_msg void OnObjectSilhouette();
	afx_msg void OnUpdateObjectSilhouette(CCmdUI *pCmdUI);
	afx_msg void OnObjectSolid();
	afx_msg void OnUpdateObjectSolid(CCmdUI *pCmdUI);
	afx_msg void OnObjectPoint();
	afx_msg void OnUpdateObjectPoint(CCmdUI *pCmdUI);
	afx_msg void OnObjectLine();
	afx_msg void OnUpdateObjectLine(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSceneTprimitive(CCmdUI *pCmdUI);
private:
	bool m_bShowTPrimitive;
public:
	afx_msg void OnStrokeSilhouette();
//	afx_msg void OnSelectEdge();
private:
	CEdge* m_pSelectedEdge;
	bool m_bRButtonDown;
public:
	afx_msg void OnToolBox();
	afx_msg void OnToolTranslate();
	afx_msg void OnSelectObject();
	afx_msg void OnToolScale();
//	afx_msg void OnLocateManual();
	afx_msg void OnTprimitiveLock();
	afx_msg void OnUpdateTprimitiveLock(CCmdUI *pCmdUI);
	afx_msg void OnSelectClear();
	afx_msg void OnPsSampling();
	afx_msg void OnStrokeSeed();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMcLewiner();
public:
	afx_msg void OnToolStop();
	afx_msg void OnToolPlay();
	afx_msg void OnZoomZoomin();
	afx_msg void OnZoomZoomout();
	afx_msg void OnAreaequalize();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnWrinkleBending();
	afx_msg void OnTypeCylinder();
	afx_msg void OnTypeSphere();
	afx_msg void OnLocateDefault();
	afx_msg void OnLocateAuto();
	afx_msg void OnLocateManual();
	afx_msg void OnUpdateTypeCylinder(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTypeSphere(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLocateManual(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLocateDefault(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLocateAuto(CCmdUI *pCmdUI);
private:
	CPlane* m_pSelectionPlane;

	CPlane* m_pProjectPlane;
	CPlane* m_pDepthPlane;
	CPlane* m_pNProjectPlane;
	CPlane* m_pNDepthPlane;
public:
	afx_msg void OnSelectRectpatch();
	int GetComponentNum(void);
	CSolid* GetComponentByIndex(int index);
	CTransitionPrimitive* GetTPrimitive(void);
	afx_msg void OnFilteringBeltrami();
	afx_msg void OnMatchSimilarity();
//	afx_msg void OnSelectVertex();
	afx_msg void OnObjectMetric();
	afx_msg void OnDetailrecoveryNonstructure();
	afx_msg void OnDetailrecoveryReconstruct();
	afx_msg void OnDetailrecoveryExtractiondetail();
	afx_msg void OnImportObjfile();
	afx_msg void OnFileSave();
	afx_msg void OnVisualizationRecparticles();
	afx_msg void OnVisualizationParticlesampling();
	afx_msg void OnUpdateVisualizationParticlesampling(CCmdUI *pCmdUI);
	afx_msg void OnVisualizationTrimmedmc();
	afx_msg void OnUpdateVisualizationTrimmedmc(CCmdUI *pCmdUI);
	afx_msg void OnWrinkleMaximumregularity();
	afx_msg void OnDeformScale();
	afx_msg void OnDeformCopy();
	afx_msg void OnSceneAntialiase();
	afx_msg void OnUpdateSceneAntialiase(CCmdUI *pCmdUI);
	int GetTComponentNum(void);
	CSolid* GetTComponentByIndex(int index);
	CSolid* GetFusionModel(void);
private:
	bool m_bPreserved;
public:
	afx_msg void OnStrokePreserve();
	afx_msg void OnUpdateStrokePreserve(CCmdUI *pCmdUI);
	afx_msg void OnSelectEdge();
private:
	bool m_bShowSilhouettePlane;
public:
	afx_msg void OnSceneSilhouetteplane();
	afx_msg void OnUpdateSceneSilhouetteplane(CCmdUI *pCmdUI);
	afx_msg void OnSelectFace();
private:
	CFace* m_pSelectedFace;
public:
	afx_msg void OnRemoveFace();
	afx_msg void OnAddFace();
private:
	CVertex* m_pSelectedVertex;
public:
	afx_msg void OnFusionRegistercomponent();
	afx_msg void OnImportTmfile();
	afx_msg void OnStrokeStructure();
public:
	afx_msg void OnToolTest();
	void UpdateSilhouettePlanes(void);
private:
	bool m_bUpdateSilhouettePlanes;
	bool m_bRBFFusion;
public:
	afx_msg void OnFtmcThroughout();
	afx_msg void OnFtmcMarching();
	afx_msg void OnFtmcWrinkle();
	afx_msg void OnFtmcRemesh();
	afx_msg void OnTprimitiveProperty();
	afx_msg void OnOmodeFusion();
	afx_msg void OnOmodeScissor();
	afx_msg void OnUpdateOmodeFusion(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOmodeScissor(CCmdUI *pCmdUI);
	afx_msg void OnSmodePart();
	afx_msg void OnUpdateSmodePart(CCmdUI *pCmdUI);
	afx_msg void OnSmodeWhole();
	afx_msg void OnUpdateSmodeWhole(CCmdUI *pCmdUI);
	afx_msg void OnConfigurationLoad();
	afx_msg void OnConfigurationSave();
	afx_msg void OnFpsSampling();
	afx_msg void OnFpsReconstruct();
	afx_msg void OnDetailrecoveryStructure();
	afx_msg void OnLocateAngle();
	afx_msg void OnReconstructionRbf();
	int GetSilhouetteNum(void);
	CStroke* GetSilhouetteByIndex(int index);
private:
	bool m_bLock;
public:
	afx_msg void OnToolLock();
	afx_msg void OnUpdateToolLock(CCmdUI *pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnStypeFull();
	afx_msg void OnUpdateStypeFull(CCmdUI *pCmdUI);
	afx_msg void OnStypeIsolate();
	afx_msg void OnUpdateStypeIsolate(CCmdUI *pCmdUI);
	afx_msg void OnStypePartial();
	afx_msg void OnUpdateStypePartial(CCmdUI *pCmdUI);
	afx_msg void OnPatchBounded();
	afx_msg void OnImportOfffile();
	afx_msg void OnSceneMagnificence();
	afx_msg void OnUpdateSceneMagnificence(CCmdUI *pCmdUI);
private:
	bool m_bMagnificence;
	bool m_bShowSilhouetteStroke;
public:
	afx_msg void OnSilhouetteShowStroke();
	afx_msg void OnUpdateSilhouetteShowStroke(CCmdUI *pCmdUI);
	afx_msg void OnCoordinateBase();
	afx_msg void OnCoordinateRaw();
	afx_msg void OnUpdateCoordinateBase(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCoordinateRaw(CCmdUI *pCmdUI);
public:
	afx_msg void OnSelectPolypatch();
	afx_msg void OnSelectVertex();
	afx_msg void OnCoordinateImport();
	afx_msg void OnCoordinateExport();
private:
	bool m_bPropagate;
public:
	afx_msg void OnCoordinateRecon();
	afx_msg void OnUpdateCoordinateRecon(CCmdUI *pCmdUI);
private:
	int m_iCoordinateShow;
public:
	afx_msg void OnSelectBoundary();
//	afx_msg void OnSelectStroke();
	afx_msg void OnSelectStroke();
	afx_msg void OnRemoveStroke();
	afx_msg void OnSilhouetteSmooth();
private:
	CPoint3* m_pCtrlPoint;
public:
	void UpdateDepthPlane(double delta);
	afx_msg void OnSilhouetteAdjust();
	afx_msg void OnToolAnimate();
	void InitSilhouettePlanes(void);
	void AdjustSilhouettePlanes(double delta,bool depth);
};

#ifndef _DEBUG  // debug version in GeometryStudioView.cpp
inline CGeometryStudioDoc* CGeometryStudioView::GetDocument() const
   { return reinterpret_cast<CGeometryStudioDoc*>(m_pDocument); }
#endif

