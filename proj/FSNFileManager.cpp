#include "StdAfx.h"
#include ".\fsnfilemanager.h"
#include "TransitionSphere.h"
#include "TransitionCylinder.h"
#include <fstream>
using namespace std;
CFSNFileManager::CFSNFileManager(void)
{
	m_pFusionModel=NULL;
	m_pTPrimitive=NULL;
	m_pParticleSys=NULL;
}

CFSNFileManager::~CFSNFileManager(void)
{
	int i;
	for (i=0;i<m_vecComponents.size();i++)
        delete m_vecComponents[i];
	for (i=0;i<m_vecTComponents.size();i++)
		delete m_vecTComponents[i];

	for (i=0;i<m_vecSilhouetteStroke.size();i++)
        delete m_vecSilhouetteStroke[i];
	
	if(m_pFusionModel!=NULL)
        delete m_pFusionModel;
	

}

void CFSNFileManager::Write(string path)
{
	ofstream ofile(path.c_str());

	if(!ofile)
	{
		return;
	}

	CSolid *pComponent;
	CFace *pF;
	CVertex *pV;
	CHalfEdge *pHE;
	CBoundary *pBound;
	CVector3d coord,normal;
	int PntT;
	int vindex[3];

	float rad;

	unsigned int i,j,k;

	int numFusion,numPS,numTPrimitive;

	if(m_pFusionModel!=NULL)
		numFusion=1;
	else
		numFusion=0;

	if(m_pTPrimitive!=NULL)
		numTPrimitive=1;
	else
		numTPrimitive=0;

	if(m_pParticleSys!=NULL)
		numPS=1;
	else
		numPS=0;


	ofile<<numFusion<<" "<<m_vecComponents.size()<<" "<<m_vecTComponents.size()<<" "<<numTPrimitive<<" "<<numPS<<" "<<m_vecSilhouetteStroke.size()<<" "<<m_vecStructureStroke.size()<<endl;

	//ofile<<"-- FSN File Created by GeometryStudio --"<<endl;
	if(m_pFusionModel!=NULL)
	{

		ofile<<m_pFusionModel->GetVertexNum()<<" "<<m_pFusionModel->GetFaceNum()<<" "<<m_pFusionModel->GetBoundaryNum()<<endl;


		for (i=0;i<m_pFusionModel->GetVertexNum();i++)
		{
			pV=m_pFusionModel->GetVertexByIndex(i);
			pV->SetIndex(i);
			coord=pV->GetCoordinate();
			ofile<<coord[0]<<" "<<coord[1]<<" "<<coord[2]<<endl;
		}
		for (i=0;i<m_pFusionModel->GetFaceNum();i++)
		{
			pF=m_pFusionModel->GetFaceByIndex(i);
			pF->SetIndex(i);

			pHE=pF->GetFirstEdge();
			vindex[0]=pHE->GetVertex()->GetIndex();

			pHE=pF->GetNextEdge(pHE);
			vindex[1]=pHE->GetVertex()->GetIndex();

			pHE=pF->GetNextEdge(pHE);
			vindex[2]=pHE->GetVertex()->GetIndex();

			ofile<<vindex[0]<<" "<<vindex[1]<<" "<<vindex[2]<<endl;
		}

		for (i=0;i<m_vecComponents.size();i++)
		{
			pComponent=m_vecComponents.at(i);

			ofile<<pComponent->GetVertexNum()<<" "<<pComponent->GetFaceNum()<<" "<<pComponent->GetBoundaryNum()<<endl;
			for (j=0;j<pComponent->GetFaceNum();j++)
			{
				pF=pComponent->GetFaceByIndex(j);
				ofile<<pF->GetIndex()<<endl;
			}
			for (j=0;j<pComponent->GetBoundaryNum();j++)
			{
				pBound=pComponent->GetBoundaryByIndex(j);
				ofile<<pBound->GetVertexNum();
				for (k=0;k<pBound->GetVertexNum();k++)
				{

					ofile<<" "<<pBound->GetVertexByIndex(k)->GetIndex();
				}
				ofile<<endl;
			}
		}
		for (i=0;i<m_vecTComponents.size();i++)
		{
			pComponent=m_vecTComponents.at(i);

			ofile<<pComponent->GetVertexNum()<<" "<<pComponent->GetFaceNum()<<" "<<pComponent->GetBoundaryNum()<<endl;
	
			for (j=0;j<pComponent->GetFaceNum();j++)
			{
				pF=pComponent->GetFaceByIndex(j);

				ofile<<pF->GetIndex()<<endl;

			}
			for (j=0;j<pComponent->GetBoundaryNum();j++)
			{
				pBound=pComponent->GetBoundaryByIndex(j);
				ofile<<pBound->GetVertexNum();
				for (k=0;k<pBound->GetVertexNum();k++)
				{

					ofile<<" "<<pBound->GetVertexByIndex(k)->GetIndex();
				}
				ofile<<endl;
			}
		}

	}
	else
	{
		for (i=0;i<m_vecComponents.size();i++)
		{
			pComponent=m_vecComponents.at(i);

			ofile<<pComponent->GetVertexNum()<<" "<<pComponent->GetFaceNum()<<" "<<pComponent->GetBoundaryNum()<<endl;
			for (j=0;j<pComponent->GetVertexNum();j++)
			{
				pV=pComponent->GetVertexByIndex(j);
				pV->SetIndex(j);
				coord=pV->GetCoordinate();
				ofile<<coord[0]<<" "<<coord[1]<<" "<<coord[2]<<endl;
			}
			for (j=0;j<pComponent->GetFaceNum();j++)
			{
				pF=pComponent->GetFaceByIndex(j);

				pHE=pF->GetFirstEdge();
				vindex[0]=pHE->GetVertex()->GetIndex();

				pHE=pF->GetNextEdge(pHE);
				vindex[1]=pHE->GetVertex()->GetIndex();

				pHE=pF->GetNextEdge(pHE);
				vindex[2]=pHE->GetVertex()->GetIndex();

				ofile<<vindex[0]<<" "<<vindex[1]<<" "<<vindex[2]<<endl;
			}
			for (j=0;j<pComponent->GetBoundaryNum();j++)
			{
				pBound=pComponent->GetBoundaryByIndex(j);
				ofile<<pBound->GetVertexNum();
				for (k=0;k<pBound->GetVertexNum();k++)
				{

					ofile<<" "<<pBound->GetVertexByIndex(k)->GetIndex();
				}
				ofile<<endl;
			}
		}

		for (i=0;i<m_vecTComponents.size();i++)
		{
			pComponent=m_vecTComponents.at(i);

			ofile<<pComponent->GetVertexNum()<<" "<<pComponent->GetFaceNum()<<" "<<pComponent->GetBoundaryNum()<<endl;
			for (j=0;j<pComponent->GetVertexNum();j++)
			{
				pV=pComponent->GetVertexByIndex(j);
				pV->SetIndex(j);
				coord=pV->GetCoordinate();
				ofile<<coord[0]<<" "<<coord[1]<<" "<<coord[2]<<endl;
			}
			for (j=0;j<pComponent->GetFaceNum();j++)
			{
				pF=pComponent->GetFaceByIndex(j);

				pHE=pF->GetFirstEdge();
				vindex[0]=pHE->GetVertex()->GetIndex();

				pHE=pF->GetNextEdge(pHE);
				vindex[1]=pHE->GetVertex()->GetIndex();

				pHE=pF->GetNextEdge(pHE);
				vindex[2]=pHE->GetVertex()->GetIndex();

				ofile<<vindex[0]<<" "<<vindex[1]<<" "<<vindex[2]<<endl;
			}
			for (j=0;j<pComponent->GetBoundaryNum();j++)
			{
				pBound=pComponent->GetBoundaryByIndex(j);
				ofile<<pBound->GetVertexNum();
				for (k=0;k<pBound->GetVertexNum();k++)
				{

					ofile<<" "<<pBound->GetVertexByIndex(k)->GetIndex();
				}
				ofile<<endl;
			}
		}

	}
	if(numTPrimitive)
	{
		CVector3d cnt=m_pTPrimitive->GetCenter();
		ofile<<m_pTPrimitive->GetType()<<endl;
		ofile<<cnt[0]<<" "<<cnt[1]<<" "<<cnt[2]<<endl;
		ofile<<m_pTPrimitive->GetLocatorNum()<<endl;
		for (i=0;i<m_pTPrimitive->GetLocatorNum();i++)
		{
			CVector3d loc=m_pTPrimitive->GetLocatorByIndex(i);
			ofile<<loc[0]<<" "<<loc[1]<<" "<<loc[2]<<endl;
		}

		if(m_pTPrimitive->GetType()==0)
		{
			ofile<<((CTransitionSphere*)m_pTPrimitive)->GetRadius()<<endl;
		}
		else
		{
			ofile<<((CTransitionCylinder*)m_pTPrimitive)->GetTopRadius()<<endl;
			ofile<<((CTransitionCylinder*)m_pTPrimitive)->GetBottomRadius()<<endl;
			ofile<<((CTransitionCylinder*)m_pTPrimitive)->GetHeight()<<endl;
			CVector3d dir=m_pTPrimitive->GetDirection();
			ofile<<dir[0]<<" "<<dir[1]<<" "<<dir[2]<<" "<<endl;
		}

	}
	if(numPS)
	{
		ofile<<m_pParticleSys->GetParticleNum()<<endl;
		for (j=0;j<m_pParticleSys->GetParticleNum();j++)
		{
			m_pParticleSys->GetParticleByIndex(j,coord,normal,rad,PntT);
			ofile<<coord[0]<<" "<<coord[1]<<" "<<coord[2]<<" "<<normal[0]<<" "<<normal[1]<<" "<<normal[2]<<" "<<rad<<" "<<PntT<<endl;
		}		
	}

	for (i=0;i<m_vecSilhouetteStroke.size();i++)
	{
		CStroke* pStroke=m_vecSilhouetteStroke[i];
		ofile<<pStroke->GetPointsNum()<<endl;
		for (j=0;j<pStroke->GetPointsNum();j++)
		{
			coord=pStroke->GetPoint(j);
			ofile<<coord[0]<<" "<<coord[1]<<" "<<coord[2]<<endl;
		}
		for (j=0;j<pStroke->GetPointsNum();j++)
		{
			coord=pStroke->GetNormal(j);
			ofile<<coord[0]<<" "<<coord[1]<<" "<<coord[2]<<endl;
		}

	}

	for (i=0;i<m_vecStructureStroke.size();i++)
	{
		CStroke* pStroke=m_vecStructureStroke[i];
		ofile<<pStroke->GetPointsNum()<<endl;
		for (j=0;j<pStroke->GetPointsNum();j++)
		{
			coord=pStroke->GetPoint(j);
			ofile<<coord[0]<<" "<<coord[1]<<" "<<coord[2]<<endl;
		}
		for (j=0;j<pStroke->GetPointsNum();j++)
		{
			coord=pStroke->GetNormal(j);
			ofile<<coord[0]<<" "<<coord[1]<<" "<<coord[2]<<endl;
		}

	}

}

