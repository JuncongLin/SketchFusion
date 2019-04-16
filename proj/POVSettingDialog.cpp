// POVSettingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GeometryStudio.h"
#include "POVSettingDialog.h"
#include ".\povsettingdialog.h"
#include "POVFileWriter.h"
#include "MainFrm.h"
#include "TransitionPrimitive.h"
#include "PrincipleAnalysis.h"
// CPOVSettingDialog dialog

IMPLEMENT_DYNAMIC(CPOVSettingDialog, CDialog)
CPOVSettingDialog::CPOVSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPOVSettingDialog::IDD, pParent)
{
	m_pCurPPOVProperty=NULL;
}
CPOVSettingDialog::CPOVSettingDialog(CGeometryStudioView* pView)
: CDialog(CPOVSettingDialog::IDD, NULL)
{
	m_pGeomView=pView;
	m_pCurPPOVProperty=NULL;
}
CPOVSettingDialog::~CPOVSettingDialog()
{
	for (int i=0;i<m_PPOVProperties.size();i++)
	{
		delete m_PPOVProperties.at(i);
	}
}

void CPOVSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRIMITIVES, m_CtrlPrimitives);
	DDX_Control(pDX, IDC_PIGMENT, m_CtrlPigment);
	DDX_Control(pDX, IDC_FINISH, m_CtrlFinish);
	DDX_Control(pDX, IDC_INTERIOR, m_CtrlInterior);
	DDX_Control(pDX, IDC_OUTPUT, m_CtrlOutput);
	DDX_Control(pDX, IDC_WBOUNDARY, m_CtrlBoundary);
	DDX_Control(pDX, IDC_WBOUNDARYDIR, m_CtrlBoundDir);
	DDX_Control(pDX, IDC_WBODYDIR, m_CtrlBodyDir);
	DDX_Control(pDX, IDC_SCENE, m_CtrlScene);
	DDX_Control(pDX, IDC_WOBJECTDIRECTION, m_CtrlObjectDir);
}


BEGIN_MESSAGE_MAP(CPOVSettingDialog, CDialog)
	ON_CBN_SELCHANGE(IDC_PRIMITIVES, OnCbnSelchangePrimitives)
	ON_BN_CLICKED(IDC_OUTPUT, OnBnClickedOutput)
	ON_BN_CLICKED(IDC_EXPORT, OnBnClickedExport)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPOVSettingDialog message handlers





void CPOVSettingDialog::OnCbnSelchangePrimitives()
{
	m_pCurPPOVProperty->bBound=m_CtrlBoundary.GetCheck();
	m_pCurPPOVProperty->bBodyDir=m_CtrlBodyDir.GetCheck();
	m_pCurPPOVProperty->bBoundDir=m_CtrlBoundDir.GetCheck();
	m_pCurPPOVProperty->bObjDir=m_CtrlObjectDir.GetCheck();
	CString str;
	m_CtrlPigment.GetWindowText(str);
	m_pCurPPOVProperty->pigment=str;
	m_CtrlFinish.GetWindowText(str);
	m_pCurPPOVProperty->finish=str;
	m_CtrlInterior.GetWindowText(str);
	m_pCurPPOVProperty->interior=str;

	int pos=m_CtrlPrimitives.GetCurSel();

	m_pCurPPOVProperty=m_PPOVProperties.at(pos);
	m_CtrlOutput.SetCheck(m_pCurPPOVProperty->bOutput);
	m_CtrlBoundDir.EnableWindow(m_pCurPPOVProperty->bOutput);
	m_CtrlBodyDir.EnableWindow(m_pCurPPOVProperty->bOutput);
	m_CtrlObjectDir.EnableWindow(m_pCurPPOVProperty->bOutput);
	m_CtrlBoundary.EnableWindow(m_pCurPPOVProperty->bOutput);
	m_CtrlPigment.EnableWindow(m_pCurPPOVProperty->bOutput);
	m_CtrlFinish.EnableWindow(m_pCurPPOVProperty->bOutput);
	m_CtrlInterior.EnableWindow(m_pCurPPOVProperty->bOutput);

	m_CtrlBoundDir.SetCheck(m_pCurPPOVProperty->bBoundDir);
	m_CtrlBodyDir.SetCheck(m_pCurPPOVProperty->bBodyDir);
	m_CtrlObjectDir.SetCheck(m_pCurPPOVProperty->bObjDir);
	m_CtrlBoundary.SetCheck(m_pCurPPOVProperty->bBound);
	pos=m_CtrlPigment.FindStringExact(-1,m_pCurPPOVProperty->pigment);
	m_CtrlPigment.SetCurSel(pos);
	pos=m_CtrlFinish.FindStringExact(-1,m_pCurPPOVProperty->finish);
	m_CtrlFinish.SetCurSel(pos);
	pos=m_CtrlInterior.FindStringExact(-1,m_pCurPPOVProperty->interior);
	m_CtrlInterior.SetCurSel(pos);

}

