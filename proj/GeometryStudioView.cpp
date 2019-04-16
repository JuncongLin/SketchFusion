// GeometryStudioView.cpp : implementation of the CGeometryStudioView class
//

#include "stdafx.h"
#include "GeometryStudio.h"
#include "GeometryStudioDoc.h"
#include "GeometryStudioView.h"
#include ".\geometrystudioview.h"
#include "PrincipleAnalysis.h"
#include "Plane.h"
#include "..\PoissonShapeRepresentation\SceneCapture.h"
#include "RBFSurface.h"
#include "MainFrm.h"
#include "MarchingCubes.h"
#include <math.h>
#include <limits>
#include "Stopwatch.hpp"
#include "IntersectionTest.h"
#include "AreaEqualizationRemesh.h"
#include "TransitionSphere.h"
#include "TransitionCylinder.h"
#include "Wrinkle.h"
#include "CSObject.h"
#include "global.h"
#include "rbfsurface.h"
#include "MeshFiltering.h"
#include "nl.h"
#include <fstream>
#include "Rec.h"
#include "GeoFanNode.h"
#include "POVFileWriter.h"
#include "StrokeProcess.h"
#include "TSphereDialog.h"
#include "TCylinderDialog.h"
#include "Vector2d.h"
#include "BasicGeometryOpeartion.h"
#include "TPAngleDialog.h"
#include "ParameterizationDialog.h"
extern ofstream outfile;
using namespace std;
extern CCSObject gCriticalSection;
extern GLubyte* gpPixelBuffer;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGeometryStudioView

IMPLEMENT_DYNCREATE(CGeometryStudioView, CView)

BEGIN_MESSAGE_MAP(CGeometryStudioView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_STROKE_CUT, OnStrokeCut)
	ON_COMMAND(ID_TOOL_ROTATE, OnToolRotate)
	ON_COMMAND(ID_STROKE_SELECT, OnStrokeSelect)
	ON_COMMAND(ID_EXPORT_OBJFILE, OnExportObjfile)
	ON_COMMAND(ID_SCENE_BACKGROUND, OnSceneBackground)
	ON_COMMAND(ID_SCENE_AXIS, OnSceneAxis)
	ON_COMMAND(ID_SCENE_TPRIMITIVE, OnSceneTprimitive)
	ON_UPDATE_COMMAND_UI(ID_SCENE_BACKGROUND, OnUpdateSceneBackground)
	ON_UPDATE_COMMAND_UI(ID_SCENE_AXIS, OnUpdateSceneAxis)
	ON_COMMAND(ID_OBJECT_COMPOSED, OnObjectComposed)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_COMPOSED, OnUpdateObjectComposed)
	ON_COMMAND(ID_OBJECT_SILHOUETTE, OnObjectSilhouette)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_SILHOUETTE, OnUpdateObjectSilhouette)
	ON_COMMAND(ID_OBJECT_SOLID, OnObjectSolid)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_SOLID, OnUpdateObjectSolid)
	ON_COMMAND(ID_OBJECT_POINT, OnObjectPoint)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_POINT, OnUpdateObjectPoint)
	ON_COMMAND(ID_OBJECT_LINE, OnObjectLine)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_LINE, OnUpdateObjectLine)
	ON_UPDATE_COMMAND_UI(ID_SCENE_TPRIMITIVE, OnUpdateSceneTprimitive)
	ON_COMMAND(ID_STROKE_SILHOUETTE, OnStrokeSilhouette)
	ON_COMMAND(ID_TOOL_TRANSLATE, OnToolTranslate)
	ON_COMMAND(ID_SELECT_OBJECT, OnSelectObject)
	ON_COMMAND(ID_TPRIMITIVE_LOCK, OnTprimitiveLock)
	ON_UPDATE_COMMAND_UI(ID_TPRIMITIVE_LOCK, OnUpdateTprimitiveLock)
	ON_COMMAND(ID_SELECT_CLEAR, OnSelectClear)
	ON_COMMAND(ID_PS_SAMPLING, OnPsSampling)
	ON_COMMAND(ID_TOOLBOX, OnToolBox)
	ON_COMMAND(ID_STROKE_SEED, OnStrokeSeed)
//	ON_WM_TIMER()
	ON_COMMAND(ID_MC_LEWINER, OnMcLewiner)
	ON_COMMAND(ID_TOOL_STOP, OnToolStop)
	ON_COMMAND(ID_TOOL_PLAY, OnToolPlay)
	ON_COMMAND(ID_ZOOM_ZOOMIN, OnZoomZoomin)
	ON_COMMAND(ID_ZOOM_ZOOMOUT, OnZoomZoomout)
	ON_COMMAND(ID_AREAEQUALIZE, OnAreaequalize)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_WRINKLE_BENDING, OnWrinkleBending)
	ON_COMMAND(ID_TYPE_CYLINDER, OnTypeCylinder)
	ON_COMMAND(ID_TYPE_SPHERE, OnTypeSphere)
	ON_COMMAND(ID_LOCATE_DEFAULT, OnLocateDefault)
	ON_COMMAND(ID_LOCATE_AUTO, OnLocateAuto)
	ON_COMMAND(ID_LOCATE_MANUAL, OnLocateManual)
	ON_UPDATE_COMMAND_UI(ID_TYPE_CYLINDER, OnUpdateTypeCylinder)
	ON_UPDATE_COMMAND_UI(ID_TYPE_SPHERE, OnUpdateTypeSphere)
	ON_UPDATE_COMMAND_UI(ID_LOCATE_MANUAL, OnUpdateLocateManual)
	ON_UPDATE_COMMAND_UI(ID_LOCATE_DEFAULT, OnUpdateLocateDefault)
	ON_UPDATE_COMMAND_UI(ID_LOCATE_AUTO, OnUpdateLocateAuto)
	ON_COMMAND(ID_SELECT_RECTPATCH, OnSelectRectpatch)
	ON_COMMAND(ID_FILTERING_BELTRAMI, OnFilteringBeltrami)
	ON_COMMAND(ID_MATCH_SIMILARITY, OnMatchSimilarity)
//	ON_COMMAND(ID_SELECT_VERTEX, OnSelectVertex)
	ON_COMMAND(ID_OBJECT_METRIC, OnObjectMetric)
	ON_COMMAND(ID_DETAILRECOVERY_NONSTRUCTURE, OnDetailrecoveryNonstructure)
	ON_COMMAND(ID_DETAILRECOVERY_RECONSTRUCT, OnDetailrecoveryReconstruct)
	ON_COMMAND(ID_DETAILRECOVERY_EXTRACTIONDETAIL, OnDetailrecoveryExtractiondetail)
	ON_COMMAND(ID_IMPORT_OBJFILE, OnImportObjfile)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_VISUALIZATION_RECPARTICLES, OnVisualizationRecparticles)
	ON_COMMAND(ID_VISUALIZATION_PARTICLESAMPLING, OnVisualizationParticlesampling)
	ON_UPDATE_COMMAND_UI(ID_VISUALIZATION_PARTICLESAMPLING, OnUpdateVisualizationParticlesampling)
	ON_COMMAND(ID_VISUALIZATION_TRIMMEDMC, OnVisualizationTrimmedmc)
	ON_UPDATE_COMMAND_UI(ID_VISUALIZATION_TRIMMEDMC, OnUpdateVisualizationTrimmedmc)
	ON_COMMAND(ID_WRINKLE_MAXIMUMREGULARITY, OnWrinkleMaximumregularity)
	ON_COMMAND(ID_DEFORM_SCALE, OnDeformScale)
	ON_COMMAND(ID_DEFORM_COPY, OnDeformCopy)
	ON_COMMAND(ID_SCENE_ANTIALIASE, OnSceneAntialiase)
	ON_UPDATE_COMMAND_UI(ID_SCENE_ANTIALIASE, OnUpdateSceneAntialiase)
	ON_COMMAND(ID_STROKE_PRESERVE, OnStrokePreserve)
	ON_UPDATE_COMMAND_UI(ID_STROKE_PRESERVE, OnUpdateStrokePreserve)
	ON_COMMAND(ID_SELECT_EDGE, OnSelectEdge)
	ON_COMMAND(ID_SCENE_SILHOUETTEPLANE, OnSceneSilhouetteplane)
	ON_UPDATE_COMMAND_UI(ID_SCENE_SILHOUETTEPLANE, OnUpdateSceneSilhouetteplane)
	ON_COMMAND(ID_SELECT_FACE, OnSelectFace)
	ON_COMMAND(ID_REMOVE_FACE, OnRemoveFace)
	ON_COMMAND(ID_ADD_FACE, OnAddFace)
	ON_COMMAND(ID_FUSION_REGISTERCOMPONENT, OnFusionRegistercomponent)
	ON_COMMAND(ID_IMPORT_TMFILE, OnImportTmfile)
	ON_COMMAND(ID_STROKE_STRUCTURE, OnStrokeStructure)
	ON_COMMAND(ID_TOOL_TEST, OnToolTest)
	ON_COMMAND(ID_FTMC_THROUGHOUT, OnFtmcThroughout)
	ON_COMMAND(ID_FTMC_MARCHING, OnFtmcMarching)
	ON_COMMAND(ID_FTMC_WRINKLE, OnFtmcWrinkle)
	ON_COMMAND(ID_FTMC_REMESH, OnFtmcRemesh)
	ON_COMMAND(ID_TPRIMITIVE_PROPERTY, OnTprimitiveProperty)
	ON_COMMAND(ID_OMODE_FUSION, OnOmodeFusion)
	ON_COMMAND(ID_OMODE_SCISSOR, OnOmodeScissor)
	ON_UPDATE_COMMAND_UI(ID_OMODE_FUSION, OnUpdateOmodeFusion)
	ON_UPDATE_COMMAND_UI(ID_OMODE_SCISSOR, OnUpdateOmodeScissor)
	ON_COMMAND(ID_SMODE_PART, OnSmodePart)
	ON_UPDATE_COMMAND_UI(ID_SMODE_PART, OnUpdateSmodePart)
	ON_COMMAND(ID_SMODE_WHOLE, OnSmodeWhole)
	ON_UPDATE_COMMAND_UI(ID_SMODE_WHOLE, OnUpdateSmodeWhole)
	ON_COMMAND(ID_CONFIGURATION_LOAD, OnConfigurationLoad)
	ON_COMMAND(ID_CONFIGURATION_SAVE, OnConfigurationSave)
	ON_COMMAND(ID_FPS_SAMPLING, OnFpsSampling)
	ON_COMMAND(ID_FPS_RECONSTRUCT, OnFpsReconstruct)
	ON_COMMAND(ID_DETAILRECOVERY_STRUCTURE, OnDetailrecoveryStructure)
	ON_COMMAND(ID_LOCATE_ANGLE, OnLocateAngle)
	ON_COMMAND(ID_RECONSTRUCTION_RBF, OnReconstructionRbf)
	ON_COMMAND(ID_TOOL_LOCK, OnToolLock)
	ON_UPDATE_COMMAND_UI(ID_TOOL_LOCK, OnUpdateToolLock)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_STYPE_FULL, OnStypeFull)
	ON_UPDATE_COMMAND_UI(ID_STYPE_FULL, OnUpdateStypeFull)
	ON_COMMAND(ID_STYPE_ISOLATE, OnStypeIsolate)
	ON_UPDATE_COMMAND_UI(ID_STYPE_ISOLATE, OnUpdateStypeIsolate)
	ON_COMMAND(ID_STYPE_PARTIAL, OnStypePartial)
	ON_UPDATE_COMMAND_UI(ID_STYPE_PARTIAL, OnUpdateStypePartial)
	ON_COMMAND(ID_PATCH_BOUNDED, OnPatchBounded)
	ON_COMMAND(ID_IMPORT_OFFFILE, OnImportOfffile)
	ON_COMMAND(ID_SCENE_MAGNIFICENCE, OnSceneMagnificence)
	ON_UPDATE_COMMAND_UI(ID_SCENE_MAGNIFICENCE, OnUpdateSceneMagnificence)
	ON_COMMAND(ID_SILHOUETTE_SHOW_STROKE_, OnSilhouetteShowStroke)
	ON_UPDATE_COMMAND_UI(ID_SILHOUETTE_SHOW_STROKE_, OnUpdateSilhouetteShowStroke)
	ON_COMMAND(ID_COORDINATE_BASE, OnCoordinateBase)
	ON_COMMAND(ID_COORDINATE_RAW, OnCoordinateRaw)
	ON_UPDATE_COMMAND_UI(ID_COORDINATE_BASE, OnUpdateCoordinateBase)
	ON_UPDATE_COMMAND_UI(ID_COORDINATE_RAW, OnUpdateCoordinateRaw)
	ON_COMMAND(ID_SELECT_POLYPATCH, OnSelectPolypatch)
	ON_COMMAND(ID_SELECT_VERTEX, OnSelectVertex)
	ON_COMMAND(ID_COORDINATE_IMPORT, OnCoordinateImport)
	ON_COMMAND(ID_COORDINATE_EXPORT, OnCoordinateExport)
	ON_COMMAND(ID_COORDINATE_RECON, OnCoordinateRecon)
	ON_UPDATE_COMMAND_UI(ID_COORDINATE_RECON, OnUpdateCoordinateRecon)
	ON_COMMAND(ID_SELECT_BOUNDARY, OnSelectBoundary)
//	ON_COMMAND(ID_SELECT_STROKE, OnSelectStroke)
ON_COMMAND(ID_SELECT_STROKE, OnSelectStroke)
ON_COMMAND(ID_REMOVE_STROKE, OnRemoveStroke)
ON_COMMAND(ID_SILHOUETTE_SMOOTH, OnSilhouetteSmooth)
ON_COMMAND(ID_SILHOUETTE_ADJUST, OnSilhouetteAdjust)
ON_COMMAND(ID_TOOL_ANIMATE, OnToolAnimate)
END_MESSAGE_MAP()

// CGeometryStudioView construction/destruction

CGeometryStudioView::CGeometryStudioView()
: m_pModel(NULL)
,m_pStroke(NULL)
,m_pPatch(NULL)
,m_pVertex(NULL)
,m_pFrame(NULL)
,m_pDepthMap(NULL)
,m_pVerticesVis(NULL)
, m_bAutoLocate(true)
, m_bShowTPrimitive(false)
, m_pSelectedEdge(NULL)
, m_bRButtonDown(false)
,m_pEdge(NULL)
,m_pBoundary(NULL)
, m_pSelectionPlane(NULL)
,m_pGeoFan(NULL)
, m_bPreserved(true)
, m_bShowSilhouettePlane(false)
, m_pSelectedFace(NULL)
, m_pSelectedVertex(NULL)
, m_bUpdateSilhouettePlanes(false)
, m_bRBFFusion(false)
, m_bLock(false)
, m_bMagnificence(false)
, m_bShowSilhouetteStroke(false)
, m_bPropagate(false)
, m_iCoordinateShow(0)
, m_pCtrlPoint(NULL)
	{
	for(int i=0;i<16;i++)
	{
			if(i==0||i==5||i==10||i==15)
			{
				m_dMatrix[i]=1;
			}
			else
				m_dMatrix[i]=0;
	}
	m_odmMode=ODM_SOLID;
	m_moType=MO_NONE;
    m_iStrokeIn=-1;
	m_iStrokeOut=-1;
	m_omMode=OM_FUSION;
	m_smMode=SM_PART;
	m_dScale=1.0;
	m_iPrimitiveType=0;
	m_iFaceIndex=0;
	m_pWPatch=NULL;
	m_pWFace=NULL;
	m_pWNFace1=NULL;
	m_pWNFace2=NULL;
	m_pWNFace3=NULL;
	m_vsMode=VIS_TMC;
	m_iPTIndex=-1;
	m_pProjectPlane=NULL;
	m_pDepthPlane=NULL;
	m_pNProjectPlane=NULL;
	m_pNDepthPlane=NULL;
	m_pDepthStroke=NULL;
	m_pSilhouetteStroke=NULL;
	m_sstType=SST_FULL;
	KVIndex=0;

}

CGeometryStudioView::~CGeometryStudioView()
{
	if(m_pModel)
	{
		m_pModel->ClearMemory();
		delete m_pModel;
	}
	if(m_pPatch)
		delete m_pPatch;

		
	if(m_pDepthMap)
		delete []m_pDepthMap;


	if(gpPixelBuffer!=NULL)
		delete []gpPixelBuffer;
	if(m_pGeoFan!=NULL)
		delete m_pGeoFan;
}

BOOL CGeometryStudioView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGeometryStudioView drawing

void CGeometryStudioView::OnDraw(CDC* /*pDC*/)
{
	CGeometryStudioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_OpenGLManager.BeginDraw();

	switch(m_omMode)
	{
	case OM_SCISSOR:	
		{
			if(m_pModel)
				m_pModel->Display(m_odmMode);
			if(m_pPatch)
				m_pPatch->Display(m_odmMode);
			if(m_pStroke)
				m_pStroke->Display();
			if(m_pSelectedEdge)
				m_pSelectedEdge->Display();
			if(m_pSelectedFace)
				m_pSelectedFace->Display();
			if(m_pSelectedVertex)
				m_pSelectedVertex->Display();
			if(m_pGeoFan)
				m_pGeoFan->Display();
			for (int i=0;i<m_vecTriVertices.size();i++)
			{
				m_vecTriVertices[i]->Display();
			}
			//glColor3f(0.8,0.8,0.8);
			glPolygonMode(GL_FRONT,GL_LINES);
			if(m_pEdge)
			{
				glLineWidth(3.0);
				glColor3f(1.0,0.0,0.0);
				glBegin(GL_LINES);
				glVertex3dv(m_pEdge->GetVertex1()->GetCoordinate().GetArray());
				glVertex3dv(m_pEdge->GetVertex2()->GetCoordinate().GetArray());
				glEnd();
				glLineWidth(1.0);
				glPointSize(4.0);
				glBegin(GL_POINTS);
				glColor3f(0.0,1.0,0.0);
				glVertex3dv(m_pEdge->GetVertex1()->GetCoordinate().GetArray());
				glColor3f(1.0,1.0,0.0);
				glVertex3dv(m_pEdge->GetVertex2()->GetCoordinate().GetArray());
				glColor3f(0.0,0.0,1.0);
				glVertex3dv(m_pEdge->GetHe1()->GetPrevEdge()->GetVertex()->GetCoordinate().GetArray());
				glEnd();
				glPointSize(1.0);
			}

		}		
		break;

	case OM_FUSION:
		{
			switch(m_smMode)
			{
			case SM_PART:
				{
					//mc.rend();
					if(m_pPatch)
						m_pPatch->Display(ODM_SOLID);

					//if(m_pGeoFan)
					//	m_pGeoFan->Display();


					if(m_bShowSilhouettePlane)
					{
						if(m_bUpdateSilhouettePlanes)
							UpdateSilhouettePlanes();
						m_pProjectPlane->Display();
						m_pDepthPlane->Display();
						m_pNProjectPlane->Display(true);
						m_pNDepthPlane->Display(true);

						int i;

						glEnable(GL_LINE_SMOOTH);
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
						glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
						glLineWidth(1.5f);

						m_pDepthStroke->Display();
						m_pSilhouetteStroke->Display();

						glDepthRange(0.0,0.99997);
						glLineWidth(2.0);
						glDisable(GL_LIGHTING);
						glColor3f(0.0,0.0,0.0);
						glBegin(GL_LINE_STRIP);
						for (i=0;i<m_vecCtrlPolygon.size();i++)
							glVertex3dv(m_vecCtrlPolygon[i]->GetCoordinate().GetArray());
						glEnd();
						glDepthRange(0.0,1.0);
						glLineWidth(1.0);
						glEnable(GL_LIGHTING);
						for (i=0;i<m_vecCtrlPolygon.size();i++)
							m_vecCtrlPolygon[i]->Display();

						glDisable(GL_LINE_SMOOTH);
						glDisable(GL_BLEND);
						glLineWidth(1.0f);

					}


					if(m_pSelectionPlane!=NULL)
						m_pSelectionPlane->Display(true);


					{
						for (int i=0;i<m_FSNManager.GetStructureNum();i++)
						{
							m_FSNManager.GetStructureByIndex(i)->Display();
						}
						//if(m_bPropagate)
						//{
						//	int i;

						//	glEnable(GL_COLOR_MATERIAL);
						//	glColor3f(0.0,1.0,0.0);
						//	for (i=0;i<UKVs.size();i++)
						//	{
						//		CVector3d pt1=m_pStroke->GetPoint(UKVs[i]);
						//		glPushMatrix ();
						//		glTranslated(pt1[0],pt1[1],pt1[2]);
						//		gluSphere(gluNewQuadric(),0.005,50,50);
						//		glPopMatrix();
						//	}
						//	glColor3f(0.0,0.0,1.0);
						//	for (i=0;i<KVs.size();i++)
						//	{
						//		CVector3d pt1=m_pStroke->GetPoint(KVs[i]);
						//		glPushMatrix ();
						//		glTranslated(pt1[0],pt1[1],pt1[2]);
						//		gluSphere(gluNewQuadric(),0.005,50,50);
						//		glPopMatrix();
						//	}

						//	glColor3f(1.0,1.0,0.0);
						//	for (i=0;i<2;i++)
						//	{
						//		CVector3d pt1=m_pStroke->GetPoint(criticalPts[i]);
						//		glPushMatrix ();
						//		glTranslated(pt1[0],pt1[1],pt1[2]);
						//		gluSphere(gluNewQuadric(),0.005,50,50);
						//		glPopMatrix();

						//	}

						//	//m_pDPatch->Display();

						//	//glColor3f(1.0,0.0,0.0);
						//	//glPushMatrix ();
						//	//glTranslated(cn1[0],cn1[1],cn1[2]);
						//	//gluSphere(gluNewQuadric(),0.01,50,50);
						//	//glPopMatrix();

						//	//glColor3f(1.0,1.0,0.0);
						//	//glPushMatrix ();
						//	//glTranslated(cn2[0],cn2[1],cn2[2]);
						//	//gluSphere(gluNewQuadric(),0.01,50,50);
						//	//glPopMatrix();

						//	//glColor3f(0.0,1.0,0.0);
						//	//glPushMatrix ();
						//	//glTranslated(cn3[0],cn3[1],cn3[2]);
						//	//gluSphere(gluNewQuadric(),0.01,50,50);
						//	//glPopMatrix();

						//	//glColor3f(0.0,1.0,1.0);
						//	//glPushMatrix ();
						//	//glTranslated(cn4[0],cn4[1],cn4[2]);
						//	//gluSphere(gluNewQuadric(),0.01,50,50);
						//	//glPopMatrix();
						//	//glDisable(GL_COLOR_MATERIAL);

						//}

					}

					//if(m_pVertex)
					//	m_pVertex->Display();
					unsigned int i,j;
					CSolid *pComponent;
					for(i=0;i<m_FSNManager.GetComponentNum();i++)
					{
						pComponent=m_FSNManager.GetComponentByIndex(i);
						if(pComponent->GetSelected())
						{
							if(m_FSNManager.GetTPrimitive()&&m_FSNManager.GetTPrimitive()->IsLocked())
							{
								glPushMatrix();
								glMultMatrixd(m_dMatrix);
								pComponent->Display(m_odmMode);
								glPopMatrix();
							}
							else
							{
								glPushMatrix();
								m_Arcball.glDraw();
								glMultMatrixd(m_dMatrix);
								pComponent->Display(m_odmMode);
								glPopMatrix();
							}
						}
						else
							pComponent->Display(m_odmMode);

					}
					for(i=0;i<m_FSNManager.GetTComponentNum();i++)
					{
						pComponent=m_FSNManager.GetTComponentByIndex(i);
						pComponent->Display(m_odmMode);
						for (j=0;j<pComponent->GetVertexNum();j++)
						{
							CVertex *pV=pComponent->GetVertexByIndex(j);
							if(pV->IsKnown()&&!pV->IsBoundary())
								pV->Display();
						}
					}
					if(m_pStroke)
						m_pStroke->Display();

					if(m_bShowSilhouetteStroke)
					{
						for (i=0;i<m_FSNManager.GetSilhouetteNum();i++)
							m_FSNManager.GetSilhouetteByIndex(i)->Display();
					}

					if(m_bShowTPrimitive)
						m_FSNManager.GetTPrimitive()->Display();

				//	if(m_FSNManager.GetPS())
				//		m_FSNManager.GetPS()->Display();			
				//
				}
				//mc.rend();
				break;
			case SM_WHOLE:
				{
					if(m_FSNManager.GetFusionModel())
					{
						if(m_bLock)
						{
							glPushMatrix();
							m_Arcball.glDraw();
							m_FSNManager.GetFusionModel()->Display(m_odmMode);
							glPopMatrix();
						}
						else
                            m_FSNManager.GetFusionModel()->Display(m_odmMode);
					}
				}
				break;
			}
		}
		break;
	default:
		break;
	}


	m_OpenGLManager.EndDraw();
	//gCriticalSection.Enter();
	//m_OpenGLManager.GetPixelData(gpPixelBuffer,0,0);
	//gCriticalSection.Leave();
}


// CGeometryStudioView printing

void CGeometryStudioView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CGeometryStudioView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGeometryStudioView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGeometryStudioView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGeometryStudioView diagnostics

#ifdef _DEBUG
void CGeometryStudioView::AssertValid() const
{
	CView::AssertValid();
}

void CGeometryStudioView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGeometryStudioDoc* CGeometryStudioView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGeometryStudioDoc)));
	return (CGeometryStudioDoc*)m_pDocument;
}
#endif //_DEBUG


// CGeometryStudioView message handlers

void CGeometryStudioView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	m_OpenGLManager.InitCamera((float)cx,(float)cy);

	if(m_pDepthMap)
		delete []m_pDepthMap;

	if(gpPixelBuffer!=NULL)
		delete []gpPixelBuffer;
	gpPixelBuffer=new GLubyte[cx*cy*3];
}

int CGeometryStudioView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	HWND hWnd = GetSafeHwnd();

	CRect rect;
	GetClientRect(&rect);

	m_OpenGLManager.SetDC(::GetDC(hWnd));

	if(m_OpenGLManager.Initialize((float)rect.Width(),(float)rect.Height())==FALSE)
		return -1;



	return 0;
}

void CGeometryStudioView::OnFileOpen()
{

	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"FSN Files (*.FSN)|*.FSN|SIR Files (*.SIR)|*.SIR||", this);


	int structsize=0; 
	DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion; 
	//检测目前的操作系统，GetVersion具体用法详见MSDN 
	dwVersion = GetVersion(); 
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion))); 
	dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion))); 
	// 如果运行的操作系统是Windows NT/2000 
	if (dwVersion < 0x80000000)  
		structsize =88;//显示新的文件对话框 
	else 
		//运行的操作系统Windows 95/98  
		structsize =76;//显示老的文件对话框 
	fileDlg.m_ofn.lStructSize=structsize; 

	if(fileDlg.DoModal()==IDOK){
		CString ext=fileDlg.GetFileExt();
		CString filePath = fileDlg.GetPathName();
		if(ext=="fsn")
		{
			m_FSNManager.Read(filePath.GetBuffer(20));

			m_OpenGLManager.BeginContext();

			int i;
			for (i=0;i<m_FSNManager.GetComponentNum();i++)
			{
				m_FSNManager.GetComponentByIndex(i)->BuildList();
			}
			for (i=0;i<m_FSNManager.GetTComponentNum();i++)
			{
				m_FSNManager.GetTComponentByIndex(i)->BuildList();
			}
			if(m_FSNManager.GetFusionModel())
				m_FSNManager.GetFusionModel()->BuildList();
			m_OpenGLManager.EndContext();
			m_omMode=OM_FUSION;

		}
		else
		{
			m_SIRManager.Read(filePath.GetBuffer(20));


			m_OpenGLManager.BeginContext();
			m_SIRManager.GetModel()->BuildList();
			m_OpenGLManager.EndContext();
			m_pModel=m_SIRManager.GetModel();
			m_omMode=OM_SCISSOR;

		}
		InvalidateRect(NULL,FALSE);
	}	

}

void CGeometryStudioView::OnLButtonDown(UINT nFlags, CPoint point)
{
	switch(m_moType)
	{
	case MO_ROTATE:
		switch(m_omMode)
		{
		case OM_SCISSOR:
			{
				if(m_bLock)
				{
					m_Arcball.SetMode(1);
					CVector3d vec = m_Arcball.Intersect(point.x,m_OpenGLManager.GetViewport().yRes()-point.y,m_OpenGLManager.GetCamera(),m_OpenGLManager.GetViewport());
					m_Arcball.BeginDrag(vec);
				}
				else
				{
					m_OpenGLManager.HandleMouseButtonDown(m_moType,m_bRButtonDown,point);
				}

			}
			break;
		case OM_FUSION:
			{
				switch(m_smMode)
				{
				case SM_WHOLE:
					{
						if(m_bLock)
						{
							m_Arcball.SetMode(1);
							CVector3d vec = m_Arcball.Intersect(point.x,m_OpenGLManager.GetViewport().yRes()-point.y,m_OpenGLManager.GetCamera(),m_OpenGLManager.GetViewport());
							m_Arcball.BeginDrag(vec);
						}
						else
						{
							m_OpenGLManager.HandleMouseButtonDown(m_moType,m_bRButtonDown,point);
						}

					}
					break;
				case SM_PART:
					{
						if(m_vecSelectedComponents.size()!=0)
						{
							if(m_FSNManager.GetTPrimitive()&&m_FSNManager.GetTPrimitive()->IsLocked())
							{
								m_PrevMousePos=point;
								for(int i=0;i<16;i++)
								{
									if(i==0||i==5||i==10||i==15)
									{
										m_dMatrix[i]=1;
									}
									else
										m_dMatrix[i]=0;
								}
							}
							else
							{
								m_Arcball.SetMode(1);
								CVector3d vec = m_Arcball.Intersect(point.x,m_OpenGLManager.GetViewport().yRes()-point.y,m_OpenGLManager.GetCamera(),m_OpenGLManager.GetViewport());
								m_Arcball.BeginDrag(vec);
							}
						}
						else
							m_OpenGLManager.HandleMouseButtonDown(m_moType,m_bRButtonDown,point);

					}
					break;
				}
			}
			break;
		}
		SetCapture();
		break;
	case MO_TRANSLATE:
		if(m_vecSelectedComponents.size()!=0)
		{
			if(m_FSNManager.GetTPrimitive()&&m_FSNManager.GetTPrimitive()->IsLocked())
			{
				m_PrevMousePos=point;
				for(int i=0;i<16;i++)
				{
					if(i==0||i==5||i==10||i==15)
					{
						m_dMatrix[i]=1;
					}
					else
						m_dMatrix[i]=0;
				}

			}
			else
			{
				if(!m_bRButtonDown)
					m_Arcball.SetMode(2);
				else 
					m_Arcball.SetMode(3);
				CVector3d vec = m_Arcball.Intersect(point.x,m_OpenGLManager.GetViewport().yRes()-point.y,m_OpenGLManager.GetCamera(),m_OpenGLManager.GetViewport());
				m_Arcball.BeginDrag(vec);
			}
		}
		else
            m_OpenGLManager.HandleMouseButtonDown(m_moType,false,point);
		SetCapture();
		break;
	case MO_SCALE:
		if(m_vecSelectedComponents.size())
		{
			if(m_vecSelectedComponents.size()!=0)
			{
				if(m_FSNManager.GetTPrimitive()->IsLocked())
				{
					m_PrevMousePos=point;
					for(int i=0;i<16;i++)
					{
						if(i==0||i==5||i==10||i==15)
						{
							m_dMatrix[i]=1;
						}
						else
							m_dMatrix[i]=0;
					}
				}
			}
			m_PrevMousePos=point;
			SetCapture();
		}
		break;

	case MO_STROKE_CUT:
		{
			SetCapture();
			int Dimx,Dimy;
			double obj[3];
			m_pStroke=new CStroke;
			m_vecStrokePoints2D.clear();
			m_OpenGLManager.GetDimension(Dimx,Dimy);
			m_pDepthMap=new float[Dimx*Dimy];
			for(int i=0;i<Dimx*Dimy;i++)
				m_pDepthMap[i]=1.0;
			m_OpenGLManager.GetDepth(m_pDepthMap,0,0,Dimx,Dimy);
			m_PrevMousePos.SetPoint(point.x,Dimy-point.y-1);
			m_vecStrokePoints2D.push_back(m_PrevMousePos);
			float depth=1.0;      
			m_OpenGLManager.GetDepth(&depth,m_PrevMousePos.x,m_PrevMousePos.y);
			depth-=0.1;
			m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,m_PrevMousePos.y,depth,obj);
			m_pStroke->AddPoint(obj[0],obj[1],obj[2]);
			InvalidateRect(NULL,FALSE);

		}
		break;
	case MO_STROKE_SEED:
		{
			SetCapture();
			int Dimx,Dimy;
			double obj[3];
			m_pStroke=new CStroke;
			m_OpenGLManager.GetDimension(Dimx,Dimy);
			m_PrevMousePos.SetPoint(point.x,Dimy-point.y-1);
			m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,m_PrevMousePos.y,0.2,obj);
			m_pStroke->AddPoint(obj[0],obj[1],obj[2]);
			InvalidateRect(NULL,FALSE);

		}
		break;
	case MO_STROKE_SELECT:
		{
			SetCapture();
			int Dimx,Dimy;
			double obj[3];
			m_pStroke=new CStroke;
			m_OpenGLManager.GetDimension(Dimx,Dimy);
			m_PrevMousePos.SetPoint(point.x,Dimy-point.y-1);
			float depth=1.0;      
			m_OpenGLManager.GetDepth(&depth,m_PrevMousePos.x,m_PrevMousePos.y);
			depth-=0.1;
			m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,m_PrevMousePos.y,depth,obj);
			m_pStroke->AddPoint(obj[0],obj[1],obj[2]);
			InvalidateRect(NULL,FALSE);
			

		}
		break;
	case MO_STROKE_SILHOUETTE:
		{
			SetCapture();

			int Dimx,Dimy;
			double obj[3];
			float depth=1.0;
			m_pStroke=new CStroke;
			m_vecStrokePoints2D.clear();
			m_OpenGLManager.GetDimension(Dimx,Dimy);
			m_PrevMousePos.SetPoint(point.x,Dimy-point.y-1);
			m_vecStrokePoints2D.push_back(m_PrevMousePos);
			m_OpenGLManager.GetDepth(&depth,m_PrevMousePos.x,m_PrevMousePos.y);

			depth-=0.1;

			m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,m_PrevMousePos.y,depth,obj);
			m_pStroke->AddPoint(obj[0],obj[1],obj[2]);
			switch(m_sstType)
			{
			case SST_FULL:
				{
					if(m_pDepthPlane!=NULL)
					{
						delete m_pDepthPlane;
						m_pDepthPlane=NULL;
					}
					if(m_pProjectPlane!=NULL)
					{
						delete m_pProjectPlane;
						m_pProjectPlane=NULL;
					}
					if(m_pNDepthPlane!=NULL)
					{
						delete m_pNDepthPlane;
						m_pNDepthPlane=NULL;
					}
					if(m_pNProjectPlane!=NULL)
					{
						delete m_pNProjectPlane;
						m_pNProjectPlane=NULL;
					}

					if(m_pDepthStroke!=NULL)
					{
						delete m_pDepthStroke;
						m_pDepthStroke=NULL;
					}
					if(m_pSilhouetteStroke!=NULL)
					{
						delete m_pSilhouetteStroke;
						m_pSilhouetteStroke=NULL;
					}
					m_bShowSilhouettePlane=false;

				}
				break;
			case SST_PARTIAL:
				{

				}
				break;
			case SST_ISOLATE:
				{

				}
				break;

			}


			InvalidateRect(NULL,FALSE);
		}
		break;
	case MO_STROKE_STRUCTURE:
		{
			SetCapture();
			int Dimx,Dimy;
			double obj[3];
			m_pStroke=new CStroke;
			m_OpenGLManager.GetDimension(Dimx,Dimy);
			m_PrevMousePos.SetPoint(point.x,Dimy-point.y-1);
			m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,m_PrevMousePos.y,obj);
			m_pStroke->AddPoint(obj[0],obj[1],obj[2]);
			InvalidateRect(NULL,FALSE);

		}
		break;
	case MO_SELECT_EDGE:
		{
			CVector3d coord1=m_pSelectedEdge->GetVertex1()->GetCoordinate();
			CVector3d coord2=m_pSelectedEdge->GetVertex2()->GetCoordinate();

			CVector3d point=0.5*(coord1+coord2);
			CVector3d normal=coord1-coord2;
			normal.Normalize();

			CPlane *section=new CPlane(point,normal);
			m_pModel->CutByPlane(section,m_pSelectedEdge);
			delete section;

			m_OpenGLManager.BeginContext();
			m_pModel->BuildList();
			m_OpenGLManager.EndContext();

		}
		m_moType=MO_NONE;
		InvalidateRect(NULL,FALSE);
		break;
	case MO_ADD_FACE:
		if(m_pSelectedVertex!=NULL)
		{
			m_vecTriVertices.push_back(m_pSelectedVertex);
			m_pSelectedVertex=NULL;
		}
		break;


	case MO_SELECT_VERTEX:
		m_moType=MO_NONE;
		break;
	case MO_SELECT_STROKE:
	case MO_SELECT_BOUNDARY:
	case MO_SELECT_FACE:
		m_moType=MO_NONE;
		break;
	case MO_SELECT_OBJECT:
		{
			CSolid *pComponent;
			int i,id;

			m_OpenGLManager.BeginDraw(1,point);
			glInitNames();
			glPushName(0);
			for(i=0;i<m_FSNManager.GetComponentNum();i++)
			{
				glLoadName(i);
				pComponent=m_FSNManager.GetComponentByIndex(i);
				pComponent->Display(ODM_SOLID);
			}
			id=m_OpenGLManager.EndDraw(1);
			if(id!=-1)
				pComponent=m_FSNManager.GetComponentByIndex(id);
			else 
				pComponent=NULL;


			if(pComponent&&!pComponent->GetSelected())
			{
				pComponent->SetSelected(true);
				m_vecSelectedComponents.push_back (pComponent);				
			}
			InvalidateRect(NULL,FALSE);
			break;


		}
	case MO_SELECT_RECTPATCH:
		{
			int x,y, Dimx,Dimy;
			double obj[3];
			m_OpenGLManager.GetDimension(Dimx,Dimy);
			x=point.x;
			y=Dimy-point.y;
			m_OpenGLManager.GetOBJCoord(x,y,0.1,obj);

			m_pSelectionPlane=new CPlane(CVector3d(obj),CVector3d(m_OpenGLManager.GetViewDirection()));

			m_pSelectionPlane->AddCorner((CVector3d(obj)));
			m_pSelectionPlane->AddCorner((CVector3d(obj)));
			m_pSelectionPlane->AddCorner((CVector3d(obj)));
			m_pSelectionPlane->AddCorner((CVector3d(obj)));
			m_PrevMousePos=point;
			SetCapture();
		}
		break;
	case MO_TPRIMITIVE_LOCATE:
		m_moType=MO_NONE;
		m_omMode=OM_SCISSOR;
		InvalidateRect(NULL,FALSE);
		break;
	case MO_SELECT_POLYPATCH:
		{
			int x,y, Dimx,Dimy;
			double obj[3];
			m_OpenGLManager.GetDimension(Dimx,Dimy);
			x=point.x;
			y=Dimy-point.y;
			m_OpenGLManager.GetOBJCoord(x,y,0.1,obj);

			if(m_pSelectionPlane==NULL)
			{
				m_pSelectionPlane=new CPlane(CVector3d(obj),CVector3d(m_OpenGLManager.GetViewDirection()));
				m_pSelectionPlane->SetColor(1.0,1.0,0.0);
			}

			m_pSelectionPlane->AddCorner((CVector3d(obj)));

			InvalidateRect(NULL,FALSE);
		}
		break;
	case MO_SILHOUETTE_ADJUST:
		{
			m_PrevMousePos=point;
			SetCapture();

		}
		break;
	case MO_SILHOUETTE_SMOOTH:
		{
			m_PrevMousePos=point;
			SetCapture();
		}
		

	}

	CView::OnLButtonDown(nFlags, point);
}
void CGeometryStudioView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_bRButtonDown=true;
	switch(m_moType)
	{
	case MO_ADD_FACE:
		{
			if(m_vecTriVertices.size()==3)
			{
				m_pModel->AddFace(m_vecTriVertices);
			}
			for (int i=0;i<m_vecTriVertices.size();i++)
			{
				m_vecTriVertices[i]->SetSelected(false);
			}
			m_vecTriVertices.clear();
			m_OpenGLManager.BeginContext();
			m_pModel->BuildList();
			m_OpenGLManager.EndContext();
		}
		break;
	case MO_SELECT_POLYPATCH:
		{
			double out[3];
			CVertex *pSeedV;
			CVector2d centroid;
			CVector3d coord,corner;
			vector<CVector2d*> vecSelectPolygon;
			int i;
			centroid.Set(0.0,0.0);
			for (i=0;i<m_pSelectionPlane->GetCornerNum();i++)
			{
				corner=m_pSelectionPlane->GetCorner(i);
				m_OpenGLManager.GetWCCoord(corner.GetArray(),out);
				CVector2d *pVec=new CVector2d(out[0],out[1]);
				vecSelectPolygon.push_back(pVec);
				centroid+=*pVec;
			}
			int dimx,dimy;
			m_OpenGLManager.GetDimension(dimx,dimy);
			centroid/=m_pSelectionPlane->GetCornerNum();
			m_OpenGLManager.GetOBJCoord(centroid[0],centroid[1],m_pDepthMap[int(centroid[1]*dimx+centroid[0])],out);
			coord.Set(out);
			double dst,mdst=DBL_MAX;
			for (i=0;i<m_FSNManager.GetFusionModel()->GetVertexNum();i++)
			{
				dst=(m_FSNManager.GetFusionModel()->GetVertexByIndex(i)->GetCoordinate()-coord).Length();
				if(dst<mdst)
				{
					mdst=dst;
					pSeedV=m_FSNManager.GetFusionModel()->GetVertexByIndex(i);
				}
			}
			m_pVertex=pSeedV;
			m_pPatch=new CPatch;
			m_pPatch->ClusteringPolygon(pSeedV,vecSelectPolygon,m_OpenGLManager);

			for (i=0;i<vecSelectPolygon.size();i++)
			{
				delete vecSelectPolygon[i];
			}

			delete m_pSelectionPlane;
			m_pSelectionPlane=NULL;

			InvalidateRect(NULL,FALSE);
		}
		break;
	}
	InvalidateRect(NULL,FALSE);

	CView::OnRButtonDown(nFlags, point);
}

