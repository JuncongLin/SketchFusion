#include "StdAfx.h"
#include ".\areaequalizationremesh.h"
#include "global.h"
#include <fstream>
extern ofstream outfile;
using namespace std;
CAreaEqualizationRemesh::CAreaEqualizationRemesh(void)
{
}

CAreaEqualizationRemesh::~CAreaEqualizationRemesh(void)
{
}

int CAreaEqualizationRemesh::Run(double TLength)
{
	CVertex *pV,*pNV;
	CFace *pF;
	CHalfEdge *pHE;
	int nIter,nSteps=3;
	int i;


	for (nIter=0;nIter<nSteps;nIter++)
	{
		//----------------------------------------------------------------------------------------
		//	Step 1: split all edges at their midpoint that are longer than 4/3 of standardLength
		outfile<<nIter<<endl;
		EdgeSplitByLength(1.5*TLength);



		//----------------------------------------------------------------------------------------
		//	Step 2: Collapse all edges shorter than 4/5 of standardLength into their midpoint
		EdgeCollapseByLength(0.8*TLength);

		//		//----------------------------------------------------------------------------------------
		//		//	Step 3: Flip edges in order to minimize the deviation from valence 6 (or 4 on boundaris)

		EdgeSwapByValence();

		//		//----------------------------------------------------------------------------------------
		//		//	Step 4: Relocate vertices on the surface by tangential smoothing
		VertexRelocationUniform(0.1,8);

		for (i=0;i<m_pEntity->GetFaceNum();i++)
		{
			pF=m_pEntity->GetFaceByIndex(i);
			pF->CalcNormal();
		}


		for (i=0;i<m_pEntity->GetVertexNum();i++)
		{
			pV=m_pEntity->GetVertexByIndex(i);
			pV->CalcNormal();
		}

	}

	return -1;

}

int CAreaEqualizationRemesh::EdgeSplitByLength(double criterion)
{
	int i;
	CEdge *pE;
	vector<CEdge*> vecEdges;


	for (i=0;i<m_pEntity->GetEdgeNum();i++)
	{
		pE=m_pEntity->GetEdgeByIndex(i);
		if(pE->Length()>criterion)vecEdges.push_back(pE);
	}
	for(i=0;i<vecEdges.size();i++)
	{
		pE=vecEdges.at(i);
		if(pE->GetHe1()->GetVertex()->IsBoundary()&&pE->GetHe2()->GetVertex()->IsBoundary())continue;
		EdgeSplit(pE);
	}
	return pE->GetIndex()+1;
}

void CAreaEqualizationRemesh::EdgeCollapseByLength(double criterion)
{
	int i;
	CEdge *pE,*pE1,*pE2;
	CVertex *pV;
	vector<CEdge*> vecEdges;
	vector<CEdge*>::iterator eit;

	CHalfEdge *pHE;

	for (i=0;i<m_pEntity->GetEdgeNum();i++)
	{
		pE=m_pEntity->GetEdgeByIndex(i);
		if(pE->Length()<criterion)
		{
			vecEdges.push_back(pE);
		}
	}
	while(vecEdges.size())
	{
		pE=vecEdges.back();
		vecEdges.pop_back();
		if(pE->GetHe1()->GetVertex()->IsBoundary()||pE->GetHe2()->GetVertex()->IsBoundary())continue;

		pE1=pE->GetHe1()->GetPrevEdge()->GetEdge();
		pE2=pE->GetHe2()->GetNextEdge()->GetEdge();
		pV=pE->GetHe2()->GetVertex();

		if(!EdgeCollapse(pE))continue;

		eit=find(vecEdges.begin(),vecEdges.end(),pE1);
		if(eit!=vecEdges.end())
			vecEdges.erase(eit);                        
		m_pEntity->DeleteEdge(pE1);
		eit=find(vecEdges.begin(),vecEdges.end(),pE2);
		if(eit!=vecEdges.end())
			vecEdges.erase(eit);                        
		m_pEntity->DeleteEdge(pE2);

		pHE=pV->GetOutHalfEdge();
		do 
		{
			eit=find(vecEdges.begin(),vecEdges.end(),pHE->GetEdge());
			if(eit!=vecEdges.end())
				vecEdges.erase(eit);                      
			pHE=pV->NextOutHalfEdge(pHE);
		} while(pHE!=pV->GetOutHalfEdge());

	}
}