void CPOVSettingDialog::OnBnClickedOutput()
{
	m_pCurPPOVProperty->bOutput=!m_pCurPPOVProperty->bOutput;

	m_CtrlBoundDir.EnableWindow(m_pCurPPOVProperty->bOutput);
	m_CtrlBodyDir.EnableWindow(m_pCurPPOVProperty->bOutput);
	m_CtrlObjectDir.EnableWindow(m_pCurPPOVProperty->bOutput);
	m_CtrlBoundary.EnableWindow(m_pCurPPOVProperty->bOutput);
	m_CtrlPigment.EnableWindow(m_pCurPPOVProperty->bOutput);
	m_CtrlFinish.EnableWindow(m_pCurPPOVProperty->bOutput);
	m_CtrlInterior.EnableWindow(m_pCurPPOVProperty->bOutput);


}
BOOL CPOVSettingDialog::Create()
{
	if(!CDialog::Create(CPOVSettingDialog::IDD))return false;

	m_CtrlScene.AddString("0");
	m_CtrlScene.AddString("1");


	m_CtrlPigment.AddString("none");
	m_CtrlPigment.AddString("GlassRuby");
	m_CtrlPigment.AddString("GlassBeer");
	m_CtrlPigment.AddString("GlassOrange");
	m_CtrlPigment.AddString("GlassVicks");
	m_CtrlPigment.AddString("GlassRed");
	m_CtrlPigment.AddString("GlassGreen");
	m_CtrlPigment.AddString("GlassBlue");
	m_CtrlPigment.AddString("GlassSkyBlue");
	m_CtrlPigment.AddString("GlassYellow");
	m_CtrlPigment.AddString("GlassGeneral");
	m_CtrlPigment.AddString("MineralAmethyst");
	m_CtrlPigment.AddString("MineralApatite");
	m_CtrlPigment.AddString("MineralAquamarine");
	m_CtrlPigment.AddString("MineralAzurite");
	m_CtrlPigment.AddString("MineralCitrine");
	m_CtrlPigment.AddString("MineralEmerald");
	m_CtrlPigment.AddString("MineralSapphire");
	m_CtrlPigment.AddString("MineralTopaz");
	m_CtrlPigment.AddString("MineralGreen");
	m_CtrlPigment.AddString("MineralRuby");
	m_CtrlPigment.AddString("MetalBrass");
	m_CtrlPigment.AddString("MetalCopper");
	m_CtrlPigment.AddString("MetalChrome");
	m_CtrlPigment.AddString("MetalSilver");
	m_CtrlPigment.AddString("MetalRed");
	m_CtrlPigment.AddString("MetalGreen");
	m_CtrlPigment.AddString("MetalBlue");
	m_CtrlPigment.AddString("MetalOrange");

	m_CtrlFinish.AddString("None");
	m_CtrlFinish.AddString("FGlass1");
	m_CtrlFinish.AddString("FGlass2");
	m_CtrlFinish.AddString("FGlass3");
	m_CtrlFinish.AddString("FGlass4");
	m_CtrlFinish.AddString("FGlass5");
	m_CtrlFinish.AddString("FMelta1");
	m_CtrlFinish.AddString("FMelta2");
	m_CtrlFinish.AddString("FMelta3");
	m_CtrlFinish.AddString("FMelta4");
	m_CtrlFinish.AddString("FMelta5");

	m_CtrlInterior.AddString("None");
	m_CtrlInterior.AddString("IGlass1");
	m_CtrlInterior.AddString("IGlass2");
	m_CtrlInterior.AddString("IDGlass1");
	m_CtrlInterior.AddString("IDGlass2");
	m_CtrlInterior.AddString("ICGlass1");
	m_CtrlInterior.AddString("ICGlass2");

	int i,pos;
	for (i=0;i<m_pGeomView->GetComponentNum();i++)
	{
		CString str;
		str.Format("FUSION COMPONENT-%d",i);
		m_CtrlPrimitives.AddString(str);
		POVProperty* pProperty=new POVProperty;
		pProperty->bBound=true;
		pProperty->bBodyDir=true;
		pProperty->bBoundDir=true;
		pProperty->bObjDir=false;
		pProperty->pigment="MetalCopper";
		pProperty->finish="None";
		pProperty->interior="None";
		m_PPOVProperties.push_back(pProperty);
	}
	for (i=0;i<m_pGeomView->GetTComponentNum();i++)
	{
		CString str;
		str.Format("TRANSITION COMPONENT-%d",i);
		m_CtrlPrimitives.AddString(str);
		POVProperty* pProperty=new POVProperty;
		pProperty->bBound=true;
		pProperty->bBodyDir=true;
		pProperty->bBoundDir=true;
		pProperty->bObjDir=false;
		pProperty->pigment="MineralEmerald";
		pProperty->finish="None";
		pProperty->interior="None";
		m_PPOVProperties.push_back(pProperty);
	}
	CTransitionPrimitive* pPrimitive=m_pGeomView->GetTPrimitive();
	if(pPrimitive!=NULL)
	{
		m_CtrlPrimitives.AddString("TRANSITION PRIMITIVE");
		POVProperty* pProperty=new POVProperty;
		pProperty->bBound=false;
		pProperty->bBodyDir=false;
		pProperty->bBoundDir=false;
		pProperty->bObjDir=false;
		pProperty->pigment="GlassSkyBlue";
		pProperty->finish="FGlass1";
		pProperty->interior="None";
		m_PPOVProperties.push_back(pProperty);

	}
	CSolid *pFusionModel=m_pGeomView->GetFusionModel();
	if(pFusionModel!=NULL)
	{
		m_CtrlPrimitives.AddString("FUSION MODEL");
		POVProperty* pProperty=new POVProperty;
		pProperty->bBound=false;
		pProperty->bBodyDir=false;
		pProperty->bBoundDir=false;
		pProperty->bObjDir=false;
		pProperty->pigment="GlassRuby";
		pProperty->finish="FGlass1";
		pProperty->interior="IGlass1";
		m_PPOVProperties.push_back(pProperty);

	}

	for (i=0;i<m_pGeomView->GetSilhouetteNum();i++)
	{
		CString str;
		str.Format("SILHOUETTE STROKE-%d",i);
		m_CtrlPrimitives.AddString(str);
		POVProperty* pProperty=new POVProperty;
		pProperty->bBound=false;
		pProperty->bBodyDir=false;
		pProperty->bBoundDir=false;
		pProperty->bObjDir=false;
		pProperty->pigment="";
		pProperty->finish="";
		pProperty->interior="";
		m_PPOVProperties.push_back(pProperty);
	}



	if(m_PPOVProperties.size()==0)
	{
		m_CtrlOutput.EnableWindow(false);
		m_CtrlBoundDir.EnableWindow(false);
		m_CtrlBodyDir.EnableWindow(false);
		m_CtrlObjectDir.EnableWindow(false);
		m_CtrlBoundary.EnableWindow(false);
		m_CtrlPigment.EnableWindow(false);
		m_CtrlFinish.EnableWindow(false);
		m_CtrlInterior.EnableWindow(false);
	}
	else
	{
		m_CtrlScene.SetCurSel(0);
		m_CtrlPrimitives.SetCurSel(0);
		m_pCurPPOVProperty=m_PPOVProperties.at(0);
		m_CtrlOutput.SetCheck(m_pCurPPOVProperty->bOutput);

		m_CtrlBoundDir.EnableWindow(m_pCurPPOVProperty->bOutput);
		m_CtrlBodyDir.EnableWindow(m_pCurPPOVProperty->bOutput);
		m_CtrlObjectDir.EnableWindow(m_pCurPPOVProperty->bOutput);
		m_CtrlBoundary.EnableWindow(m_pCurPPOVProperty->bOutput);
		m_CtrlPigment.EnableWindow(m_pCurPPOVProperty->bOutput);
		m_CtrlFinish.EnableWindow(m_pCurPPOVProperty->bOutput);
		m_CtrlInterior.EnableWindow(m_pCurPPOVProperty->bOutput);

		m_CtrlBoundDir.SetCheck(m_pCurPPOVProperty->bBoundDir);
		m_CtrlBodyDir.SetCheck(m_pCurPPOVProperty->bBodyDir);
		m_CtrlObjectDir.SetCheck(m_pCurPPOVProperty->bObjDir);
		m_CtrlBoundary.SetCheck(m_pCurPPOVProperty->bBound);
		pos=m_CtrlPigment.FindStringExact(-1,m_pCurPPOVProperty->pigment);
		m_CtrlPigment.SetCurSel(pos);
		pos=m_CtrlFinish.FindStringExact(-1,m_pCurPPOVProperty->finish);
		m_CtrlFinish.SetCurSel(pos);
		pos=m_CtrlInterior.FindStringExact(-1,m_pCurPPOVProperty->interior);
		m_CtrlInterior.SetCurSel(pos);

	}
	
}