void CGeometryStudioView::OnLButtonUp(UINT nFlags, CPoint point)
{
	switch(m_moType)
	{
	case MO_ROTATE:
		switch(m_omMode)
		{
		case OM_SCISSOR:
			{
				if(m_bLock)
				{

					unsigned int i;
					CVector3d vec = m_Arcball.Intersect(point.x,m_OpenGLManager.GetViewport().yRes()-point.y,m_OpenGLManager.GetCamera(),m_OpenGLManager.GetViewport());
					m_Arcball.EndDrag(vec);
					CSolid *pComponent;
					CMatrix44 mat=m_Arcball.GetMatrix();
					m_pModel->Transform(mat);
	
					m_OpenGLManager.BeginContext();
					m_pModel->BuildList();
					m_OpenGLManager.EndContext();
					m_Arcball.Clear();

				}
				else
				{
					m_OpenGLManager.HandleMouseButtonUp(m_moType,m_bRButtonDown,point);
					if(m_odmMode==ODM_SILHOUETTE)
					{
						int i,dimx,dimy;
						m_OpenGLManager.GetDimension(dimx,dimy);
						if(m_pDepthMap==NULL)
							m_pDepthMap=new float[dimx*dimy];
						for(i=0;i<dimx*dimy;i++)
							m_pDepthMap[i]=1.0;

						m_OpenGLManager.BeginDraw();
						m_pModel->Display(ODM_SOLID);
						m_OpenGLManager.EndDraw();

						m_OpenGLManager.GetDepth(m_pDepthMap,0,0,dimx,dimy);
						m_pModel->UpdateContour(m_OpenGLManager,m_pDepthMap);
					}

				}

			}
			break;
		case OM_FUSION:
			{
				switch(m_smMode)
				{
				case SM_PART:
					{
						if(m_vecSelectedComponents.size()!=0)
						{
							if(m_FSNManager.GetTPrimitive()&&m_FSNManager.GetTPrimitive()->IsLocked())
							{

								CMatrix44 mat,tmat;
								mat.Set(m_dMatrix);
								tmat=mat.Transpose();
								CSolid *pComponent=m_vecSelectedComponents.at(0);
								pComponent->Transform(tmat);
								m_OpenGLManager.BeginContext();
								pComponent->BuildList();
								m_OpenGLManager.EndContext();

								for(int i=0;i<16;i++)
								{
									if(i==0||i==5||i==10||i==15)
									{
										m_dMatrix[i]=1;
									}
									else
										m_dMatrix[i]=0;
								}

							}
							else
							{


								unsigned int i;
								CVector3d vec = m_Arcball.Intersect(point.x,m_OpenGLManager.GetViewport().yRes()-point.y,m_OpenGLManager.GetCamera(),m_OpenGLManager.GetViewport());
								m_Arcball.EndDrag(vec);
								CSolid *pComponent;
								CMatrix44 mat=m_Arcball.GetMatrix();
								CMatrix44 tmat=mat.Transpose();
								for(i=0;i<m_vecSelectedComponents.size();i++)
								{
									pComponent=m_vecSelectedComponents.at(i);
									pComponent->Transform(mat);
								}
								m_OpenGLManager.BeginContext();
								for(i=0;i<m_vecSelectedComponents.size();i++)
								{
									pComponent=m_vecSelectedComponents.at(i);
									pComponent->BuildList();
								}
								m_OpenGLManager.EndContext();
								m_Arcball.Clear();

							}
						}
						else
						{
							m_OpenGLManager.HandleMouseButtonUp(m_moType,m_bRButtonDown,point);
							if(m_odmMode==ODM_SILHOUETTE)
							{
								int i,dimx,dimy;
								m_OpenGLManager.GetDimension(dimx,dimy);
								if(m_pDepthMap==NULL)
									m_pDepthMap=new float[dimx*dimy];
								for(i=0;i<dimx*dimy;i++)
									m_pDepthMap[i]=1.0;
								m_OpenGLManager.BeginDraw();
								switch(m_omMode)
								{
								case OM_SCISSOR:
									if(m_pModel)
										m_pModel->Display(ODM_SOLID);
									break;

								case OM_FUSION:
									{
										CSolid *pComponent;
										int i;
										for(i=0;i<m_FSNManager.GetComponentNum();i++)
										{
											pComponent=m_FSNManager.GetComponentByIndex(i);
											pComponent->Display(ODM_SOLID);
										}				
									}
									break;
								default:
									break;
								}
								m_OpenGLManager.EndDraw();
								m_OpenGLManager.GetDepth(m_pDepthMap,0,0,dimx,dimy);
								switch(m_omMode)
								{
								case OM_SCISSOR:
									if(m_pModel)
										m_pModel->UpdateContour(m_OpenGLManager,m_pDepthMap);
									break;

								case OM_FUSION:
									{
										CSolid *pComponent;
										unsigned int i;
										for(i=0;i<m_FSNManager.GetComponentNum();i++)
										{
											pComponent=m_FSNManager.GetComponentByIndex(i);
											pComponent->UpdateContour(m_OpenGLManager,m_pDepthMap);
										}				
									}
									break;
								default:
									break;
								}
							}
						}

					}
					break;
				case SM_WHOLE:
					{
						if(m_bLock)
						{

							unsigned int i;
							CVector3d vec = m_Arcball.Intersect(point.x,m_OpenGLManager.GetViewport().yRes()-point.y,m_OpenGLManager.GetCamera(),m_OpenGLManager.GetViewport());
							m_Arcball.EndDrag(vec);
							CSolid *pComponent;
							CMatrix44 mat=m_Arcball.GetMatrix();
							CMatrix44 tmat=mat.Transpose();
							m_FSNManager.GetFusionModel()->Transform(mat);
							for (i=0;i<m_FSNManager.GetSilhouetteNum();i++)
							{
								m_FSNManager.GetSilhouetteByIndex(i)->Transform(mat);
							}

							m_OpenGLManager.BeginContext();
							m_FSNManager.GetFusionModel()->BuildList();
							for(i=0;i<m_FSNManager.GetComponentNum();i++)
							{
								pComponent=m_FSNManager.GetComponentByIndex(i);
								pComponent->BuildList();
							}
							for (i=0;i<m_FSNManager.GetTComponentNum();i++)
							{
								pComponent=m_FSNManager.GetTComponentByIndex(i);
								pComponent->BuildList();
							}
							m_OpenGLManager.EndContext();
							m_Arcball.Clear();

						}
						else
						{
							m_OpenGLManager.HandleMouseButtonUp(m_moType,m_bRButtonDown,point);
							if(m_odmMode==ODM_SILHOUETTE)
							{
								int i,dimx,dimy;
								m_OpenGLManager.GetDimension(dimx,dimy);
								if(m_pDepthMap==NULL)
									m_pDepthMap=new float[dimx*dimy];
								for(i=0;i<dimx*dimy;i++)
									m_pDepthMap[i]=1.0;

								m_OpenGLManager.BeginDraw();
								m_FSNManager.GetFusionModel()->Display(ODM_SOLID);
								m_OpenGLManager.EndDraw();

								m_OpenGLManager.GetDepth(m_pDepthMap,0,0,dimx,dimy);
								m_FSNManager.GetFusionModel()->UpdateContour(m_OpenGLManager,m_pDepthMap);
							}

						}
					}
					break;
				}

			}
			break;
		}
		InvalidateRect(NULL,FALSE);
		ReleaseCapture();
		break;
	case MO_TRANSLATE:
		if(m_vecSelectedComponents.size()!=0)
		{
			if(!m_FSNManager.GetTPrimitive()->IsLocked())
			{
				unsigned int i;
				CVector3d vec = m_Arcball.Intersect(point.x,m_OpenGLManager.GetViewport().yRes()-point.y,m_OpenGLManager.GetCamera(),m_OpenGLManager.GetViewport());
				m_Arcball.EndDrag(vec);
				CSolid *pComponent;
				CMatrix44 mat=m_Arcball.GetMatrix();
				for(i=0;i<m_vecSelectedComponents.size();i++)
				{
					pComponent=m_vecSelectedComponents.at(i);
					pComponent->Transform(mat);
				}
				m_OpenGLManager.BeginContext();
				for(i=0;i<m_vecSelectedComponents.size();i++)
				{
					pComponent=m_vecSelectedComponents.at(i);
					pComponent->BuildList();
				}
				m_OpenGLManager.EndContext();
				m_Arcball.Clear();
			}
			else
			{
				CMatrix44 mat,tmat;
				mat.Set(m_dMatrix);
				tmat=mat.Transpose();
				CSolid *pComponent=m_vecSelectedComponents.at(0);
				pComponent->Transform(tmat);
				m_OpenGLManager.BeginContext();
				pComponent->BuildList();
				m_OpenGLManager.EndContext();

				CVector3d loc=m_FSNManager.GetTPrimitive()->GetCurLocator();
				m_FSNManager.GetTPrimitive()->SetLocator(pComponent->GetIndex(),loc);

				for(int i=0;i<16;i++)
				{
					if(i==0||i==5||i==10||i==15)
					{
						m_dMatrix[i]=1;
					}
					else
						m_dMatrix[i]=0;
				}

				InvalidateRect(NULL,FALSE);

			}


		}
		else
		{
            m_OpenGLManager.HandleMouseButtonUp(m_moType,false,point);
			if(m_odmMode==ODM_SILHOUETTE)
			{
				int i,dimx,dimy;
				m_OpenGLManager.GetDimension(dimx,dimy);
				if(m_pDepthMap==NULL)
					m_pDepthMap=new float[dimx*dimy];
				for(i=0;i<dimx*dimy;i++)
					m_pDepthMap[i]=1.0;
				m_OpenGLManager.BeginDraw();
				switch(m_omMode)
				{
				case OM_SCISSOR:
					if(m_pModel)
							m_pModel->Display(ODM_SOLID);
					break;

				case OM_FUSION:
					{
						CSolid *pComponent;
						int i;
						for(i=0;i<m_FSNManager.GetComponentNum();i++)
						{
							pComponent=m_FSNManager.GetComponentByIndex(i);
							pComponent->Display(ODM_SOLID);
						}				
					}
					break;
				default:
					break;
				}
				m_OpenGLManager.EndDraw();
				m_OpenGLManager.GetDepth(m_pDepthMap,0,0,dimx,dimy);
				switch(m_omMode)
				{
				case OM_SCISSOR:
					if(m_pModel)
						m_pModel->UpdateContour(m_OpenGLManager,m_pDepthMap);
					break;

				case OM_FUSION:
					{
						CSolid *pComponent;
						int i,j;
						for(i=0;i<m_FSNManager.GetComponentNum();i++)
						{
							pComponent=m_FSNManager.GetComponentByIndex(i);
							pComponent->UpdateContour(m_OpenGLManager,m_pDepthMap);
						}	

						CStroke *pStroke;
						CVector3d n1,n2;
						CVector3d view=m_OpenGLManager.GetViewDirection();
						for (i=0;i<m_FSNManager.GetSilhouetteNum();i++)
						{
							pStroke=m_FSNManager.GetSilhouetteByIndex(i);
							pStroke->ClearCriticalPoints();
							for (j=0;j<pStroke->GetPointsNum()-1;j++)
							{
								n1=pStroke->GetNormal(j);
								n2=pStroke->GetNormal(j+1);
								if(((n1.Dot(view))*(n2.Dot(view)))<0.0001)	
								{
									pStroke->AddCriticalPoint(j);
								}
							}
						}


					}
					break;
				default:
					break;
				}
			}
		}
		InvalidateRect(NULL,FALSE);
		ReleaseCapture();
		break;
	case MO_SCALE:
		{
			if(m_vecSelectedComponents.size())
			{
				CMatrix44 mat,tmat;
				mat.Set(m_dMatrix);
				tmat=mat.Transpose();

				unsigned int i;
				CSolid *pComponent;
				for(i=0;i<m_vecSelectedComponents.size();i++)
				{
					pComponent=m_vecSelectedComponents.at(i);
					pComponent->Transform(tmat);
				}
				m_OpenGLManager.BeginContext();
				for(i=0;i<m_vecSelectedComponents.size();i++)
				{
					pComponent=m_vecSelectedComponents.at(i);
					pComponent->BuildList();
				}
				m_OpenGLManager.EndContext();

				for(int i=0;i<16;i++)
				{
					if(i==0||i==5||i==10||i==15)
					{
						m_dMatrix[i]=1;
					}
					else
						m_dMatrix[i]=0;
				}
			}
		}
		InvalidateRect(NULL,FALSE);
		ReleaseCapture();
		break;

	case MO_STROKE_CUT:
		{
			m_pPatch=new CPatch;
			CVertex *pVS,*pV,*pVE;
			CVector3d sp;
			double dst,mindst;
			double oout[3];
			int i,Dimx,Dimy;
			m_OpenGLManager.GetDimension(Dimx,Dimy);

			double *iVerticesDst=new double[m_pModel->GetVertexNum()];


			CVector3d view=m_OpenGLManager.GetViewDirection();

			CPoint pt1,pt2;
			float x,y;
			pt1=m_vecStrokePoints2D.at(m_iStrokeIn);
			pt2=m_vecStrokePoints2D.at(m_iStrokeIn-1);

			float r=(float)(pt2.y-pt1.y)/(float)(pt2.x-pt1.x);

			x=pt1.x;
			y=pt1.y;
			do
			{

				x-=1;
				y-=r;

			}while(m_pDepthMap[((int)y)*Dimx+(int)x]<0.999999&&x>pt2.x);
			
			x+=1;
			y+=r;
			
			m_OpenGLManager.GetOBJCoord(x,y,m_pDepthMap[((int)y)*Dimx+(int)x],oout);
			sp.Set(oout);
			pVS=NULL;


			mindst=1000;

			for(i=0;i<m_pModel->GetVertexNum();i++)
			{
				iVerticesDst[i]=-1.0;
				pV=m_pModel->GetVertexByIndex(i);

				dst=((pV->GetCoordinate()-sp).Length());
				if(dst<mindst)
				{
					mindst=dst;
					pVS=pV;
				}
			}
			outfile<<"1"<<endl;
			if(!m_pPatch->ClusteringAround2DStroke(m_OpenGLManager,pVS,iVerticesDst,m_vecStrokePoints2D))
			{
				CString str;
				str.Format("Scissoring Error! Try again!");
				CMainFrame *pFrameWnd = (CMainFrame*)AfxGetMainWnd();
				pFrameWnd->m_wndOutput.FillOperationInfo(str);

				delete m_pStroke;
				m_pStroke=NULL;

				delete m_pPatch;
				m_pPatch=NULL;

				delete []iVerticesDst;

				m_iStrokeIn=-1;
				m_iStrokeOut=-1;
				InvalidateRect(NULL,FALSE);
				ReleaseCapture();
				break;

			}

			outfile<<"2"<<endl;

			double out[3];
			for(i=0;i<m_pModel->GetVertexNum();i++)
				(m_pModel->GetVertexByIndex(i))->SetVisible(true);

			for(i=0;i<m_pPatch->GetVertexNum();i++)
			{
				pV=m_pPatch->GetVertexByIndex(i);
				m_OpenGLManager.GetWCCoord(pV->GetCoordinate().GetArray(),out);
				if(out[2]>m_pDepthMap[((int)out[1])*Dimx+(int)out[0]]+0.0001)
					pV->SetVisible(false);
			}

			pVE=NULL;

			pt1=m_vecStrokePoints2D.at(m_iStrokeOut);
			pt2=m_vecStrokePoints2D.at(m_iStrokeOut+1);

			r=(float)(pt2.y-pt1.y)/(float)(pt2.x-pt1.x);

			x=pt1.x;
			y=pt1.y;
			do
			{

				x+=1;
				y+=r;

			}while(m_pDepthMap[((int)y)*Dimx+(int)x]<0.99&&x<pt2.x);
			
			x-=1;
			y-=r;

			m_OpenGLManager.GetOBJCoord(x,y,m_pDepthMap[((int)y)*Dimx+(int)x],oout);
			sp.Set(oout);


			mindst=1000;

			for(i=0;i<m_pPatch->GetVertexNum();i++)
			{
				pV=m_pPatch->GetVertexByIndex(i);
				dst=(pV->GetCoordinate()-sp).Length();
				if(dst<mindst)
				{
					mindst=dst;
					pVE=pV;
				}
			}
			CBoundary *pBoundary=new CBoundary;
			outfile<<"3"<<endl;
			if(!m_pPatch->ScissorByDijkstra(m_OpenGLManager,pVS,pVE,iVerticesDst,m_pModel->GetVertexNum(),pBoundary)||!pBoundary->IsLegal())
			{
				CString str;
				str.Format("Scissoring Error! Try again!");
				CMainFrame *pFrameWnd = (CMainFrame*)AfxGetMainWnd();
				pFrameWnd->m_wndOutput.FillOperationInfo(str);

				delete pBoundary;

				m_pPatch->ClearFlag();
				delete m_pPatch;
				m_pPatch=NULL;		
			}
			else
			{
				pBoundary->CollectEdges();
				m_pModel->RegisterBoundary(pBoundary);

				m_pPatch->ClearFlag();
				delete m_pPatch;
				m_pPatch=NULL;		

				m_pModel->Split(pBoundary);
				m_OpenGLManager.BeginContext();
				m_pModel->BuildList();
				m_OpenGLManager.EndContext();


				//m_pBoundary=pBoundary;
			}
			outfile<<"4"<<endl;


			delete m_pStroke;
			m_pStroke=NULL;

			delete []iVerticesDst;
			m_iStrokeIn=-1;
			m_iStrokeOut=-1;
			m_moType=MO_NONE;


			InvalidateRect(NULL,FALSE);
			ReleaseCapture();
		}
		break;
	case MO_STROKE_SEED:
		{
			ReleaseCapture();
			m_moType=MO_NONE;
		}
		break;
	case MO_STROKE_STRUCTURE:
		{

			m_FSNManager.RegisterStructure(m_pStroke);
			m_pStroke=NULL;

			ReleaseCapture();
			m_moType=MO_NONE;
			InvalidateRect(NULL,FALSE);

		}
		break;
	case MO_STROKE_SELECT:
		{

			//Stopwatch time;
			//CString str;
			double dst,mindst;
			CVector3d sp;
			CVertex *pV,*pSV;
			m_pPatch=new CPatch;
			int i,k;


			vector<CVertex*> vecSeeds;

			sp=m_pStroke->GetPoint(0);

			double out[3],oout[3];

			m_OpenGLManager.GetWCCoord(sp.GetArray(),out);
			out[2]+=0.1;
			m_OpenGLManager.GetOBJCoord(out[0],out[1],out[2],oout);
			sp.Set(oout);


            
			//time.Start();
			mindst=1000;
			for(i=0;i<m_pModel->GetVertexNum();i++)
			{
				pV=m_pModel->GetVertexByIndex(i);

				dst=((pV->GetCoordinate()-sp).Length());
				if(dst<mindst)
				{
					mindst=dst;
					pSV=pV;
				}
			}

			vecSeeds.push_back(pSV);
			delete m_pStroke;
			m_pStroke=NULL;

		
			m_pPatch->ClusteringBoundedwithSeed(vecSeeds);

			delete m_pStroke;
			m_pStroke=NULL;

			InvalidateRect(NULL,FALSE);
			ReleaseCapture();
		}
		break;
	case MO_STROKE_SILHOUETTE:
		{
			switch(m_sstType)
			{
			case SST_FULL:
				{
					CSolid *pComponent;
					CBoundary *pBoundary;
					CVertex *pVertex,*pBSV,*pBEV;
					CPoint se,ss,pt;
					double dx,dy,dst,out[3],msdst=10000,medst=10000;
					unsigned int i,j,k;

					ss=m_vecStrokePoints2D.at(0);
					se=m_vecStrokePoints2D.at(m_vecStrokePoints2D.size()-1);

					for(i=0;i<m_FSNManager.GetComponentNum();i++)
					{
						pComponent=m_FSNManager.GetComponentByIndex(i);
						for(j=0;j<pComponent->GetBoundaryNum();j++)
						{
							pBoundary=pComponent->GetBoundaryByIndex(j);
							for(k=0;k<pBoundary->GetCriticalVerticeNum();k++)
							{
								pVertex=pBoundary->GetCriticalVerticeByIndex(k);
								m_OpenGLManager.GetWCCoord(pVertex->GetCoordinate().GetArray(),out);
								dx=out[0]-ss.x;
								dy=out[1]-ss.y;
								dst=sqrt(dx*dx+dy*dy);
								if(dst<msdst)
								{
									msdst=dst;
									pBSV=pVertex;
								}

								dx=out[0]-se.x;
								dy=out[1]-se.y;
								dst=sqrt(dx*dx+dy*dy);
								if(dst<medst)
								{
									medst=dst;
									pBEV=pVertex;
								}


							}

						}

					}

					CVector3d v,v1,v2,vm,vse,vpn,vdn,vEyePos,raw,proj,tang,vdir,norm;
					vEyePos=m_OpenGLManager.GetEyePos();
					v1=pBSV->GetCoordinate()-vEyePos;
					v1.Normalize();

					v2=pBEV->GetCoordinate()-vEyePos;
					v2.Normalize();

					vdn=v1.Cross(v2);
					vdn*=(vdn.Dot(pBSV->GetNormal()));
					vdn.Normalize();

					vse=pBEV->GetCoordinate()-pBSV->GetCoordinate();
					dst=0.5*vse.Length();
					vse.Normalize();

					vpn=vdn.Cross(vse);
					vpn.Normalize();

					vm= (pBSV->GetCoordinate()+pBEV->GetCoordinate())/2;

					if(m_pProjectPlane!=NULL)
						delete m_pProjectPlane;
					if(m_pDepthPlane!=NULL)
						delete m_pDepthPlane;

					m_pProjectPlane=new CPlane(vm,vpn);

					raw=pBSV->GetCoordinate()-dst*vdn;
					m_pProjectPlane->AddCorner(raw);

					raw=pBEV->GetCoordinate()-dst*vdn;
					m_pProjectPlane->AddCorner(raw);

					raw=pBEV->GetCoordinate()+dst*vdn;
					m_pProjectPlane->AddCorner(raw);

					raw=pBSV->GetCoordinate()+dst*vdn;
					m_pProjectPlane->AddCorner(raw);


					m_pDepthPlane=new CPlane(vm,vdn);

					raw=pBSV->GetCoordinate()-dst*vpn;
					m_pDepthPlane->AddCorner(raw);

					raw=pBEV->GetCoordinate()-dst*vpn;
					m_pDepthPlane->AddCorner(raw);


					raw=pBEV->GetCoordinate()+dst*vpn;
					m_pDepthPlane->AddCorner(raw);

					raw=pBSV->GetCoordinate()+dst*vpn;
					m_pDepthPlane->AddCorner(raw);

					m_pDepthPlane->SetColor(0.0,1.0,0.0);



					for(i=0;i<m_pStroke->GetPointsNum();i++)
					{
						raw=m_pStroke->GetPoint(i);
						double t;
						t=(m_pProjectPlane->d()+m_pProjectPlane->a()*vEyePos[0]+m_pProjectPlane->b()*vEyePos[1]+m_pProjectPlane->c()*vEyePos[2])/(m_pProjectPlane->a()*(vEyePos[0]-raw[0])+m_pProjectPlane->b()*(vEyePos[1]-raw[1])+m_pProjectPlane->c()*(vEyePos[2]-raw[2]));
						proj=(1-t)*vEyePos+t*raw;
						m_pStroke->ModifyPoint(i,proj);
					}

					InitSilhouettePlanes();

					raw=m_pStroke->GetPoint(0);
					tang=(m_pStroke->GetPoint(1)-m_pStroke->GetPoint(0));
					tang.Normalize();
					vdir=raw-vEyePos;
					vdir.Normalize();
					norm=tang.Cross(vdir);
					norm*=norm.Dot(raw-m_FSNManager.GetTPrimitive()->GetCenter());
					norm.Normalize();
					m_pStroke->AddNormal(norm);
					for (i=1;i<m_pStroke->GetPointsNum()-1;i++)
					{
						raw=m_pStroke->GetPoint(i);
						tang=(m_pStroke->GetPoint(i+1)-m_pStroke->GetPoint(i-1));
						tang.Normalize();
						vdir=raw-vEyePos;
						vdir.Normalize();
						norm=tang.Cross(vdir);
						norm*=norm.Dot(raw-m_FSNManager.GetTPrimitive()->GetCenter());
						norm.Normalize();
						m_pStroke->AddNormal(norm);
					}
					raw=m_pStroke->GetPoint(i);
					tang=(m_pStroke->GetPoint(0)-m_pStroke->GetPoint(i-1));
					tang.Normalize();
					norm=tang.Cross(vdir);
					norm*=norm.Dot(raw-m_FSNManager.GetTPrimitive()->GetCenter());
					norm.Normalize();
					m_pStroke->AddNormal(norm);

					m_FSNManager.RegisterSilhouette(m_pStroke);

					m_vecStrokePoints2D.clear();

					m_bShowSilhouettePlane=true;
				}
				break;
			case SST_PARTIAL:
				{				

					CSolid *pComponent;
					CBoundary *pBoundary;
					CStroke* pStroke;
					unsigned int i,j,k;
					CPoint se,ss,pt;
					double dx,dy,dst,out[3],mdst;
					CVertex *pVertex,*pBSV;
				


					ss=m_vecStrokePoints2D.at(0);
					se=m_vecStrokePoints2D.at(m_vecStrokePoints2D.size()-1);

					mdst=DBL_MAX;
					for(i=0;i<m_FSNManager.GetComponentNum();i++)
					{
						pComponent=m_FSNManager.GetComponentByIndex(i);
						for(j=0;j<pComponent->GetBoundaryNum();j++)
						{
							pBoundary=pComponent->GetBoundaryByIndex(j);
							for(k=0;k<pBoundary->GetCriticalVerticeNum();k++)
							{
								pVertex=pBoundary->GetCriticalVerticeByIndex(k);
								m_OpenGLManager.GetWCCoord(pVertex->GetCoordinate().GetArray(),out);
								dx=out[0]-ss.x;
								dy=out[1]-ss.y;
								dst=sqrt(dx*dx+dy*dy);
								if(dst<mdst)
								{
									mdst=dst;
									pBSV=pVertex;
								}
							}

						}

					}

					CVector3d SSE;
					mdst=DBL_MAX;
					for (i=0;i<m_FSNManager.GetSilhouetteNum();i++)
					{
						pStroke=m_FSNManager.GetSilhouetteByIndex(i);
						for (j=0;j<pStroke->GetCriticalPointNum();j++)
						{
							m_OpenGLManager.GetWCCoord(pStroke->GetCriticalPointByIndex(j).GetArray(),out);

							dx=out[0]-se.x;
							dy=out[1]-se.y;
							dst=sqrt(dx*dx+dy*dy);
							if(dst<mdst)
							{
								mdst=dst;
								SSE=pStroke->GetCriticalPointByIndex(j);
							}
						}
					}


					CVector3d v,v1,v2,vm,vse,vpn,vdn,vEyePos,raw,proj,tang,vdir,norm,eye;
					vEyePos=m_OpenGLManager.GetEyePos();
					v1=pBSV->GetCoordinate()-vEyePos;
					v1.Normalize();

					v2=SSE-vEyePos;
					v2.Normalize();

					vdn=v1.Cross(v2);
					vdn*=(vdn.Dot(pBSV->GetNormal()));
					vdn.Normalize();

					vse=SSE-pBSV->GetCoordinate();
					dst=0.5*vse.Length();
					vse.Normalize();

					vpn=vdn.Cross(vse);
					vpn.Normalize();

					vm= (pBSV->GetCoordinate()+SSE)/2;

					if(m_pProjectPlane!=NULL)
						delete m_pProjectPlane;

					m_pProjectPlane=new CPlane(vm,vpn);

					eye=m_OpenGLManager.GetEyePos();

					for(i=0;i<m_pStroke->GetPointsNum();i++)
					{
						raw=m_pStroke->GetPoint(i);
						double t;
						t=(m_pProjectPlane->d()+m_pProjectPlane->a()*eye[0]+m_pProjectPlane->b()*eye[1]+m_pProjectPlane->c()*eye[2])/(m_pProjectPlane->a()*(eye[0]-raw[0])+m_pProjectPlane->b()*(eye[1]-raw[1])+m_pProjectPlane->c()*(eye[2]-raw[2]));
						proj=(1-t)*eye+t*raw;
						m_pStroke->ModifyPoint(i,proj);
					}

					m_pStroke->Optimization();

					raw=m_pStroke->GetPoint(0);
					tang=(m_pStroke->GetPoint(1)-m_pStroke->GetPoint(0));
					tang.Normalize();
					vdir=raw-vEyePos;
					vdir.Normalize();
					norm=tang.Cross(vdir);
					norm*=norm.Dot(pBSV->GetNormal());
					norm.Normalize();
					m_pStroke->AddNormal(norm);
					for (i=1;i<m_pStroke->GetPointsNum()-1;i++)
					{
						raw=m_pStroke->GetPoint(i);
						tang=(m_pStroke->GetPoint(i+1)-m_pStroke->GetPoint(i-1));
						tang.Normalize();
						vdir=raw-vEyePos;
						vdir.Normalize();
						norm=tang.Cross(vdir);
						norm*=norm.Dot(pBSV->GetNormal());
						norm.Normalize();
						m_pStroke->AddNormal(norm);
					}
					raw=m_pStroke->GetPoint(i);
					tang=(m_pStroke->GetPoint(0)-m_pStroke->GetPoint(i-1));
					tang.Normalize();
					norm=tang.Cross(vdir);
					norm*=norm.Dot(pBSV->GetNormal());
					norm.Normalize();
					m_pStroke->AddNormal(norm);

					m_FSNManager.RegisterSilhouette(m_pStroke);

					m_vecStrokePoints2D.clear();


				}
				break;
			case SST_ISOLATE:
				{
					CVector3d vm,vpn,eye,raw,proj,norm,tang,centroid;
					int i;

					if(m_pProjectPlane!=NULL)
						delete m_pProjectPlane;

					eye=m_OpenGLManager.GetEyePos();

					vm=m_FSNManager.GetTPrimitive()->GetCenter();
					vpn=eye-vm;
					vpn.Normalize();

					m_pProjectPlane=new CPlane(vm,vpn);

					for(i=0;i<m_pStroke->GetPointsNum();i++)
					{
						raw=m_pStroke->GetPoint(i);
						double t;
						t=(m_pProjectPlane->d()+m_pProjectPlane->a()*eye[0]+m_pProjectPlane->b()*eye[1]+m_pProjectPlane->c()*eye[2])/(m_pProjectPlane->a()*(eye[0]-raw[0])+m_pProjectPlane->b()*(eye[1]-raw[1])+m_pProjectPlane->c()*(eye[2]-raw[2]));
						proj=(1-t)*eye+t*raw;
						m_pStroke->ModifyPoint(i,proj);
					}

					m_pStroke->Optimization();

					centroid.Set(0.0,0.0,0.0);
					for (i=0;i<m_pStroke->GetPointsNum();i++)
					{
                        centroid+=m_pStroke->GetPoint(i);                        
					}
					centroid/=m_pStroke->GetPointsNum();

					raw=m_pStroke->GetPoint(0);
					tang=(m_pStroke->GetPoint(1)-m_pStroke->GetPoint(m_pStroke->GetPointsNum()-1));
					tang.Normalize();
					norm=tang.Cross(vpn);
					norm*=norm.Dot(centroid-raw);
					norm.Normalize();
					m_pStroke->AddNormal(norm);
					for (i=1;i<m_pStroke->GetPointsNum()-1;i++)
					{
						raw=m_pStroke->GetPoint(i);
						tang=(m_pStroke->GetPoint(i+1)-m_pStroke->GetPoint(i-1));
						tang.Normalize();
						norm=tang.Cross(vpn);
						norm*=norm.Dot(centroid-raw);
						norm.Normalize();
						m_pStroke->AddNormal(norm);
					}
					raw=m_pStroke->GetPoint(i);
					tang=(m_pStroke->GetPoint(0)-m_pStroke->GetPoint(i-1));
					tang.Normalize();
					norm=tang.Cross(vpn);
					norm*=norm.Dot(centroid-raw);
					norm.Normalize();
					m_pStroke->AddNormal(norm);

					m_FSNManager.RegisterSilhouette(m_pStroke);

					m_vecStrokePoints2D.clear();

				}
				break;
			}

			ReleaseCapture();
			InvalidateRect(NULL,FALSE);

		}
		break;
	case MO_SELECT_RECTPATCH:
		{
			CVector3d corner,c1,c2,c3,c4,cnt;
			double out[3];
			CVertex* pSeedV,*pV;


			corner=m_pSelectionPlane->GetCorner(0);
			m_OpenGLManager.GetWCCoord(corner.GetArray(),out);
			c1.Set(out);

			corner=m_pSelectionPlane->GetCorner(1);
			m_OpenGLManager.GetWCCoord(corner.GetArray(),out);
			c2.Set(out);

			corner=m_pSelectionPlane->GetCorner(2);
			m_OpenGLManager.GetWCCoord(corner.GetArray(),out);
			c3.Set(out);

			corner=m_pSelectionPlane->GetCorner(3);
			m_OpenGLManager.GetWCCoord(corner.GetArray(),out);
			c4.Set(out);

			cnt=(c1+c2+c3+c4)/4;
			m_OpenGLManager.GetOBJCoord(cnt[0],cnt[1],out);
			cnt.Set(out);

			CSolid *pFusionModel=m_FSNManager.GetFusionModel();

			double diff,mindiff=DBL_MAX;
			for (int i=0;i<pFusionModel->GetVertexNum();i++)
			{
				pV=pFusionModel->GetVertexByIndex(i);
				diff=(pV->GetCoordinate()-cnt).Length();
                if(diff<mindiff)
				{
					pSeedV=pV;
					mindiff=diff;
				}
			}

			//m_pVertex=pSeedV;

			m_pPatch=new CPatch;
			m_pPatch->ClusteringRectangle(pSeedV,c1,c2,c3,c4,m_OpenGLManager);

			delete m_pSelectionPlane;
			m_pSelectionPlane=NULL;
			m_moType=MO_NONE;
			InvalidateRect(NULL,FALSE);
			ReleaseCapture();
		}
		break;
	case MO_SILHOUETTE_ADJUST:
	case MO_SILHOUETTE_SMOOTH:
		{
			m_moType=MO_NONE;
			InvalidateRect(NULL,FALSE);
			ReleaseCapture();
		}
		break;
	}
	CView::OnLButtonUp(nFlags, point);
}