void CFSNFileManager::Read(string path)
{
	ifstream ifile(path.c_str());

	//ifile>>str;

	int numFus,numComp,numTComp,numTPri,numV,numF,numB,numBV,numPS,numSil,numStr;
	ifile>>numFus>>numComp>>numTComp>>numTPri>>numPS>>numSil>>numStr;

	double coord[3],normal[3];
	int vindex[3],bvindex,svindex,findex;

	vector<CVertex*> vecVertices;
	vector<CFace*> vecFaces;

	int i,j,k,PntT;
	float rad;

	if(numFus)
	{
		vector<CFace*> vecSubFaces;
		m_pFusionModel=new CSolid;
		ifile>>numV>>numF>>numB;
		for (i=0;i<numV;i++)
		{
			ifile>>coord[0]>>coord[1]>>coord[2];
			CVertex* pV=new CVertex(coord[0],coord[1],coord[2],i);
			vecVertices.push_back(pV);
		}
		for (i=0;i<numF;i++)
		{
			ifile>>vindex[0]>>vindex[1]>>vindex[2];

			CVertex *p1=vecVertices.at(vindex[0]);
			CVertex *p2=vecVertices.at(vindex[1]);
			CVertex *p3=vecVertices.at(vindex[2]);

			CFace *pF=new CFace(m_pFusionModel);
			pF->Init(p1,p2,p3,i);
			vecFaces.push_back(pF);

		}

		m_pFusionModel->ImportFromList(vecVertices,vecFaces);
		for (i=0;i<numB;i++)
		{
			CBoundary *pBound=new CBoundary;
			ifile>>numBV;
			for (k=0;k<numBV;k++)
			{
				ifile>>bvindex;
				pBound->AddVertex(vecVertices.at(bvindex));
			}
			pBound->CollectEdges();
			m_pFusionModel->AddBoundary(pBound);

		}
	
		for (i=0;i<numComp;i++)
		{
			CSolid* pComponent=new CSolid;
			pComponent->SetIndex(i);
			ifile>>numV>>numF>>numB;
			for (j=0;j<numF;j++)
			{
				ifile>>findex;
				vecSubFaces.push_back(vecFaces[findex]);
			}
			pComponent->ImportFromFaceList(vecSubFaces);
			for (j=0;j<numB;j++)
			{
				CBoundary *pBound=new CBoundary;
				ifile>>numBV;
				for (k=0;k<numBV;k++)
				{
					ifile>>bvindex;

					pBound->AddVertex(vecVertices.at(bvindex));
				}
				pBound->CollectEdges();
				pComponent->AddBoundary(pBound);

			}
			m_vecComponents.push_back(pComponent);
			vecSubFaces.clear();
		}

		for (i=0;i<numTComp;i++)
		{
			CSolid* pComponent=new CSolid;
			pComponent->SetMaterial("Light Blue");
			pComponent->SetBoundaryShow(false);
			ifile>>numV>>numF>>numB;
			for (j=0;j<numF;j++)
			{
				ifile>>findex;
				vecSubFaces.push_back(vecFaces[findex]);
			}

			pComponent->ImportFromFaceList(vecSubFaces);

			for (j=0;j<numB;j++)
			{
				CBoundary *pBound=new CBoundary;
				ifile>>numBV;
				for (k=0;k<numBV;k++)
				{
					ifile>>bvindex;
					pBound->AddVertex(vecVertices.at(bvindex));
				}
				pBound->CollectEdges();
				pComponent->AddBoundary(pBound);

			}
			m_vecTComponents.push_back(pComponent);
		}

	}
	else
	{
		for (i=0;i<numComp;i++)
		{
			CSolid* pComponent=new CSolid;
			pComponent->SetIndex(i);
			ifile>>numV>>numF>>numB;
			for (j=0;j<numV;j++)
			{
				ifile>>coord[0]>>coord[1]>>coord[2];
				CVertex* pV=new CVertex(coord[0],coord[1],coord[2],j);
				vecVertices.push_back(pV);
			}
			for (j=0;j<numF;j++)
			{
				ifile>>vindex[0]>>vindex[1]>>vindex[2];

				CVertex *p1=vecVertices.at(vindex[0]);
				CVertex *p2=vecVertices.at(vindex[1]);
				CVertex *p3=vecVertices.at(vindex[2]);

				CFace *pF=new CFace(pComponent);
				pF->Init(p1,p2,p3,j);
				vecFaces.push_back(pF);
			}
			pComponent->ImportFromList(vecVertices,vecFaces);
			for (j=0;j<numB;j++)
			{
				CBoundary *pBound=new CBoundary;
				ifile>>numBV;
				for (k=0;k<numBV;k++)
				{
					ifile>>bvindex;
					pBound->AddVertex(vecVertices.at(bvindex));
				}
				pBound->CollectEdges();
				pComponent->AddBoundary(pBound);

			}
			m_vecComponents.push_back(pComponent);
			vecVertices.clear();
			vecFaces.clear();

		}

		for (i=0;i<numTComp;i++)
		{
			CSolid* pComponent=new CSolid;
			pComponent->SetMaterial("Light Blue");
			pComponent->SetBoundaryShow(false);
			ifile>>numV>>numF>>numB;
			for (j=0;j<numV;j++)
			{
				ifile>>coord[0]>>coord[1]>>coord[2];
				CVertex* pV=new CVertex(coord[0],coord[1],coord[2],j);
				vecVertices.push_back(pV);
			}
			for (j=0;j<numF;j++)
			{
				ifile>>vindex[0]>>vindex[1]>>vindex[2];

				CVertex *p1=vecVertices.at(vindex[0]);
				CVertex *p2=vecVertices.at(vindex[1]);
				CVertex *p3=vecVertices.at(vindex[2]);

				CFace *pF=new CFace(pComponent);
				pF->Init(p1,p2,p3,j);
				vecFaces.push_back(pF);
			}
			pComponent->ImportFromList(vecVertices,vecFaces);

			for (j=0;j<numB;j++)
			{
				CBoundary *pBound=new CBoundary;
				ifile>>numBV;
				for (k=0;k<numBV;k++)
				{
					ifile>>bvindex;
					pBound->AddVertex(vecVertices.at(bvindex));
				}
				pBound->CollectEdges();
				pComponent->AddBoundary(pBound);
				

			}
			m_vecTComponents.push_back(pComponent);
		}

	}

	if(numTPri)
	{
		int type,i,lnum;
		double cnt[3],loc[3],radius,height;
		
		ifile>>type;

		if(type==0)
		{
			CTransitionSphere *pTSphere=new CTransitionSphere;
			m_pTPrimitive=pTSphere;
			ifile>>cnt[0]>>cnt[1]>>cnt[2];
			pTSphere->SetCenter(CVector3d(cnt));
			ifile>>lnum;

			for (i=0;i<lnum;i++)
			{
				ifile>>loc[0]>>loc[1]>>loc[2];
				pTSphere->AddLocator(CVector3d(loc));
			}
			ifile>>radius;
			pTSphere->SetRadius(radius);


		}
		else
		{
			CTransitionCylinder *pTCylinder=new CTransitionCylinder;
			m_pTPrimitive=pTCylinder;
			ifile>>cnt[0]>>cnt[1]>>cnt[2];
			pTCylinder->SetCenter(CVector3d(cnt));
			ifile>>lnum;

			for (i=0;i<lnum;i++)
			{
				ifile>>loc[0]>>loc[1]>>loc[2];
				pTCylinder->AddLocator(CVector3d(loc));
			}
			ifile>>radius;
			pTCylinder->SetTopRadius(radius);
			ifile>>radius;
			pTCylinder->SetBottomRadius(radius);
			ifile>>height;
			pTCylinder->SetHeight(height);
			ifile>>loc[0]>>loc[1]>>loc[2];
			pTCylinder->SetDirection(CVector3d(loc));
			pTCylinder->CalcLimit();

		}



	}

	if(numPS)
	{
		m_pParticleSys=new CParticleSamplingSys;

		ifile>>numV;
		for (i=0;i<numV;i++)
		{
			ifile>>coord[0]>>coord[1]>>coord[2]>>normal[0]>>normal[1]>>normal[2]>>rad>>PntT;
			m_pParticleSys->InsertParticle(CVector3d(coord),CVector3d(normal),rad,PntT);
		}

	}
	int numP;
	for (i=0;i<numSil;i++)
	{
		CStroke *pStroke=new CStroke;
		ifile>>numP;

		for(j=0;j<numP;j++)
		{
			ifile>>coord[0]>>coord[1]>>coord[2];
			pStroke->AddPoint(CVector3d(coord));
		}
		for(j=0;j<numP;j++)
		{
			ifile>>coord[0]>>coord[1]>>coord[2];
			pStroke->AddNormal(CVector3d(coord));
		}

		m_vecSilhouetteStroke.push_back(pStroke);

	}
	for (i=0;i<numStr;i++)
	{
		CStroke *pStroke=new CStroke;
		ifile>>numP;

		for(j=0;j<numP;j++)
		{
			ifile>>coord[0]>>coord[1]>>coord[2];
			pStroke->AddPoint(CVector3d(coord));
		}
		for(j=0;j<numP;j++)
		{
			ifile>>coord[0]>>coord[1]>>coord[2];
			pStroke->AddNormal(CVector3d(coord));
		}

		m_vecSilhouetteStroke.push_back(pStroke);

	}


}