void CPOVSettingDialog::OnBnClickedExport()
{
	if(m_pCurPPOVProperty!=NULL)
	{
		m_pCurPPOVProperty->bBound=m_CtrlBoundary.GetCheck();
		m_pCurPPOVProperty->bBodyDir=m_CtrlBodyDir.GetCheck();
		m_pCurPPOVProperty->bBoundDir=m_CtrlBoundDir.GetCheck();
		m_pCurPPOVProperty->bObjDir=m_CtrlObjectDir.GetCheck();
		CString str;
		m_CtrlPigment.GetWindowText(str);
		m_pCurPPOVProperty->pigment=str;
		m_CtrlFinish.GetWindowText(str);
		m_pCurPPOVProperty->finish=str;
		m_CtrlInterior.GetWindowText(str);
		m_pCurPPOVProperty->interior=str;
	}

	CPOVFileWriter POVWriter;
	POVWriter.InitFile("e:\\Scene.pov");
	POVWriter.SetupEnvironment(m_pGeomView->m_OpenGLManager,m_CtrlScene.GetCurSel());

	int i,j,k;
	POVProperty * pProperty;
	CSolid* pComponent;
	CBoundary* pBound;
	CVector3d np,nb,n,cb,cp,ct;
	double eimag[3];
	CPrincipleAnalysis pa;
	vector<CVector3d> pointcloud;

	for (i=0;i<m_pGeomView->GetComponentNum();i++)
	{
		pComponent=m_pGeomView->GetComponentByIndex(i);
		pProperty=m_PPOVProperties.at(i);

		if(!pProperty->bOutput)continue;
		CString material;
		material.Format("%s|%s|%s",pProperty->pigment,pProperty->finish,pProperty->interior);
		POVWriter.WriteEntity(pComponent,material);
		if(pProperty->bBound)
		{
			for (j=0;j<pComponent->GetBoundaryNum();j++)
			{
				pBound=pComponent->GetBoundaryByIndex(j);
				POVWriter.WriteBoundary(pBound);
			}
		}
		if(pProperty->bBoundDir||pProperty->bBodyDir||pProperty->bObjDir)
		{
			pBound=pComponent->GetBoundaryByIndex(0);
			CFrame *pFrame=new CFrame;				
			cb.Set(0.0,0.0,0.0);
			for (k=0;k<pBound->GetVertexNum();k++)
				cb+=pBound->GetVertexByIndex(k)->GetCoordinate();
			cb/=pBound->GetVertexNum();
			pFrame->SetOrigin(cb);
			for(k=0;k<pBound->GetVertexNum();k++)
				pointcloud.push_back((pBound->GetVertexByIndex(k)->GetCoordinate()-cb));
			pa.Execute(pointcloud,pBound->GetVertexNum(),pFrame,eimag);
			nb=(*pFrame)[2];
			pointcloud.clear();


			cp.Set(0.0,0.0,0.0);
			for(j=0;j<pComponent->GetVertexNum();j++)
				cp+=pComponent->GetVertexByIndex(j)->GetCoordinate();
			cp/=pComponent->GetVertexNum();
			for(j=0;j<pComponent->GetVertexNum();j++)
				pointcloud.push_back((pComponent->GetVertexByIndex(j)->GetCoordinate()-cp));
			pa.Execute(pointcloud,pComponent->GetVertexNum(),pFrame,eimag);
			np=(*pFrame)[0];
			pointcloud.clear();
			delete pFrame;

			nb*=nb.Dot(cb-cp);
			nb.Normalize();

			np*=np.Dot(cb-cp);
			np.Normalize();

			if(pProperty->bBoundDir)
				POVWriter.WriteVector(cb,nb);
			if(pProperty->bBodyDir)
				POVWriter.WriteVector(cp,np);


		}
		if(pProperty->bObjDir)
		{
			n=nb+0.2*np;
			n.Normalize();
			POVWriter.WriteVector(cb,n);
		}
	}

	for (i=0;i<m_pGeomView->GetTComponentNum();i++)
	{
		pComponent=m_pGeomView->GetTComponentByIndex(i);
		pProperty=m_PPOVProperties.at(m_pGeomView->GetComponentNum()+i);

		if(!pProperty->bOutput)continue;

		CString material;
		material.Format("%s|%s|%s",pProperty->pigment,pProperty->finish,pProperty->interior);
		POVWriter.WriteEntity(pComponent,material);
		if(pProperty->bBound)
		{
			for (j=0;j<pComponent->GetBoundaryNum();j++)
			{
				pBound=pComponent->GetBoundaryByIndex(j);
				POVWriter.WriteBoundary(pBound);
			}
		}
		if(pProperty->bBoundDir||pProperty->bObjDir)
		{
			for (j=0;j<pComponent->GetBoundaryNum();j++)
			{
				pBound=pComponent->GetBoundaryByIndex(j);
				CFrame *pFrame=new CFrame;				
				cb.Set(0.0,0.0,0.0);
				for (k=0;k<pBound->GetVertexNum();k++)
					cb+=pBound->GetVertexByIndex(k)->GetCoordinate();
				cb/=pBound->GetVertexNum();
				pFrame->SetOrigin(cb);
				for(k=0;k<pBound->GetVertexNum();k++)
					pointcloud.push_back((pBound->GetVertexByIndex(k)->GetCoordinate()-cb));
				pa.Execute(pointcloud,pBound->GetVertexNum(),pFrame,eimag);
				nb=(*pFrame)[2];
				pointcloud.clear();
				delete pFrame;
				if(pProperty->bBoundDir)
					POVWriter.WriteVector(cb,nb);
			}

		}
		if(pProperty->bBodyDir||pProperty->bObjDir)
		{
			CFrame *pFrame=new CFrame;				
			cp.Set(0.0,0.0,0.0);
			for(j=0;j<pComponent->GetVertexNum();j++)
				cp+=pComponent->GetVertexByIndex(j)->GetCoordinate();
			cp/=pComponent->GetVertexNum();
			for(j=0;j<pComponent->GetVertexNum();j++)
				pointcloud.push_back((pComponent->GetVertexByIndex(j)->GetCoordinate()-cp));
			pa.Execute(pointcloud,pComponent->GetVertexNum(),pFrame,eimag);
			np=(*pFrame)[0];
			pointcloud.clear();
			delete pFrame;
			if(pProperty->bBodyDir)
				POVWriter.WriteVector(cp,np);
		}
		if(pProperty->bObjDir)
		{
			n=nb+0.2*np;
			n.Normalize();
			POVWriter.WriteVector(cb,n);
		}
	}

	int count=m_pGeomView->GetComponentNum()+m_pGeomView->GetTComponentNum();

	CTransitionPrimitive* pPrimitive=m_pGeomView->GetTPrimitive();
	if(pPrimitive!=NULL)
	{
		count++;
		pProperty=m_PPOVProperties.at(m_pGeomView->GetComponentNum()+m_pGeomView->GetTComponentNum());
		if(pProperty->bOutput)
		{
			CString material;
			material.Format("%s|%s|%s",pProperty->pigment,pProperty->finish,pProperty->interior);
			if(!pPrimitive->GetType())
				POVWriter.WriteTransitionSphere((CTransitionSphere*)pPrimitive,material);
			else
				POVWriter.WriteTransitionCylinder((CTransitionCylinder*)pPrimitive,material);

			if(pProperty->bBodyDir)
			{
				CVector3d loc=pPrimitive->GetLocatorByIndex(0);
				CVector3d dir=pPrimitive->GetCenter()-loc;
				dir.Normalize();
				POVWriter.WriteVector(loc,dir);
				loc=pPrimitive->GetLocatorByIndex(1);
				dir=pPrimitive->GetCenter()-loc;
				dir.Normalize();
				POVWriter.WriteVector(loc,dir);
				loc=pPrimitive->GetLocatorByIndex(2);
				dir=pPrimitive->GetCenter()-loc;
				dir.Normalize();
				POVWriter.WriteVector(loc,dir);
			}



		}
	}
	CSolid* pFusionModel=m_pGeomView->GetFusionModel();
	if(pFusionModel!=NULL)
	{
		count++;
		pProperty=m_PPOVProperties.at(m_pGeomView->GetComponentNum()+m_pGeomView->GetTComponentNum()+1);
		if(pProperty->bOutput)
		{
			CString material;
			material.Format("%s|%s|%s",pProperty->pigment,pProperty->finish,pProperty->interior);
			POVWriter.WriteEntity(pFusionModel,material);
			if(pProperty->bBound)
			{
				for (j=0;j<pFusionModel->GetBoundaryNum();j++)
				{
					pBound=pFusionModel->GetBoundaryByIndex(j);
					POVWriter.WriteBoundary(pBound);
				}
			}
			if(pProperty->bBoundDir||pProperty->bObjDir)
			{
				for (j=0;j<pComponent->GetBoundaryNum();j++)
				{
					pBound=pComponent->GetBoundaryByIndex(j);
					CFrame *pFrame=new CFrame;				
					cb.Set(0.0,0.0,0.0);
					for (k=0;k<pBound->GetVertexNum();k++)
						cb+=pBound->GetVertexByIndex(k)->GetCoordinate();
					cb/=pBound->GetVertexNum();
					pFrame->SetOrigin(cb);
					for(k=0;k<pBound->GetVertexNum();k++)
						pointcloud.push_back((pBound->GetVertexByIndex(k)->GetCoordinate()-cb));
					pa.Execute(pointcloud,pBound->GetVertexNum(),pFrame,eimag);
					nb=(*pFrame)[2];
					pointcloud.clear();
					delete pFrame;
					if(pProperty->bBoundDir)
						POVWriter.WriteVector(cb,nb);
				}

			}
			if(pProperty->bBodyDir||pProperty->bObjDir)
			{
				CFrame *pFrame=new CFrame;				
				cp.Set(0.0,0.0,0.0);
				for(j=0;j<pComponent->GetVertexNum();j++)
					cp+=pComponent->GetVertexByIndex(j)->GetCoordinate();
				cp/=pComponent->GetVertexNum();
				for(j=0;j<pComponent->GetVertexNum();j++)
					pointcloud.push_back((pComponent->GetVertexByIndex(j)->GetCoordinate()-cp));
				pa.Execute(pointcloud,pComponent->GetVertexNum(),pFrame,eimag);
				np=(*pFrame)[0];
				pointcloud.clear();
				delete pFrame;
				if(pProperty->bBodyDir)
					POVWriter.WriteVector(cp,np);
			}
			if(pProperty->bObjDir)
			{
				n=nb+0.2*np;
				n.Normalize();
				POVWriter.WriteVector(cb,n);
			}
		}
	}

	CStroke *pStroke;
	for (i=0;i<m_pGeomView->GetSilhouetteNum();i++)
	{
		pStroke=m_pGeomView->GetSilhouetteByIndex(i);
		pProperty=m_PPOVProperties.at(count+i);
		if(pProperty->bOutput)
			POVWriter.WriteStroke(pStroke);

	}


	POVWriter.CloseFile();



}

void CPOVSettingDialog::SetView(CGeometryStudioView* pView)
{
	m_pGeomView=pView;
}

void CPOVSettingDialog::OnClose()
{
	CMainFrame *pFrameWnd = (CMainFrame*)AfxGetMainWnd();
	pFrameWnd->OnViewPovray();
}