void CGeometryStudioView::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_bRButtonDown=false;
	CView::OnRButtonUp(nFlags, point);
}

void CGeometryStudioView::OnMouseMove(UINT nFlags, CPoint point)
{
	switch(m_moType)
	{
	case MO_ROTATE:
		if(GetCapture()==this)
		{
			switch(m_omMode)
			{
			case OM_SCISSOR:
				{
					if(m_bLock)
					{
						CVector3d vec = m_Arcball.Intersect(point.x,m_OpenGLManager.GetViewport().yRes()-point.y,m_OpenGLManager.GetCamera(),m_OpenGLManager.GetViewport());
						m_Arcball.Motion(vec);

					}
					else
					{
						m_OpenGLManager.HandleMouseMove(m_moType,m_bRButtonDown,point);
					}

				}
				break;
			case OM_FUSION:
				{
					switch(m_smMode)
					{
					case SM_PART:
						{
							if(m_vecSelectedComponents.size()!=0)
							{
								if(m_FSNManager.GetTPrimitive()!=NULL&&m_FSNManager.GetTPrimitive()->IsLocked())
								{
									double ang=(point.x-m_PrevMousePos.x)/fabs((double)(point.x-m_PrevMousePos.x))*sqrt((double)((point.x-m_PrevMousePos.x)*(point.x-m_PrevMousePos.x)+(point.y-m_PrevMousePos.y)*(point.y-m_PrevMousePos.y)))/50;

									CVector3d cnt=m_FSNManager.GetTPrimitive()->GetCenter();
									CSolid *pComponent=m_vecSelectedComponents.at(0);
									CVector3d  loc=m_FSNManager.GetTPrimitive()->GetLocatorByIndex(pComponent->GetIndex());

									CVector3d rotaxis=m_FSNManager.GetTPrimitive()->GetLocDirectionByIndex(pComponent->GetIndex());
									rotaxis.Normalize();

									CVector3d rao1=Ortho3(rotaxis);
									rao1.Normalize();

									CVector3d rao2=rotaxis.Cross(rao1);
									rao2.Normalize();

									double data[16];
									CMatrix44 g2lmat,l2gmat,rotmat,transmat1,transmat2,compmat,accumat,tmpmat;

									data[0]=rotaxis[0];data[1]=rotaxis[1];data[2]=rotaxis[2];data[3]=0;
									data[4]=rao1[0];data[5]=rao1[1];data[6]=rao1[2];data[7]=0;
									data[8]=rao2[0];data[9]=rao2[1];data[10]=rao2[2];data[11]=0;
									data[12]=0;data[13]=0;data[14]=0;data[15]=1;
									g2lmat.Set(data);

									data[0]=1;data[1]=0;data[2]=0;data[3]=0;
									data[4]=0;data[5]=cos(ang);data[6]=-sin(ang);data[7]=0;
									data[8]=0;data[9]=sin(ang);data[10]=cos(ang);data[11]=0;
									data[12]=0;data[13]=0;data[14]=0;data[15]=1;
									rotmat.Set(data);

									data[0]=rotaxis[0];data[1]=rao1[0];data[2]=rao2[0];data[3]=0;
									data[4]=rotaxis[1];data[5]=rao1[1];data[6]=rao2[1];data[7]=0;
									data[8]=rotaxis[2];data[9]=rao1[2];data[10]=rao2[2];data[11]=0;
									data[12]=0;data[13]=0;data[14]=0;data[15]=1;
									l2gmat.Set(data);

									data[0]=1;data[1]=0;data[2]=0;data[3]=-loc.x();
									data[4]=0;data[5]=1;data[6]=0;data[7]=-loc.y();
									data[8]=0;data[9]=0;data[10]=1;data[11]=-loc.z();
									data[12]=0;data[13]=0;data[14]=0;data[15]=1;
									transmat1.Set(data);

									data[0]=1;data[1]=0;data[2]=0;data[3]=loc.x();
									data[4]=0;data[5]=1;data[6]=0;data[7]=loc.y();
									data[8]=0;data[9]=0;data[10]=1;data[11]=loc.z();
									data[12]=0;data[13]=0;data[14]=0;data[15]=1;
									transmat2.Set(data);

									compmat=transmat2.MultRight(l2gmat.MultRight(rotmat.MultRight(g2lmat.MultRight(transmat1))));
									//				compmat=transmat2.MultRight(l2gmat.MultRight(g2lmat.MultRight(transmat1)));

									double* matdata=compmat.GetData();
									for(int i=0;i<4;i++)
										for(int j=0;j<4;j++)
											m_dMatrix[j*4+i]=matdata[i*4+j];

								}
								else
								{
									CVector3d vec = m_Arcball.Intersect(point.x,m_OpenGLManager.GetViewport().yRes()-point.y,m_OpenGLManager.GetCamera(),m_OpenGLManager.GetViewport());
									m_Arcball.Motion(vec);
								}
							}
							else
							{
								m_OpenGLManager.HandleMouseMove(m_moType,m_bRButtonDown,point);
								m_bUpdateSilhouettePlanes=true;
							}
						}
						break;
					case SM_WHOLE:
						{
							if(m_bLock)
							{
								CVector3d vec = m_Arcball.Intersect(point.x,m_OpenGLManager.GetViewport().yRes()-point.y,m_OpenGLManager.GetCamera(),m_OpenGLManager.GetViewport());
								m_Arcball.Motion(vec);

							}
							else
							{
								m_OpenGLManager.HandleMouseMove(m_moType,m_bRButtonDown,point);
							}
						}
						break;
					}

				}
				break;
			}
		}
		InvalidateRect(NULL,FALSE);
		break;
	case MO_TRANSLATE:
		if(GetCapture()==this)
		{
			if(m_vecSelectedComponents.size()!=0)
			{
				if(m_FSNManager.GetTPrimitive()&&m_FSNManager.GetTPrimitive()->IsLocked())
				{
					CSolid *pComponent=m_vecSelectedComponents[0];
					CVector3d cb=m_FSNManager.GetTPrimitive()->GetLocatorByIndex(pComponent->GetIndex());
					CVector3d n=m_FSNManager.GetTPrimitive()->GetLocDirectionByIndex(pComponent->GetIndex());

					outfile<<"m1"<<endl;


					CMatrix44 transmat,p2gmat,g2cmat,compmat,tmpmat;
					double data[16];

					double wcout[3],objout1[3],objout2[3];

					m_OpenGLManager.GetWCCoord(cb.GetArray(),wcout);

					double dx=(point.x-m_PrevMousePos.x)/20.0;
					double dy=(point.y-m_PrevMousePos.y)/20.0;

					m_OpenGLManager.GetOBJCoord(wcout[0]+dx,wcout[1]-dy,0.1,objout1);
					m_OpenGLManager.GetOBJCoord(wcout[0]+dx,wcout[1]-dy,0.5,objout2);

					CRay3 ray(CPoint3(objout1[0],objout1[1],objout1[2]),CPoint3(objout2[0],objout2[1],objout2[2]));
					m_FSNManager.GetTPrimitive()->SetManualLocator(ray);  //??p


					CVector3d loc=m_FSNManager.GetTPrimitive()->GetCurLocator();

					CVector3d ltc=m_FSNManager.GetTPrimitive()->GetCurDirection();
					ltc.Normalize();

					CVector3d ltco1=Ortho3(ltc);
					ltco1.Normalize();
					CVector3d ltco2=ltc.Cross(ltco1);
					ltco2.Normalize();

					data[0]=ltc[0];data[1]=ltco1[0];data[2]=ltco2[0];data[3]=0;
					data[4]=ltc[1];data[5]=ltco1[1];data[6]=ltco2[1];data[7]=0;
					data[8]=ltc[2];data[9]=ltco1[2];data[10]=ltco2[2];data[11]=0;
					data[12]=0;data[13]=0;data[14]=0;data[15]=1;
					p2gmat.Set(data);

					CVector3d no1=Ortho3(n);
					no1.Normalize();
					CVector3d no2=n.Cross(no1);
					no2.Normalize();

					data[0]=n[0];data[1]=n[1];data[2]=n[2];data[3]=0;
					data[4]=no1[0];data[5]=no1[1];data[6]=no1[2];data[7]=0;
					data[8]=no2[0];data[9]=no2[1];data[10]=no2[2];data[11]=0;
					data[12]=0;data[13]=0;data[14]=0;data[15]=1;
					g2cmat.Set(data);

					data[0]=1;data[1]=0;data[2]=0;data[3]=-cb.x();
					data[4]=0;data[5]=1;data[6]=0;data[7]=-cb.y();
					data[8]=0;data[9]=0;data[10]=1;data[11]=-cb.z();
					data[12]=0;data[13]=0;data[14]=0;data[15]=1;
					transmat.Set(data);
					compmat=p2gmat.MultRight(g2cmat);
					tmpmat=compmat.MultRight(transmat);

					data[0]=1;data[1]=0;data[2]=0;data[3]=loc.x();
					data[4]=0;data[5]=1;data[6]=0;data[7]=loc.y();
					data[8]=0;data[9]=0;data[10]=1;data[11]=loc.z();
					data[12]=0;data[13]=0;data[14]=0;data[15]=1;
					transmat.Set(data);

					compmat=transmat.MultRight(tmpmat);

					outfile<<"m2"<<endl;

					double* matdata=compmat.GetData();

					for(int i=0;i<4;i++)
					{
						for(int j=0;j<4;j++)
						{
							m_dMatrix[j*4+i]=matdata[i*4+j];
						}
					}
					outfile<<"m3"<<endl;
					InvalidateRect(NULL,FALSE);
				}
				else
				{
					CVector3d vec = m_Arcball.Intersect(point.x,m_OpenGLManager.GetViewport().yRes()-point.y,m_OpenGLManager.GetCamera(),m_OpenGLManager.GetViewport());
					m_Arcball.Motion(vec);
				}

			}
			else
			{
                m_OpenGLManager.HandleMouseMove(m_moType,m_bRButtonDown,point);
				m_bUpdateSilhouettePlanes=true;
			}
			InvalidateRect(NULL,FALSE);
		}
		break;
	case MO_SCALE:
		if(GetCapture()==this)
		{
			if(m_vecSelectedComponents.size())
			{
				double data[16];
				m_dScale=1.0+(point.x-m_PrevMousePos.x)/fabs((double)(point.x-m_PrevMousePos.x))*sqrt((double)((point.x-m_PrevMousePos.x)*(point.x-m_PrevMousePos.x)+(point.y-m_PrevMousePos.y)*(point.y-m_PrevMousePos.y)))/50;
				if(m_FSNManager.GetTPrimitive()->IsLocked())
				{
					CSolid *pComponent=m_vecSelectedComponents[0];

					CVector3d cb=m_FSNManager.GetTPrimitive()->GetLocatorByIndex(pComponent->GetIndex());

					CMatrix44 transmat,scalemat,tmpmat,compmat;


					data[0]=1;data[1]=0;data[2]=0;data[3]=-cb.x();
					data[4]=0;data[5]=1;data[6]=0;data[7]=-cb.y();
					data[8]=0;data[9]=0;data[10]=1;data[11]=-cb.z();
					data[12]=0;data[13]=0;data[14]=0;data[15]=1;
					transmat.Set(data);

					data[0]=m_dScale;data[1]=0;data[2]=0;data[3]=0;
					data[4]=0;data[5]=m_dScale;data[6]=0;data[7]=0;
					data[8]=0;data[9]=0;data[10]=m_dScale;data[11]=0;
					data[12]=0;data[13]=0;data[14]=0;data[15]=1;

					scalemat.Set(data);

					tmpmat=scalemat.MultRight(transmat);


					data[0]=1;data[1]=0;data[2]=0;data[3]=cb.x();
					data[4]=0;data[5]=1;data[6]=0;data[7]=cb.y();
					data[8]=0;data[9]=0;data[10]=1;data[11]=cb.z();
					data[12]=0;data[13]=0;data[14]=0;data[15]=1;
					transmat.Set(data);

					compmat=transmat.MultRight(tmpmat);

					double* matdata=compmat.GetData();

					for(int i=0;i<4;i++)
						for(int j=0;j<4;j++)
							m_dMatrix[j*4+i]=matdata[i*4+j];
				}
				else
				{
					CMatrix44 compmat;
					data[0]=m_dScale;data[1]=0;data[2]=0;data[3]=0;
					data[4]=0;data[5]=m_dScale;data[6]=0;data[7]=0;
					data[8]=0;data[9]=0;data[10]=m_dScale;data[11]=0;
					data[12]=0;data[13]=0;data[14]=0;data[15]=1;
					compmat.Set(data);

					double* matdata=compmat.GetData();

					for(int i=0;i<4;i++)
						for(int j=0;j<4;j++)
							m_dMatrix[j*4+i]=matdata[i*4+j];


				}
				InvalidateRect(NULL,FALSE);
			}
		}
		break;
	case MO_STROKE_CUT:
		if(GetCapture()==this)
		{
			double dst;
			int dx,dy,Dimx,Dimy;
			double obj[3];
			m_OpenGLManager.GetDimension(Dimx,Dimy);
			dx=m_PrevMousePos.x-point.x;
			dy=m_PrevMousePos.y-Dimy+point.y+1;
			dst=sqrt(double(dx*dx+dy*dy));
			if(dst>10)
			{
				m_PrevMousePos.SetPoint(point.x,Dimy-point.y-1);
				m_vecStrokePoints2D.push_back(m_PrevMousePos);
				float depth=1.0;      
				m_OpenGLManager.GetDepth(&depth,m_PrevMousePos.x,m_PrevMousePos.y);
				depth-=0.1;
				if (depth<0.89)
				{
					if(m_iStrokeIn==-1)
					{
						m_iStrokeOut=m_iStrokeIn=m_vecStrokePoints2D.size()-1;
					}
					else
						m_iStrokeOut++;
				}
				m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,m_PrevMousePos.y,depth,obj);
				m_pStroke->AddPoint(obj[0],obj[1],obj[2]);
			}
			InvalidateRect(NULL,FALSE);
		}
		break;
	case MO_STROKE_SEED:
		if(GetCapture()==this)
		{
			double dst;
			int dx,dy,Dimx,Dimy;
			double obj[3];
			m_OpenGLManager.GetDimension(Dimx,Dimy);
			dx=m_PrevMousePos.x-point.x;
			dy=m_PrevMousePos.y-Dimy+point.y+1;
			dst=sqrt(double(dx*dx+dy*dy));
			if(dst>10)
			{
				m_PrevMousePos.SetPoint(point.x,Dimy-point.y-1);
				m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,m_PrevMousePos.y,0.02,obj);
				m_pStroke->AddPoint(obj[0],obj[1],obj[2]);
			}
			InvalidateRect(NULL,FALSE);

		}
	case MO_STROKE_SELECT:
		if(GetCapture()==this)
		{
			double dst;
			int dx,dy,Dimx,Dimy;
			double obj[3];
			m_OpenGLManager.GetDimension(Dimx,Dimy);
			dx=m_PrevMousePos.x-point.x;
			dy=m_PrevMousePos.y-Dimy+point.y+1;
			dst=sqrt(double(dx*dx+dy*dy));
			if(dst>10)
			{
				m_PrevMousePos.SetPoint(point.x,Dimy-point.y-1);
				float depth=1.0;      
		    	m_OpenGLManager.GetDepth(&depth,m_PrevMousePos.x,m_PrevMousePos.y);
				depth-=0.1;
				m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,m_PrevMousePos.y,depth,obj);
				m_pStroke->AddPoint(obj[0],obj[1],obj[2]);
			}
			InvalidateRect(NULL,FALSE);

		}
		break;
	case MO_STROKE_STRUCTURE:
		if(GetCapture()==this)
		{
			double dst;
			int dx,dy,Dimx,Dimy;
			double obj[3];
			m_OpenGLManager.GetDimension(Dimx,Dimy);
			dx=m_PrevMousePos.x-point.x;
			dy=m_PrevMousePos.y-Dimy+point.y+1;
			dst=sqrt(double(dx*dx+dy*dy));
			if(dst>10)
			{
				m_PrevMousePos.SetPoint(point.x,Dimy-point.y-1);
				m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,m_PrevMousePos.y,obj);
				m_pStroke->AddPoint(obj[0],obj[1],obj[2]);
			}
			InvalidateRect(NULL,FALSE);

		}
		break;
	case MO_STROKE_SILHOUETTE:
		if(GetCapture()==this)
		{
			double dst;
			int dx,dy,Dimx,Dimy;
			double obj[3];
			m_OpenGLManager.GetDimension(Dimx,Dimy);
			m_vecStrokePoints2D.push_back(m_PrevMousePos);
			dx=m_PrevMousePos.x-point.x;
			dy=m_PrevMousePos.y-Dimy+point.y+1;
			dst=sqrt(double(dx*dx+dy*dy));
			if(dst>5)
			{
				m_PrevMousePos.SetPoint(point.x,Dimy-point.y-1);
				float depth=1.0;     
				m_OpenGLManager.GetDepth(&depth,m_PrevMousePos.x,m_PrevMousePos.y);
				depth-=0.1;
				m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,m_PrevMousePos.y,depth,obj);
				m_pStroke->AddPoint(obj[0],obj[1],obj[2]);

				InvalidateRect(NULL,FALSE);
			}
		}
		break;
	case MO_SELECT_EDGE:
		{
			int dimx,dimy,x,y,i;
			m_OpenGLManager.GetDimension(dimx,dimy);
			x=point.x;
			y=dimy-point.y-1;
			double v1[3],v2[3],a[2],b[2];
			CEdge* pE;
			for(i=0;i<m_pModel->GetEdgeNum();i++)
			{
				pE=m_pModel->GetEdgeByIndex(i);
				m_OpenGLManager.GetWCCoord(pE->GetVertex1()->GetCoordinate().GetArray(),v1);
				m_OpenGLManager.GetWCCoord(pE->GetVertex2()->GetCoordinate().GetArray(),v2);

				a[0]=v2[0]-v1[0];
				a[1]=v2[1]-v1[1];

				b[0]=x-v1[0];
				b[1]=y-v1[1];

				if(a[0]*b[0]+a[1]*b[1]<0)continue;

				b[0]=x-v2[0];
				b[1]=y-v2[1];
				if(a[0]*b[0]+a[1]*b[1]>0)continue;

				if(fabs((a[0]*b[1]-a[1]*b[0])/sqrt(a[0]*a[0]+a[1]*a[1]))<2)
				{
					m_pSelectedEdge=pE;
					break;
				}

			}
			InvalidateRect(NULL,FALSE);

		}

		break;
	case MO_ADD_FACE:
	case MO_SELECT_VERTEX:
		{
			int dimx,dimy,x,y,i;
			double out[3],dist,mindist=DBL_MAX;
			CVector3d curPos,Coord;
			if(m_pSelectedVertex)
				m_pSelectedVertex->SetSelected(false);
			m_OpenGLManager.GetDimension(dimx,dimy);
			x=point.x;
			y=dimy-point.y-1;
			m_OpenGLManager.GetOBJCoord(x,y,out);
			curPos.Set(out);
			CVertex* pV;
			for (i=0;i<m_pModel->GetVertexNum();i++)
			{
				pV=m_pModel->GetVertexByIndex(i);
				Coord=pV->GetCoordinate();
				dist=(curPos-Coord).Length();
				if(dist<mindist)
				{
					mindist=dist;
					m_pSelectedVertex=pV;
				}
			}
			InvalidateRect(NULL,FALSE);

		}
		break;
	case MO_SELECT_FACE:
		{
			int dimx,dimy,x,y,i;
			double out[3],dist,mindist=DBL_MAX;
			CVector3d curPos,Coord;
			if(m_pSelectedFace)
				m_pSelectedFace->SetSelected(false);
			m_OpenGLManager.GetDimension(dimx,dimy);
			x=point.x;
			y=dimy-point.y-1;
			m_OpenGLManager.GetOBJCoord(x,y,out);
			curPos.Set(out);
			CFace *pF;
			for (i=0;i<m_pModel->GetFaceNum();i++)
			{
				pF=m_pModel->GetFaceByIndex(i);
				Coord=pF->CalcCentroid();
				dist=(curPos-Coord).Length();
				if(dist<mindist)
				{
					mindist=dist;
					m_pSelectedFace=pF;
				}
			}
			m_pSelectedFace->SetSelected(true);
			InvalidateRect(NULL,FALSE);

		}
		break;
	case MO_SELECT_RECTPATCH:
		{
			if(GetCapture()==this)
			{
				int i;
				int dimx,dimy;
				double obj[3];
				m_OpenGLManager.GetDimension(dimx,dimy);
				double dx,dy,dst;
				dx=m_PrevMousePos.x-point.x;
				dy=m_PrevMousePos.y-point.y;
				dst=sqrt(double(dx*dx+dy*dy));
				if(dst>5)
				{
					if(point.x>m_PrevMousePos.x)
					{
						if(point.y>m_PrevMousePos.y)
						{

							m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,dimy-point.y,0.1,obj);
							m_pSelectionPlane->SetCorner(0,CVector3d(obj));
							m_OpenGLManager.GetOBJCoord(point.x,dimy-point.y,0.1,obj);
							m_pSelectionPlane->SetCorner(1,CVector3d(obj));
							m_OpenGLManager.GetOBJCoord(point.x,dimy-m_PrevMousePos.y,0.1,obj);
							m_pSelectionPlane->SetCorner(2,CVector3d(obj));
							m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,dimy-m_PrevMousePos.y,0.1,obj);
							m_pSelectionPlane->SetCorner(3,CVector3d(obj));
						}
						else
						{
							m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,dimy-m_PrevMousePos.y,0.1,obj);
							m_pSelectionPlane->SetCorner(0,CVector3d(obj));
							m_OpenGLManager.GetOBJCoord(point.x,dimy-m_PrevMousePos.y,0.1,obj);
							m_pSelectionPlane->SetCorner(1,CVector3d(obj));
							m_OpenGLManager.GetOBJCoord(point.x,dimy-point.y,0.1,obj);
							m_pSelectionPlane->SetCorner(2,CVector3d(obj));
							m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,dimy-point.y,0.1,obj);
							m_pSelectionPlane->SetCorner(3,CVector3d(obj));
						}
					}
					else
					{
						if(point.y>m_PrevMousePos.y)
						{
							m_OpenGLManager.GetOBJCoord(point.x,dimy-point.y,0.1,obj);
							m_pSelectionPlane->SetCorner(0,CVector3d(obj));
							m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,dimy-point.y,0.1,obj);
							m_pSelectionPlane->SetCorner(1,CVector3d(obj));
							m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,dimy-m_PrevMousePos.y,0.1,obj);
							m_pSelectionPlane->SetCorner(2,CVector3d(obj));
							m_OpenGLManager.GetOBJCoord(point.x,dimy-m_PrevMousePos.y,0.1,obj);
							m_pSelectionPlane->SetCorner(3,CVector3d(obj));
						}
						else
						{
							m_OpenGLManager.GetOBJCoord(point.x,dimy-m_PrevMousePos.y,0.1,obj);
							m_pSelectionPlane->SetCorner(0,CVector3d(obj));
							m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,dimy-m_PrevMousePos.y,0.1,obj);
							m_pSelectionPlane->SetCorner(1,CVector3d(obj));
							m_OpenGLManager.GetOBJCoord(m_PrevMousePos.x,dimy-point.y,0.1,obj);
							m_pSelectionPlane->SetCorner(2,CVector3d(obj));
							m_OpenGLManager.GetOBJCoord(point.x,dimy-point.y,0.1,obj);
							m_pSelectionPlane->SetCorner(3,CVector3d(obj));
						}
					}
					for (i=0;i<4;i++)
					{
						CVector3d v=m_pSelectionPlane->GetCorner(i);
						//outfile<<"ok"<<endl;
					}

				}

	
			}
			InvalidateRect(NULL,FALSE);

		}
		break;

	case MO_TPRIMITIVE_LOCATE:
		{
			double out1[3],out2[3];
			int dimx,dimy;
			m_OpenGLManager.GetDimension(dimx,dimy);
			m_OpenGLManager.GetOBJCoord(point.x,dimy-point.y,0.1,out1);
			m_OpenGLManager.GetOBJCoord(point.x,dimy-point.y,0.5,out2);

			CRay3 ray(CPoint3(out1[0],out1[1],out1[2]),CPoint3(out2[0],out2[1],out2[2]));
			m_FSNManager.GetTPrimitive()->SetManualLocator(ray);
			InvalidateRect(NULL,FALSE);
			break;

		}
		break;
	case MO_SELECT_BOUNDARY:
		{
			int i,j,dimx,dimy;
			CVertex *pV;
			double dx,dy,dst,mindst=DBL_MAX,out[3];
			bool changed;
			m_OpenGLManager.GetDimension(dimx,dimy);
			CSolid *pTComponent=m_FSNManager.GetTComponentByIndex(0);

			for (i=0;i<pTComponent->GetBoundaryNum();i++)
			{
				outfile<<i<<endl;
				CBoundary *pBound=pTComponent->GetBoundaryByIndex(i);
				changed=false;
				for (j=0;j<pBound->GetVertexNum();j++)
				{
					pV=pBound->GetVertexByIndex(j);

					m_OpenGLManager.GetWCCoord(pV->GetCoordinate().GetArray(),out);

					dx=out[0]-point.x;
					dy=out[1]-dimy+point.y;

					dst=dx*dx+dy*dy;
					outfile<<"dst: "<<dst<<endl;
					outfile<<"mindst: "<<mindst<<endl;
					if(dst<mindst)
					{
						mindst=dst;
						if(m_pVertex)
							m_pVertex->SetDisplayStyle(0);
						m_pVertex=pV;
						m_pVertex->SetDisplayStyle(1);
						changed=true;
					}

				}
				if(changed)
				{
					outfile<<"changed"<<endl;
					if(m_pBoundary)
						m_pBoundary->SetSelected(false);
					pBound->SetSelected(true);
					m_pBoundary=pBound;
				}

			}


			InvalidateRect(NULL,FALSE);
		}
		break;
	case MO_SELECT_STROKE:
		{
			int i,j,dimx,dimy;
			CVector3d pt;
			double dx,dy,dst,mindst=DBL_MAX,out[3];
			bool changed;
			m_OpenGLManager.GetDimension(dimx,dimy);

			for (i=0;i<m_FSNManager.GetStructureNum();i++)
			{
				outfile<<i<<endl;
				CStroke *pStroke=m_FSNManager.GetStructureByIndex(i);
				changed=false;
				for (j=0;j<pStroke->GetPointsNum();j++)
				{
					pt=pStroke->GetPoint(j);

					m_OpenGLManager.GetWCCoord(pt.GetArray(),out);

					dx=out[0]-point.x;
					dy=out[1]-dimy+point.y;

					dst=dx*dx+dy*dy;
					if(dst<mindst)
					{
						mindst=dst;
						changed=true;
					}

				}
				if(changed)
				{
					if(m_pStroke)
						m_pStroke->SetSelected(false);
					pStroke->SetSelected(true);
					m_pStroke=pStroke;
				}

			}


			InvalidateRect(NULL,FALSE);
		}
		break;
	case MO_SILHOUETTE_ADJUST:
		if (GetCapture()==this)
		{
			CRect rect;
			GetClientRect(rect);

			double dx=point.x-m_PrevMousePos.x;
			double dy=point.y-m_PrevMousePos.y;
			double dst=sqrt(dx*dx+dy*dy);
			if(dst>10)
			{
				double sign=((point.x-m_PrevMousePos.x)>0?1.0:-1.0);
				AdjustSilhouettePlanes(5*sign*dst/rect.Width(),true);
				m_PrevMousePos=point;
			}
			
		}
		else
		{
			int dimx,dimy;
			m_OpenGLManager.GetDimension(dimx,dimy);
			double dx,dy,dst,minDst=DBL_MAX;
			if(m_pCtrlPoint!=NULL)
			{
				m_pCtrlPoint->SetSelected(false);
			}
			for (int i=2;i<4;i++)
			{
				double out[3];
				m_OpenGLManager.GetWCCoord(m_vecCtrlPolygon[i]->GetCoordinate().GetArray(),out);
				dx=point.x-out[0];
				dy=dimy-point.y-out[1];
				dst=dx*dx+dy*dy;
				if(dst<minDst)
				{
					minDst=dst;
					m_pCtrlPoint=m_vecCtrlPolygon[i];
				}
			}
			m_pCtrlPoint->SetSelected(true);
			if(minDst>1000)
			{
				m_pCtrlPoint->SetSelected(false);
				m_pCtrlPoint=NULL;
			}

		}
		InvalidateRect(NULL,FALSE);
		break;
	case MO_SILHOUETTE_SMOOTH:
		{
			if(GetCapture()==this)
			{
				CRect rect;
				GetClientRect(rect);

				double dx=point.x-m_PrevMousePos.x;
				double dy=point.y-m_PrevMousePos.y;
				double dst=sqrt(dx*dx+dy*dy);
				if(dst>10)
				{

					CVector3d ov1=m_pNProjectPlane->GetCorner(0);
					CVector3d ov2=m_pNProjectPlane->GetCorner(1);
					CVector3d ov3=m_pNProjectPlane->GetCorner(2);
					CVector3d ov4=m_pNProjectPlane->GetCorner(3);

					double out1[3],out2[3];

					m_OpenGLManager.GetWCCoord(ov1.GetArray(),out1);
					m_OpenGLManager.GetWCCoord(ov3.GetArray(),out2);

					int dimx,dimy;
					m_OpenGLManager.GetDimension(dimx,dimy);

					if((out1[0]<m_PrevMousePos.x)&&(out2[0]>m_PrevMousePos.x)&&(out1[1]<(dimy-m_PrevMousePos.y))&&(out2[1]>(dimy-m_PrevMousePos.y)))
						AdjustSilhouettePlanes(5,false);
					m_PrevMousePos=point;
				}

			}

		}
		InvalidateRect(NULL,FALSE);
		break;


	}

	CView::OnMouseMove(nFlags, point);
}

void CGeometryStudioView::OnStrokeCut()
{
	m_moType=MO_STROKE_CUT;
}

void CGeometryStudioView::OnToolRotate()
{
	m_moType=MO_ROTATE;

}


void CGeometryStudioView::OnStrokeSelect()
{
	m_moType=MO_STROKE_SELECT;
	if(m_pStroke!=NULL)
		delete m_pStroke;
	m_pStroke=NULL;
}




void CGeometryStudioView::OnExportObjfile()
{
	CFileDialog fileDlg(FALSE, ".OBJ", "*.OBJ", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"OBJ Files (*.OBJ)|*.OBJ||", this);

	//获得2000新风格

	int structsize=0; 
	DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion; 
	//检测目前的操作系统，GetVersion具体用法详见MSDN 
	dwVersion = GetVersion(); 
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion))); 
	dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion))); 
	// 如果运行的操作系统是Windows NT/2000 
	if (dwVersion < 0x80000000)  
		structsize =88;//显示新的文件对话框 
	else 
		//运行的操作系统Windows 95/98  
		structsize =76;//显示老的文件对话框 
	fileDlg.m_ofn.lStructSize=structsize; 

	if(fileDlg.DoModal()==IDOK){
		CString filePath = fileDlg.GetPathName();
		switch(m_omMode)
		{
		case OM_SCISSOR:
			m_pModel->ExportToOBJ(filePath.GetBuffer (20),true);
			break;
		case OM_FUSION:
			m_FSNManager.GetFusionModel()->ExportToOBJ(filePath.GetBuffer (20),true);
			break;
		}
	}	
}

void CGeometryStudioView::OnSceneBackground()
{
	m_OpenGLManager.SetBackgroundFlag(!m_OpenGLManager.GetBackgroundFlag());
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnSceneAxis()
{
	m_OpenGLManager.SetAxisFlag(!m_OpenGLManager.GetAxisFlag());
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnSceneTprimitive()
{
	m_bShowTPrimitive=!m_bShowTPrimitive;
	InvalidateRect(NULL,FALSE);
}
void CGeometryStudioView::OnUpdateSceneTprimitive(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_bShowTPrimitive);
}


void CGeometryStudioView::OnUpdateSceneBackground(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_OpenGLManager.GetBackgroundFlag());
}

void CGeometryStudioView::OnUpdateSceneAxis(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_OpenGLManager.GetAxisFlag());
}

void CGeometryStudioView::OnObjectComposed()
{
	m_odmMode=ODM_COMPOSED;
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnUpdateObjectComposed(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_odmMode==ODM_COMPOSED);

}

void CGeometryStudioView::OnObjectSilhouette()
{
	m_odmMode=ODM_SILHOUETTE;
	int i,dimx,dimy;
	m_OpenGLManager.GetDimension(dimx,dimy);
	if(m_pDepthMap==NULL)
		m_pDepthMap=new float[dimx*dimy];
	for(i=0;i<dimx*dimy;i++)
		m_pDepthMap[i]=1.0;
	m_OpenGLManager.BeginDraw();
	switch(m_omMode)
	{
	case OM_SCISSOR:
		if(m_pModel)
				m_pModel->Display(ODM_SOLID);
		break;

	case OM_FUSION:
		{
			CSolid *pComponent;
			unsigned int i;
			for(i=0;i<m_FSNManager.GetComponentNum();i++)
			{
				pComponent=m_FSNManager.GetComponentByIndex(i);
				pComponent->Display(ODM_SOLID);
			}				
		}
		break;
	default:
		break;
	}
	m_OpenGLManager.EndDraw();
	m_OpenGLManager.GetDepth(m_pDepthMap,0,0,dimx,dimy);
	switch(m_omMode)
	{
	case OM_SCISSOR:
		if(m_pModel)
			m_pModel->UpdateContour(m_OpenGLManager,m_pDepthMap);
		break;

	case OM_FUSION:
		{
			CSolid *pComponent;
			int i,j;
			for(i=0;i<m_FSNManager.GetComponentNum();i++)
			{
				pComponent=m_FSNManager.GetComponentByIndex(i);
				pComponent->UpdateContour(m_OpenGLManager,m_pDepthMap);
			}				
			CStroke *pStroke;
			CVector3d n1,n2;
			CVector3d view=m_OpenGLManager.GetViewDirection();
			for (i=0;i<m_FSNManager.GetSilhouetteNum();i++)
			{
				pStroke=m_FSNManager.GetSilhouetteByIndex(i);
				pStroke->ClearCriticalPoints();
				for (j=0;j<pStroke->GetPointsNum()-1;j++)
				{
					n1=pStroke->GetNormal(j);
					n2=pStroke->GetNormal(j+1);
					if(((n1.Dot(view))*(n2.Dot(view)))<0.0001)	
					{
						pStroke->AddCriticalPoint(j);
					}
				}
			}

		}
		break;
	default:
		break;
	}
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnUpdateObjectSilhouette(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_odmMode==ODM_SILHOUETTE);
}

void CGeometryStudioView::OnObjectSolid()
{
	m_odmMode=ODM_SOLID;
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnUpdateObjectSolid(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_odmMode==ODM_SOLID);
}

void CGeometryStudioView::OnObjectPoint()
{
	m_odmMode=ODM_POINT;
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnUpdateObjectPoint(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_odmMode==ODM_POINT);
}

void CGeometryStudioView::OnObjectLine()
{
	m_odmMode=ODM_LINE;
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnUpdateObjectLine(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_odmMode==ODM_LINE);
}


void CGeometryStudioView::OnStrokeSilhouette()
{
	m_moType=MO_STROKE_SILHOUETTE;
	m_odmMode=ODM_SILHOUETTE;
	int i,j,dimx,dimy;
	m_OpenGLManager.GetDimension(dimx,dimy);
	if(m_pDepthMap==NULL)
		m_pDepthMap=new float[dimx*dimy];
	for(i=0;i<dimx*dimy;i++)
		m_pDepthMap[i]=1.0;
	m_OpenGLManager.BeginDraw();
	CSolid *pComponent;
	for(i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(i);
		pComponent->Display(ODM_SOLID);
	}				
	m_OpenGLManager.EndDraw();
	m_OpenGLManager.GetDepth(m_pDepthMap,0,0,dimx,dimy);
	for(i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(i);
		pComponent->UpdateContour(m_OpenGLManager,m_pDepthMap);
	}				
	CStroke *pStroke;
	CVector3d n1,n2;
	CVector3d view=m_OpenGLManager.GetViewDirection();
	for (i=0;i<m_FSNManager.GetSilhouetteNum();i++)
	{
		pStroke=m_FSNManager.GetSilhouetteByIndex(i);
		pStroke->ClearCriticalPoints();
		for (j=0;j<pStroke->GetPointsNum()-1;j++)
		{
			n1=pStroke->GetNormal(j);
			n2=pStroke->GetNormal(j+1);
			if(((n1.Dot(view))*(n2.Dot(view)))<0.0001)	
			{
				pStroke->AddCriticalPoint(j);
			}
		}
	}

	InvalidateRect(NULL,FALSE);

}

//void CGeometryStudioView::OnSelectEdge()
//{
//	m_moType=MO_SELECT_EDGE;
//	m_odmMode=ODM_LINE;
//	InvalidateRect(NULL,FALSE);
//}

void CGeometryStudioView::OnToolTranslate()
{
	m_moType=MO_TRANSLATE;
}

void CGeometryStudioView::OnSelectObject()
{
	int i;
	CSolid *pComponent;
	m_moType=MO_SELECT_OBJECT;
	for(i=0;i<m_vecSelectedComponents.size();i++)
	{
		pComponent=m_vecSelectedComponents.at(i);
		pComponent->SetSelected(false);
	}
	m_vecSelectedComponents.clear();
}