void CAreaEqualizationRemesh::EdgeSwapByValence(void)
{
	int i;
	CEdge *pE;
	CHalfEdge *pHE1,*pHE2,*pPrevHE1,*pPrevHE2;
	CVertex *pV1,*pV2,*pV3,*pV4;

	int beforeValenceError,afterValenceError,dd,sd=6;

	for (i=0;i<m_pEntity->GetEdgeNum();i++)
	{
		pE=m_pEntity->GetEdgeByIndex(i);
		pHE1=pE->GetHe1();
		pHE2=pE->GetHe2();
		if (pHE1==NULL||pHE2==NULL)
		{
			continue;
		}
		pPrevHE1=pHE1->GetPrevEdge();
		pPrevHE2=pHE2->GetPrevEdge();

		pV1=pHE1->GetVertex();
		pV2=pHE2->GetVertex();

		if(pV1->IsBoundary()&&pV2->IsBoundary())continue;
        

		pV3=pPrevHE1->GetVertex();
		pV4=pPrevHE2->GetVertex();


		beforeValenceError=0;	afterValenceError=0;

		dd=pV1->GetValency()-sd;
		beforeValenceError+=dd*dd;
		dd-=1;
		afterValenceError+=dd*dd;

		dd=pV2->GetValency()-sd;
		beforeValenceError+=dd*dd;
		dd-=1;
		afterValenceError+=dd*dd;

		dd=pV3->GetValency()-sd;
		beforeValenceError+=dd*dd;
		dd+=1;
		afterValenceError+=dd*dd;

		dd=pV4->GetValency()-sd;
		beforeValenceError+=dd*dd;
		dd+=1;
		afterValenceError+=dd*dd;


		if (afterValenceError<beforeValenceError)
		{
			EdgeSwap(pE);
		}
	}

}

void CAreaEqualizationRemesh::VertexRelocationUniform(double dampingFactor, int nSteps)
{
	int nIter;
	double *VertexArea=new double [m_pEntity->GetVertexNum()];
	CVertex *pV,*pNV;
	CHalfEdge *pHE;
	double denominator,factor,NN[3][3],area;
	CVector3d gp,pp,tp,normal;
	CVector3d *pCentroids=new CVector3d [m_pEntity->GetVertexNum()];
	int i,j;
	CFace* pF;

	for (i=0;i<m_pEntity->GetVertexNum();i++)
	{
		pV=m_pEntity->GetVertexByIndex(i);
		pV->SetIndex(i);
	}

	CBoundary* pBound;


	for (nIter=0;nIter<nSteps;nIter++)
	{
		for (i=0;i<m_pEntity->GetVertexNum();i++)
		{
			pV=m_pEntity->GetVertexByIndex(i);
			VertexArea[i]=0;
			pHE=pV->GetOutHalfEdge();
			do 
			{
				pHE->GetFace()->CalcArea(area);
				VertexArea[i]+=area;
				pHE=pV->NextOutHalfEdge(pHE);
			} while(pHE!=pV->GetOutHalfEdge());
		}
		for (i=0;i<m_pEntity->GetVertexNum();i++)
		{
			pV=m_pEntity->GetVertexByIndex(i);
			if(pV->IsBoundary())continue;
			denominator=0.0;	gp.Set(0.0,0.0,0.0);
			pHE=pV->GetOutHalfEdge();
			do 
			{
				pNV=pHE->GetNextEdge()->GetVertex();
				pHE=pV->NextOutHalfEdge(pHE);
				factor=VertexArea[pNV->GetIndex()];		denominator+=factor;
				gp+=factor*pNV->GetCoordinate();
			} while(pHE!=pV->GetOutHalfEdge());
			if(denominator!=0)
				gp/=denominator;
			pCentroids[i]=gp;
		}

		for (i=0;i<m_pEntity->GetVertexNum();i++)
		{
			pV=m_pEntity->GetVertexByIndex(i);
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
			pp+=gp*dampingFactor;
			pV->SetCoordinate(pp);
		}

	}
	delete []pCentroids;
	delete []VertexArea;

}

void CAreaEqualizationRemesh::SetEntity(CEntity* pEntity)
{
	m_pEntity=pEntity;
}