int CFSNFileManager::GetComponentNum(void)
{
	return m_vecComponents.size();
}

CSolid* CFSNFileManager::GetComponentByIndex(int index)
{
	return m_vecComponents.at(index);
}

int CFSNFileManager::GetTComponentNum(void)
{
	return m_vecTComponents.size();
}

CSolid* CFSNFileManager::GetTComponentByIndex(int index)
{
	return m_vecTComponents.at(index);
}

void CFSNFileManager::RegisterComponent(CSolid* pComponent)
{
	m_vecComponents.push_back(pComponent);
}

void CFSNFileManager::RegisterTComponent(CSolid* pComponent)
{
	m_vecTComponents.push_back(pComponent);
}

void CFSNFileManager::RegisterTPrimitive(CTransitionPrimitive* pPrimitive)
{
	m_pTPrimitive=pPrimitive;
}

void CFSNFileManager::RegisterFusionModel(CSolid* pFusModel)
{
	m_pFusionModel=pFusModel;
}

CSolid* CFSNFileManager::GetFusionModel(void)
{
	return m_pFusionModel;
}

void CFSNFileManager::RegisterPS(CParticleSamplingSys* pPS)
{
	m_pParticleSys=pPS;
}


CTransitionPrimitive* CFSNFileManager::GetTPrimitive(void)
{
	return m_pTPrimitive;
}