void CGeometryStudioView::OnToolScale()
{
	m_moType=MO_SCALE;
}

void SetupBmpHeader(BITMAPINFOHEADER *pbmih, int sx, int sy, int bpp)
{
	pbmih->biSize = sizeof(BITMAPINFOHEADER);
	pbmih->biWidth = sx;
	pbmih->biHeight = sy;
	pbmih->biPlanes = 1;
	pbmih->biBitCount = bpp;
	pbmih->biCompression = BI_RGB;
	pbmih->biSizeImage = sx * sy * (bpp/8);
	pbmih->biXPelsPerMeter = 2925;
	pbmih->biYPelsPerMeter = 2925;
	pbmih->biClrUsed = 0;
	pbmih->biClrImportant = 0;
}

void CGeometryStudioView::OnTprimitiveLock()
{
	if(m_FSNManager.GetTPrimitive()!=NULL)
	{
		if(m_FSNManager.GetTPrimitive()->IsLocked())
			m_FSNManager.GetTPrimitive()->Unlock();
		else
			m_FSNManager.GetTPrimitive()->Lock();
	}
}

void CGeometryStudioView::OnUpdateTprimitiveLock(CCmdUI *pCmdUI)
{
	if(m_FSNManager.GetTPrimitive()!=NULL)
        pCmdUI->SetRadio(m_FSNManager.GetTPrimitive()->IsLocked());
	else
		pCmdUI->Enable(false);
}

void CGeometryStudioView::OnSelectClear()
{
	int i;
	CSolid *pComponent;
	for(i=0;i<m_vecSelectedComponents.size();i++)
	{
		pComponent=m_vecSelectedComponents.at(i);
		pComponent->SetSelected(false);
	}
	m_vecSelectedComponents.clear();
}



void CGeometryStudioView::OnPsSampling()
{
   //else
   //{
	  // double v1,v2,v;
	  // double p1[3]={0.0,0.0,0.0};
	  // double p2[3]={1.0,0.0,0.0};
	  // double p[3];

	  // v1=m_Solver.GetValueAt(p1);
	  // ASSERT(v1<0);
	  // v2=m_Solver.GetValueAt(p2);
	  // while (v2<0)
	  // {
		 //  p2[0]*=2;
		 //  v2=m_Solver.GetValueAt(p2);
	  // }
	  // if(v2==0)
		 //  m_Solver.SetInitialSample(p2[0],p2[1],p2[2]);
	  // else
	  // {
		 //  p[0]=(p1[0]+p2[0])/2;
		 //  p[1]=(p1[1]+p2[1])/2;
		 //  p[2]=(p1[2]+p2[2])/2;
		 //  v=m_Solver.GetValueAt(p);
		 //  while (v<-THRESHOLD||v>THRESHOLD) 
		 //  {
			//   if(v>0)
			//   {
			//	   p2[0]=p[0];
			//	   p2[1]=p[1];
			//	   p2[2]=p[2];
			//   }
			//   else
			//   {
			//	   p1[0]=p[0];
			//	   p1[1]=p[1];
			//	   p1[2]=p[2];
			//   }
			//   p[0]=(p1[0]+p2[0])/2;
			//   p[1]=(p1[1]+p2[1])/2;
			//   p[2]=(p1[2]+p2[2])/2;
			//   v=m_Solver.GetValueAt(p);
		 //  }
		 //  m_Solver.SetInitialSample(p[0],p[1],p[2]);
	  // }
   //}

}
void CGeometryStudioView::OnToolBox() 
{
	int nPage = -1;
	int nTool = -1;

	CGeometryStudioApp *pApp = (CGeometryStudioApp *)AfxGetApp();
	CMainFrame *pFrame = (CMainFrame *)pApp->m_pMainWnd;
	nTool = ((CToolBoxBarEx*)pFrame->GetToolBoxBar())->GetLastClickedTool (nPage);

	switch(nTool)
	{
	case 0:
		m_moType=MO_SELECT_OBJECT;
		break;
	case 1:
		m_moType=MO_TRANSLATE;
		break;
	case 2:
		m_moType=MO_ROTATE;
		break;
	}
}


void CGeometryStudioView::OnStrokeSeed()
{
	m_moType=MO_STROKE_SEED;
}

void CGeometryStudioView::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_OpenGLManager.HandleTimer();
	InvalidateRect(NULL,FALSE);

	CView::OnTimer(nIDEvent);
}

void CGeometryStudioView::OnMcLewiner()
{

}

void CGeometryStudioView::OnToolStop()
{
	//KillTimer(1);
	long LATTICE_RESOLUTION=(long)pow((int)2,(int)20);
	//CBox3 box(-1.0,-1.0,-1.0,1.0,1.0,1.0);
	//CTriangle3 tri;
	//tri.Set_A(CVector3d(0.5,0.5,0.8));
	//tri.Set_B(CVector3d(0.5,0.8,0.5));
	//tri.Set_C(CVector3d(0.8,0.5,0.5));
	//if(CIntersectionTest::Box_Triangle(box,tri))
	//{
	//	AfxMessageBox("ok");
	//}
	//m_pPatch=new CPatch;
	//CBoundary *pBoundary=m_pModel->GetBoundaryByIndex(0);
	//m_pPatch->ClusteringNRing(*pBoundary,5);
	//InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnToolPlay()
{
	this->SetTimer(1,33,NULL);
}

void CGeometryStudioView::OnZoomZoomin()
{
	m_OpenGLManager.Zoom(0.9);
	m_bUpdateSilhouettePlanes=true;
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnZoomZoomout()
{
	m_OpenGLManager.Zoom(1.1);
	m_bUpdateSilhouettePlanes=true;
	InvalidateRect(NULL,FALSE);
}


void CGeometryStudioView::OnAreaequalize()
{
	CAreaEqualizationRemesh remesh;

	int i,totVtNum;

	double avgLen,totLen;

	totLen=0;

	CEdge* pE;

	CVertex *pV1,*pV2;

	for (i=0;i<m_pModel->GetEdgeNum();i++)
	{
		pE=m_pModel->GetEdgeByIndex(i);
		pV1=pE->GetVertex1();
		pV2=pE->GetVertex2();
		totLen+=(pV1->GetCoordinate()-pV2->GetCoordinate()).Length();
	}

	avgLen=totLen/m_pModel->GetEdgeNum();

	remesh.SetEntity(m_pModel);

	int ei=remesh.Run(1.5*avgLen);


	m_OpenGLManager.BeginContext();
	m_pModel->BuildList();
	m_OpenGLManager.EndContext();
	InvalidateRect(NULL,FALSE);

}

void CGeometryStudioView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case 'W':
		{
			m_moType=MO_STROKE_STRUCTURE;
		}
		break;
	case 'E':
		{
			m_moType=MO_SELECT_POLYPATCH;
			int dimx,dimy;
			m_OpenGLManager.GetDimension(dimx,dimy);
			if(m_pDepthMap==NULL)
				m_pDepthMap=new float[dimx*dimy];

			for(int i=0;i<dimx*dimy;i++)
				m_pDepthMap[i]=1.0;
			m_OpenGLManager.EndDraw();
			m_OpenGLManager.GetDepth(m_pDepthMap,0,0,dimx,dimy);
		}
		break;
	case 'B':
		{
			m_moType=MO_SELECT_BOUNDARY;
		}
		break;
	case 'Z':
		{
			m_OpenGLManager.HandleTimer();
			InvalidateRect(NULL,FALSE);
		}
		break;
	case 'Q':
		{
			m_moType=MO_SILHOUETTE_ADJUST;
		}
		break;
	case 'R':
		{
			m_moType=MO_ROTATE;
		}
		break;
	case 'T':
		{
			m_moType=MO_TRANSLATE;
		}
		break;
	case 'C':
		{
			m_moType=MO_STROKE_CUT;
			//int i;
			//CSolid *pComponent;
			//m_moType=MO_SELECT_OBJECT;
			//for(i=0;i<m_vecSelectedComponents.size();i++)
			//{
			//	pComponent=m_vecSelectedComponents.at(i);
			//	pComponent->SetSelected(false);
			//}
			//m_vecSelectedComponents.clear();
		}
		break;

	case 'S':
		{
			m_moType=MO_STROKE_SILHOUETTE;
			m_odmMode=ODM_SILHOUETTE;
			int i,j,dimx,dimy;
			m_OpenGLManager.GetDimension(dimx,dimy);
			if(m_pDepthMap==NULL)
				m_pDepthMap=new float[dimx*dimy];
			for(i=0;i<dimx*dimy;i++)
				m_pDepthMap[i]=1.0;
			m_OpenGLManager.BeginDraw();
			CSolid *pComponent;
			for(i=0;i<m_FSNManager.GetComponentNum();i++)
			{
				pComponent=m_FSNManager.GetComponentByIndex(i);
				pComponent->Display(ODM_SOLID);
			}				
			m_OpenGLManager.EndDraw();
			m_OpenGLManager.GetDepth(m_pDepthMap,0,0,dimx,dimy);
			for(i=0;i<m_FSNManager.GetComponentNum();i++)
			{
				pComponent=m_FSNManager.GetComponentByIndex(i);
				pComponent->UpdateContour(m_OpenGLManager,m_pDepthMap);
			}				
			CStroke *pStroke;
			CVector3d n1,n2;
			CVector3d view=m_OpenGLManager.GetViewDirection();
			for (i=0;i<m_FSNManager.GetSilhouetteNum();i++)
			{
				pStroke=m_FSNManager.GetSilhouetteByIndex(i);
				pStroke->ClearCriticalPoints();
				for (j=0;j<pStroke->GetPointsNum()-1;j++)
				{
					n1=pStroke->GetNormal(j);
					n2=pStroke->GetNormal(j+1);
					if(((n1.Dot(view))*(n2.Dot(view)))<0.0001)	
					{
						pStroke->AddCriticalPoint(j);
					}
				}
			}

			InvalidateRect(NULL,FALSE);
		}
		break;
	case 'P':
		{
			int i,j;
			CVector3d centroid,coord;
			CFace *pF,*pSF;
			CStrokeProcess SP;
			double dist,minDist=DBL_MAX;

			SP.BSplineFitting(m_pStroke);

			for (i=0;i<m_pStroke->GetPointsNum();i++)
			{
				coord=m_pStroke->GetPoint(i);

				pSF=NULL;

				minDist=DBL_MAX;

				for (j=0;j<m_FSNManager.GetFusionModel()->GetFaceNum();j++)
				{
					pF=m_FSNManager.GetFusionModel()->GetFaceByIndex(j);
					centroid=pF->GetCentroid();
					dist=(coord-centroid).Length();
					if(dist<minDist)
					{
						minDist=dist;
						pSF=pF;
					}
				}

				m_pStroke->AddNormal(pSF->GetNormal());

			}

			CVertex *pSeedV,*pV;

			CVector3d v1,v2,x,y,z,origin;
			double m_dXLen=0.12;
			double m_dYLen=0.12;

			CMatrix44 g2lmat,tranmat,tmpmat,commat;
			double data[16];

			z=m_pStroke->GetNormal(1);
			origin=m_pStroke->GetPoint(1);

			v1=m_pStroke->GetPoint(0);
			v2=m_pStroke->GetPoint(2);

			x=v2-v1;
			x-=(x.Dot(z))*z;
			x.Normalize();

			y=z.Cross(x);
			y.Normalize();

			cn1=origin-1.5*m_dXLen*x-1.5*m_dYLen*y;
			cn2=origin-1.5*m_dXLen*x+1.5*m_dYLen*y;
			cn3=origin+1.5*m_dXLen*x+1.5*m_dYLen*y;
			cn4=origin+1.5*m_dXLen*x-1.5*m_dYLen*y;

			data[0]=1.0;data[1]=0.0;data[2]=0.0;data[3]=-origin[0];
			data[4]=0.0;data[5]=1.0;data[6]=0.0;data[7]=-origin[1];
			data[8]=0.0;data[9]=0.0;data[10]=1.0;data[11]=-origin[2];
			data[12]=0.0;data[13]=0.0;data[14]=0.0;data[15]=1.0;
			tranmat.Set(data);

			data[0]=x[0];data[1]=x[1];data[2]=x[2];data[3]=0.0;
			data[4]=y[0];data[5]=y[1];data[6]=y[2];data[7]=0.0;
			data[8]=z[0];data[9]=z[1];data[10]=z[2];data[11]=0.0;
			data[12]=0.0;data[13]=0.0;data[14]=0.0;data[15]=1.0;
			g2lmat.Set(data);

			commat=g2lmat.MultRight(tranmat);

			minDist=DBL_MAX;
			for (j=0;j<m_FSNManager.GetFusionModel()->GetVertexNum();j++)
			{
				pV=m_FSNManager.GetFusionModel()->GetVertexByIndex(j);
				dist=(pV->GetCoordinate()-origin).Length();
				if(dist<minDist)
				{
					minDist=dist;
					pSeedV=pV;
				}
			}

			CPatch *pPatchI,*pPatchO;

			CDetailPatch* pDPatch,*pMatchDPatch;


			pPatchI=new CPatch;
			pPatchI->ClusteringRectangle(pSeedV,CVector3d(-m_dXLen,-m_dYLen,0.0),CVector3d(-m_dXLen,m_dYLen,0.0),CVector3d(m_dXLen,m_dYLen,0.0),CVector3d(m_dXLen,-m_dYLen,0.0),commat);
			pPatchI->ClearFlag();

			pPatchO=new CPatch;
			pPatchO->ClusteringRectangle(pSeedV,CVector3d(-1.5*m_dXLen,-1.5*m_dYLen,0.0),CVector3d(-1.5*m_dXLen,1.5*m_dYLen,0.0),CVector3d(1.5*m_dXLen,1.5*m_dYLen,0.0),CVector3d(1.5*m_dXLen,-1.5*m_dYLen,0.0),commat);
			pPatchO->ExtractBoundary();
			pPatchO->ClearFlag();


			pDPatch=new CDetailPatch;
			pDPatch->SetIPatch(pPatchI);
			pDPatch->SetOPatch(pPatchO);
			pDPatch->SetFrame(x,y,z);

			vector<CVector3d*> vDetails;
			for (j=0;j<pPatchO->GetVertexNum();j++)
				vDetails.push_back(m_vDetails[pPatchO->GetVertexByIndex(j)->GetIndex()]);

			pDPatch->ImportDetails(vDetails);

			pDPatch->DetermineCorner(cn1,cn2,cn3,cn4);
			pDPatch->ClearFlag();
			pDPatch->ConvexMapping();

			m_pDPatch=pDPatch;


			CParameterizationDialog dlg(m_pDPatch);
			dlg.DoModal();


		}
		break;
	case 'A':
		{
			//for (int i=0;i<m_pPatch->GetVertexNum();i++)
			//{
			//	m_pPatch->GetVertexByIndex(i)->SetKnown(true);
			//}
			//delete m_pPatch;
			//m_pPatch=NULL;
			m_moType=MO_SILHOUETTE_SMOOTH;

		}
		InvalidateRect(NULL,FALSE);
		break;
	case 'U':
		{
			int i,j;
			outfile<<"u1"<<endl;
			CSolid *pTComponent=m_FSNManager.GetTComponentByIndex(0);
			pTComponent->SetKnown(false);
			for (i=0;i<pTComponent->GetBoundaryNum();i++)
			{
				CBoundary *pBound=pTComponent->GetBoundaryByIndex(i);
				for (j=0;j<pBound->GetVertexNum();j++)
				{
					pBound->GetVertexByIndex(j)->SetKnown(true);
				}
				m_bPropagate=false;
			}
			outfile<<"u2"<<endl;

			m_FSNManager.GetFusionModel()->UpdateVerticesCoords(m_vRawCoords);
			m_OpenGLManager.BeginContext();
			for (int i=0;i<m_FSNManager.GetComponentNum();i++)
			{
				m_FSNManager.GetComponentByIndex(i)->BuildList();
			}
			m_FSNManager.GetFusionModel()->BuildList();
			m_FSNManager.GetTComponentByIndex(0)->BuildList();
			m_OpenGLManager.EndContext();	
			m_iCoordinateShow=0;

			outfile<<"u3"<<endl;
			InvalidateRect(NULL,FALSE);


		}
		break;
	case 'F':
		{
			double lam=0.0001;
			int i,j,k;
			CVertex *pV,*pNV,*pVNxt,*pVPre;
			CHalfEdge *pHe1,*pHe2;
			vector<CVector3d> vecCoords;
			CVector3d u,v,w,lap,coord;
			double ASub,A,weight,tweight,cotA,cotB;


			for (k=0;k<15;k++)
			{
				for (i=0;i<m_pPatch->GetVertexNum();i++)
				{
					m_pPatch->GetVertexByIndex(i)->SetIndex(i);
					vecCoords.push_back(m_pPatch->GetVertexByIndex(i)->GetCoordinate());
				}

				for (i=0;i<m_pPatch->GetVertexNum();i++)
				{
					pV=m_pPatch->GetVertexByIndex(i);

					A=0;
					tweight=0;
					lap.Set(0.0,0.0,0.0);
					pHe1=pV->GetOutHalfEdge();
					outfile<<i<<endl;
					do
					{
						pNV=pHe1->GetMate()->GetVertex();
						pVPre=pHe1->GetMate()->GetPrevEdge()->GetVertex();
						pVNxt=pHe1->GetNextEdge()->GetNextEdge()->GetVertex();

						if(m_pPatch->Find(pVPre))
						{
							u=(vecCoords[pV->GetIndex()])-(vecCoords[pVPre->GetIndex()]);

							if(m_pPatch->Find(pNV))
								v=(vecCoords[pNV->GetIndex()])-(vecCoords[pVPre->GetIndex()]);
							else
								v=pNV->GetCoordinate()-(vecCoords[pVPre->GetIndex()]);



						}
						else
						{

							u=(vecCoords[pV->GetIndex()])-pVPre->GetCoordinate();

							if(m_pPatch->Find(pNV))
								v=(vecCoords[pNV->GetIndex()])-pVPre->GetCoordinate();
							else
								v=pNV->GetCoordinate()-pVPre->GetCoordinate();
						}


						outfile<<u[0]<<","<<u[1]<<","<<u[2]<<endl;
						outfile<<v[0]<<","<<v[1]<<","<<v[2]<<endl;


						w=u.Cross(v);

						ASub=w.Length()/2;

						cotA=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

						if(m_pPatch->Find(pVNxt))
						{
							u=(vecCoords[pV->GetIndex()])-(vecCoords[pVNxt->GetIndex()]);


							if(m_pPatch->Find(pNV))
								v=(vecCoords[pNV->GetIndex()])-(vecCoords[pVNxt->GetIndex()]);
							else
								v=pNV->GetCoordinate()-(vecCoords[pVNxt->GetIndex()]);
						}
						else
						{
							u=(vecCoords[pV->GetIndex()])-pVNxt->GetCoordinate();

							if(m_pPatch->Find(pNV))
								v=(vecCoords[pNV->GetIndex()])-pVNxt->GetCoordinate();
							else
								v=pNV->GetCoordinate()-pVNxt->GetCoordinate();
						}


						w=u.Cross(v);

						cotB=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

						weight=cotA+cotB;
						tweight+=weight;
						A+=ASub;

						if(m_pPatch->Find(pNV))
                            lap+=weight*vecCoords[pNV->GetIndex()];
						else
							lap+=weight*pNV->GetCoordinate();


						pHe1=pV->NextOutHalfEdge(pHe1); 

					} while(pHe1!=pV->GetOutHalfEdge());
					outfile<<"tw: "<<tweight<<endl;
					outfile<<"A: "<<A<<endl;
					lap-=tweight*vecCoords[pV->GetIndex()];
					lap*=lam/(4*A);

					outfile<<lap[0]<<","<<lap[1]<<","<<lap[2]<<endl;

					coord=vecCoords[pV->GetIndex()]+lap;
					pV->SetCoordinate(coord);

				}
				vecCoords.clear();

			}

			m_FSNManager.GetTComponentByIndex(0)->CalcFNormal();
			m_FSNManager.GetTComponentByIndex(0)->CalcVNormal();

			double *VertexArea=new double [m_pPatch->GetVertexNum()];
			CVector3d *pCentroids=new CVector3d [m_pPatch->GetVertexNum()];
			double denominator,factor,NN[3][3],area;
			CVector3d gp,pp,normal,tp;

			for (k=0;k<5;k++)
			{
				for (i=0;i<m_pPatch->GetVertexNum();i++)
				{
					pV=m_pPatch->GetVertexByIndex(i);
					VertexArea[i]=0;
					pHe1=pV->GetOutHalfEdge();
					do 
					{
						pHe1->GetFace()->CalcArea(area);
						VertexArea[i]+=area;
						pHe1=pV->NextOutHalfEdge(pHe1);
					} while(pHe1!=pV->GetOutHalfEdge());
				}
				for (i=0;i<m_pPatch->GetVertexNum();i++)
				{
					pV=m_pPatch->GetVertexByIndex(i);
					denominator=0.0;	gp.Set(0.0,0.0,0.0);
					pHe1=pV->GetOutHalfEdge();
					do 
					{
						pNV=pHe1->GetNextEdge()->GetVertex();
						pHe1=pV->NextOutHalfEdge(pHe1);
						if(m_pPatch->Find(pNV))
                            factor=VertexArea[pNV->GetIndex()];	
						else
						{
							factor=0.0;
							pHe2=pNV->GetOutHalfEdge();
							do 
							{
								pHe2->GetFace()->CalcArea(area);
								factor+=area;
								pHe2=pNV->NextOutHalfEdge(pHe2);
							} while(pHe2!=pNV->GetOutHalfEdge());
						}
						
						denominator+=factor;
						gp+=factor*pNV->GetCoordinate();
					} while(pHe1!=pV->GetOutHalfEdge());
					if(denominator!=0)
						gp/=denominator;
					pCentroids[i]=gp;
				}

				for (i=0;i<m_pPatch->GetVertexNum();i++)
				{
					pV=m_pPatch->GetVertexByIndex(i);
					if(pV->IsBoundary())continue;
					pp=pV->GetCoordinate();
					gp=pCentroids[i];
					normal=pV->GetNormal();
					NN[0][0]=1.0-normal[0]*normal[0];		NN[0][1]=-normal[0]*normal[1];		NN[0][2]=-normal[0]*normal[2];
					NN[1][0]=NN[0][1];						NN[1][1]=1.0-normal[1]*normal[1];	NN[1][2]=-normal[1]*normal[2];
					NN[2][0]=NN[0][2];						NN[2][1]=NN[1][2];					NN[2][2]=1.0-normal[2]*normal[2];
					tp=gp-pp;
					for (j=0;j<3;j++)
						gp[j]=NN[j][0]*tp[0]+NN[j][1]*tp[1]+NN[j][2]*tp[2];
					pp+=gp*0.1;
					pV->SetCoordinate(pp);
				}

				m_FSNManager.GetTComponentByIndex(0)->CalcFNormal();
				m_FSNManager.GetTComponentByIndex(0)->CalcVNormal();

			}





			m_OpenGLManager.BeginContext();
			m_FSNManager.GetFusionModel()->BuildList();
			m_FSNManager.GetTComponentByIndex(0)->BuildList();
			m_OpenGLManager.EndContext();
			delete m_pPatch;
			m_pPatch=NULL;
			InvalidateRect(NULL,FALSE);

		}
		break;

	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGeometryStudioView::OnWrinkleBending()
{

	//int i;
	//double x,y,z;
	//CVector3d norm;

	//CBoundary *m_pBound1,*m_pBound2;

	//CVertex *pV1, *pV2;

	//m_pBound1=new CBoundary;
	//m_pBound2=new CBoundary;
	//for (i=0;i<50;i++)
	//{
	//	z=1.0*cos(i*M_PI/25)+0.05*(my_random()-0.5);
	//	x=1.0*sin(i*M_PI/25)+0.05*(my_random()-0.5);
	//
	//	norm.Set(x,0,z);
	//	norm.Normalize();

	//	y=0.2+0.06*(my_random()-0.5);
	//	pV1=new CVertex(x,y,z,0);
	//	pV1->SetNormal(norm);
	//	m_pBound1->AddVertex(pV1);

	//	y=-0.2-0.06*(my_random()-0.5);
	//	pV2=new CVertex(x,y,z,0);
	//	pV2->SetNormal(norm);
	//	m_pBound2->AddVertex(pV2);

	//}
	//CWrinkle wrinkle;
	//wrinkle.ImportCurvePair(m_pBound1,m_pBound2);
	//wrinkle.Initialize();
	//m_pPatch=wrinkle.GlobalMinimumBendingBridgeTriangulation();

	double totLen,minTotLen,minLen,len;
	CVertex *pV1,*pV2;
	CBoundary *pBound,*pBound1,*pBound2,*pBoundTmp;
	CSolid *pComponent;
	int i,j,k,m,n,pnum,bnum;
	CSolid *pTComponent=m_FSNManager.GetTComponentByIndex(0);
	//outfile<<"optimize ok"<<endl;

	int totBnum=0;
	pnum=m_FSNManager.GetComponentNum();
	for(i=0;i<pnum;i++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(i);
		bnum=pComponent->GetBoundaryNum();
		for(j=0;j<bnum;j++)
		{
			pBound=pComponent->GetBoundaryByIndex(j);
			pBound->SetIndex(totBnum+j);
		}
		totBnum+=bnum;
	}	

	int *BFlag=new int[totBnum];
	for (i=0;i<totBnum;i++)
	{
		BFlag[i]=0;
	}
	pTComponent->BoundaryOptimization();
	for (i=0;i<pTComponent->GetBoundaryNum();i++)
	{
		pBound1=pTComponent->GetBoundaryByIndex(i);
		//outfile<<"pBound1="<<pBound1<<endl;
		minTotLen=DBL_MAX;
		for (j=0;j<pnum;j++)
		{
			pComponent=m_FSNManager.GetComponentByIndex(j);
			//pComponent->BoundaryOptimization();
			for (k=0;k<pComponent->GetBoundaryNum();k++)
			{
				pBoundTmp=pComponent->GetBoundaryByIndex(k);
				if (BFlag[pBoundTmp->GetIndex()])continue;	
				//outfile<<"pBoundTmp="<<pBoundTmp<<endl;
				totLen=0;
				for(m=0;m<pBound1->GetVertexNum();m++)
				{
					pV1=pBound1->GetVertexByIndex(m);
					minLen=DBL_MAX;
					for (n=0;n<pBoundTmp->GetVertexNum();n++)
					{
						pV2=pBoundTmp->GetVertexByIndex(n);
						len=(pV1->GetCoordinate()-pV2->GetCoordinate()).Length();
						if(minLen>len)minLen=len;
					}
					totLen+=minLen;
				}
				if(minTotLen>totLen)
				{
					minTotLen=totLen;
					pBound2=pBoundTmp;
				}
			}
		}


		BFlag[pBound2->GetIndex()]=1;
		CWrinkle wrinkle;
		wrinkle.ImportCurvePair(pBound1,pBound2);
		wrinkle.Initialize();
		if(m_pWPatch==NULL)
			m_pWPatch=wrinkle.GlobalMinimumBendingBridgeTriangulation();
		else
		{
			m_pWPatch->Merge(wrinkle.GlobalMinimumBendingBridgeTriangulation());
		}
	}
	delete []BFlag;

	//pTComponent->ClearBoundary();
	pTComponent->Merge(m_pWPatch);
	m_pWPatch=NULL;

	for (j=0;j<m_FSNManager.GetComponentNum();j++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(j);
		for (k=0;k<pComponent->GetBoundaryNum();k++)
		{
			pBound=pComponent->GetBoundaryByIndex(k);
			pTComponent->AddBoundary(pBound);
		}
	}

	pTComponent->SetMaterial("Polished silver");

	CSolid *pFusionModel=new CSolid;
	for (i=0;i<m_FSNManager.GetComponentNum();i++)
		pFusionModel->Merge(m_FSNManager.GetComponentByIndex(i));
	pFusionModel->Merge(pTComponent);

	m_FSNManager.RegisterFusionModel(pFusionModel);


	m_OpenGLManager.BeginContext();
	pFusionModel->BuildList();
	pTComponent->BuildList();
	for (i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		m_FSNManager.GetComponentByIndex(i)->BuildList();
	}
	m_OpenGLManager.EndContext();
	InvalidateRect(NULL,FALSE);

}

void CGeometryStudioView::OnTypeCylinder()
{
	m_iPrimitiveType=1;
}

void CGeometryStudioView::OnTypeSphere()
{
	m_iPrimitiveType=0;
}

void CGeometryStudioView::OnLocateDefault()
{
	m_FSNManager.GetTPrimitive()->SetLocatingStyle(0);
}

void CGeometryStudioView::OnLocateAuto()
{
	m_FSNManager.GetTPrimitive()->SetLocatingStyle(1);
}

void CGeometryStudioView::OnLocateManual()
{
	m_moType=MO_TPRIMITIVE_LOCATE;	
	m_FSNManager.GetTPrimitive()->SetLocatingStyle(2);
}

void CGeometryStudioView::OnUpdateTypeCylinder(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_iPrimitiveType==1);
}

void CGeometryStudioView::OnUpdateTypeSphere(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_iPrimitiveType==0);
}

void CGeometryStudioView::OnUpdateLocateManual(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_FSNManager.GetTPrimitive()!=NULL);
	if(m_FSNManager.GetTPrimitive())
        pCmdUI->SetRadio(m_FSNManager.GetTPrimitive()->GetLocatingStyle()==2);
}

void CGeometryStudioView::OnUpdateLocateDefault(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_FSNManager.GetTPrimitive()!=NULL);
	if(m_FSNManager.GetTPrimitive())
		pCmdUI->SetRadio(m_FSNManager.GetTPrimitive()->GetLocatingStyle()==0);
}

void CGeometryStudioView::OnUpdateLocateAuto(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_FSNManager.GetTPrimitive()!=NULL);
	if(m_FSNManager.GetTPrimitive())
		pCmdUI->SetRadio(m_FSNManager.GetTPrimitive()->GetLocatingStyle()==1);
}

void CGeometryStudioView::OnSelectRectpatch()
{
	m_moType=MO_SELECT_RECTPATCH;
}

int CGeometryStudioView::GetComponentNum(void)
{
	return m_FSNManager.GetComponentNum();
}


CSolid* CGeometryStudioView::GetComponentByIndex(int index)
{
	return m_FSNManager.GetComponentByIndex(index);
}

CTransitionPrimitive* CGeometryStudioView::GetTPrimitive(void)
{
	return m_FSNManager.GetTPrimitive();
}

void CGeometryStudioView::OnFilteringBeltrami()
{
	unsigned int i;
	CMeshFiltering mf;
	vector<CVector3d*> baseCoords;
	mf.CurvFlowLapImplicit(m_pModel,baseCoords,5);
	m_pModel->UpdateVerticesCoords(baseCoords);
	for (i=0;i<baseCoords.size();i++)
	{
		delete baseCoords[i];
	}

	m_OpenGLManager.BeginContext();
	m_pModel->BuildList();
	m_OpenGLManager.EndContext();

	InvalidateRect(NULL,FALSE);

}

void CGeometryStudioView::OnMatchSimilarity()
{
	if(m_pVertex)
	{
		int i;
		vector<CVector3d*> baseCoords;
		vector<CVector3d*> rawCoords;
		vector<CVector3d*> vDetails;
		vector<double> SimiMetrics;
		CVertex *pVert1, * pVert2,*pVNxt,*pVPre;
		CHalfEdge *pHe;

		CVector3d u,v,w,coord,*pDetail,rawLap,baseLap;
		double btweight,tweight,weight,ASub,cotA,cotB,A,bA,maxDiff=-DBL_MAX,minDiff=DBL_MAX;

		for (i=0;i<m_pModel->GetVertexNum();i++)
		{
			pVert1=m_pModel->GetVertexByIndex(i);
			pVert1->SetIndex(i);
		}		
		
		m_pModel->OutputVerticesCoords(rawCoords);

		CMeshFiltering mf;
		mf.CurvFlowLapImplicit(m_pModel,baseCoords,5);

        for (i=0;i<m_pModel->GetVertexNum();i++)
        {
			pVert1=m_pModel->GetVertexByIndex(i);
			pHe=pVert1->GetOutHalfEdge();
			A=bA=tweight=btweight=0.0;
			rawLap.Set(0.0,0.0,0.0);
			baseLap.Set(0.0,0.0,0.0);
			do
			{
				pVert2=pHe->GetMate()->GetVertex();
				pVPre=pHe->GetMate()->GetPrevEdge()->GetVertex();
				pVNxt=pHe->GetNextEdge()->GetNextEdge()->GetVertex();

				u=*(rawCoords[pVert1->GetIndex()])-*(rawCoords[pVPre->GetIndex()]);

				v=*(rawCoords[pVert2->GetIndex()])-*(rawCoords[pVPre->GetIndex()]);

				w=u.Cross(v);

				ASub=w.Length()/2;

				cotA=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

				u=*(rawCoords[pVert1->GetIndex()])-*(rawCoords[pVNxt->GetIndex()]);

				v=*(rawCoords[pVert2->GetIndex()])-*(rawCoords[pVNxt->GetIndex()]);

				w=u.Cross(v);

				cotB=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

				weight=cotA+cotB;
				rawLap+=weight*(*rawCoords[pVert2->GetIndex()]);
				tweight+=weight;
				A+=ASub;


				u=*(baseCoords[pVert1->GetIndex()])-*(baseCoords[pVPre->GetIndex()]);

				v=*(baseCoords[pVert2->GetIndex()])-*(baseCoords[pVPre->GetIndex()]);

				w=u.Cross(v);

				ASub=w.Length()/2;

				cotA=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

				u=*(baseCoords[pVert1->GetIndex()])-*(baseCoords[pVNxt->GetIndex()]);

				v=*(baseCoords[pVert2->GetIndex()])-*(baseCoords[pVNxt->GetIndex()]);

				w=u.Cross(v);

				cotB=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

				weight=cotA+cotB;
				baseLap+=weight*(*baseCoords[pVert2->GetIndex()]);
				btweight+=weight;
				bA+=ASub;


				pHe=pVert1->NextOutHalfEdge(pHe); 

			} while(pHe!=pVert1->GetOutHalfEdge());
			rawLap-=tweight*(*rawCoords[pVert1->GetIndex()]);
			rawLap/=4*A;
			baseLap-=btweight*(*baseCoords[pVert1->GetIndex()]);
			baseLap/=4*bA;
			pDetail=new CVector3d;
			*pDetail=rawLap-baseLap;
			//*pDetail=baseLap;
			vDetails.push_back(pDetail);
        }



		CBox3 bb=m_pModel->GetBoundBox();		
		if(m_pGeoFan!=NULL)
			delete m_pGeoFan;
		m_pGeoFan=new CGeodesicFan;
		m_pGeoFan->Construct(m_pVertex,0.03*bb.MaxAxisLength());

		for (i=0;i<m_pModel->GetVertexNum();i++)
		{
			pVert1=m_pModel->GetVertexByIndex(i);
			if(pVert1==m_pVertex)
				SimiMetrics.push_back(0.0);
			else
			{
				CGeodesicFan *pGeoFan=new CGeodesicFan;
				if(!pGeoFan->Construct(pVert1,0.03*bb.MaxAxisLength()))
				{
					SimiMetrics.push_back(1000.0);
					delete pGeoFan;
					continue;
				}
				double diff=m_pGeoFan->CompareVector(pGeoFan,vDetails);
				outfile<<"diff="<<diff<<endl;
				if(diff>maxDiff)maxDiff=diff;
				if(diff<minDiff)minDiff=diff;
				SimiMetrics.push_back(diff);
				delete pGeoFan;
			}
		}

		outfile<<"minDiff="<<minDiff<<"; maxDiff="<<maxDiff<<endl;


		vector<double>::iterator it=SimiMetrics.begin();
		for (;it!=SimiMetrics.end();it++)
		{
			if((*it)==1000.0)
				(*it)=0.0;
			else if((*it)==0.0)
				(*it)=1.0;
			else
                (*it)=1-(((*it)-minDiff)/(maxDiff-minDiff));
			outfile<<"sim="<<(*it)<<endl;
		
		}


		m_OpenGLManager.BeginContext();
		m_pModel->BuildMetricList(SimiMetrics);
		m_OpenGLManager.EndContext();
		m_odmMode=ODM_METRIC;
		InvalidateRect(NULL,FALSE);

		for (i=0;i<rawCoords.size();i++)
		{
			delete rawCoords.at(i);
			delete baseCoords.at(i);
			delete vDetails.at(i);
		}

	}


}

//void CGeometryStudioView::OnSelectVertex()
//{
//	m_moType=MO_SELECT_VERTEX;
//	m_pVertex=NULL;
//}

void CGeometryStudioView::OnObjectMetric()
{
	m_odmMode=ODM_METRIC;
}

void CGeometryStudioView::OnDetailrecoveryNonstructure()
{
	int i,j;
	CGeodesicFan *pUnknownGeoFan,*pKnownGeoFan;
	CVertex *pV,*pNV,*pUnknownV,*pKnownV,*pMatchV=NULL;
	CVector3d *pUnknownDetail, *pKnownDetal;
	double diff,minDiff;
	CSolid * pTComponent=m_FSNManager.GetTComponentByIndex(0);

	list<CGeoFanNode*> vecUGFNs; 
	list<CGeoFanNode*>::iterator pos;
	vector<CGeoFanNode*> vecKGFNs;
	CGeoFanNode *pGNode;
	CHalfEdge *pHe,*pFHe;
	CFace* pFace;
	bool known;

	outfile<<"1"<<endl;

	outfile<<"TComponent size: "<<pTComponent->GetVertexNum()<<endl;


	for (i=0;i<pTComponent->GetBoundaryNum();i++)
	{
		pTComponent->GetBoundaryByIndex(i)->SetFlag();
	}

	for (j=0;j<m_pPatch->GetVertexNum();j++)
	{
		pKnownV=m_pPatch->GetVertexByIndex(j);
		pKnownGeoFan=new CGeodesicFan;
		pKnownGeoFan->Construct(pKnownV,0.1);

		pGNode=new CGeoFanNode;
		pGNode->m_pVertex=pKnownV;
		pGNode->m_pGeoFan=pKnownGeoFan;

		vecKGFNs.push_back(pGNode);
	}

	outfile<<"2"<<endl;

	for (j=0;j<m_pBoundary->GetVertexNum();j++)
	{
		if(m_pBoundary->GetVertexByIndex(j)==m_pVertex)
			break;
	}
	if(j==m_pBoundary->GetVertexNum())
		outfile<<"error"<<endl;

	bool stop;
	stop=false;
	for (i=j;i<m_pBoundary->GetVertexNum();i++)
	{
		pV=m_pBoundary->GetVertexByIndex(i);

		pHe=pV->GetOutHalfEdge();

        do 
        {
			pNV=pHe->GetMate()->GetVertex();
			if(pNV->IsKnown()&&!pNV->IsBoundary()&&pTComponent->Find(pNV))
			{
				stop=true;
				break;
			}

			pHe=pV->NextOutHalfEdge(pHe);
        } while(pHe!=pV->GetOutHalfEdge());
		if(stop)
			break;

		pHe=pV->GetOutHalfEdge();
		do 
		{
			pNV=pHe->GetMate()->GetVertex();
			if(!pNV->IsKnown())
			{
				pUnknownGeoFan=new CGeodesicFan;
				pUnknownGeoFan->Construct(pNV,0.1);

				pGNode=new CGeoFanNode;
				pGNode->m_pVertex=pNV;
				pGNode->m_pGeoFan=pUnknownGeoFan;
				pGNode->SetValue(pUnknownGeoFan->NumOfKnownSample());

				//outfile<<"value: "<<pUnknownGeoFan->NumOfKnownSample()<<endl;

				pos=vecUGFNs.begin();
				while(pos!=vecUGFNs.end())
				{
					if(GreaterHeapNode(pGNode,(*pos)))
					{
						vecUGFNs.insert(pos,pGNode);
						break;
					}
					else
						pos++;
				}
				if(pos==vecUGFNs.end())
					vecUGFNs.push_back(pGNode);
			}
			pHe=pV->NextOutHalfEdge(pHe);
		} while(pHe!=pV->GetOutHalfEdge());

	}




	stop=false;
	for (i=j;i>=0;i--)
	{
		pV=m_pBoundary->GetVertexByIndex(i);

		pHe=pV->GetOutHalfEdge();

		do 
		{
			pNV=pHe->GetMate()->GetVertex();
			if(pNV->IsKnown()&&!pNV->IsBoundary()&&pTComponent->Find(pNV))
			{
				stop=true;
				break;
			}

			pHe=pV->NextOutHalfEdge(pHe);
		} while(pHe!=pV->GetOutHalfEdge());
		if(stop)
			break;

		pHe=pV->GetOutHalfEdge();
		do 
		{
			pNV=pHe->GetMate()->GetVertex();
			if(!pNV->IsKnown())
			{
				pUnknownGeoFan=new CGeodesicFan;
				pUnknownGeoFan->Construct(pNV,0.06);

				pGNode=new CGeoFanNode;
				pGNode->m_pVertex=pNV;
				pGNode->m_pGeoFan=pUnknownGeoFan;
				pGNode->SetValue(pUnknownGeoFan->NumOfKnownSample());

				//outfile<<"value: "<<pUnknownGeoFan->NumOfKnownSample()<<endl;

				pos=vecUGFNs.begin();
				while(pos!=vecUGFNs.end())
				{
					if(GreaterHeapNode(pGNode,(*pos)))
					{
						vecUGFNs.insert(pos,pGNode);
						break;
					}
					else
						pos++;
				}
				if(pos==vecUGFNs.end())
					vecUGFNs.push_back(pGNode);
			}
			pHe=pV->NextOutHalfEdge(pHe);
		} while(pHe!=pV->GetOutHalfEdge());

	}


	outfile<<"3"<<endl;
	outfile<<"GFNs Size:"<<vecKGFNs.size()<<endl;
	outfile<<"UGFNs Size:"<<vecUGFNs.size()<<endl;
	while(vecUGFNs.size())
	{
	
		pGNode=vecUGFNs.front();
		vecUGFNs.pop_front();

		outfile<<"UGFNs Size:"<<vecUGFNs.size()<<": "<<pGNode->GetValue()<<endl;

		pUnknownGeoFan=pGNode->m_pGeoFan;
		pUnknownV=pGNode->m_pVertex;
		minDiff=DBL_MAX;
		for (i=0;i<vecKGFNs.size();i++)
		{
			//outfile<<"i="<<i<<" ";
			pKnownGeoFan=vecKGFNs[i]->m_pGeoFan;

			//outfile<<"Compare"<<endl;
			diff=pUnknownGeoFan->CompareVector(pKnownGeoFan,m_vDetails);
			//outfile<<"diff="<<diff<<endl;
			if(diff<minDiff)
			{
				minDiff=diff;
				pMatchV=vecKGFNs[i]->m_pVertex;
			}
		}

		*(m_vDetails[pUnknownV->GetIndex()])=*(m_vDetails[pMatchV->GetIndex()]);
		//outfile<<"assigned"<<endl;
		pUnknownV->SetKnown(true);
		delete pGNode;

		pHe=pUnknownV->GetOutHalfEdge();
		do 
		{
			pFace=pHe->GetFace();
			pFHe=pFace->GetFirstEdge();
			known=true;
			do 
			{
				if (!pFHe->GetVertex()->IsKnown())
				{
					known=false;
					break;
				}
				pFHe=pFace->GetNextEdge(pFHe);
			} while(pFHe!=pFace->GetFirstEdge());
			pFace->SetKnown(known);
			pNV=pHe->GetMate()->GetVertex();
			if(!pNV->IsKnown())
			{
				pos=vecUGFNs.begin();
				while (pos!=vecUGFNs.end())
				{
					pGNode=(*pos);
					if(pGNode->m_pVertex==pNV)
						break;
					pos++;
				}
				if(pGNode->m_pVertex==pNV)
				{
					vecUGFNs.erase(pos);
					delete pGNode;
				}

				pUnknownGeoFan=new CGeodesicFan;
				pUnknownGeoFan->Construct(pNV,0.06);

				pGNode=new CGeoFanNode;
				pGNode->m_pVertex=pNV;
				pGNode->m_pGeoFan=pUnknownGeoFan;
				pGNode->SetValue(pUnknownGeoFan->NumOfKnownSample());
				//outfile<<"value: "<<pUnknownGeoFan->NumOfKnownSample()<<endl;

				pos=vecUGFNs.begin();
				while(pos!=vecUGFNs.end())
				{
					if(GreaterHeapNode(pGNode,(*pos)))
					{
						vecUGFNs.insert(pos,pGNode);
						break;
					}
					else
						pos++;
				}
				if(pos==vecUGFNs.end())
					vecUGFNs.push_back(pGNode);
			}
			pHe=pUnknownV->NextOutHalfEdge(pHe);
		} while(pHe!=pUnknownV->GetOutHalfEdge());

	}
	outfile<<"4"<<endl;
	pos=vecUGFNs.begin();
	while (pos!=vecUGFNs.end())
	{
		delete (*pos);
		pos++;
	}

	for (i=0;i<vecKGFNs.size();i++)
	{
		delete vecKGFNs[i];
	}
}

void CGeometryStudioView::OnDetailrecoveryReconstruct()
{
	int i,j,k,num,anum,start,index,acount;
	CVertex *pVert1, * pVert2,*pVNxt,*pVPre;
	CHalfEdge *pHe;
	CBoundary *pBound;
	vector<CVector3d*> baseCoords;
	vector<CVector3d*> rawCoords;
	vector<CVector3d*> Details;
	vector<CVector3d*> ReconLaps;
	vector<int> vecIndices;

	CVector3d u,v,w,*LapCoord,coord,*pVector;

	CPatch *pTComponent=new CPatch;
	pTComponent->FromSolid(m_FSNManager.GetTComponentByIndex(0));

	num=pTComponent->GetVertexNum();

	anum=0;

	outfile<<"1"<<endl;
	for (i=0;i<pTComponent->GetBoundaryNum();i++)
	{
		pBound=pTComponent->GetBoundaryByIndex(i);

		CPatch patch;
		patch.ClusteringNRing(*pBound,2);
		patch.Subtract(pTComponent);
		pTComponent->Merge(&patch);

		num+=patch.GetVertexNum();
		anum+=(patch.GetVertexNum()+pBound->GetVertexNum());

		for (j=0;j<patch.GetVertexNum();j++)
		{
			patch.GetVertexByIndex(j)->SetAnchor(true);
		}
		for (j=0;j<pBound->GetVertexNum();j++)
		{
			pBound->GetVertexByIndex(j)->SetAnchor(true);
		}
	}

	outfile<<"2"<<endl;

	for (i=0;i<pTComponent->GetVertexNum();i++)
	{
		pVert1=pTComponent->GetVertexByIndex(i);


		pVector=m_vBaseCoords[pVert1->GetIndex()];
		baseCoords.push_back(pVector);
		pVector=m_vDetails[pVert1->GetIndex()];
		Details.push_back(pVector);
		pVector=m_vRawCoords[pVert1->GetIndex()];
		rawCoords.push_back(pVector);

		vecIndices.push_back(pVert1->GetIndex());

		pVert1->SetIndex(i);
		pVert1->SetInPatch(true);
		//rawCoords.push_back(pVert1->GetCoordinate());
	}
	outfile<<"3"<<endl;

	unsigned long* ijL=new unsigned long[20*num];
	double *vL=new double[20*num];
	double cotA,cotB,rcotA,rcotB,ASub,rASub,A,rA,weight,tweight,rtweight,AnchorCoeff=-DBL_MAX;
	
	index=num+1;
	for (i=0;i<pTComponent->GetVertexNum();i++)
	{
		ijL[i]=index;
		pVert1=pTComponent->GetVertexByIndex(i);
		LapCoord=new CVector3d;
		LapCoord->Set(0.0,0.0,0.0);
		A=rA=tweight=rtweight=0.0;
		if(pVert1->IsAnchor())
		{
			outfile<<"An"<<endl;
			start=index;
			pHe=pVert1->GetOutHalfEdge();
			do 
			{
				pVert2=pHe->GetNextEdge()->GetVertex();
				if(!pVert2->IsInPatch())
				{
					pHe=pVert1->NextOutHalfEdge(pHe);
					if(pHe==NULL)break;
					else
                        continue;
				}

				pVPre=pHe->GetMate()->GetPrevEdge()->GetVertex();
				pVNxt=pHe->GetNextEdge()->GetNextEdge()->GetVertex();

				if(pVPre->IsInPatch())
				{
					u=*(baseCoords[pVert1->GetIndex()])-*(baseCoords[pVPre->GetIndex()]);
					v=*(baseCoords[pVert2->GetIndex()])-*(baseCoords[pVPre->GetIndex()]);
					w=u.Cross(v);
					ASub=w.Length()/2;
					cotA=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

					u=*(rawCoords[pVert1->GetIndex()])-*(rawCoords[pVPre->GetIndex()]);
					v=*(rawCoords[pVert2->GetIndex()])-*(rawCoords[pVPre->GetIndex()]);
					w=u.Cross(v);
					rASub=w.Length()/2;
					rcotA=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

				}
				else
				{

					u=*(baseCoords[pVert1->GetIndex()])-*(baseCoords[pVert2->GetIndex()]);
					ASub=sqrt(3.0)*u.Length()/4.0;
					cotA=1/sqrt(3.0);


					u=*(rawCoords[pVert1->GetIndex()])-*(rawCoords[pVert2->GetIndex()]);
					rASub=sqrt(3.0)*u.Length()/4.0;
					rcotA=1/sqrt(3.0);

				}


				if(pVNxt->IsInPatch())
				{

					u=*(baseCoords[pVert1->GetIndex()])-*(baseCoords[pVNxt->GetIndex()]);
					v=*(baseCoords[pVert2->GetIndex()])-*(baseCoords[pVNxt->GetIndex()]);
					w=u.Cross(v);
					cotB=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));


					u=*(rawCoords[pVert1->GetIndex()])-*(rawCoords[pVNxt->GetIndex()]);
					v=*(rawCoords[pVert2->GetIndex()])-*(rawCoords[pVNxt->GetIndex()]);
					w=u.Cross(v);
					rcotB=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

				}
				else
				{

					u=*(baseCoords[pVert1->GetIndex()])-*(baseCoords[pVert2->GetIndex()]);
					cotB=1/sqrt(3.0);

					u=*(rawCoords[pVert1->GetIndex()])-*(rawCoords[pVert2->GetIndex()]);
					rcotB=1/sqrt(3.0);

				}

				weight=cotA+cotB;
				tweight+=weight;
				A+=ASub;
	
				weight=rcotA+rcotB;
				rA+=rASub;
				rtweight+=weight;
				*LapCoord+=weight*(*(rawCoords[pVert2->GetIndex()]));
				vL[index]=weight;
				ijL[index++]=pVert2->GetIndex();


				pHe=pVert1->NextOutHalfEdge(pHe); 

			} while(pHe!=pVert1->GetOutHalfEdge());

			outfile<<"post"<<endl;
			*LapCoord-=rtweight*(*(rawCoords[pVert1->GetIndex()]));
			*LapCoord/=(4*rA);
			ReconLaps.push_back(LapCoord);
			for(j=start;j<index;j++)
				vL[j]/=(4*rA);
			vL[i]=-rtweight/(4*rA);

			if(vL[i]>AnchorCoeff)
				AnchorCoeff=vL[i];

			outfile<<"done"<<endl;


		}
		else
		{
			outfile<<"non an"<<endl;
			pHe=pVert1->GetOutHalfEdge();
			start=index;
			do 
			{
				pVert2=pHe->GetMate()->GetVertex();
				pVPre=pHe->GetMate()->GetPrevEdge()->GetVertex();
				pVNxt=pHe->GetNextEdge()->GetNextEdge()->GetVertex();

				u=*(rawCoords[pVert1->GetIndex()])-*(rawCoords[pVPre->GetIndex()]);
				v=*(rawCoords[pVert2->GetIndex()])-*(rawCoords[pVPre->GetIndex()]);
				w=u.Cross(v);
				ASub=w.Length()/2;
				cotA=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

				u=*(rawCoords[pVert1->GetIndex()])-*(rawCoords[pVNxt->GetIndex()]);

				v=*(rawCoords[pVert2->GetIndex()])-*(rawCoords[pVNxt->GetIndex()]);

				w=u.Cross(v);

				cotB=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

				weight=cotA+cotB;
				*LapCoord+=weight*(*(baseCoords[pVert2->GetIndex()]));
				tweight+=weight;
				A+=ASub;

				vL[index]=weight;
				ijL[index++]=pVert2->GetIndex();
				pHe=pVert1->NextOutHalfEdge(pHe); 
			} while(pHe!=pVert1->GetOutHalfEdge());
			outfile<<"post"<<endl;
			*LapCoord-=tweight*(*(baseCoords[pVert1->GetIndex()]));
			*LapCoord/=(4*A);
			*LapCoord+=Details[pVert1->GetIndex()];
			ReconLaps.push_back(LapCoord);
			for(j=start;j<index;j++)
				vL[j]/=(4*A);
			vL[i]=-tweight/(4*A);
			if(vL[i]>AnchorCoeff)
				AnchorCoeff=vL[i];

			outfile<<"done"<<endl;


		}
	}
	ijL[num]=index;
	vL[num]=0.0;

	int used=anum+index-1;

	outfile<<"4"<<endl;

	nlNewContext() ;
	if(nlInitExtension("SUPERLU")) {
		nlSolverParameteri(NL_SOLVER, NL_PERM_SUPERLU_EXT) ;
	} 
	else {
		nlSolverParameteri(NL_SOLVER, NL_CG) ;
		nlSolverParameteri(NL_PRECONDITIONER, NL_PRECOND_JACOBI) ;
	}
	nlSolverParameteri(NL_NB_VARIABLES, 3*num) ;
	nlSolverParameteri(NL_LEAST_SQUARES, NL_TRUE) ;
	nlSolverParameteri(NL_MAX_ITERATIONS, 15*num) ;
	nlSolverParameterd(NL_THRESHOLD, 1e-10) ;	

	nlBegin(NL_SYSTEM);


	//acount=0;

	for(i=0; i<num; i++) 
	{
		pVert1=pTComponent->GetVertexByIndex(i);
		coord=(rawCoords[pVert1->GetIndex()]);
		nlSetVariable(i,coord[0]);
		nlSetVariable(num+i,coord[1]);
		nlSetVariable(2*num+i,coord[2]);
	}	

	nlBegin(NL_MATRIX);

	for (i=0;i<3;i++)
	{
		for (j=0;j<num;j++)
		{
			pVert1=pTComponent->GetVertexByIndex(j);
			coord=ReconLaps[pVert1->GetIndex()];
			nlRowParameterd(NL_RIGHT_HAND_SIDE,coord[i]);
			nlBegin(NL_ROW) ;
			for (k=ijL[j];k<ijL[j+1];k++)
			{
				nlCoefficient(i*num+ijL[k],vL[k]);
			}
			nlCoefficient(i*num+j,vL[j]);
			nlEnd(NL_ROW) ;
		}
		for (j=0;j<num;j++)
		{
			pVert1=pTComponent->GetVertexByIndex(j);
			if(pVert1->IsAnchor())
			{
				coord=rawCoords[pVert1->GetIndex()];
				nlRowParameterd(NL_RIGHT_HAND_SIDE,1000*AnchorCoeff*coord[i]);
				nlBegin(NL_ROW) ;
				nlCoefficient(i*num+j,1000*AnchorCoeff);
				nlEnd(NL_ROW) ;

			}
		}
	}
	nlEnd(NL_MATRIX);

	nlEnd(NL_SYSTEM);

	outfile<<"5"<<endl;
	nlSolve() ;

	outfile<<"6"<<endl;


	vector<CVector3d*>::iterator it=m_vReconCoords.begin();
	for(i=0;i<num;i++)
		*(*(it+vecIndices[i]))=CVector3d(nlGetVariable(i),nlGetVariable(num+i),nlGetVariable(2*num+i));
	nlDeleteContext(nlGetCurrent());
	delete []ijL;
	delete []vL;


	m_FSNManager.GetFusionModel()->UpdateVerticesCoords(m_vReconCoords);

	m_OpenGLManager.BeginContext();
	m_FSNManager.GetFusionModel()->BuildList();
	for (i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		m_FSNManager.GetComponentByIndex(i)->BuildList();
	}
	m_FSNManager.GetTComponentByIndex(0)->BuildList();
	m_OpenGLManager.EndContext();

	m_iCoordinateShow=2;

	delete pTComponent;

	outfile<<"ok"<<endl;
	InvalidateRect(NULL,FALSE);

}

void CGeometryStudioView::OnDetailrecoveryExtractiondetail()
{
	CSolid *pFusModel=m_FSNManager.GetFusionModel();
	//CMeshFiltering mf;
	//pFusModel->OutputVerticesCoords(m_vRawCoords);
	//m_vReconCoords.resize(m_vRawCoords.size());
	//copy(m_vRawCoords.begin(),m_vRawCoords.end(),m_vReconCoords.begin());
	//
	//mf.CurvFlowLapImplicit(pFusModel,m_vBaseCoords,10);


	unsigned int i,j,k;
	CVertex *pVert1, * pVert2,*pVNxt,*pVPre;
	CHalfEdge *pHe;

	CVector3d u,v,w,*pDetail,rawLap,baseLap;

	double btweight,tweight,weight,ASub,cotA,cotB,A,bA;

	for (i=0;i<m_FSNManager.GetTComponentNum();i++)
	{
		CSolid *pComponent=m_FSNManager.GetTComponentByIndex(i);
		pComponent->SetKnown(false);
	}	
	for (i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		CSolid *pComponent=m_FSNManager.GetComponentByIndex(i);
		pComponent->GetBoundaryByIndex(0)->ClearFlag();
		pComponent->SetKnown(true);
	}

	for (i=0;i<pFusModel->GetVertexNum();i++)
	{
		pVert1=pFusModel->GetVertexByIndex(i);
		pHe=pVert1->GetOutHalfEdge();
		A=bA=tweight=btweight=0.0;
		rawLap.Set(0.0,0.0,0.0);
		baseLap.Set(0.0,0.0,0.0);
		do
		{
			pVert2=pHe->GetMate()->GetVertex();
			pVPre=pHe->GetMate()->GetPrevEdge()->GetVertex();
			pVNxt=pHe->GetNextEdge()->GetNextEdge()->GetVertex();

			u=*(m_vRawCoords[pVert1->GetIndex()])-*(m_vRawCoords[pVPre->GetIndex()]);

			v=*(m_vRawCoords[pVert2->GetIndex()])-*(m_vRawCoords[pVPre->GetIndex()]);

			w=u.Cross(v);

			ASub=w.Length()/2;

			cotA=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

			u=*(m_vRawCoords[pVert1->GetIndex()])-*(m_vRawCoords[pVNxt->GetIndex()]);

			v=*(m_vRawCoords[pVert2->GetIndex()])-*(m_vRawCoords[pVNxt->GetIndex()]);

			w=u.Cross(v);

			cotB=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

			weight=cotA+cotB;
			rawLap+=weight*(*m_vRawCoords[pVert2->GetIndex()]);
			tweight+=weight;
			A+=ASub;


			u=*(m_vBaseCoords[pVert1->GetIndex()])-*(m_vBaseCoords[pVPre->GetIndex()]);

			v=*(m_vBaseCoords[pVert2->GetIndex()])-*(m_vBaseCoords[pVPre->GetIndex()]);

			w=u.Cross(v);

			ASub=w.Length()/2;

			cotA=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

			u=*(m_vBaseCoords[pVert1->GetIndex()])-*(m_vBaseCoords[pVNxt->GetIndex()]);

			v=*(m_vBaseCoords[pVert2->GetIndex()])-*(m_vBaseCoords[pVNxt->GetIndex()]);

			w=u.Cross(v);

			cotB=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

			weight=cotA+cotB;
			baseLap+=weight*(*m_vBaseCoords[pVert2->GetIndex()]);
			btweight+=weight;
			bA+=ASub;

			pHe=pVert1->NextOutHalfEdge(pHe); 

		} while(pHe!=pVert1->GetOutHalfEdge());
		rawLap-=tweight*(*m_vRawCoords[pVert1->GetIndex()]);
		rawLap/=(4*A);
		baseLap-=btweight*(*m_vBaseCoords[pVert1->GetIndex()]);
		baseLap/=(4*bA);
		pDetail=new CVector3d;
		*pDetail=rawLap-baseLap;
		m_vDetails.push_back(pDetail);
	}   

	for (i=0;i<m_FSNManager.GetTComponentNum();i++)
	{
		CSolid *pComponent=m_FSNManager.GetTComponentByIndex(i);
		pComponent->SetKnown(false);
	}	
	for (i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		CSolid *pComponent=m_FSNManager.GetComponentByIndex(i);
		pComponent->SetKnown(true);
	}


	pFusModel->UpdateVerticesCoords(m_vBaseCoords);
	m_OpenGLManager.BeginContext();
	pFusModel->BuildList();
	for (i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		m_FSNManager.GetComponentByIndex(i)->BuildList();
	}
	for (i=0;i<m_FSNManager.GetTComponentNum();i++)
	{
		m_FSNManager.GetTComponentByIndex(i)->BuildList();
	}
	m_OpenGLManager.EndContext();
	m_iCoordinateShow=1;

	InvalidateRect(NULL,FALSE);

}

void CGeometryStudioView::OnImportObjfile()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"OBJ Files (*.OBJ)|*.OBJ||", this);


	int structsize=0; 
	DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion; 
	//检测目前的操作系统，GetVersion具体用法详见MSDN 
	dwVersion = GetVersion(); 
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion))); 
	dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion))); 
	// 如果运行的操作系统是Windows NT/2000 
	if (dwVersion < 0x80000000)  
		structsize =88;//显示新的文件对话框 
	else 
		//运行的操作系统Windows 95/98  
		structsize =76;//显示老的文件对话框 
	fileDlg.m_ofn.lStructSize=structsize; 

	if(fileDlg.DoModal()==IDOK){
		CString filePath = fileDlg.GetPathName();
		if(m_pModel)
			delete m_pModel;
		m_pModel=new CSolid;
		CString fileExt=fileDlg.GetFileExt();
		m_pModel->ReadFromOBJ(filePath.GetBuffer (20));

		m_omMode=OM_SCISSOR;


		m_OpenGLManager.BeginContext();
		m_pModel->BuildList();
		m_OpenGLManager.EndContext();

		m_SIRManager.RegisterModel(m_pModel);

		InvalidateRect(NULL,FALSE);
	}	
}

void CGeometryStudioView::OnFileSave()
{
	CFileDialog fileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"FSN Files (*.FSN)|*.FSN|SIR Files (*.SIR)|*.SIR|BMP Files (*.BMP)|*.BMP||", this);


	int structsize=0; 
	DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion; 
	//检测目前的操作系统，GetVersion具体用法详见MSDN 
	dwVersion = GetVersion(); 
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion))); 
	dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion))); 
	// 如果运行的操作系统是Windows NT/2000 
	if (dwVersion < 0x80000000)  
		structsize =88;//显示新的文件对话框 
	else 
		//运行的操作系统Windows 95/98  
		structsize =76;//显示老的文件对话框 
	fileDlg.m_ofn.lStructSize=structsize; 

	if(fileDlg.DoModal()==IDOK){
		CString ext=fileDlg.GetFileExt();
		CString filePath = fileDlg.GetPathName();
		if(ext=="fsn")
		{
			m_FSNManager.Write(filePath.GetBuffer(20));
		}
		else if(ext="sir")
		{
			m_SIRManager.Write(filePath.GetBuffer(20));
		}
		else 
		{
			CSceneCapture csc;
			csc.SaveAsBMP(filePath.GetBuffer(20),m_OpenGLManager);
		}
		

		InvalidateRect(NULL,FALSE);
	}	
}

void CGeometryStudioView::OnVisualizationRecparticles()
{
	int i,j,k;
	CSolid *pComponent;
	CBoundary *pBound;
	CVertex *pVertex;
	CRec PSRec;
	for (i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(i);
		for (j=0;j<pComponent->GetBoundaryNum();j++)
		{
			pBound=pComponent->GetBoundaryByIndex(j);
			for (k=0;k<pBound->GetVertexNum();k++)
			{
				pVertex=pBound->GetVertexByIndex(k);
				PSRec.AddPoint(pVertex->GetCoordinate(),pVertex);
			}
		}
	}

	int PntT;
	CVector3d coord;

	CParticleSamplingSys *pPS=m_FSNManager.GetPS();
	for (i=0;i<pPS->GetParticleNum();i++)
	{
		pPS->GetParticleByIndex(i,coord,PntT);

		if(PntT==PT_NORMAL)
			PSRec.AddPoint(coord);
	}
	PSRec.Run();
	CSolid * pTComponent;
	pTComponent->ImportFromRec(PSRec);

	m_FSNManager.RegisterTComponent(pTComponent);

	m_OpenGLManager.BeginContext();
	pTComponent->BuildList();
	m_OpenGLManager.EndContext();
	InvalidateRect(NULL,FALSE);

}

void CGeometryStudioView::OnVisualizationParticlesampling()
{
	m_vsMode=VIS_PS;
}

void CGeometryStudioView::OnUpdateVisualizationParticlesampling(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_vsMode==VIS_PS);
}

void CGeometryStudioView::OnVisualizationTrimmedmc()
{
	m_vsMode=VIS_TMC;
}

void CGeometryStudioView::OnUpdateVisualizationTrimmedmc(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_vsMode==VIS_TMC);
}

void CGeometryStudioView::OnWrinkleMaximumregularity()
{

}

void CGeometryStudioView::OnDeformScale()
{
	m_moType=MO_SCALE;	
}

void CGeometryStudioView::OnDeformCopy()
{
	CVector3d loc,ltc,ltco1,ltco2,n,no1,no2,cb,nb,ct,cp,np;
	CVector3d cnt=m_FSNManager.GetTPrimitive()->GetCenter();

	CMatrix44 transmat,p2gmat,g2cmat,compmat,tmpmat;
	CFrame frame;

	double eimag[3];

	vector<CVector3d> pointcloud;

	CPrincipleAnalysis pa;

	double data[16];


	CBoundary* pBoundary;
	int i,k;

	for (k=0;k<m_vecSelectedComponents.size();k++)
	{
		CSolid *pComponent=new CSolid;
		pComponent->ImportFromSolid(m_vecSelectedComponents[k]);
		pComponent->SetIndex(m_FSNManager.GetComponentNum()+k);

		pBoundary=pComponent->GetBoundaryByIndex(0);

		cb.Set(0.0,0.0,0.0);
		for(i=0;i<pBoundary->GetVertexNum();i++)
			cb+=pBoundary->GetVertexByIndex(i)->GetCoordinate();
		cb/=pBoundary->GetVertexNum();

		for(i=0;i<pBoundary->GetVertexNum();i++)
			pointcloud.push_back((pBoundary->GetVertexByIndex(i)->GetCoordinate()-cb));


		pa.Execute(pointcloud,pBoundary->GetVertexNum(),&frame,eimag);
		nb=frame[2];

		pointcloud.clear();

		cp.Set(0.0,0.0,0.0);
		//outfile<<"3"<<endl;
		for(i=0;i<pComponent->GetVertexNum();i++)
			cp+=pComponent->GetVertexByIndex(i)->GetCoordinate();
		cp/=pComponent->GetVertexNum();

		for(i=0;i<pComponent->GetVertexNum();i++)
			pointcloud.push_back((pComponent->GetVertexByIndex(i)->GetCoordinate()-cp));
		pa.Execute(pointcloud,pComponent->GetVertexNum(),&frame,eimag);
	
		np=frame[0];
		pointcloud.clear();

		nb*=(cb-cp).Dot(nb);
		np*=(cb-cp).Dot(np);
		np.Normalize();
		nb.Normalize();
		n=nb+0.2*np;
		n.Normalize();


		loc=m_FSNManager.GetTPrimitive()->GetLocatorByIndex(m_vecSelectedComponents[k]->GetIndex());


		m_FSNManager.GetTPrimitive()->OffsetLocator(loc);

		m_FSNManager.GetTPrimitive()->SaveLocator();
		ltc=m_FSNManager.GetTPrimitive()->GetCurDirection();

		ltco1=Ortho3(ltc);
		ltco1.Normalize();
		ltco2=ltc.Cross(ltco1);
		ltco2.Normalize();
		data[0]=ltc[0];data[1]=ltco1[0];data[2]=ltco2[0];data[3]=0;
		data[4]=ltc[1];data[5]=ltco1[1];data[6]=ltco2[1];data[7]=0;
		data[8]=ltc[2];data[9]=ltco1[2];data[10]=ltco2[2];data[11]=0;
		data[12]=0;data[13]=0;data[14]=0;data[15]=1;
		p2gmat.Set(data);


		no1=Ortho3(n);
		no1.Normalize();
		no2=n.Cross(no1);
		no2.Normalize();

		data[0]=n[0];data[1]=n[1];data[2]=n[2];data[3]=0;
		data[4]=no1[0];data[5]=no1[1];data[6]=no1[2];data[7]=0;
		data[8]=no2[0];data[9]=no2[1];data[10]=no2[2];data[11]=0;
		data[12]=0;data[13]=0;data[14]=0;data[15]=1;
		g2cmat.Set(data);

		data[0]=1;data[1]=0;data[2]=0;data[3]=-cb.x();
		data[4]=0;data[5]=1;data[6]=0;data[7]=-cb.y();
		data[8]=0;data[9]=0;data[10]=1;data[11]=-cb.z();
		data[12]=0;data[13]=0;data[14]=0;data[15]=1;
		transmat.Set(data);
		compmat=p2gmat.MultRight(g2cmat);
		tmpmat=compmat.MultRight(transmat);

		ct=m_FSNManager.GetTPrimitive()->GetCurLocator();
		data[0]=1;data[1]=0;data[2]=0;data[3]=ct.x();
		data[4]=0;data[5]=1;data[6]=0;data[7]=ct.y();
		data[8]=0;data[9]=0;data[10]=1;data[11]=ct.z();
		data[12]=0;data[13]=0;data[14]=0;data[15]=1;
		transmat.Set(data);

		compmat=transmat.MultRight(tmpmat);
		pComponent->Transform(compmat);

		m_OpenGLManager.BeginContext();
		pComponent->BuildList();
		m_OpenGLManager.EndContext();

		m_FSNManager.RegisterComponent(pComponent);
	}
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnSceneAntialiase()
{
	m_OpenGLManager.SetAntialiase();
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnUpdateSceneAntialiase(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_OpenGLManager.GetAntialiase());
}



int CGeometryStudioView::GetTComponentNum(void)
{
	return m_FSNManager.GetTComponentNum();
}

CSolid* CGeometryStudioView::GetTComponentByIndex(int index)
{
	return m_FSNManager.GetTComponentByIndex(index);
}

CSolid* CGeometryStudioView::GetFusionModel(void)
{
	return m_FSNManager.GetFusionModel();
}

void CGeometryStudioView::OnStrokePreserve()
{
	m_bPreserved=!m_bPreserved;
}

void CGeometryStudioView::OnUpdateStrokePreserve(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bPreserved);
}



void CGeometryStudioView::OnSelectEdge()
{
	m_moType=MO_SELECT_EDGE;
}

void CGeometryStudioView::OnSceneSilhouetteplane()
{
	m_bShowSilhouettePlane=!m_bShowSilhouettePlane;
}

void CGeometryStudioView::OnUpdateSceneSilhouetteplane(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowSilhouettePlane);
}

void CGeometryStudioView::OnSelectFace()
{
	m_moType=MO_SELECT_FACE;
	if(m_pSelectedFace!=NULL)
		m_pSelectedFace->SetSelected(false);
}

void CGeometryStudioView::OnRemoveFace()
{
	m_pModel->RemoveFace(m_pSelectedFace);
	m_pSelectedFace=NULL;
	m_OpenGLManager.BeginContext();
	m_pModel->BuildList();
	m_OpenGLManager.EndContext();
}

void CGeometryStudioView::OnAddFace()
{
	m_moType=MO_ADD_FACE;
}

void CGeometryStudioView::OnFusionRegistercomponent()
{
	if(m_bPreserved)
	{
		m_pModel->RemoveNPatch();
		m_pPatch->ClearFlag();
	}
	else
		m_pModel->RemovePatch();

	outfile<<"1"<<endl;

	delete m_pPatch;
	m_pPatch=NULL;

	CVector3d np,nb,n,cb,cp,ct;
	double eimag[3],radius,maxr=-1;
	CPrincipleAnalysis pa;
	m_pFrame=new CFrame;				
	vector<CVector3d> pointcloud;
	CBoundary *pBoundary;

	cb.Set(0.0,0.0,0.0);
	int k,i,count=0;
	for (k=0;k<m_pModel->GetBoundaryNum();k++)
	{
		pBoundary=m_pModel->GetBoundaryByIndex(k);
		count+=pBoundary->GetVertexNum();
		for(i=0;i<pBoundary->GetVertexNum();i++)
			cb+=pBoundary->GetVertexByIndex(i)->GetCoordinate();
	}
	cb/=count;
	m_pFrame->SetOrigin(cb);
	for (k=0;k<m_pModel->GetBoundaryNum();k++)
	{
		pBoundary=m_pModel->GetBoundaryByIndex(k);
		for(i=0;i<pBoundary->GetVertexNum();i++)
			pointcloud.push_back((pBoundary->GetVertexByIndex(i)->GetCoordinate()-cb));
	}
	pa.Execute(pointcloud,pBoundary->GetVertexNum(),m_pFrame,eimag);
	nb=(*m_pFrame)[2];


	cp.Set(0.0,0.0,0.0);
	for(i=0;i<m_pModel->GetVertexNum();i++)
		cp+=m_pModel->GetVertexByIndex(i)->GetCoordinate();
	cp/=m_pModel->GetVertexNum();


	nb*=(cb-cp).Dot(nb);
	nb.Normalize();
	n=nb;

	outfile<<"2"<<endl;

	if(m_FSNManager.GetComponentNum()==0)
	{
		outfile<<"21"<<endl;

		for(i=1;i<pBoundary->GetVertexNum();i++)
		{
			radius=(cb-pBoundary->GetVertexByIndex(i)->GetCoordinate()).Length();
			if(radius>maxr)
				maxr=radius;
		}
		//outfile<<"maxr ok"<<endl;

		radius=maxr;
		switch(m_iPrimitiveType)
		{
		case 0:
			{
				CTransitionSphere *pTSphere=new CTransitionSphere;
				pTSphere->SetRadius(radius);
				ct=cb+0.5*radius*n;
				pTSphere->SetCenter(ct);
				pTSphere->SetDirection(n);
				m_FSNManager.RegisterTPrimitive(pTSphere);
			}
			break;
		case 1:
			{
				CTransitionCylinder *pTCylinder=new CTransitionCylinder;
				pTCylinder->SetBottomRadius(radius);
				pTCylinder->SetCenter(cb);
				pTCylinder->SetDirection(n);
				pTCylinder->CalcLimit();
				m_FSNManager.RegisterTPrimitive(pTCylinder);
			}
			break;
		}
		m_FSNManager.GetTPrimitive()->SetCurLocator(cb[0],cb[1],cb[2]);
		m_FSNManager.GetTPrimitive()->SaveLocator();
		m_FSNManager.GetTPrimitive()->SetDefLocator();
		m_bShowTPrimitive=true;
	}
	else
	{
		outfile<<"22"<<endl;
		CMatrix44 transmat,p2gmat,g2cmat,compmat,tmpmat;
		double data[16];
		if(m_FSNManager.GetTPrimitive()->GetLocatingStyle()==1)
		{
			if(m_FSNManager.GetTPrimitive()->GetType()==0)
			{
				m_FSNManager.GetTPrimitive()->SetAutoLocator(n);
				m_FSNManager.GetTPrimitive()->SaveLocator();
				ct=m_FSNManager.GetTPrimitive()->GetCurLocator();
				cp=ct-cb;
				data[0]=1;data[1]=0;data[2]=0;data[3]=cp.x();
				data[4]=0;data[5]=1;data[6]=0;data[7]=cp.y();
				data[8]=0;data[9]=0;data[10]=1;data[11]=cp.z();
				data[12]=0;data[13]=0;data[14]=0;data[15]=1;
				transmat.Set(data);
				m_pModel->Transform(transmat);

			}
		}
		else
		{
			m_FSNManager.GetTPrimitive()->SaveLocator();
			CVector3d ltc=m_FSNManager.GetTPrimitive()->GetCurDirection();
			ltc.Normalize();
			CVector3d ltco1=Ortho3(ltc);
			ltco1.Normalize();
			CVector3d ltco2=ltc.Cross(ltco1);
			ltco2.Normalize();
			data[0]=ltc[0];data[1]=ltco1[0];data[2]=ltco2[0];data[3]=0;
			data[4]=ltc[1];data[5]=ltco1[1];data[6]=ltco2[1];data[7]=0;
			data[8]=ltc[2];data[9]=ltco1[2];data[10]=ltco2[2];data[11]=0;
			data[12]=0;data[13]=0;data[14]=0;data[15]=1;
			p2gmat.Set(data);

			CVector3d no1=Ortho3(n);
			no1.Normalize();
			CVector3d no2=n.Cross(no1);
			no2.Normalize();

			data[0]=n[0];data[1]=n[1];data[2]=n[2];data[3]=0;
			data[4]=no1[0];data[5]=no1[1];data[6]=no1[2];data[7]=0;
			data[8]=no2[0];data[9]=no2[1];data[10]=no2[2];data[11]=0;
			data[12]=0;data[13]=0;data[14]=0;data[15]=1;
			g2cmat.Set(data);

			data[0]=1;data[1]=0;data[2]=0;data[3]=-cb.x();
			data[4]=0;data[5]=1;data[6]=0;data[7]=-cb.y();
			data[8]=0;data[9]=0;data[10]=1;data[11]=-cb.z();
			data[12]=0;data[13]=0;data[14]=0;data[15]=1;
			transmat.Set(data);
			compmat=p2gmat.MultRight(g2cmat);
			tmpmat=compmat.MultRight(transmat);

			ct=m_FSNManager.GetTPrimitive()->GetCurLocator();
			data[0]=1;data[1]=0;data[2]=0;data[3]=ct.x();
			data[4]=0;data[5]=1;data[6]=0;data[7]=ct.y();
			data[8]=0;data[9]=0;data[10]=1;data[11]=ct.z();
			data[12]=0;data[13]=0;data[14]=0;data[15]=1;
			transmat.Set(data);

			compmat=transmat.MultRight(tmpmat);
			m_pModel->Transform(compmat);
		}

	}

	outfile<<"3"<<endl;

	m_OpenGLManager.BeginContext();
	m_pModel->BuildList();
	m_OpenGLManager.EndContext();

	m_pModel->SetIndex(m_FSNManager.GetComponentNum());
	m_FSNManager.RegisterComponent(m_pModel);

	m_omMode=OM_FUSION;
	m_pModel=NULL;
	outfile<<"4"<<endl;
	delete m_pFrame;
	m_pFrame=NULL;
	m_moType=MO_TPRIMITIVE_LOCATE;
	outfile<<"5"<<endl;
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnImportTmfile()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"TM Files (*.TM)|*.TM||", this);


	int structsize=0; 
	DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion; 
	//检测目前的操作系统，GetVersion具体用法详见MSDN 
	dwVersion = GetVersion(); 
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion))); 
	dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion))); 
	// 如果运行的操作系统是Windows NT/2000 
	if (dwVersion < 0x80000000)  
		structsize =88;//显示新的文件对话框 
	else 
		//运行的操作系统Windows 95/98  
		structsize =76;//显示老的文件对话框 
	fileDlg.m_ofn.lStructSize=structsize; 

	if(fileDlg.DoModal()==IDOK){
		CString filePath = fileDlg.GetPathName();
		if(m_pModel)
			delete m_pModel;
		m_pModel=new CSolid;
		CString fileExt=fileDlg.GetFileExt();
		m_pModel->ReadFromTM(filePath.GetBuffer (20));

		m_omMode=OM_SCISSOR;


		m_OpenGLManager.BeginContext();
		m_pModel->BuildList();
		m_OpenGLManager.EndContext();

		InvalidateRect(NULL,FALSE);
	}	
}