CParticleSamplingSys* CFSNFileManager::GetPS(void)
{
	return m_pParticleSys;
}

void CFSNFileManager::RegisterSilhouette(CStroke* pStroke)
{
	m_vecSilhouetteStroke.push_back(pStroke);
}

CStroke* CFSNFileManager::GetSilhouetteByIndex(int index)
{
	return m_vecSilhouetteStroke[index];
}

int CFSNFileManager::GetSilhouetteNum(void)
{
	return m_vecSilhouetteStroke.size();
}

void CFSNFileManager::RemoveSilhouette(CStroke* pStroke)
{
	vector<CStroke*>::iterator it=find(m_vecSilhouetteStroke.begin(),m_vecSilhouetteStroke.end(),pStroke);
	m_vecSilhouetteStroke.erase(it);
}

void CFSNFileManager::RegisterStructure(CStroke* pStroke)
{
	m_vecStructureStroke.push_back(pStroke);
}

int CFSNFileManager::GetStructureNum(void)
{
	return m_vecStructureStroke.size();
}

CStroke* CFSNFileManager::GetStructureByIndex(int index)
{
	return m_vecStructureStroke.at(index);
}

void CFSNFileManager::RemoveStructure(CStroke* pStroke)
{
	vector<CStroke*>::iterator it=find(m_vecStructureStroke.begin(),m_vecStructureStroke.end(),pStroke);
	if (it!=m_vecStructureStroke.end())
	{
		m_vecStructureStroke.erase(it);
		delete pStroke;
	}
}