void CGeometryStudioView::OnStrokeStructure()
{
	m_moType=MO_STROKE_STRUCTURE;
}

void CGeometryStudioView::OnToolTest()
{
	int i,j,k,num,anum,start,index,acount;
	CVertex *pVert1, * pVert2,*pVNxt,*pVPre;
	CHalfEdge *pHe;
	CBoundary *pBound;
	vector<CVector3d> baseCoords;
	vector<CVector3d> rawCoords;
	vector<CVector3d> Details;
	vector<CVector3d> ReconLaps;

	CVector3d u,v,w,LapCoord,coord;

	num=m_pModel->GetVertexNum();

	anum=0;
	for (i=0;i<m_pModel->GetBoundaryNum();i++)
	{
		pBound=m_pModel->GetBoundaryByIndex(i);
		CPatch patch;
		patch.ClusteringNRing(*pBound,1);

		anum+=patch.GetVertexNum();

		for (j=0;j<patch.GetVertexNum();j++)
		{
			patch.GetVertexByIndex(j)->SetAnchor(true);
		}

	}


	for (i=0;i<m_pModel->GetVertexNum();i++)
	{
		pVert1=m_pModel->GetVertexByIndex(i);

		rawCoords.push_back(pVert1->GetCoordinate());
	}

	for (i=0;i<m_pModel->GetVertexNum();i++)
	{
		pVert1=m_pModel->GetVertexByIndex(i);
		pVert1->SetIndex(i);
	}

	unsigned long* ijL=new unsigned long[20*num];
	double *vL=new double[20*num];
	double cotA,cotB,ASub,A,rA,weight,tweight,rtweight;

	index=num+1;
	for (i=0;i<m_pModel->GetVertexNum();i++)
	{
		ijL[i]=index;
		pVert1=m_pModel->GetVertexByIndex(i);
		LapCoord.Set(0.0,0.0,0.0);
		A=rA=tweight=rtweight=0.0;
		//if(pVert1->IsAnchor())
		{
			start=index;
			pHe=pVert1->GetOutHalfEdge();
			do 
			{
				pVert2=pHe->GetMate()->GetVertex();

				pVPre=pHe->GetMate()->GetPrevEdge()->GetVertex();
				pVNxt=pHe->GetNextEdge()->GetNextEdge()->GetVertex();

				u=(rawCoords[pVert1->GetIndex()])-(rawCoords[pVPre->GetIndex()]);

				v=(rawCoords[pVert2->GetIndex()])-(rawCoords[pVPre->GetIndex()]);

				w=u.Cross(v);

				ASub=w.Length()/2;

				cotA=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

				u=(rawCoords[pVert1->GetIndex()])-(rawCoords[pVNxt->GetIndex()]);

				v=(rawCoords[pVert2->GetIndex()])-(rawCoords[pVNxt->GetIndex()]);

				w=u.Cross(v);

				cotB=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

				weight=cotA+cotB;
				rtweight+=weight;
				rA+=ASub;

				vL[index]=weight;
				ijL[index++]=pVert2->GetIndex();

				LapCoord+=weight*(rawCoords[pVert2->GetIndex()]);


				pHe=pVert1->NextOutHalfEdge(pHe); 

			} while(pHe!=pVert1->GetOutHalfEdge());
			LapCoord-=rtweight*(rawCoords[pVert1->GetIndex()]);
			LapCoord/=(4*rA);
			ReconLaps.push_back(LapCoord);
			for(j=start;j<index;j++)
				vL[j]/=(4*rA);
			vL[i]=-rtweight/(4*rA);

		}
	}

	ijL[num]=index;
	vL[num]=0.0;


	nlNewContext() ;
	if(nlInitExtension("SUPERLU")) {
		nlSolverParameteri(NL_SOLVER, NL_PERM_SUPERLU_EXT) ;
	} else {
		nlSolverParameteri(NL_SOLVER, NL_CG) ;
		nlSolverParameteri(NL_PRECONDITIONER, NL_PRECOND_JACOBI) ;
	}
	nlSolverParameteri(NL_NB_VARIABLES, 3*num) ;
	nlSolverParameteri(NL_LEAST_SQUARES, NL_TRUE) ;
	nlSolverParameteri(NL_MAX_ITERATIONS, 15*num) ;
	nlSolverParameterd(NL_THRESHOLD, 1e-10) ;	

	nlBegin(NL_SYSTEM);


	acount=0;

	for(i=0; i<num; i++) 
	{
		pVert1=m_pModel->GetVertexByIndex(i);
		coord=(rawCoords[pVert1->GetIndex()]);
		nlSetVariable(i,coord[0]);
		nlSetVariable(num+i,coord[1]);
		nlSetVariable(2*num+i,coord[2]);
	}	

	nlBegin(NL_MATRIX);

	for (i=0;i<3;i++)
	{
		for (j=0;j<num;j++)
		{
			pVert1=m_pModel->GetVertexByIndex(j);
			coord=ReconLaps[pVert1->GetIndex()];
			nlRowParameterd(NL_RIGHT_HAND_SIDE,coord[i]);
			nlBegin(NL_ROW) ;
			for (k=ijL[j];k<ijL[j+1];k++)
			{
				nlCoefficient(i*num+ijL[k],vL[k]);
			}
			nlCoefficient(i*num+j,vL[j]);
			nlEnd(NL_ROW) ;
		}
		for (j=0;j<num;j++)
		{
			pVert1=m_pModel->GetVertexByIndex(j);
			if(pVert1->IsAnchor())
			{
				coord=rawCoords[pVert1->GetIndex()];
				nlRowParameterd(NL_RIGHT_HAND_SIDE,coord[i]);
				nlBegin(NL_ROW) ;
				nlCoefficient(i*num+j,1.0);
				nlEnd(NL_ROW) ;

			}
		}
	}
	nlEnd(NL_MATRIX);

	nlEnd(NL_SYSTEM);

	nlSolve() ;


	for(i=0;i<num;i++)
	{
		pVert1=m_pModel->GetVertexByIndex(i);
		outfile<<"i="<<i<<endl;
		outfile<<rawCoords[i][0]<<","<<rawCoords[i][1]<<","<<rawCoords[i][2]<<">"<<endl;
		outfile<<nlGetVariable(i)<<","<<nlGetVariable(num+i)<<","<<nlGetVariable(2*num+i)<<">"<<endl;
		pVert1->SetCoordinate(CVector3d(nlGetVariable(i),nlGetVariable(num+i),nlGetVariable(2*num+i)));
	}
	nlDeleteContext(nlGetCurrent());
	delete []ijL;
	delete []vL;

	m_OpenGLManager.BeginContext();
	m_pModel->BuildList();
	m_OpenGLManager.EndContext();
	InvalidateRect(NULL,FALSE);

}

void CGeometryStudioView::UpdateSilhouettePlanes(void)
{
	int i,j;

	int dimx,dimy,dd;
	m_OpenGLManager.GetDimension(dimx,dimy);

	dd=dimx<dimy?dimx:dimy;

	int gap=dd/30;
	int length=dd/3;

	double out[3];
	double wv1[2],wv2[2],wv3[2],wv4[2];
	CVector3d ov1, ov2,ov3,ov4;

	CVector3d v,v1,v2,vm,vse,vpn,vdn,vEyePos,raw,proj;

	CMatrix44 transmat,scalemat,ltgmat,gtlmat,ndp2g_mat,g2ndp_mat,tmpmat,g2pj_mat,npj2g_mat;
	double data[16];

	CVector3d x,y,z;
	double sx,sy,sz;

	ov1=m_pProjectPlane->GetCorner(0);
	ov2=m_pProjectPlane->GetCorner(1);
	ov3=m_pProjectPlane->GetCorner(2);
	ov4=m_pProjectPlane->GetCorner(3);

	x=ov2-ov1;x.Normalize();
	y=ov4-ov1;y.Normalize();
	z.Set(m_pProjectPlane->a(),m_pProjectPlane->b(),m_pProjectPlane->c());
	z.Normalize();

	sx=1.0/((ov2-ov1).Length());
	sy=1.0/((ov4-ov1).Length());
	sz=1.0;

	data[0]=1;data[1]=0;data[2]=0;data[3]=-ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=-ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=-ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	data[0]=x[0];data[1]=x[1];data[2]=x[2];data[3]=0;
	data[4]=y[0];data[5]=y[1];data[6]=y[2];data[7]=0;
	data[8]=z[0];data[9]=z[1];data[10]=z[2];data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	gtlmat.Set(data);

	data[0]=sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=gtlmat.MultRight(transmat);

	g2pj_mat=scalemat.MultRight(tmpmat);


	outfile<<"modify silhouette"<<endl;
	for(i=0;i<m_pStroke->GetPointsNum();i++)
	{
		raw=m_pStroke->GetPoint(i);
		m_pSilhouetteStroke->ModifyPoint(i,g2pj_mat.MultMatVec(raw-((raw-ov1).Dot(z))*z));
	}

	wv1[0]=gap;
	wv1[1]=gap;
	m_OpenGLManager.GetOBJCoord(wv1[0],wv1[1],0.1,out);
	ov1.Set(out);

	wv2[0]=wv1[0]+length;
	wv2[1]=wv1[1];
	m_OpenGLManager.GetOBJCoord(wv2[0],wv2[1],0.1,out);
	ov2.Set(out);

	wv3[0]=wv1[0]+length;
	wv3[1]=wv1[1]+length;
	m_OpenGLManager.GetOBJCoord(wv3[0],wv3[1],0.1,out);
	ov3.Set(out);


	wv4[0]=wv1[0];
	wv4[1]=wv1[1]+length;
	m_OpenGLManager.GetOBJCoord(wv4[0],wv4[1],0.1,out);
	ov4.Set(out);

	vpn=(ov3-ov1).Cross(ov2-ov1);
	vpn.Normalize();

	delete m_pNProjectPlane;
	m_pNProjectPlane=new CPlane(ov1,vpn);

	m_pNProjectPlane->AddCorner(ov1);
	m_pNProjectPlane->AddCorner(ov2);
	m_pNProjectPlane->AddCorner(ov3);
	m_pNProjectPlane->AddCorner(ov4);


	x=ov2-ov1;x.Normalize();
	y=ov4-ov1;y.Normalize();
	z=vpn;
	sx=(ov2-ov1).Length();
	sy=(ov4-ov1).Length();
	sz=1.0;

	data[0]=x[0];data[1]=y[0];data[2]=z[0];data[3]=0;
	data[4]=x[1];data[5]=y[1];data[6]=z[1];data[7]=0;
	data[8]=x[2];data[9]=y[2];data[10]=z[2];data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;

	ltgmat.Set(data);

	data[0]=1;data[1]=0;data[2]=0;data[3]=ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	data[0]=sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=ltgmat.MultRight(scalemat);
	npj2g_mat=transmat.MultRight(tmpmat);

	for (i=0;i<m_pSilhouetteStroke->GetPointsNum();i++)
	{
		raw=m_pSilhouetteStroke->GetPoint(i);
		m_pSilhouetteStroke->ModifyPoint(i,npj2g_mat.MultMatVec(raw));
	}


	ov1=m_pNDepthPlane->GetCorner(0);
	ov2=m_pNDepthPlane->GetCorner(1);
	ov3=m_pNDepthPlane->GetCorner(2);
	ov4=m_pNDepthPlane->GetCorner(3);

	m_pNDepthPlane->SetColor(0.0,1.0,0.0);


	x=ov2-ov1;x.Normalize();
	y=ov4-ov1;y.Normalize();
	z=m_pNDepthPlane->Normal();
	sx=(ov2-ov1).Length();
	sy=(ov4-ov1).Length();
	sz=1.0;

	data[0]=x[0];data[1]=x[1];data[2]=x[2];data[3]=0;
	data[4]=y[0];data[5]=y[1];data[6]=y[2];data[7]=0;
	data[8]=z[0];data[9]=z[1];data[10]=z[2];data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	gtlmat.Set(data);

	data[0]=1;data[1]=0;data[2]=0;data[3]=-ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=-ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=-ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	data[0]=1/sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=1/sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=1/sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=gtlmat.MultRight(transmat);
	g2ndp_mat=scalemat.MultRight(tmpmat);

	delete m_pNDepthPlane;

	wv1[0]=length+2*gap;
	wv1[1]=gap;
	m_OpenGLManager.GetOBJCoord(wv1[0],wv1[1],0.1,out);
	ov1.Set(out);

	wv2[0]=wv1[0]+length;
	wv2[1]=wv1[1];
	m_OpenGLManager.GetOBJCoord(wv2[0],wv2[1],0.1,out);
	ov2.Set(out);

	wv3[0]=wv1[0]+length;
	wv3[1]=wv1[1]+length;
	m_OpenGLManager.GetOBJCoord(wv3[0],wv3[1],0.1,out);
	ov3.Set(out);


	wv4[0]=wv1[0];
	wv4[1]=wv1[1]+length;
	m_OpenGLManager.GetOBJCoord(wv4[0],wv4[1],0.1,out);
	ov4.Set(out);

	vdn=(ov3-ov1).Cross(ov2-ov1);
	vdn.Normalize();

	m_pNDepthPlane=new CPlane(ov1,vdn);

	m_pNDepthPlane->AddCorner(ov1);
	m_pNDepthPlane->AddCorner(ov2);
	m_pNDepthPlane->AddCorner(ov3);
	m_pNDepthPlane->AddCorner(ov4);

	m_pNDepthPlane->SetColor(0.0,1.0,0.0);


	x=ov2-ov1;x.Normalize();
	y=ov4-ov1;y.Normalize();
	z=vdn;
	sx=(ov2-ov1).Length();
	sy=(ov4-ov1).Length();
	sz=1.0;

	data[0]=x[0];data[1]=y[0];data[2]=z[0];data[3]=0;
	data[4]=x[1];data[5]=y[1];data[6]=z[1];data[7]=0;
	data[8]=x[2];data[9]=y[2];data[10]=z[2];data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;

	ltgmat.Set(data);

	data[0]=1;data[1]=0;data[2]=0;data[3]=ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	data[0]=sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=ltgmat.MultRight(scalemat);
	ndp2g_mat=transmat.MultRight(tmpmat);

	CVector3d gcoord,lcoord[6];
	CPoint3 *pcp;

	for (i=0;i<6;i++)
	{		
		lcoord[i]=g2ndp_mat.MultMatVec(m_vecCtrlPolygon[i]->GetCoordinate());
		//lcoord[i][2]=0.0;
	}

	m_vecCtrlPolygon.clear();

	gcoord=ndp2g_mat.MultMatVec(lcoord[0]);
	pcp=new CPoint3;
	pcp->Set(gcoord);
	m_vecCtrlPolygon.push_back(pcp);

	gcoord=ndp2g_mat.MultMatVec(lcoord[1]);
	pcp=new CPoint3;
	pcp->Set(gcoord);
	m_vecCtrlPolygon.push_back(pcp);

	gcoord=ndp2g_mat.MultMatVec(lcoord[2]);
	pcp=new CPoint3;
	pcp->Set(gcoord);
	m_vecCtrlPolygon.push_back(pcp);
	pcp->SetMaterial("Blue");

	gcoord=ndp2g_mat.MultMatVec(lcoord[3]);
	pcp=new CPoint3;
	pcp->Set(gcoord);
	m_vecCtrlPolygon.push_back(pcp);
	pcp->SetMaterial("Blue");

	gcoord=ndp2g_mat.MultMatVec(lcoord[4]);
	pcp=new CPoint3;
	pcp->Set(gcoord);
	m_vecCtrlPolygon.push_back(pcp);

	gcoord=ndp2g_mat.MultMatVec(lcoord[5]);
	pcp=new CPoint3;
	pcp->Set(gcoord);
	m_vecCtrlPolygon.push_back(pcp);

	outfile<<"modify depth"<<endl;

	double t=0.0;
	for (i=0;i<50;i++)
	{
		raw.Set(0.0,0.0,0.0);
		for(j=0;j<6;j++)
			raw+=CalcBernstein(5,j,t)*lcoord[j];
		gcoord=ndp2g_mat.MultMatVec(raw);
		m_pDepthStroke->ModifyPoint(i,gcoord);
		t+=0.02;			
	}


	m_bUpdateSilhouettePlanes=false;


}

void CGeometryStudioView::OnFtmcThroughout()
{
	CSolid *pComponent;
	CBoundary *pBound;
	CVertex *pVertex;
	CVector3d normal,coord,projcoord, vmin,vmax,prevCoord;
	double len,avgLen,totLen,minLen;

	Stopwatch RbfTime;

	unsigned int i,j,k,l,m,n,bnum,pnum,vnum,totConsNum,totVtNum,pieces,step,totBnum;

	CString str;
	CMainFrame *pFrameWnd = (CMainFrame*)AfxGetMainWnd();
	pFrameWnd->m_Progress.SetPos(0);


	str.Format("Fusion using rbf surface");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	outfile<<"RBF Solving"<<endl;

	pieces=0;
	pnum=m_FSNManager.GetComponentNum();
	totConsNum=0;
	totBnum=0;
	vector<CPatch*> vecPatches;
	CPatch *pPatch;
	for(i=0;i<pnum;i++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(i);
		bnum=pComponent->GetBoundaryNum();
		for(j=0;j<bnum;j++)
		{
			pBound=pComponent->GetBoundaryByIndex(j);
			pPatch=new CPatch;
			pPatch->ClusteringNRing(*pBound,2);
			vecPatches.push_back(pPatch);
			pBound->SetIndex(pieces+j);
			totConsNum+=pPatch->GetVertexNum();
		}
		pieces+=bnum;
	}
	totBnum=pieces;
	pieces*=2;

	pieces+=7;

	step=100/pieces;

	totVtNum=totConsNum;
	totConsNum*=3;

	CStroke *pStroke;



	for(i=0;i<m_FSNManager.GetSilhouetteNum();i++)
	{
		pStroke=m_FSNManager.GetSilhouetteByIndex(i);
		totConsNum+=pStroke->GetPointsNum();
	}

	Solver.Initialize(totConsNum);


	vmin.Set(DBL_MAX,DBL_MAX,DBL_MAX);
	vmax.Set(-DBL_MAX,-DBL_MAX,-DBL_MAX);
	minLen=DBL_MAX;
	totLen=0;

	for (i=0;i<vecPatches.size();i++)
	{
		pPatch=vecPatches[i];
		k=0;
		pVertex=pPatch->GetVertexByIndex(k);
		prevCoord=coord=pVertex->GetCoordinate();
		for (l=0;l<3;l++)
		{
			vmax[l]=vmax[l]>coord[l]?vmax[l]:coord[l];
			vmin[l]=vmin[l]<coord[l]?vmin[l]:coord[l];
		}

		Solver.AddConstraint(coord.GetArray(),0.0,0.0);
		normal=pVertex->GetNormal();
		projcoord=coord+normal*0.01;
		Solver.AddConstraint(projcoord.GetArray(),0.01,0.0);
		projcoord=coord-normal*0.01;
		Solver.AddConstraint(projcoord.GetArray(),-0.01,0.0);

		k++;

		for (;k<pPatch->GetVertexNum();k++)
		{
			pVertex=pPatch->GetVertexByIndex(k);
			coord=pVertex->GetCoordinate();

			len=(coord-prevCoord).Length();
			if(minLen>len)minLen=len;
			totLen+=len;
			prevCoord=coord;

			for (l=0;l<3;l++)
			{
				vmax[l]=vmax[l]>coord[l]?vmax[l]:coord[l];
				vmin[l]=vmin[l]<coord[l]?vmin[l]:coord[l];
			}

			Solver.AddConstraint(coord.GetArray(),0.0,0.0);
			normal=pVertex->GetNormal();
			projcoord=coord+normal*0.01;
			Solver.AddConstraint(projcoord.GetArray(),0.01,0.0);
			projcoord=coord-normal*0.01;
			Solver.AddConstraint(projcoord.GetArray(),-0.01,0.0);
		}

	}
	avgLen=totLen/totVtNum;

	for (i=0;i<vecPatches.size();i++)
	{
		delete vecPatches[i];
	}

	for(i=0;i<m_FSNManager.GetSilhouetteNum();i++)
	{
		pStroke=m_FSNManager.GetSilhouetteByIndex(i);
		for(j=0;j<pStroke->GetPointsNum();j++)
		{
			coord=pStroke->GetPoint(j);
			Solver.AddConstraint(coord.GetArray(),0.0,0.0);
		}

	}
	RbfTime.Start();
	Solver.Solve();
	RbfTime.Stop();

	pFrameWnd->m_Progress.OffsetPos(step);

	str.Format("Rbf Fitting Statistics:");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	str.Format("Time=%f",RbfTime.GetTime());
	pFrameWnd->m_wndOutput.FillOperationInfo(str);
	str.Format("Constraints Number=%d",totConsNum);
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	str.Format("Marching Cube Extraction");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);


	for (i=0;i<m_FSNManager.GetComponentNum();i++)
		m_FSNManager.GetComponentByIndex(i)->BoundaryOptimization();

	Stopwatch MCTime,WrinkleTime,RemeshTime;
	int sizeX,sizeY,sizeZ;

	if(avgLen>1.5*minLen)avgLen=1.5*minLen;

	avgLen*=0.8;

	len=vmax[0]-vmin[0];
	sizeX=(int)(len/avgLen)+1;

	len=vmax[1]-vmin[1];
	sizeY=(int)(len/avgLen)+1;

	len=vmax[2]-vmin[2];
	sizeZ=(int)(len/avgLen)+1;

	MarchingCubes mc ;

	MCTime.Start();
	mc.set_resolution( sizeX,sizeY,sizeZ) ;
	mc.set_boundingbox(vmin,vmax);
	mc.init_all();

	for (i=0;i<mc.size_x();i++)
	{
		for (j=0;j<mc.size_y();j++)
		{
			for (k=0;k<mc.size_z();k++)
			{
				coord=mc.get_grid_vert(i,j,k);
				mc.set_data( (float)Solver.GetValueAt(coord),i,j,k ) ;
			}
		}
	}

	mc.determine_cubes_cases();

	vector<CPoint3> vecPts;
	vector<CTriangle3> vecTris1;
	CPoint3 pt;
	CTriangle3 tri;
	CFace *pFace;
	CHalfEdge *pHe;

	outfile<<"clarifying cubes"<<endl;
	for (i=0;i<pnum;i++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(i);
		bnum=pComponent->GetBoundaryNum();
		for (j=0;j<bnum;j++)
		{
			pBound=pComponent->GetBoundaryByIndex(j);

			vnum=pBound->GetVertexNum();
			for (k=0;k<vnum;k++)
			{
				pVertex=pBound->GetVertexByIndex(k);
				pt.Set(pVertex->GetCoordinate());				
				vecPts.push_back(pt);
			}

			CPatch patch;
			patch.ClusteringNRing(*pBound,2);


			outfile<<"converting patch: "<<patch.GetFaceNum()<<endl;
			for(k=0;k<patch.GetFaceNum();k++)
			{
				pFace=patch.GetFaceByIndex(k);

				pHe=pFace->GetFirstEdge();
				pVertex=pHe->GetVertex();

				coord=pVertex->GetCoordinate();
				tri.Set_A(coord);

				pHe=pFace->GetNextEdge(pHe);
				pVertex=pHe->GetVertex();
				coord=pVertex->GetCoordinate();
				tri.Set_B(coord);

				pHe=pFace->GetNextEdge(pHe);
				pVertex=pHe->GetVertex();
				coord=pVertex->GetCoordinate();
				tri.Set_C(coord);
				vecTris1.push_back(tri);
			}

			//outfile<<"clarify"<<endl;

			mc.clarify_cubes(vecPts,vecTris1);
			//outfile<<"clarify ok"<<endl;
			vecPts.clear();
			vecTris1.clear();

		}
	}


	mc.run() ;
	MCTime.Stop(); 
	str.Format("Marching Cube Extraction Statistics:");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);
	str.Format("Time=%f seconds",MCTime.GetTime());
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	mc.clean_temps();
	outfile<<"Importing"<<endl;
	CSolid* pTComponent=new CSolid;
	pTComponent->ImportFromMC(mc,false);
	mc.clean_all();
	pTComponent->SetMaterial("Polished silver");
	pTComponent->BoundaryOptimization();



	m_FSNManager.RegisterTComponent(pTComponent);

	str.Format("Triangle Number=%d ",pTComponent->GetFaceNum());
	pFrameWnd->m_wndOutput.FillOperationInfo(str);
	str.Format("Vertex Number=%d ",pTComponent->GetVertexNum());
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	pFrameWnd->m_Progress.OffsetPos(2*step);


	str.Format("Zippering the gap using Wrinkle Technique");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	int *BFlag=new int[totBnum];
	for (i=0;i<totBnum;i++)
	{
		BFlag[i]=0;
	}


	double minTotLen;
	CVertex *pV1,*pV2;
	CBoundary *pBound1,*pBound2,*pBoundTmp;
	for (i=0;i<pTComponent->GetBoundaryNum();i++)
	{
		pBound1=pTComponent->GetBoundaryByIndex(i);
		minTotLen=DBL_MAX;
		for (j=0;j<pnum;j++)
		{
			pComponent=m_FSNManager.GetComponentByIndex(j);
			for (k=0;k<pComponent->GetBoundaryNum();k++)
			{
				pBoundTmp=pComponent->GetBoundaryByIndex(k);
				if (BFlag[pBoundTmp->GetIndex()])continue;	
				totLen=0;
				for(m=0;m<pBound1->GetVertexNum();m++)
				{
					pV1=pBound1->GetVertexByIndex(m);
					minLen=DBL_MAX;
					for (n=0;n<pBoundTmp->GetVertexNum();n++)
					{
						pV2=pBoundTmp->GetVertexByIndex(n);
						len=(pV1->GetCoordinate()-pV2->GetCoordinate()).Length();
						if(minLen>len)minLen=len;
					}
					totLen+=minLen;
				}
				if(minTotLen>totLen)
				{
					minTotLen=totLen;
					pBound2=pBoundTmp;
				}
			}
		}
		BFlag[pBound2->GetIndex()]=1;
		CWrinkle wrinkle;
		wrinkle.ImportCurvePair(pBound1,pBound2);
		wrinkle.Initialize();
		WrinkleTime.Reset();
		WrinkleTime.Start();
		if(m_pWPatch==NULL)
			m_pWPatch=wrinkle.GlobalMaximumTriRegularityBridgeTriangulation();
		else
		{
			m_pWPatch->Merge(wrinkle.GlobalMaximumTriRegularityBridgeTriangulation());
		}
		WrinkleTime.Stop();
		str.Format("Wrinkle <Pass-%d> Statistics:",i);
		pFrameWnd->m_wndOutput.FillOperationInfo(str);

		str.Format("Wrinkling Time=%f",WrinkleTime.GetTime());
		pFrameWnd->m_wndOutput.FillOperationInfo(str);

		str.Format("Node number of Curve one=%d:",pBound1->GetVertexNum());
		pFrameWnd->m_wndOutput.FillOperationInfo(str);

		str.Format("Node number of Curve two=%d:",pBound2->GetVertexNum());
		pFrameWnd->m_wndOutput.FillOperationInfo(str);
		pFrameWnd->m_Progress.OffsetPos(2*step);
	}

	delete []BFlag;

	pTComponent->ClearBoundary();
	pTComponent->Merge(m_pWPatch);

	for (j=0;j<m_FSNManager.GetComponentNum();j++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(j);
		for (k=0;k<pComponent->GetBoundaryNum();k++)
		{
			pBound=pComponent->GetBoundaryByIndex(k);
			pTComponent->AddBoundary(pBound);
		}
	}

	str.Format("Remeshing");	
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	str.Format("Remeshing Statistics:");	
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	str.Format("Triangle Number <Before>=%d",pTComponent->GetFaceNum());	
	pFrameWnd->m_wndOutput.FillOperationInfo(str);
	str.Format("Vertex Number <Before>=%d",pTComponent->GetVertexNum());	
	pFrameWnd->m_wndOutput.FillOperationInfo(str);


	outfile<<"Remeshing"<<endl;

	CAreaEqualizationRemesh remesh;
	remesh.SetEntity(pTComponent);

	RemeshTime.Start();
	int ei=remesh.Run(avgLen);
	RemeshTime.Stop();
	
	str.Format("Triangle Number <After>=%d",pTComponent->GetFaceNum());	
	pFrameWnd->m_wndOutput.FillOperationInfo(str);
	str.Format("Vertex Number <After>=%d",pTComponent->GetVertexNum());	
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	//str.Format("Remeshing Time=%f",RemeshTime.GetTime());
	//pFrameWnd->m_wndOutput.FillOperationInfo(str);

	//pFrameWnd->m_Progress.OffsetPos(4*step);

	//outfile<<"Adjust boundary"<<endl;
	//for (i=0;i<pTComponent->GetBoundaryNum();i++)
	//{
	//	CPatch patch;
	//	pBound1=pTComponent->GetBoundaryByIndex(i);
	//	patch.ClusteringNRing(*pBound1,3,2);
	//	for (j=0;j<patch.GetVertexNum();j++)
	//	{
	//		pVertex=patch.GetVertexByIndex(j);
	//		coord=pVertex->GetCoordinate();
	//		projcoord=Solver.Project(coord);
	//		pVertex->SetCoordinate(coord);
	//		normal=Solver.GradientAt(projcoord);
	//		pVertex->SetNormal(normal);

	//	}
	//	//patch.Clear();

	//}
	
	m_OpenGLManager.BeginContext();
	pTComponent->BuildList();
	for (i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		m_FSNManager.GetComponentByIndex(i)->BuildList();
	}
	m_OpenGLManager.EndContext();
	InvalidateRect(NULL,FALSE);
	pFrameWnd->m_Progress.SetPos(100);

}

void CGeometryStudioView::OnFtmcMarching()
{
	CSolid *pComponent;
	CBoundary *pBound;
	CVertex *pVertex;
	CVector3d normal,coord,projcoord, vmin,vmax,prevCoord;
	double len,avgLen,totLen,minLen;

	Stopwatch RbfTime;

	unsigned int i,j,k,l,m,n,bnum,pnum,vnum,totConsNum,totVtNum,pieces,step,totBnum;

	CString str;
	CMainFrame *pFrameWnd = (CMainFrame*)AfxGetMainWnd();
	pFrameWnd->m_Progress.SetPos(0);


	//Rbf Fusion
	str.Format("Fusion using rbf surface");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	outfile<<"RBF Solving"<<endl;

	pieces=0;
	pnum=m_FSNManager.GetComponentNum();
	totConsNum=0;
	totBnum=0;
	vector<CPatch*> vecPatches;
	CPatch *pPatch;
	for(i=0;i<pnum;i++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(i);
		bnum=pComponent->GetBoundaryNum();
		for(j=0;j<bnum;j++)
		{
			pBound=pComponent->GetBoundaryByIndex(j);
			pPatch=new CPatch;
			pPatch->ClusteringNRing(*pBound,0);
			vecPatches.push_back(pPatch);
			pBound->SetIndex(pieces+j);
			totConsNum+=pPatch->GetVertexNum();
		}
		pieces+=bnum;
	}
	totBnum=pieces;
	pieces*=2;

	pieces+=7;

	step=100/pieces;

	totVtNum=totConsNum;
	totConsNum*=3;

	CStroke *pStroke;

	for(i=0;i<m_FSNManager.GetSilhouetteNum();i++)
	{
		pStroke=m_FSNManager.GetSilhouetteByIndex(i);
		totConsNum+=3*pStroke->GetPointsNum();
	}

	outfile<<"Cons Num:"<<totConsNum<<endl;

	Solver.Initialize(totConsNum);


	vmin.Set(DBL_MAX,DBL_MAX,DBL_MAX);
	vmax.Set(-DBL_MAX,-DBL_MAX,-DBL_MAX);
	minLen=DBL_MAX;
	totLen=0;

	for (i=0;i<vecPatches.size();i++)
	{
		pPatch=vecPatches[i];
		k=0;
		pVertex=pPatch->GetVertexByIndex(k);
		prevCoord=coord=pVertex->GetCoordinate();
		for (l=0;l<3;l++)
		{
			vmax[l]=vmax[l]>coord[l]?vmax[l]:coord[l];
			vmin[l]=vmin[l]<coord[l]?vmin[l]:coord[l];
		}

		Solver.AddConstraint(coord.GetArray(),0.0,0.0);
		normal=pVertex->GetNormal();
		projcoord=coord+normal*0.01;
		Solver.AddConstraint(projcoord.GetArray(),0.01,0.0);
		projcoord=coord-normal*0.01;
		Solver.AddConstraint(projcoord.GetArray(),-0.01,0.0);

		k++;

		for (;k<pPatch->GetVertexNum();k++)
		{
			pVertex=pPatch->GetVertexByIndex(k);
			coord=pVertex->GetCoordinate();

			len=(coord-prevCoord).Length();
			if(minLen>len)minLen=len;
			totLen+=len;
			prevCoord=coord;

			for (l=0;l<3;l++)
			{
				vmax[l]=vmax[l]>coord[l]?vmax[l]:coord[l];
				vmin[l]=vmin[l]<coord[l]?vmin[l]:coord[l];
			}

			Solver.AddConstraint(coord.GetArray(),0.0,0.0);
			normal=pVertex->GetNormal();
			projcoord=coord+normal*0.01;
			Solver.AddConstraint(projcoord.GetArray(),0.01,0.0);
			projcoord=coord-normal*0.01;
			Solver.AddConstraint(projcoord.GetArray(),-0.01,0.0);
		}

	}
	avgLen=totLen/totVtNum;

	for (i=0;i<vecPatches.size();i++)
	{
		delete vecPatches[i];
	}

	for(i=0;i<m_FSNManager.GetSilhouetteNum();i++)
	{
		pStroke=m_FSNManager.GetSilhouetteByIndex(i);
		for(j=0;j<pStroke->GetPointsNum();j++)
		{
			coord=pStroke->GetPoint(j);
			Solver.AddConstraint(coord.GetArray(),0.0,0.0);

			normal=pStroke->GetNormal(j);
			projcoord=coord+normal*0.01;
			Solver.AddConstraint(projcoord.GetArray(),0.01,0.0);
			projcoord=coord-normal*0.01;
			Solver.AddConstraint(projcoord.GetArray(),-0.01,0.0);
		}

	}
	outfile<<"Solving"<<endl;
	RbfTime.Start();
	Solver.Solve();
	RbfTime.Stop();

	pFrameWnd->m_Progress.OffsetPos(step);

	str.Format("Rbf Fitting Statistics:");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	str.Format("Time=%f",RbfTime.GetTime());
	pFrameWnd->m_wndOutput.FillOperationInfo(str);
	str.Format("Constraints Number=%d",totConsNum);
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	str.Format("Marching Cube Extraction");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);


	for (i=0;i<m_FSNManager.GetComponentNum();i++)
		m_FSNManager.GetComponentByIndex(i)->BoundaryOptimization();

	outfile<<"Marching Cube"<<endl;
	Stopwatch MCTime,WrinkleTime,RemeshTime;
	int sizeX,sizeY,sizeZ;
	MarchingCubes mc;
	//if(avgLen>1.5*minLen)avgLen=1.5*minLen;

	avgLen*=0.15;

	//outfile<<::

	len=vmax[0]-vmin[0];
	sizeX=(int)(len/avgLen)+1;

	len=vmax[1]-vmin[1];
	sizeY=(int)(len/avgLen)+1;

	len=vmax[2]-vmin[2];
	sizeZ=(int)(len/avgLen)+1;

	outfile<<"resolution: <"<<sizeX<<","<<sizeY<<","<<sizeZ<<">"<<endl;


	MCTime.Start();
	mc.set_resolution( sizeX,sizeY,sizeZ) ;
	mc.set_boundingbox(vmin,vmax);
	mc.init_all();

	for (i=0;i<mc.size_x();i++)
	{
		for (j=0;j<mc.size_y();j++)
		{
			for (k=0;k<mc.size_z();k++)
			{
				coord=mc.get_grid_vert(i,j,k);
				mc.set_data( (float)Solver.GetValueAt(coord),i,j,k ) ;
			}
		}
	}

	mc.determine_cubes_cases();

	vector<CPoint3> vecPts;
	vector<CTriangle3> vecTris1;
	CPoint3 pt;
	CTriangle3 tri;
	CFace *pFace;
	CHalfEdge *pHe;

	for (i=0;i<pnum;i++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(i);
		bnum=pComponent->GetBoundaryNum();
		for (j=0;j<bnum;j++)
		{
			pBound=pComponent->GetBoundaryByIndex(j);
			//POVWriter1.WriteBoundary(pBound);

			vnum=pBound->GetVertexNum();
			//outfile<<"converting bound vert"<<endl;
			for (k=0;k<vnum;k++)
			{
				pVertex=pBound->GetVertexByIndex(k);
				pt.Set(pVertex->GetCoordinate());				
				vecPts.push_back(pt);
			}

			CPatch patch;
			patch.ClusteringNRing(*pBound,2);


			//CString material;
			//material="MineralSapphire|FMelta1|None";
			//POVWriter2.WriteEntity(&patch,material);
			outfile<<"converting patch: "<<patch.GetFaceNum()<<endl;
			for(k=0;k<patch.GetFaceNum();k++)
			{
				pFace=patch.GetFaceByIndex(k);

				pHe=pFace->GetFirstEdge();
				pVertex=pHe->GetVertex();

				coord=pVertex->GetCoordinate();
				tri.Set_A(coord);

				pHe=pFace->GetNextEdge(pHe);
				pVertex=pHe->GetVertex();
				coord=pVertex->GetCoordinate();
				tri.Set_B(coord);

				pHe=pFace->GetNextEdge(pHe);
				pVertex=pHe->GetVertex();
				coord=pVertex->GetCoordinate();
				tri.Set_C(coord);
				vecTris1.push_back(tri);
			}

			outfile<<"clarify"<<endl;

			mc.clarify_cubes(vecPts,vecTris1);
			outfile<<"clarify ok"<<endl;
			vecPts.clear();
			vecTris1.clear();

		}
	}




	mc.run() ;
	MCTime.Stop(); 
	str.Format("Marching Cube Extraction Statistics:");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);
	str.Format("Time=%f seconds",MCTime.GetTime());
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	mc.clean_temps();
	outfile<<"Importing"<<endl;
	CSolid* pTComponent=new CSolid;
	pTComponent->ImportFromMC(mc,false);
	mc.clean_all();


	pTComponent->SetMaterial("Brass");
	pTComponent->BoundaryOptimization();


	m_FSNManager.RegisterTComponent(pTComponent);

	str.Format("Triangle Number=%d ",pTComponent->GetFaceNum());
	pFrameWnd->m_wndOutput.FillOperationInfo(str);
	str.Format("Vertex Number=%d ",pTComponent->GetVertexNum());
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	pFrameWnd->m_Progress.OffsetPos(2*step);


	str.Format("Zippering the gap using Wrinkle Technique");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	int *BFlag=new int[totBnum];
	for (i=0;i<totBnum;i++)
	{
		BFlag[i]=0;
	}

	m_OpenGLManager.BeginContext();
	pTComponent->BuildList();
	for (i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		m_FSNManager.GetComponentByIndex(i)->BuildList();
	}
	m_OpenGLManager.EndContext();
	InvalidateRect(NULL,FALSE);
	pFrameWnd->m_Progress.SetPos(100);
}

void CGeometryStudioView::OnFtmcWrinkle()
{
	double totLen,minTotLen,minLen,len;
	CVertex *pV1,*pV2;
	CBoundary *pBound,*pBound1,*pBound2,*pBoundTmp;
	CSolid *pComponent;
	int i,j,k,m,n,pnum,bnum;
	CSolid *pTComponent=m_FSNManager.GetTComponentByIndex(0);
	//outfile<<"optimize ok"<<endl;

	int totBnum=0;
	pnum=m_FSNManager.GetComponentNum();
	for(i=0;i<pnum;i++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(i);
		bnum=pComponent->GetBoundaryNum();
		for(j=0;j<bnum;j++)
		{
			pBound=pComponent->GetBoundaryByIndex(j);
			pBound->SetIndex(totBnum+j);
		}
		totBnum+=bnum;
	}	

	int *BFlag=new int[totBnum];
	for (i=0;i<totBnum;i++)
	{
		BFlag[i]=0;
	}
	for (i=0;i<pTComponent->GetBoundaryNum();i++)
	{
		pBound1=pTComponent->GetBoundaryByIndex(i);
		minTotLen=DBL_MAX;
		for (j=0;j<pnum;j++)
		{
			pComponent=m_FSNManager.GetComponentByIndex(j);
			for (k=0;k<pComponent->GetBoundaryNum();k++)
			{
				pBoundTmp=pComponent->GetBoundaryByIndex(k);
				if (BFlag[pBoundTmp->GetIndex()])continue;	
				totLen=0;
				for(m=0;m<pBound1->GetVertexNum();m++)
				{
					pV1=pBound1->GetVertexByIndex(m);
					minLen=DBL_MAX;
					for (n=0;n<pBoundTmp->GetVertexNum();n++)
					{
						pV2=pBoundTmp->GetVertexByIndex(n);
						len=(pV1->GetCoordinate()-pV2->GetCoordinate()).Length();
						if(minLen>len)minLen=len;
					}
					totLen+=minLen;
				}
				if(minTotLen>totLen)
				{
					minTotLen=totLen;
					pBound2=pBoundTmp;
				}
			}
		}


		BFlag[pBound2->GetIndex()]=1;
		CWrinkle wrinkle;
		wrinkle.ImportCurvePair(pBound1,pBound2);
		wrinkle.Initialize();
		if(m_pWPatch==NULL)
			m_pWPatch=wrinkle.GlobalMaximumTriRegularityBridgeTriangulation();
		else
		{
			m_pWPatch->Merge(wrinkle.GlobalMaximumTriRegularityBridgeTriangulation());
		}
	}
	delete []BFlag;

	pTComponent->ClearBoundary();
	pTComponent->Merge(m_pWPatch);
	m_pWPatch=NULL;

	for (j=0;j<m_FSNManager.GetComponentNum();j++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(j);
		for (k=0;k<pComponent->GetBoundaryNum();k++)
		{
			pBound=pComponent->GetBoundaryByIndex(k);
			pTComponent->AddBoundary(pBound);
		}
	}

	CSolid *pFusionModel=new CSolid;
	for (i=0;i<m_FSNManager.GetComponentNum();i++)
		pFusionModel->Merge(m_FSNManager.GetComponentByIndex(i));
	pFusionModel->Merge(pTComponent);

	m_FSNManager.RegisterFusionModel(pFusionModel);


	m_OpenGLManager.BeginContext();
	pFusionModel->BuildList();
	pTComponent->BuildList();
	for (i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		m_FSNManager.GetComponentByIndex(i)->BuildList();
	}
	m_OpenGLManager.EndContext();
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnFtmcRemesh()
{
	CAreaEqualizationRemesh remesh;

	CSolid *pComponent;
	CSolid *pTComponent=m_FSNManager.GetTComponentByIndex(0);
	m_FSNManager.GetFusionModel()->Subtract(pTComponent);

	int i,j,k,bnum,pnum,vnum,totVtNum;

	CVertex *pVertex;
	CBoundary *pBound;

	CVector3d prevCoord, coord;
	double avgLen,totLen;

	totLen=0;
	totVtNum=0;
	pnum=m_FSNManager.GetComponentNum();
	for (i=0;i<pnum;i++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(i);
		bnum=pComponent->GetBoundaryNum();
		for (j=0;j<bnum;j++)
		{
			pBound=pComponent->GetBoundaryByIndex(j);
			vnum=pBound->GetVertexNum();
			totVtNum+=vnum;

			k=0;
			pVertex=pBound->GetVertexByIndex(k);
			prevCoord=pVertex->GetCoordinate();

			k++;

			for (;k<vnum;k++)
			{
				pVertex=pBound->GetVertexByIndex(k);
				coord=pVertex->GetCoordinate();

				totLen+=(coord-prevCoord).Length();
				prevCoord=coord;
			}
		}
	}

	avgLen=totLen/totVtNum;

	remesh.SetEntity(pTComponent);

	int ei=remesh.Run(avgLen);

	m_FSNManager.GetFusionModel()->Merge(pTComponent);

	m_OpenGLManager.BeginContext();
	pTComponent->BuildList();
	m_FSNManager.GetFusionModel()->BuildList();
	m_OpenGLManager.EndContext();
	InvalidateRect(NULL,FALSE);

	outfile<<"inv"<<endl;
}

void CGeometryStudioView::OnTprimitiveProperty()
{
	switch(m_FSNManager.GetTPrimitive()->GetType())
	{
	case 0:
		{
			CTransitionSphere *pTSphere=(CTransitionSphere*)m_FSNManager.GetTPrimitive();
			CTSphereDialog dlg(pTSphere);
			if(dlg.DoModal()==IDOK)
			{
				CVector3d dir,nLoc,oLoc;

				double r=dlg.GetRadius();
				pTSphere->SetRadius(r);

				CVector3d ocnt=pTSphere->GetCenter();
				CVector3d cnt=dlg.GetCenter();
				pTSphere->SetCenter(cnt);

				int num=pTSphere->GetLocatorNum();
				if(num>1)
				{
					int i;

					CMatrix44 transmat;
					double data[16];
					for (i=1;i<num;i++)
					{
						oLoc=pTSphere->GetLocatorByIndex(i);

						dir=oLoc-ocnt;
						dir.Normalize();
						nLoc=cnt+r*dir;
						pTSphere->ModifyLocator(i,nLoc);

						data[0]=1;data[1]=0;data[2]=0;data[3]=nLoc[0]-oLoc[0];
						data[4]=0;data[5]=1;data[6]=0;data[7]=nLoc[1]-oLoc[1];
						data[8]=0;data[9]=0;data[10]=1;data[11]=nLoc[2]-oLoc[2];
						data[12]=0;data[13]=0;data[14]=0;data[15]=1;
						transmat.Set(data);

						CSolid *pComponent=m_FSNManager.GetComponentByIndex(i);
						pComponent->Transform(transmat);

						m_OpenGLManager.BeginContext();
						pComponent->BuildList();
						m_OpenGLManager.EndContext();


					}
				}
			}	

		}
		break;
	case 1:
		{
			CTransitionCylinder *pTCylinder=(CTransitionCylinder*)m_FSNManager.GetTPrimitive();
			CTCylinderDialog dlg(pTCylinder);
			if(dlg.DoModal()==IDOK)
			{
				CVector3d dir,nLoc,oLoc;
				//double height=dlg.GetHeight();
				//pTCylinder->SetHeight(height);

				double br=dlg.GetBRadius();
				double obr=pTCylinder->GetBottomRadius();
				double ratio=br/obr;
				double otr=pTCylinder->GetTopRadius();
				double height=pTCylinder->GetHeight();
				pTCylinder->SetBottomRadius(br);
				pTCylinder->SetTopRadius(otr*ratio);
				
				double offset=((br-obr)/(obr-otr))*(height*(obr-otr))/(sqrt(height*height+(obr-otr)*(obr-otr)));


				int num=pTCylinder->GetLocatorNum();
				if(num>1)
				{
					int i;

					CMatrix44 transmat;
					double data[16];
					for (i=2;i<num;i++)
					{
						oLoc=pTCylinder->GetLocatorByIndex(i);

						dir=pTCylinder->GetLocDirectionByIndex(i);

						nLoc=oLoc-offset*dir;
						pTCylinder->ModifyLocator(i,nLoc);

						data[0]=1;data[1]=0;data[2]=0;data[3]=nLoc[0]-oLoc[0];
						data[4]=0;data[5]=1;data[6]=0;data[7]=nLoc[1]-oLoc[1];
						data[8]=0;data[9]=0;data[10]=1;data[11]=nLoc[2]-oLoc[2];
						data[12]=0;data[13]=0;data[14]=0;data[15]=1;
						transmat.Set(data);

						CSolid *pComponent=m_FSNManager.GetComponentByIndex(i);
						pComponent->Transform(transmat);

						m_OpenGLManager.BeginContext();
						pComponent->BuildList();
						m_OpenGLManager.EndContext();

					}
				}
			}	


		}
		break;
	}
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnOmodeFusion()
{
	m_omMode=OM_FUSION;
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnOmodeScissor()
{
	m_omMode=OM_SCISSOR;
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnUpdateOmodeFusion(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_omMode==OM_FUSION);
}

void CGeometryStudioView::OnUpdateOmodeScissor(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_omMode==OM_SCISSOR);
}

void CGeometryStudioView::OnSmodePart()
{
	m_smMode=SM_PART;
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnUpdateSmodePart(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_smMode==SM_PART);
}

void CGeometryStudioView::OnSmodeWhole()
{
	m_smMode=SM_WHOLE;
	InvalidateRect(NULL,FALSE);

}

void CGeometryStudioView::OnUpdateSmodeWhole(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_smMode==SM_WHOLE);
}

void CGeometryStudioView::OnConfigurationLoad()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"CAM Files (*.CAM)|*.CAM||", this);


	int structsize=0; 
	DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion; 
	//检测目前的操作系统，GetVersion具体用法详见MSDN 
	dwVersion = GetVersion(); 
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion))); 
	dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion))); 
	// 如果运行的操作系统是Windows NT/2000 
	if (dwVersion < 0x80000000)  
		structsize =88;//显示新的文件对话框 
	else 
		//运行的操作系统Windows 95/98  
		structsize =76;//显示老的文件对话框 
	fileDlg.m_ofn.lStructSize=structsize; 

	if(fileDlg.DoModal()==IDOK){
		CString filePath = fileDlg.GetPathName();
		m_OpenGLManager.LoadCameraFile(filePath.GetBuffer(20));
		InvalidateRect(NULL,FALSE);
	}	
}

void CGeometryStudioView::OnConfigurationSave()
{
	CFileDialog fileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"CAM Files (*.CAM)|*.CAM||", this);


	int structsize=0; 
	DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion; 
	//检测目前的操作系统，GetVersion具体用法详见MSDN 
	dwVersion = GetVersion(); 
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion))); 
	dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion))); 
	// 如果运行的操作系统是Windows NT/2000 
	if (dwVersion < 0x80000000)  
		structsize =88;//显示新的文件对话框 
	else 
		//运行的操作系统Windows 95/98  
		structsize =76;//显示老的文件对话框 
	fileDlg.m_ofn.lStructSize=structsize; 

	if(fileDlg.DoModal()==IDOK){
		CString filePath = fileDlg.GetPathName();
		m_OpenGLManager.SaveCameraFile(filePath.GetBuffer(20));

		InvalidateRect(NULL,FALSE);
	}	
}

void CGeometryStudioView::OnFpsSampling()
{
	CSolid *pComponent;
	CBoundary *pBound;
	CVertex *pVertex;
	CVector3d normal,coord,projcoord, vmin,vmax,prevCoord;
	double len,avgLen,totLen,minLen;

	Stopwatch RbfTime;

	unsigned int i,j,k,l,m,n,bnum,pnum,vnum,totConsNum,totVtNum,pieces,step,totBnum;

	CString str;
	CMainFrame *pFrameWnd = (CMainFrame*)AfxGetMainWnd();
	pFrameWnd->m_Progress.SetPos(0);


	//Rbf Fusion
	str.Format("Fusion using rbf surface");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	outfile<<"RBF Solving"<<endl;

	pieces=0;
	pnum=m_FSNManager.GetComponentNum();
	totConsNum=0;
	totBnum=0;
	vector<CPatch*> vecPatches;
	CPatch *pPatch;
	for(i=0;i<pnum;i++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(i);
		bnum=pComponent->GetBoundaryNum();
		for(j=0;j<bnum;j++)
		{
			pBound=pComponent->GetBoundaryByIndex(j);
			pPatch=new CPatch;
			pPatch->ClusteringNRing(*pBound,2);
			vecPatches.push_back(pPatch);
			pBound->SetIndex(pieces+j);
			totConsNum+=pPatch->GetVertexNum();
		}
		pieces+=bnum;
	}
	totBnum=pieces;
	pieces*=2;

	pieces+=7;

	step=100/pieces;

	totVtNum=totConsNum;
	totConsNum*=3;

	CStroke *pStroke;

	for(i=0;i<m_FSNManager.GetSilhouetteNum();i++)
	{
		pStroke=m_FSNManager.GetSilhouetteByIndex(i);
		totConsNum+=pStroke->GetPointsNum();
	}

	Solver.Initialize(totConsNum);


	vmin.Set(DBL_MAX,DBL_MAX,DBL_MAX);
	vmax.Set(-DBL_MAX,-DBL_MAX,-DBL_MAX);
	minLen=DBL_MAX;
	totLen=0;

	for (i=0;i<vecPatches.size();i++)
	{
		pPatch=vecPatches[i];
		k=0;
		pVertex=pPatch->GetVertexByIndex(k);
		prevCoord=coord=pVertex->GetCoordinate();
		for (l=0;l<3;l++)
		{
			vmax[l]=vmax[l]>coord[l]?vmax[l]:coord[l];
			vmin[l]=vmin[l]<coord[l]?vmin[l]:coord[l];
		}

		Solver.AddConstraint(coord.GetArray(),0.0,0.0);
		normal=pVertex->GetNormal();
		projcoord=coord+normal*0.01;
		Solver.AddConstraint(projcoord.GetArray(),0.01,0.0);
		projcoord=coord-normal*0.01;
		Solver.AddConstraint(projcoord.GetArray(),-0.01,0.0);

		k++;

		for (;k<pPatch->GetVertexNum();k++)
		{
			pVertex=pPatch->GetVertexByIndex(k);
			coord=pVertex->GetCoordinate();

			len=(coord-prevCoord).Length();
			if(minLen>len)minLen=len;
			totLen+=len;
			prevCoord=coord;

			for (l=0;l<3;l++)
			{
				vmax[l]=vmax[l]>coord[l]?vmax[l]:coord[l];
				vmin[l]=vmin[l]<coord[l]?vmin[l]:coord[l];
			}

			Solver.AddConstraint(coord.GetArray(),0.0,0.0);
			normal=pVertex->GetNormal();
			projcoord=coord+normal*0.01;
			Solver.AddConstraint(projcoord.GetArray(),0.01,0.0);
			projcoord=coord-normal*0.01;
			Solver.AddConstraint(projcoord.GetArray(),-0.01,0.0);
		}

	}
	avgLen=totLen/totVtNum;

	for (i=0;i<vecPatches.size();i++)
	{
		delete vecPatches[i];
	}

	for(i=0;i<m_FSNManager.GetSilhouetteNum();i++)
	{
		pStroke=m_FSNManager.GetSilhouetteByIndex(i);
		for(j=0;j<pStroke->GetPointsNum();j++)
		{
			coord=pStroke->GetPoint(j);
			Solver.AddConstraint(coord.GetArray(),0.0,0.0);
		}

	}
	RbfTime.Start();
	Solver.Solve();
	RbfTime.Stop();

	pFrameWnd->m_Progress.OffsetPos(step);

	str.Format("Rbf Fitting Statistics:");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	str.Format("Time=%f",RbfTime.GetTime());
	pFrameWnd->m_wndOutput.FillOperationInfo(str);
	str.Format("Constraints Number=%d",totConsNum);
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	str.Format("Particle System Sampling");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);


	int vpnum,tnum=0;
	double vlen,vplen;

	CVertex *pVertex1,*pVertex2;

	CParticleSamplingSys *pPS=new CParticleSamplingSys(&Solver,0.3,avgLen);
	m_FSNManager.RegisterPS(pPS);

	str.Format("Initialize Particle System OK");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	vlen=0.1*avgLen;

	for(i=0;i<pnum;i++)
	{
		pComponent=m_FSNManager.GetComponentByIndex(i);
		bnum=pComponent->GetBoundaryNum();
		for(j=0;j<bnum;j++)
		{
			pBound=pComponent->GetBoundaryByIndex(j);
			vnum=pBound->GetVertexNum();
			for(k=0;k<vnum;k++)
			{
				pVertex1=pBound->GetVertexByIndex(k);
				CVector3d bparticle=pVertex1->GetCoordinate();
				pPS->InsertParticle(&bparticle,0.6*len,0);
				l=k+1;
				if(l==vnum)l=0;
				pVertex2=pBound->GetVertexByIndex(l);
				CVector3d bnparticle=pVertex2->GetCoordinate();
				vplen=(bparticle-bnparticle).Length();
				vpnum=vplen/vlen;
				if(vpnum<2)vpnum=2;
				for (l=0;l<vpnum;l++)
				{
					CVector3d vparticle=bparticle*(1-(((double)l)/((double)vpnum+1)))+bnparticle*(((double)l)/((double)vpnum+1));
					pPS->InsertParticle(&vparticle,0.6*len,1);
				}
			}
		}
	}
	str.Format("Insert Boundary and Virtual Particles OK");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	CVector3d coord1,coord2,vEyePos,v;
	double v1,v2,val;
	double t;
	double dst;
	double THRESHOLD=0.0001;

	vEyePos=m_OpenGLManager.GetEyePos();
	for(i=0;i<m_pStroke->GetPointsNum();i++)
	{
		coord1=m_pStroke->GetPoint(i);

		v=coord1-vEyePos;

		v.Normalize();

		t=0;

		v1=Solver.GetValueAt(coord1);
		ASSERT(v1>0);
		for(j=0;j<1000;j++)
		{
			coord2=coord1+v*t;
			v2=Solver.GetValueAt(coord2);
			if(v2<=THRESHOLD)break;
			t+=0.01;
		}
		if(v2<=THRESHOLD)
		{
			if(v2>-THRESHOLD&&v2<THRESHOLD)
			{
				if(coord2[0]>vmax[0]||coord2[1]>vmax[1]||coord2[2]>vmax[2]||coord2[0]<vmin[0]||coord2[1]<vmin[1]||coord2[2]<vmin[2])
					continue;
				else
					pPS->InsertParticle(&coord2,len,2);
			}
			else
			{
				coord=(coord1+coord2)/2;
				val=Solver.GetValueAt(coord);
				while (val<-THRESHOLD||val>THRESHOLD) 
				{
					if(val>0)
						coord1=coord;
					else
						coord2=coord;
					coord=(coord1+coord2)/2;
					val=Solver.GetValueAt(coord);
				}
				if(coord[0]>vmax[0]||coord[1]>vmax[1]||coord[2]>vmax[2]||coord[0]<vmin[0]||coord[1]<vmin[1]||coord[2]<vmin[2])
					continue;
				else
					pPS->InsertParticle(&coord,len,2);
			}
		}
		else
			continue;
	}
	str.Format("Insert Seed Particles OK");
	pFrameWnd->m_wndOutput.FillOperationInfo(str);

	for (int i=0;i<1000;i++)
	{
		pPS->UpdateParticles();
	}
	//SetTimer(1,33,NULL);

}

void CGeometryStudioView::OnFpsReconstruct()
{
	// TODO: Add your command handler code here
}

void CGeometryStudioView::OnDetailrecoveryStructure()
{


	double dist,minDist=DBL_MAX;
	CVertex *pV,*pNV;
	int i,j;

	CStrokeProcess SP;
	SP.BSplineFitting(m_pStroke);




	CVector3d centroid;
	CFace *pF,*pSF;
	CHalfEdge *pFHe,*pHe;

	pSF=NULL;
	minDist=DBL_MAX;

	CVector3d coord,*bnorm;
	vector<CVector3d*> vecNorms;

	int numCriticalPts=0;

	bool known=true;

	for (i=0;i<m_pStroke->GetPointsNum();i++)
	{
		coord=m_pStroke->GetPoint(i);

		pSF=NULL;

		minDist=DBL_MAX;

		for (j=0;j<m_FSNManager.GetFusionModel()->GetFaceNum();j++)
		{
			pF=m_FSNManager.GetFusionModel()->GetFaceByIndex(j);
			centroid=pF->GetCentroid();
			dist=(coord-centroid).Length();
			if(dist<minDist)
			{
				minDist=dist;
				pSF=pF;
			}
		}

		m_pStroke->AddNormal(pSF->GetNormal());

		if(known!=pSF->IsKnown())
		{
			criticalPts[numCriticalPts++]=i;
			known=pSF->IsKnown();
		}
	}
	m_bPropagate=true;

	double l=0.08;
	int num=((criticalPts[1]-criticalPts[0])*m_pStroke->Length()*0.02)/l;
	double deltat=((criticalPts[1]-criticalPts[0])*0.02)/num;

	double t;
	int tindex;

	UKVs.push_back(criticalPts[0]);

	for(i=1;i<num;i++)
	{
		t=criticalPts[0]*0.02+i*deltat;
		tindex=(int)(t*50);
		UKVs.push_back(tindex);
	}

	UKVs.push_back(criticalPts[1]);

	t=criticalPts[0]*0.02-deltat*1.5;
	while(t>0.1)
	{
		tindex=(int)(t*50);
		KVs.push_back(tindex);
		t-=deltat/3;
	}
	t=criticalPts[1]*0.02+deltat*1.5;

	while(t<0.9)
	{
		tindex=(int)(t*50);
		KVs.push_back(tindex);
		t+=deltat/3;
	}

	double m_dXLen,m_dYLen,minDst,dst;
	CVector3d v1,v2,x,y,z,origin;
	//CVector3d cn1,cn2,cn3,cn4;
	CMatrix44 g2lmat,tranmat,tmpmat,commat;
	double data[16];

	m_dXLen=0.1;
	m_dYLen=0.1;



	CPatch *pPatchI,*pPatchO;

	vector<CDetailPatch*> vecKDPs;
	vector<CDetailPatch*> vecUKDPs;
	CDetailPatch* pDPatch,*pMatchDPatch;

	CVertex *pSeedV;

	outfile<<"build KDPs"<<endl;
	for (i=0;i<KVs.size();i++)
	{
		z=m_pStroke->GetNormal(KVs[i]);
		origin=m_pStroke->GetPoint(KVs[i]);

		v1=m_pStroke->GetPoint(KVs[i]-1);
		v2=m_pStroke->GetPoint(KVs[i]+1);

		x=v2-v1;
		x-=(x.Dot(z))*z;
		x.Normalize();

		y=z.Cross(x);
		y.Normalize();

		cn1=origin-1.5*m_dXLen*x-1.5*m_dYLen*y;
		cn2=origin-1.5*m_dXLen*x+1.5*m_dYLen*y;
		cn3=origin+1.5*m_dXLen*x+1.5*m_dYLen*y;
		cn4=origin+1.5*m_dXLen*x-1.5*m_dYLen*y;

		data[0]=1.0;data[1]=0.0;data[2]=0.0;data[3]=-origin[0];
		data[4]=0.0;data[5]=1.0;data[6]=0.0;data[7]=-origin[1];
		data[8]=0.0;data[9]=0.0;data[10]=1.0;data[11]=-origin[2];
		data[12]=0.0;data[13]=0.0;data[14]=0.0;data[15]=1.0;
		tranmat.Set(data);

		data[0]=x[0];data[1]=x[1];data[2]=x[2];data[3]=0.0;
		data[4]=y[0];data[5]=y[1];data[6]=y[2];data[7]=0.0;
		data[8]=z[0];data[9]=z[1];data[10]=z[2];data[11]=0.0;
		data[12]=0.0;data[13]=0.0;data[14]=0.0;data[15]=1.0;
		g2lmat.Set(data);

		commat=g2lmat.MultRight(tranmat);

		minDst=DBL_MAX;
		for (j=0;j<m_FSNManager.GetFusionModel()->GetVertexNum();j++)
		{
			pV=m_FSNManager.GetFusionModel()->GetVertexByIndex(j);
			dst=(pV->GetCoordinate()-origin).Length();
			if(dst<minDst)
			{
				minDst=dst;
				pSeedV=pV;
			}
		}



		pPatchI=new CPatch;
		pPatchI->ClusteringRectangle(pSeedV,CVector3d(-m_dXLen,-m_dYLen,0.0),CVector3d(-m_dXLen,m_dYLen,0.0),CVector3d(m_dXLen,m_dYLen,0.0),CVector3d(m_dXLen,-m_dYLen,0.0),commat);
		pPatchI->ClearFlag();

		pPatchO=new CPatch;
		pPatchO->ClusteringRectangle(pSeedV,CVector3d(-1.5*m_dXLen,-1.5*m_dYLen,0.0),CVector3d(-1.5*m_dXLen,1.5*m_dYLen,0.0),CVector3d(1.5*m_dXLen,1.5*m_dYLen,0.0),CVector3d(1.5*m_dXLen,-1.5*m_dYLen,0.0),commat);
		pPatchO->ExtractBoundary();
		pPatchO->ClearFlag();


		pDPatch=new CDetailPatch;
		pDPatch->SetIPatch(pPatchI);
		pDPatch->SetOPatch(pPatchO);
		pDPatch->SetFrame(x,y,z);

		vector<CVector3d*> vDetails;
		for (j=0;j<pPatchO->GetVertexNum();j++)
			vDetails.push_back(m_vDetails[pPatchO->GetVertexByIndex(j)->GetIndex()]);

		pDPatch->ImportDetails(vDetails);

		pDPatch->DetermineCorner(cn1,cn2,cn3,cn4);
		pDPatch->ClearFlag();
		vecKDPs.push_back(pDPatch);		
	}


	outfile<<"build UKDPs"<<endl;
	for (i=0;i<UKVs.size();i++)
	{
		z=m_pStroke->GetNormal(UKVs[i]);
		origin=m_pStroke->GetPoint(UKVs[i]);

		v1=m_pStroke->GetPoint(UKVs[i]-1);
		v2=m_pStroke->GetPoint(UKVs[i]+1);

		x=v2-v1;
		x-=(x.Dot(z))*z;
		x.Normalize();

		y=z.Cross(x);
		y.Normalize();

		cn1=origin-1.5*m_dXLen*x-1.5*m_dYLen*y;
		cn2=origin-1.5*m_dXLen*x+1.5*m_dYLen*y;
		cn3=origin+1.5*m_dXLen*x+1.5*m_dYLen*y;
		cn4=origin+1.5*m_dXLen*x-1.5*m_dYLen*y;

		data[0]=1.0;data[1]=0.0;data[2]=0.0;data[3]=-origin[0];
		data[4]=0.0;data[5]=1.0;data[6]=0.0;data[7]=-origin[1];
		data[8]=0.0;data[9]=0.0;data[10]=1.0;data[11]=-origin[2];
		data[12]=0.0;data[13]=0.0;data[14]=0.0;data[15]=1.0;
		tranmat.Set(data);

		data[0]=x[0];data[1]=x[1];data[2]=x[2];data[3]=0.0;
		data[4]=y[0];data[5]=y[1];data[6]=y[2];data[7]=0.0;
		data[8]=z[0];data[9]=z[1];data[10]=z[2];data[11]=0.0;
		data[12]=0.0;data[13]=0.0;data[14]=0.0;data[15]=1.0;
		g2lmat.Set(data);

		commat=g2lmat.MultRight(tranmat);

		minDst=DBL_MAX;
		for (j=0;j<m_FSNManager.GetFusionModel()->GetVertexNum();j++)
		{
			pV=m_FSNManager.GetFusionModel()->GetVertexByIndex(j);
			dst=(pV->GetCoordinate()-origin).Length();
			if(dst<minDst)
			{
				minDst=dst;
				pSeedV=pV;
			}
		}

		pPatchI=new CPatch;
		pPatchI->ClusteringRectangle(pSeedV,CVector3d(-m_dXLen,-m_dYLen,0.0),CVector3d(-m_dXLen,m_dYLen,0.0),CVector3d(m_dXLen,m_dYLen,0.0),CVector3d(m_dXLen,-m_dYLen,0.0),commat);
		pPatchI->ClearFlag();

		pPatchO=new CPatch;
		pPatchO->ClusteringRectangle(pSeedV,CVector3d(-1.5*m_dXLen,-1.5*m_dYLen,0.0),CVector3d(-1.5*m_dXLen,1.5*m_dYLen,0.0),CVector3d(1.5*m_dXLen,1.5*m_dYLen,0.0),CVector3d(1.5*m_dXLen,-1.5*m_dYLen,0.0),commat);
		pPatchO->ExtractBoundary();
		pPatchO->ClearFlag();

		pDPatch=new CDetailPatch;
		pDPatch->SetIPatch(pPatchI);
		pDPatch->SetOPatch(pPatchO);
		pDPatch->SetFrame(x,y,z);

		vector<CVector3d*> vDetails;
		for (j=0;j<pPatchO->GetVertexNum();j++)
			vDetails.push_back(m_vDetails[pPatchO->GetVertexByIndex(j)->GetIndex()]);

		pDPatch->ImportDetails(vDetails);
		pDPatch->DetermineCorner(cn1,cn2,cn3,cn4);
		pDPatch->ClearFlag();
		vecUKDPs.push_back(pDPatch);		


	}


	outfile<<"Convex Mapping"<<endl;

	for (i=0;i<vecKDPs.size();i++)
	{
		pDPatch=vecKDPs[i];
		pDPatch->ConvexMapping();
	}

	for (i=0;i<vecUKDPs.size();i++)
	{
		pDPatch=vecUKDPs[i];
		pDPatch->ConvexMapping();
	}

	outfile<<"compare & fill"<<endl;

	double minDiff,diff;
	for (i=0;i<vecUKDPs.size();i++)
	{
		outfile<<i<<endl;
		pDPatch=vecUKDPs[i];
		minDiff=DBL_MAX;
		for (j=0;j<vecKDPs.size();j++)
		{
			outfile<<j<<endl;
			diff=pDPatch->Compare(vecKDPs[j]);
			outfile<<diff<<endl;
			if(diff<minDiff)
			{
				minDiff=diff;
				pMatchDPatch=vecKDPs[j];
			}
			outfile<<"fill"<<endl;
			pDPatch->FillUnknownFromSample(pMatchDPatch);
		}
	}

	outfile<<"apply"<<endl;

	for (i=0;i<m_FSNManager.GetFusionModel()->GetVertexNum();i++)
	{
		m_FSNManager.GetFusionModel()->GetVertexByIndex(i)->SetIndex(i);
	}

	for (i=0;i<vecUKDPs.size();i++)
	{
		vecUKDPs[i]->ApplyDetail(m_vDetails);
	}

	outfile<<"clear memory"<<endl;
	for (i=0;i<vecKDPs.size();i++)
	{
		delete vecKDPs[i];
	}
	for (i=0;i<vecUKDPs.size();i++)
	{
		delete vecUKDPs[i];
	}
	outfile<<"done"<<endl;







}

void CGeometryStudioView::OnLocateAngle()
{
	CTPAngleDialog dlg;
	if(dlg.DoModal()==IDOK)
	{

		double ang=dlg.m_dAngle*M_PI/180;



		int dimx,dimy;
		m_OpenGLManager.GetDimension(dimx,dimy);
		double out[3];
		CVector3d coord1,coord2,coord3,view,dirx,diry;

		m_OpenGLManager.GetOBJCoord(dimx/4.0,dimy/4.0,0.1,out);
		coord1.Set(out);

		m_OpenGLManager.GetOBJCoord(dimx/2.0,dimy/4.0,0.1,out);
		coord2.Set(out);

		m_OpenGLManager.GetOBJCoord(dimx/2.0,dimy/2.0,0.1,out);
		coord3.Set(out);

		view=(coord1-coord2).Cross(coord3-coord2);
		view.Normalize();



		dirx=(m_FSNManager.GetTPrimitive()->GetLocDirectionByIndex(0));
		diry=view.Cross(dirx);
		diry.Normalize();


		if(m_vecSelectedComponents.size())
		{
			CSolid *pComponent=m_vecSelectedComponents[0];
			CVector3d cb=m_FSNManager.GetTPrimitive()->GetLocatorByIndex(pComponent->GetIndex());
			CVector3d n=m_FSNManager.GetTPrimitive()->GetLocDirectionByIndex(pComponent->GetIndex());

			CMatrix44 transmat,p2gmat,g2cmat,compmat,tmpmat;
			double data[16];



			CVector3d ltc=cos(ang)*dirx+sin(ang)*diry;
			ltc.Normalize();

			m_FSNManager.GetTPrimitive()->SetAutoLocator(ltc);
			CVector3d loc=m_FSNManager.GetTPrimitive()->GetCurLocator();

			m_FSNManager.GetTPrimitive()->SetLocator(pComponent->GetIndex(),loc);

			CVector3d ltco1=Ortho3(ltc);
			ltco1.Normalize();
			CVector3d ltco2=ltc.Cross(ltco1);
			ltco2.Normalize();

			data[0]=ltc[0];data[1]=ltco1[0];data[2]=ltco2[0];data[3]=0;
			data[4]=ltc[1];data[5]=ltco1[1];data[6]=ltco2[1];data[7]=0;
			data[8]=ltc[2];data[9]=ltco1[2];data[10]=ltco2[2];data[11]=0;
			data[12]=0;data[13]=0;data[14]=0;data[15]=1;
			p2gmat.Set(data);

			CVector3d no1=Ortho3(n);
			no1.Normalize();
			CVector3d no2=n.Cross(no1);
			no2.Normalize();

			data[0]=n[0];data[1]=n[1];data[2]=n[2];data[3]=0;
			data[4]=no1[0];data[5]=no1[1];data[6]=no1[2];data[7]=0;
			data[8]=no2[0];data[9]=no2[1];data[10]=no2[2];data[11]=0;
			data[12]=0;data[13]=0;data[14]=0;data[15]=1;
			g2cmat.Set(data);

			data[0]=1;data[1]=0;data[2]=0;data[3]=-cb[0];
			data[4]=0;data[5]=1;data[6]=0;data[7]=-cb[1];
			data[8]=0;data[9]=0;data[10]=1;data[11]=-cb[2];
			data[12]=0;data[13]=0;data[14]=0;data[15]=1;
			transmat.Set(data);
			compmat=p2gmat.MultRight(g2cmat);
			tmpmat=compmat.MultRight(transmat);
	
			data[0]=1;data[1]=0;data[2]=0;data[3]=loc[0];
			data[4]=0;data[5]=1;data[6]=0;data[7]=loc[1];
			data[8]=0;data[9]=0;data[10]=1;data[11]=loc[2];
			data[12]=0;data[13]=0;data[14]=0;data[15]=1;
			transmat.Set(data);

			compmat=transmat.MultRight(tmpmat);
		
			pComponent->Transform(compmat);
			m_OpenGLManager.BeginContext();
			pComponent->BuildList();
			m_OpenGLManager.EndContext();


		}
	}

	InvalidateRect(NULL,FALSE);

}

void CGeometryStudioView::OnReconstructionRbf()
{
	CRBFSurface Solver;


	CVector3d coord,projCoord,normal;

	int i,j,k;

	double len,avgLen;

	CVector3d vmin,vmax;
	int sizeX,sizeY,sizeZ;
	m_pModel->GetBoundBox(vmin,vmax);

	Solver.Initialize(m_pModel->GetVertexNum());

	len=0.0;
	for (i=0;i<m_pModel->GetVertexNum()-1;i++)
	{
		coord=m_pModel->GetVertexByIndex(i)->GetCoordinate();
		Solver.AddConstraint(coord.GetArray(),0.0,0.0);
		normal=m_pModel->GetVertexByIndex(i)->GetNormal();
		projCoord=coord+normal*0.001;
		Solver.AddConstraint(projCoord.GetArray(),0.001,0.0);
		projCoord=coord-normal*0.001;
		Solver.AddConstraint(projCoord.GetArray(),-0.001,0.0);
	}

	avgLen=(vmax-vmin).Length()*0.01;

	Solver.Solve();


	len=vmax[0]-vmin[0];
	sizeX=(int)(len/avgLen)+1;

	len=vmax[1]-vmin[1];
	sizeY=(int)(len/avgLen)+1;

	len=vmax[2]-vmin[2];
	sizeZ=(int)(len/avgLen)+1;

	MarchingCubes mc;

	mc.set_resolution( sizeX,sizeY,sizeZ) ;
	mc.set_boundingbox(vmin,vmax);
	mc.init_all();

	for (i=0;i<mc.size_x();i++)
	{
		for (j=0;j<mc.size_y();j++)
		{
			for (k=0;k<mc.size_z();k++)
			{
				coord=mc.get_grid_vert(i,j,k);
				mc.set_data( (float)Solver.GetValueAt(coord),i,j,k ) ;
			}
		}
	}

	mc.determine_cubes_cases();

	mc.run();

	mc.clean_temps();
	m_pModel=new CSolid;
	m_pModel->ImportFromMC(mc,false);
	mc.clean_all();

	m_OpenGLManager.BeginContext();
	m_pModel->BuildList();
	m_OpenGLManager.EndContext();
	InvalidateRect(NULL,FALSE);

}

int CGeometryStudioView::GetSilhouetteNum(void)
{
	return m_FSNManager.GetSilhouetteNum();
}

CStroke* CGeometryStudioView::GetSilhouetteByIndex(int index)
{
	return m_FSNManager.GetSilhouetteByIndex(index);
}

void CGeometryStudioView::OnToolLock()
{
	m_bLock=!m_bLock;
}

void CGeometryStudioView::OnUpdateToolLock(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bLock);
}

void CGeometryStudioView::OnEditUndo()
{
	if(m_pDepthPlane!=NULL)
	{
		delete m_pDepthPlane;
		m_pDepthPlane=NULL;
	}
	if(m_pProjectPlane!=NULL)
	{
		delete m_pProjectPlane;
		m_pProjectPlane=NULL;
	}
	if(m_pNDepthPlane!=NULL)
	{
		delete m_pNDepthPlane;
		m_pNDepthPlane=NULL;
	}
	if(m_pNProjectPlane!=NULL)
	{
		delete m_pNProjectPlane;
		m_pNProjectPlane=NULL;
	}

	if(m_pDepthStroke!=NULL)
	{
		delete m_pDepthStroke;
		m_pDepthStroke=NULL;
	}
	if(m_pSilhouetteStroke!=NULL)
	{
		delete m_pSilhouetteStroke;
		m_pSilhouetteStroke=NULL;
	}
	m_FSNManager.RemoveSilhouette(m_pStroke);

	m_bShowSilhouettePlane=false;
	delete m_pStroke;
	m_pStroke=NULL;

	InvalidateRect(NULL,FALSE);

}

void CGeometryStudioView::OnStypeFull()
{
	m_sstType=SST_FULL;
}

void CGeometryStudioView::OnUpdateStypeFull(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_sstType==SST_FULL);
}

void CGeometryStudioView::OnStypeIsolate()
{
	m_sstType=SST_ISOLATE;
}

void CGeometryStudioView::OnUpdateStypeIsolate(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_sstType==SST_ISOLATE);
}

void CGeometryStudioView::OnStypePartial()
{
	m_sstType=SST_PARTIAL;
}

void CGeometryStudioView::OnUpdateStypePartial(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_sstType==SST_PARTIAL);
}

void CGeometryStudioView::OnPatchBounded()
{
	m_moType=MO_STROKE_SELECT;
	if(m_pStroke!=NULL)
		delete m_pStroke;
	m_pStroke=NULL;
}

void CGeometryStudioView::OnImportOfffile()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"OFF Files (*.OFF)|*.OFF||", this);


	int structsize=0; 
	DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion; 
	//检测目前的操作系统，GetVersion具体用法详见MSDN 
	dwVersion = GetVersion(); 
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion))); 
	dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion))); 
	// 如果运行的操作系统是Windows NT/2000 
	if (dwVersion < 0x80000000)  
		structsize =88;//显示新的文件对话框 
	else 
		//运行的操作系统Windows 95/98  
		structsize =76;//显示老的文件对话框 
	fileDlg.m_ofn.lStructSize=structsize; 

	if(fileDlg.DoModal()==IDOK){
		CString filePath = fileDlg.GetPathName();
		if(m_pModel)
			delete m_pModel;
		m_pModel=new CSolid;
		CString fileExt=fileDlg.GetFileExt();
		m_pModel->ReadFromOFF(filePath.GetBuffer (20));

		m_omMode=OM_SCISSOR;


		m_OpenGLManager.BeginContext();
		m_pModel->BuildList();
		m_OpenGLManager.EndContext();

		m_SIRManager.RegisterModel(m_pModel);

		InvalidateRect(NULL,FALSE);
	}	
}

void CGeometryStudioView::OnSceneMagnificence()
{
	m_bMagnificence=!m_bMagnificence;
	switch(m_omMode)
	{
	case OM_SCISSOR:
		{
			if(m_bMagnificence)
			{
				for (int i=0;i<m_pModel->GetBoundaryNum();i++)
				{
					m_pModel->GetBoundaryByIndex(i)->SetDisplayStyle(1);
				}
			}
			else
			{
				for (int i=0;i<m_pModel->GetBoundaryNum();i++)
				{
					m_pModel->GetBoundaryByIndex(i)->SetDisplayStyle(0);
				}

			}

		}
		break;
	case OM_FUSION:
		{
			CSolid *pComponent;
			int i,j;
			if (m_bMagnificence)
			{
				for(i=0;i<m_FSNManager.GetComponentNum();i++)
				{
					pComponent=m_FSNManager.GetComponentByIndex(i);
					for (j=0;j<pComponent->GetBoundaryNum();j++)
					{
						pComponent->GetBoundaryByIndex(j)->SetDisplayStyle(1);
					}
				}
				for (i=0;i<m_FSNManager.GetSilhouetteNum();i++)
				{
					m_FSNManager.GetSilhouetteByIndex(i)->SetDisplayStyle(1);
				}			
			}
			else
			{
				for(i=0;i<m_FSNManager.GetComponentNum();i++)
				{
					pComponent=m_FSNManager.GetComponentByIndex(i);
					for (j=0;j<pComponent->GetBoundaryNum();j++)
					{
						pComponent->GetBoundaryByIndex(j)->SetDisplayStyle(0);
					}
				}
				for (i=0;i<m_FSNManager.GetSilhouetteNum();i++)
				{
					m_FSNManager.GetSilhouetteByIndex(i)->SetDisplayStyle(0);
				}
			}


		}
		break;
	}
	InvalidateRect(NULL,FALSE);
}

void CGeometryStudioView::OnUpdateSceneMagnificence(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bMagnificence);
}

void CGeometryStudioView::OnSilhouetteShowStroke()
{
	m_bShowSilhouetteStroke=!m_bShowSilhouetteStroke;
}

void CGeometryStudioView::OnUpdateSilhouetteShowStroke(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowSilhouetteStroke);
}

void CGeometryStudioView::OnCoordinateBase()
{
	m_FSNManager.GetFusionModel()->UpdateVerticesCoords(m_vBaseCoords);
	m_OpenGLManager.BeginContext();
	for (int i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		m_FSNManager.GetComponentByIndex(i)->BuildList();
	}
	m_FSNManager.GetFusionModel()->BuildList();
	m_FSNManager.GetTComponentByIndex(0)->BuildList();
	m_OpenGLManager.EndContext();	
	m_iCoordinateShow=1;
}

void CGeometryStudioView::OnCoordinateRaw()
{
	m_FSNManager.GetFusionModel()->UpdateVerticesCoords(m_vRawCoords);
	m_OpenGLManager.BeginContext();
	for (int i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		m_FSNManager.GetComponentByIndex(i)->BuildList();
	}
	m_FSNManager.GetFusionModel()->BuildList();
	m_FSNManager.GetTComponentByIndex(0)->BuildList();
	m_OpenGLManager.EndContext();	
	m_iCoordinateShow=0;
}

void CGeometryStudioView::OnUpdateCoordinateBase(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_iCoordinateShow==1);
}

void CGeometryStudioView::OnUpdateCoordinateRaw(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_iCoordinateShow==0);
}

void CGeometryStudioView::OnSelectPolypatch()
{
	m_moType=MO_SELECT_POLYPATCH;
	int dimx,dimy;
	m_OpenGLManager.GetDimension(dimx,dimy);
	if(m_pDepthMap==NULL)
		m_pDepthMap=new float[dimx*dimy];

	for(int i=0;i<dimx*dimy;i++)
		m_pDepthMap[i]=1.0;
	m_OpenGLManager.EndDraw();
	m_OpenGLManager.GetDepth(m_pDepthMap,0,0,dimx,dimy);

}

void CGeometryStudioView::OnSelectVertex()
{
	m_moType=MO_SELECT_VERTEX;
}

void CGeometryStudioView::OnCoordinateImport()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"Coordinate Files (*.CRD)|*.CRD||", this);


	int structsize=0; 
	DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion; 
	//检测目前的操作系统，GetVersion具体用法详见MSDN 
	dwVersion = GetVersion(); 
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion))); 
	dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion))); 
	// 如果运行的操作系统是Windows NT/2000 
	if (dwVersion < 0x80000000)  
		structsize =88;//显示新的文件对话框 
	else 
		//运行的操作系统Windows 95/98  
		structsize =76;//显示老的文件对话框 
	fileDlg.m_ofn.lStructSize=structsize; 

	if(fileDlg.DoModal()==IDOK){
		CString filePath = fileDlg.GetPathName();
		{
			int i,numV;
			double coord[3];
			ifstream ifile((LPCTSTR)filePath);
			ifile>>numV;
			for (i=0;i<numV;i++)
			{
				ifile>>coord[0]>>coord[1]>>coord[2];
				CVector3d *pVec=new CVector3d(coord);
				m_vRawCoords.push_back(pVec);
			}
			for (i=0;i<numV;i++)
			{
				ifile>>coord[0]>>coord[1]>>coord[2];
				CVector3d *pVec=new CVector3d(coord);
				m_vBaseCoords.push_back(pVec);
			}
			for (i=0;i<numV;i++)
			{
				ifile>>coord[0]>>coord[1]>>coord[2];
				CVector3d *pVec=new CVector3d(coord);
				m_vReconCoords.push_back(pVec);
			}
		}
		InvalidateRect(NULL,FALSE);
	}	
}

void CGeometryStudioView::OnCoordinateExport()
{
	CFileDialog fileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"Coordinate Files (*.CRD)|*.CRD||", this);


	int structsize=0; 
	DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion; 
	//检测目前的操作系统，GetVersion具体用法详见MSDN 
	dwVersion = GetVersion(); 
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion))); 
	dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion))); 
	// 如果运行的操作系统是Windows NT/2000 
	if (dwVersion < 0x80000000)  
		structsize =88;//显示新的文件对话框 
	else 
		//运行的操作系统Windows 95/98  
		structsize =76;//显示老的文件对话框 
	fileDlg.m_ofn.lStructSize=structsize; 

	if(fileDlg.DoModal()==IDOK){
		CString filePath = fileDlg.GetPathName();
		{
			int i,numV;
			ofstream ofile((LPCTSTR)filePath);
			numV=m_vRawCoords.size();
			CVector3d *pVec;
			ofile<<numV<<endl;
			for (i=0;i<numV;i++)
			{
				pVec=m_vRawCoords[i];
				ofile<<(*pVec)[0]<<" "<<(*pVec)[1]<<" "<<(*pVec)[2]<<endl;
			}
			for (i=0;i<numV;i++)
			{
				pVec=m_vBaseCoords[i];
				ofile<<(*pVec)[0]<<" "<<(*pVec)[1]<<" "<<(*pVec)[2]<<endl;
			}
			for (i=0;i<numV;i++)
			{
				pVec=m_vReconCoords[i];
				ofile<<(*pVec)[0]<<" "<<(*pVec)[1]<<" "<<(*pVec)[2]<<endl;
			}
		}
		InvalidateRect(NULL,FALSE);
	}	
}

void CGeometryStudioView::OnCoordinateRecon()
{
	m_FSNManager.GetFusionModel()->UpdateVerticesCoords(m_vReconCoords);
	m_OpenGLManager.BeginContext();
	for (int i=0;i<m_FSNManager.GetComponentNum();i++)
	{
		m_FSNManager.GetComponentByIndex(i)->BuildList();
	}
	m_FSNManager.GetFusionModel()->BuildList();
	m_FSNManager.GetTComponentByIndex(0)->BuildList();
	m_OpenGLManager.EndContext();	
	m_iCoordinateShow=2;
}

void CGeometryStudioView::OnUpdateCoordinateRecon(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_iCoordinateShow==2);
}

void CGeometryStudioView::OnSelectBoundary()
{
	m_moType=MO_SELECT_BOUNDARY;
}

void CGeometryStudioView::OnSelectStroke()
{
	m_moType=MO_SELECT_STROKE;
}


void CGeometryStudioView::OnRemoveStroke()
{
	m_FSNManager.RemoveStructure(m_pStroke);
	
	m_pStroke=NULL;
}

void CGeometryStudioView::OnSilhouetteSmooth()
{
	m_moType=MO_SILHOUETTE_SMOOTH;
	m_pCtrlPoint=NULL;
}

void CGeometryStudioView::UpdateDepthPlane(double delta)
{


	CVector3d ov1,ov2,ov3,ov4,x,y,z;
	double sx,sy,sz,data[16];
	CMatrix44 transmat,scalemat,ltgmat,gtlmat,ndp2g_mat,tmpmat,g2ndp_mat;
	int i,j;

	ov1=m_pNDepthPlane->GetCorner(0);
	ov2=m_pNDepthPlane->GetCorner(1);
	ov3=m_pNDepthPlane->GetCorner(2);
	ov4=m_pNDepthPlane->GetCorner(3);


	x=ov2-ov1;x.Normalize();
	y=ov4-ov1;y.Normalize();
	z=m_pNDepthPlane->Normal();
	z.Normalize();
	sx=(ov2-ov1).Length();
	sy=(ov4-ov1).Length();
	sz=1.0;


	data[0]=x[0];data[1]=y[0];data[2]=z[0];data[3]=0;
	data[4]=x[1];data[5]=y[1];data[6]=z[1];data[7]=0;
	data[8]=x[2];data[9]=y[2];data[10]=z[2];data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	ltgmat.Set(data);

	data[0]=1;data[1]=0;data[2]=0;data[3]=ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	data[0]=sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=ltgmat.MultRight(scalemat);
	ndp2g_mat=transmat.MultRight(tmpmat);


	data[0]=1;data[1]=0;data[2]=0;data[3]=-ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=-ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=-ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	gtlmat=ltgmat.Transpose();

	data[0]=1/sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=1/sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=1/sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=gtlmat.MultRight(transmat);
	g2ndp_mat=scalemat.MultRight(tmpmat);
	CVector3d gcoord=m_pCtrlPoint->GetCoordinate();
	CVector3d dpcoord=g2ndp_mat.MultMatVec(gcoord);
    if((dpcoord[1]+delta)>1.0)
		dpcoord[1]=1.0;
	else if((dpcoord[1]+delta)<0.0)
		dpcoord[1]=0.0;
	else
        dpcoord[1]+=delta;

	gcoord=ndp2g_mat.MultMatVec(dpcoord);
	m_pCtrlPoint->Set(gcoord);


	CVector3d raw,lcoord[6];
	double t=0.0;

	for (i=0;i<6;i++)
		lcoord[i]=g2ndp_mat.MultMatVec(m_vecCtrlPolygon[i]->GetCoordinate());

	for (i=0;i<50;i++)
	{
		raw.Set(0.0,0.0,0.0);
		for(j=0;j<6;j++)
			raw+=CalcBernstein(5,j,t)*lcoord[j];
		gcoord=ndp2g_mat.MultMatVec(raw);
		m_pDepthStroke->ModifyPoint(i,gcoord);
		t+=0.02;			
	}
	CVector3d update,offset,dir,proj;
	double len;
	ov1=m_pProjectPlane->GetCorner(0);
	ov2=m_pProjectPlane->GetCorner(1);
	x=ov2-ov1;len=x.Length();x.Normalize();
	y=m_pProjectPlane->Normal();y.Normalize();
	z=m_pDepthPlane->Normal();z.Normalize();
	for (i=0;i<m_pStroke->GetPointsNum();i++)
	{
		raw=m_pStroke->GetPoint(i);
		dir=raw-ov1;
		proj=raw-(dir.Dot(y))*y;
		t=(dir.Dot(x))/(len);
		offset.Set(0.0,0.0,0.0);
		for(j=0;j<6;j++)
			offset+=CalcBernstein(5,j,t)*lcoord[j];	
		update=proj+(offset[1]-0.5)*len*y;		
		m_pStroke->ModifyPoint(i,update);
		//m_pStroke->ModifyPoint(i,proj);
	}



}

void CGeometryStudioView::OnSilhouetteAdjust()
{
	m_moType=MO_SILHOUETTE_ADJUST;
	m_pCtrlPoint=NULL;
}

void CGeometryStudioView::OnToolAnimate()
{
	SetTimer(0,33,NULL);
}

void CGeometryStudioView::InitSilhouettePlanes(void)
{
	if(m_pSilhouetteStroke==NULL)
		m_pSilhouetteStroke=new CStroke(m_pStroke->GetPointsNum());
	if(m_pDepthStroke==NULL)
		m_pDepthStroke=new CStroke(50);

	int i,j;
	CVector3d ov1, ov2,ov3,ov4;
	CMatrix44 transmat,scalemat,ltgmat,gtlmat,ndp2g_mat,tmpmat,g2pj_mat,pj2g_mat,npj2g_mat,g2npj_mat;

	double data[16];
	double wv1[2],wv2[2],wv3[2],wv4[2];

	CVector3d v,v1,v2,vm,vse,vpn,vdn,vEyePos,raw,proj,loc,update,offset;

	CVector3d x,y,z;
	double sx,sy,sz;
	double len,out[3];

	int dimx,dimy,dd;
	m_OpenGLManager.GetDimension(dimx,dimy);

	dd=dimx<dimy?dimx:dimy;

	int gap=dd/30;
	int length=dd/3;


	ov1=m_pProjectPlane->GetCorner(0);
	ov2=m_pProjectPlane->GetCorner(1);
	ov3=m_pProjectPlane->GetCorner(2);
	ov4=m_pProjectPlane->GetCorner(3);

	x=ov2-ov1;len=x.Length();x.Normalize();
	y=ov4-ov1;y.Normalize();
	z.Set(m_pProjectPlane->a(),m_pProjectPlane->b(),m_pProjectPlane->c());
	z.Normalize();

	sx=1.0/((ov2-ov1).Length());
	sy=1.0/((ov4-ov1).Length());
	sz=1.0;

	data[0]=1;data[1]=0;data[2]=0;data[3]=-ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=-ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=-ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	data[0]=x[0];data[1]=x[1];data[2]=x[2];data[3]=0;
	data[4]=y[0];data[5]=y[1];data[6]=y[2];data[7]=0;
	data[8]=z[0];data[9]=z[1];data[10]=z[2];data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	gtlmat.Set(data);

	data[0]=sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=gtlmat.MultRight(transmat);

	g2pj_mat=scalemat.MultRight(tmpmat);

	data[0]=1;data[1]=0;data[2]=0;data[3]=ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;

	transmat.Set(data);
	ltgmat=gtlmat.Transpose();
	data[0]=1/sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=1/sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=1/sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=ltgmat.MultRight(scalemat);
	pj2g_mat=transmat.MultRight(tmpmat);


	for(i=0;i<m_pStroke->GetPointsNum();i++)
	{
		raw=m_pStroke->GetPoint(i);
		m_pSilhouetteStroke->ModifyPoint(i,g2pj_mat.MultMatVec(raw));
		//		m_pSilhouetteStroke->ModifyPoint(i,g2pj_mat.MultMatVec(raw-((raw-ov1).Dot(z))*z));
	}

	wv1[0]=gap;
	wv1[1]=gap;
	m_OpenGLManager.GetOBJCoord(wv1[0],wv1[1],0.1,out);
	ov1.Set(out);

	wv2[0]=wv1[0]+length;
	wv2[1]=wv1[1];
	m_OpenGLManager.GetOBJCoord(wv2[0],wv2[1],0.1,out);
	ov2.Set(out);

	wv3[0]=wv1[0]+length;
	wv3[1]=wv1[1]+length;
	m_OpenGLManager.GetOBJCoord(wv3[0],wv3[1],0.1,out);
	ov3.Set(out);


	wv4[0]=wv1[0];
	wv4[1]=wv1[1]+length;
	m_OpenGLManager.GetOBJCoord(wv4[0],wv4[1],0.1,out);
	ov4.Set(out);

	vpn=(ov3-ov1).Cross(ov2-ov1);
	vpn.Normalize();

	m_pNProjectPlane=new CPlane(ov1,vpn);

	m_pNProjectPlane->AddCorner(ov1);
	m_pNProjectPlane->AddCorner(ov2);
	m_pNProjectPlane->AddCorner(ov3);
	m_pNProjectPlane->AddCorner(ov4);


	x=ov2-ov1;x.Normalize();
	y=ov4-ov1;y.Normalize();
	z=vpn;
	sx=(ov2-ov1).Length();
	sy=(ov4-ov1).Length();
	sz=1.0;

	data[0]=x[0];data[1]=y[0];data[2]=z[0];data[3]=0;
	data[4]=x[1];data[5]=y[1];data[6]=z[1];data[7]=0;
	data[8]=x[2];data[9]=y[2];data[10]=z[2];data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;

	ltgmat.Set(data);

	data[0]=1;data[1]=0;data[2]=0;data[3]=ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	data[0]=sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=ltgmat.MultRight(scalemat);
	npj2g_mat=transmat.MultRight(tmpmat);

	data[0]=1;data[1]=0;data[2]=0;data[3]=-ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=-ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=-ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);
	gtlmat=ltgmat.Transpose();
	data[0]=1/sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=1/sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=1/sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=gtlmat.MultRight(transmat);
	g2npj_mat=scalemat.MultRight(tmpmat);

	for (i=0;i<m_pSilhouetteStroke->GetPointsNum();i++)
	{
		raw=m_pSilhouetteStroke->GetPoint(i);
		m_pSilhouetteStroke->ModifyPoint(i,npj2g_mat.MultMatVec(raw));
	}


	wv1[0]=length+2*gap;
	wv1[1]=gap;
	m_OpenGLManager.GetOBJCoord(wv1[0],wv1[1],0.1,out);
	ov1.Set(out);

	wv2[0]=wv1[0]+length;
	wv2[1]=wv1[1];
	m_OpenGLManager.GetOBJCoord(wv2[0],wv2[1],0.1,out);
	ov2.Set(out);

	wv3[0]=wv1[0]+length;
	wv3[1]=wv1[1]+length;
	m_OpenGLManager.GetOBJCoord(wv3[0],wv3[1],0.1,out);
	ov3.Set(out);


	wv4[0]=wv1[0];
	wv4[1]=wv1[1]+length;
	m_OpenGLManager.GetOBJCoord(wv4[0],wv4[1],0.1,out);
	ov4.Set(out);

	vdn=(ov3-ov1).Cross(ov2-ov1);
	vdn.Normalize();

	m_pNDepthPlane=new CPlane(ov1,vdn);

	m_pNDepthPlane->AddCorner(ov1);
	m_pNDepthPlane->AddCorner(ov2);
	m_pNDepthPlane->AddCorner(ov3);
	m_pNDepthPlane->AddCorner(ov4);

	m_pNDepthPlane->SetColor(0.0,1.0,0.0);


	x=ov2-ov1;x.Normalize();
	y=ov4-ov1;y.Normalize();
	z=vdn;
	sx=(ov2-ov1).Length();
	sy=(ov4-ov1).Length();
	sz=1.0;

	data[0]=x[0];data[1]=y[0];data[2]=z[0];data[3]=0;
	data[4]=x[1];data[5]=y[1];data[6]=z[1];data[7]=0;
	data[8]=x[2];data[9]=y[2];data[10]=z[2];data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;

	ltgmat.Set(data);

	data[0]=1;data[1]=0;data[2]=0;data[3]=ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	data[0]=sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=ltgmat.MultRight(scalemat);
	ndp2g_mat=transmat.MultRight(tmpmat);

	m_vecCtrlPolygon.clear();
	CVector3d gcoord,lcoord[6];
	CPoint3 *pcp;

	lcoord[0].Set(0.0,0.5,0.0);
	gcoord=ndp2g_mat.MultMatVec(lcoord[0]);
	pcp=new CPoint3;
	pcp->Set(gcoord);
	m_vecCtrlPolygon.push_back(pcp);

	lcoord[1].Set(0.2,0.6,0.0);
	gcoord=ndp2g_mat.MultMatVec(lcoord[1]);
	pcp=new CPoint3;
	pcp->Set(gcoord);
	m_vecCtrlPolygon.push_back(pcp);

	lcoord[2].Set(0.4,0.2,0.0);
	gcoord=ndp2g_mat.MultMatVec(lcoord[2]);
	pcp=new CPoint3;
	pcp->Set(gcoord);
	m_vecCtrlPolygon.push_back(pcp);
	pcp->SetMaterial("Blue");

	lcoord[3].Set(0.6,0.8,0.0);
	gcoord=ndp2g_mat.MultMatVec(lcoord[3]);
	pcp=new CPoint3;
	pcp->Set(gcoord);
	m_vecCtrlPolygon.push_back(pcp);
	pcp->SetMaterial("Blue");
	lcoord[4].Set(0.8,0.4,0.0);
	gcoord=ndp2g_mat.MultMatVec(lcoord[4]);
	pcp=new CPoint3;
	pcp->Set(gcoord);
	m_vecCtrlPolygon.push_back(pcp);

	lcoord[5].Set(1.0,0.5,0.0);
	gcoord=ndp2g_mat.MultMatVec(lcoord[5]);
	pcp=new CPoint3;
	pcp->Set(gcoord);
	m_vecCtrlPolygon.push_back(pcp);

	double t=0.0;
	for (i=0;i<50;i++)
	{
		raw.Set(0.0,0.0,0.0);
		for(j=0;j<6;j++)
			raw+=CalcBernstein(5,j,t)*lcoord[j];
		gcoord=ndp2g_mat.MultMatVec(raw);
		m_pDepthStroke->ModifyPoint(i,gcoord);
		t+=0.02;			
	}

	vpn=m_pProjectPlane->Normal();
	for (i=0;i<m_pStroke->GetPointsNum();i++)
	{
		raw=m_pSilhouetteStroke->GetPoint(i);
		loc=g2npj_mat.MultMatVec(raw);
		proj=pj2g_mat.MultMatVec(loc);

		t=loc[0];
		offset.Set(0.0,0.0,0.0);
		for(j=0;j<6;j++)
			offset+=CalcBernstein(5,j,t)*lcoord[j];	
		update=proj+(offset[1]-0.5)*len*vpn;		
		m_pStroke->ModifyPoint(i,update);
	}
	m_bUpdateSilhouettePlanes=false;



}

void CGeometryStudioView::AdjustSilhouettePlanes(double delta,bool depth)
{
	double sx,sy,sz,data[16];
	CMatrix44 transmat,scalemat,ltgmat,gtlmat,ndp2g_mat,tmpmat,g2ndp_mat,g2npj_mat,npj2g_mat,pj2g_mat;
	int i,j;
	CVector3d ov1,ov2,ov3,ov4,x,y,z;

	CVector3d raw,lcoord[6];
	double e1,e2,t=0.0;

	ov1=m_pNDepthPlane->GetCorner(0);
	ov2=m_pNDepthPlane->GetCorner(1);
	ov3=m_pNDepthPlane->GetCorner(2);
	ov4=m_pNDepthPlane->GetCorner(3);


	x=ov2-ov1;x.Normalize();
	y=ov4-ov1;y.Normalize();
	z=m_pNDepthPlane->Normal();
	z.Normalize();
	sx=(ov2-ov1).Length();
	sy=(ov4-ov1).Length();
	sz=1.0;


	data[0]=1;data[1]=0;data[2]=0;data[3]=-ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=-ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=-ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	data[0]=x[0];data[1]=x[1];data[2]=x[2];data[3]=0;
	data[4]=y[0];data[5]=y[1];data[6]=y[2];data[7]=0;
	data[8]=z[0];data[9]=z[1];data[10]=z[2];data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	gtlmat.Set(data);

	data[0]=1/sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=1/sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=1/sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=gtlmat.MultRight(transmat);
	g2ndp_mat=scalemat.MultRight(tmpmat);

	ltgmat=gtlmat.Transpose();

	data[0]=1;data[1]=0;data[2]=0;data[3]=ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	data[0]=sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=ltgmat.MultRight(scalemat);
	ndp2g_mat=transmat.MultRight(tmpmat);

	ov1=m_pNProjectPlane->GetCorner(0);
	ov2=m_pNProjectPlane->GetCorner(1);
	ov3=m_pNProjectPlane->GetCorner(2);
	ov4=m_pNProjectPlane->GetCorner(3);


	x=ov2-ov1;x.Normalize();
	y=ov4-ov1;y.Normalize();
	z=m_pNProjectPlane->Normal();
	sx=(ov2-ov1).Length();
	sy=(ov4-ov1).Length();
	sz=1.0;

	data[0]=x[0];data[1]=y[0];data[2]=z[0];data[3]=0;
	data[4]=x[1];data[5]=y[1];data[6]=z[1];data[7]=0;
	data[8]=x[2];data[9]=y[2];data[10]=z[2];data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;

	ltgmat.Set(data);

	data[0]=1;data[1]=0;data[2]=0;data[3]=ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	data[0]=sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=ltgmat.MultRight(scalemat);
	npj2g_mat=transmat.MultRight(tmpmat);

	data[0]=1;data[1]=0;data[2]=0;data[3]=-ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=-ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=-ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	data[0]=1/sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=1/sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=1/sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	gtlmat=ltgmat.Transpose();

	tmpmat=gtlmat.MultRight(transmat);
	g2npj_mat=scalemat.MultRight(tmpmat);


	if(depth)
	{
		CVector3d gcoord=m_pCtrlPoint->GetCoordinate();
		CVector3d dpcoord=g2ndp_mat.MultMatVec(gcoord);
		if((dpcoord[1]+delta)>1.0)
			dpcoord[1]=1.0;
		else if((dpcoord[1]+delta)<0.0)
			dpcoord[1]=0.0;
		else
			dpcoord[1]+=delta;

		gcoord=ndp2g_mat.MultMatVec(dpcoord);
		m_pCtrlPoint->Set(gcoord);

		for (i=0;i<6;i++)
			lcoord[i]=g2ndp_mat.MultMatVec(m_vecCtrlPolygon[i]->GetCoordinate());

		for (i=0;i<50;i++)
		{
			raw.Set(0.0,0.0,0.0);
			for(j=0;j<6;j++)
				raw+=CalcBernstein(5,j,t)*lcoord[j];
			gcoord=ndp2g_mat.MultMatVec(raw);
			m_pDepthStroke->ModifyPoint(i,gcoord);
			t+=0.02;			
		}

	}
	else
	{
		for (i=0;i<6;i++)
			lcoord[i]=g2ndp_mat.MultMatVec(m_vecCtrlPolygon[i]->GetCoordinate());

		for (i=0;i<m_pSilhouetteStroke->GetPointsNum();i++)
		{
			raw=m_pSilhouetteStroke->GetPoint(i);
			m_pSilhouetteStroke->ModifyPoint(i,g2npj_mat.MultMatVec(raw));
		}

		vector<CVector3d> vecCoords;
		vector<CVector3d>::iterator it;

		for (i=0;i<m_pSilhouetteStroke->GetPointsNum();i++)
		{
			vecCoords.push_back(m_pSilhouetteStroke->GetPoint(i));
		}
		CVector3d lap;
		for (j=0;j<5;j++)
		{
			for (i=1;i<m_pSilhouetteStroke->GetPointsNum()-1;i++)
			{
				e1=(vecCoords[i-1]-vecCoords[i]).Length();
				e2=(vecCoords[i+1]-vecCoords[i]).Length();

				lap=2*(e1*vecCoords[i-1]+e2*vecCoords[i+1]-(e1+e2)*vecCoords[i])/(e1+e2);
				m_pSilhouetteStroke->ModifyPoint(i,vecCoords[i]+0.01*delta*lap);
			}
			it=vecCoords.begin();
			for (i=0;it!=vecCoords.end();it++,i++)
				(*it)=m_pSilhouetteStroke->GetPoint(i);
		}
		for (i=0;i<m_pSilhouetteStroke->GetPointsNum();i++)
		{
			raw=m_pSilhouetteStroke->GetPoint(i);
			m_pSilhouetteStroke->ModifyPoint(i,npj2g_mat.MultMatVec(raw));
		}


	}
	CVector3d vpn,loc,proj,offset,update;
	double len;

	ov1=m_pProjectPlane->GetCorner(0);
	ov2=m_pProjectPlane->GetCorner(1);
	ov3=m_pProjectPlane->GetCorner(2);
	ov4=m_pProjectPlane->GetCorner(3);

	x=ov2-ov1;len=x.Length();x.Normalize();
	y=ov4-ov1;y.Normalize();
	z.Set(m_pProjectPlane->a(),m_pProjectPlane->b(),m_pProjectPlane->c());
	z.Normalize();

	sx=1.0/((ov2-ov1).Length());
	sy=1.0/((ov4-ov1).Length());
	sz=1.0;

	data[0]=1;data[1]=0;data[2]=0;data[3]=ov1[0];
	data[4]=0;data[5]=1;data[6]=0;data[7]=ov1[1];
	data[8]=0;data[9]=0;data[10]=1;data[11]=ov1[2];
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	transmat.Set(data);

	data[0]=x[0];data[1]=y[0];data[2]=z[0];data[3]=0;
	data[4]=x[1];data[5]=y[1];data[6]=z[1];data[7]=0;
	data[8]=x[2];data[9]=y[2];data[10]=z[2];data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	ltgmat.Set(data);

	data[0]=1/sx;data[1]=0;data[2]=0;data[3]=0;
	data[4]=0;data[5]=1/sy;data[6]=0;data[7]=0;
	data[8]=0;data[9]=0;data[10]=1/sz;data[11]=0;
	data[12]=0;data[13]=0;data[14]=0;data[15]=1;
	scalemat.Set(data);

	tmpmat=ltgmat.MultRight(scalemat);
	pj2g_mat=transmat.MultRight(tmpmat);

	vpn=m_pProjectPlane->Normal();
	for (i=0;i<m_pStroke->GetPointsNum();i++)
	{
		raw=m_pSilhouetteStroke->GetPoint(i);
		loc=g2npj_mat.MultMatVec(raw);
		proj=pj2g_mat.MultMatVec(loc);

		t=loc[0];
		offset.Set(0.0,0.0,0.0);
		for(j=0;j<6;j++)
			offset+=CalcBernstein(5,j,t)*lcoord[j];	
		update=proj+(offset[1]-0.5)*len*vpn;		
		m_pStroke->ModifyPoint(i,update);
	}

}
