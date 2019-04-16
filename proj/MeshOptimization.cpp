#include "StdAfx.h"
#include ".\meshoptimization.h"

#include <fstream>
extern ofstream outfile;
using namespace std;

CMeshOptimization::CMeshOptimization(void)
{
}

CMeshOptimization::~CMeshOptimization(void)
{
}

bool CMeshOptimization::EdgeSplit(CEdge* pE)
{
	int i,j;
	CEdge *pNE,*pOE;
	vector<CEdge*> vecEdges;
	CVertex *pV1,*pV2,*pV3,*pV4,*pVN;
	CFace *pF1,*pF2,*pFN;

	CHalfEdge *pHE1,*pHE2,*pNxtHE1,*pNxtHE2,*pPreHE1,*pPreHE2,*pNHE1,*pNHE2;

	pHE1=pE->GetHe1();
	pHE2=pE->GetHe2();

	//outfile<<"pHE1="<<pHE1<<"; pHE2="<<pHE2<<endl;


	pV1=pHE1->GetVertex();
	pV2=pHE2->GetVertex();

	pF1=pHE1->GetFace();
	pF2=pHE2->GetFace();

	pNxtHE1=pHE1->GetNextEdge();
	pNxtHE2=pHE2->GetNextEdge();
	//outfile<<"pNxtHE1="<<pNxtHE1<<"; pNxtHE2"<<pNxtHE2<<endl;

	pPreHE1=pHE1->GetPrevEdge();
	pPreHE2=pHE2->GetPrevEdge();
	//outfile<<"pPreHE1="<<pPreHE1<<"; pPreHE2"<<pPreHE2<<endl;

	pV3=pPreHE1->GetVertex();
	pV4=pPreHE2->GetVertex();    

	CVector3d mid=pV1->GetCoordinate()+pV2->GetCoordinate();

	mid*=0.5;

	pVN=new CVertex(mid);
	m_pEntity->AddVertex(pVN);

	pNE=new CEdge;
	m_pEntity->AddEdge(pNE);

	pNHE1=new CHalfEdge(pF1,pVN);
	pVN->SetOutHalfEdge(pNHE1);

	pNHE1->SetEdge(pNE);
	pF1->ReplaceEdge(pNxtHE1,pNHE1);
	pNHE1->SetFace(pF1);

	pFN=new CFace;
	m_pEntity->AddFace(pFN);

	pFN->AddEdge(pNxtHE1);
	pNxtHE1->SetFace(pFN);

	pNHE2=new CHalfEdge(pFN,pV3);
	pNHE2->SetEdge(pNE);
	pNHE2->SetMate(pNHE1);
	pNHE1->SetMate(pNHE2);

	//outfile<<"pNHE1="<<pNHE1<<"; pNHE2"<<pNHE2<<endl;

	pNE->SetHe1(pNHE1);
	pNE->SetHe2(pNHE2);
	pFN->AddEdge(pNHE2);

	pNE=new CEdge;
	m_pEntity->AddEdge(pNE);

	pNHE1=new CHalfEdge(pFN,pVN);
	pNHE1->SetEdge(pNE);
	pNE->SetHe1(pNHE1);
	pFN->AddEdge(pNHE1);


	pFN=new CFace;
	m_pEntity->AddFace(pFN);

	pFN->AddEdge(pPreHE2);
	pPreHE2->SetFace(pFN);
	pNHE2=new CHalfEdge(pFN,pV2);
	pFN->AddEdge(pNHE2);

	pNE->SetHe2(pNHE2);
	pNHE2->SetEdge(pNE);
	pNHE2->SetMate(pNHE1);
	pNHE1->SetMate(pNHE2);

	//outfile<<"pNHE1="<<pNHE1<<"; pNHE2"<<pNHE2<<endl;

	pNE=new CEdge;
	m_pEntity->AddEdge(pNE);

	pNHE1=new CHalfEdge(pFN,pVN);
	pNHE1->SetEdge(pNE);
	pFN->AddEdge(pNHE1);

	pNHE2=new CHalfEdge(pF2,pV4);
	pNHE2->SetEdge(pNE);
	pF2->ReplaceEdge(pPreHE2,pNHE2);
	pNHE2->SetMate(pNHE1);
	pNHE1->SetMate(pNHE2);
	pNE->SetHe1(pNHE1);
	pNE->SetHe2(pNHE2);
	pHE2->SetVertex(pVN);
	pV2->SetOutHalfEdge(pNxtHE1);

	//outfile<<"pNHE1="<<pNHE1<<"; pNHE2"<<pNHE2<<endl;

	pHE1=pV1->GetOutHalfEdge();
	do 
	{
		pF1=pHE1->GetFace();
		pF1->CalcNormal();
		pHE1=pV1->NextOutHalfEdge(pHE1);
	} while(pHE1!=pV1->GetOutHalfEdge());
	pV1->CalcNormal();

	pHE1=pV2->GetOutHalfEdge();
	do 
	{
		pF1=pHE1->GetFace();
		pF1->CalcNormal();
		pHE1=pV2->NextOutHalfEdge(pHE1);
	} while(pHE1!=pV2->GetOutHalfEdge());
	pV2->CalcNormal();

	pHE1=pV3->GetOutHalfEdge();
	do 
	{
		pF1=pHE1->GetFace();
		pF1->CalcNormal();
		pHE1=pV3->NextOutHalfEdge(pHE1);
	} while(pHE1!=pV3->GetOutHalfEdge());
	pV3->CalcNormal();

	pHE1=pV4->GetOutHalfEdge();
	do 
	{
		pF1=pHE1->GetFace();
		pF1->CalcNormal();
		pHE1=pV4->NextOutHalfEdge(pHE1);
	} while(pHE1!=pV4->GetOutHalfEdge());
	pV4->CalcNormal();

	pHE1=pVN->GetOutHalfEdge();
	do 
	{
		pF1=pHE1->GetFace();
		pF1->CalcNormal();
		pHE1=pVN->NextOutHalfEdge(pHE1);
	} while(pHE1!=pVN->GetOutHalfEdge());
	pVN->CalcNormal();

	return true;

}

bool CMeshOptimization::EdgeCollapse(CEdge* pE)
{
	CHalfEdge *pSHE,*pEHE,*pHE1,*pHE2,*pHE,*pMHE1,*pMHE2;
	CVertex *pV1,*pV2,*pV,*pV3,*pV4;
	CEdge *pE1,*pE2;
	CFace *pF1;
	bool check;

	pHE1=pE->GetHe1();
	pHE2=pE->GetHe2();

	pV1=pHE1->GetVertex();
	pV2=pHE2->GetVertex();


	pV3=pHE1->GetPrevEdge()->GetVertex();
	pV4=pHE2->GetPrevEdge()->GetVertex();

	check=true;
	pSHE=pV1->GetOutHalfEdge();
	do 
	{
		pV=pSHE->GetNextEdge()->GetVertex();
		if(pV!=pV3&&pV!=pV4)
		{
			pEHE=pV2->GetOutHalfEdge();
			do 
			{
				if(pV==pEHE->GetNextEdge()->GetVertex())
				{
					check=false;
					break;
				}
				pEHE=pV2->NextOutHalfEdge(pEHE);
			} while(pEHE!=pV2->GetOutHalfEdge());
		}
		if(!check)break;
		pSHE=pV1->NextOutHalfEdge(pSHE);
	} while(pSHE!=pV1->GetOutHalfEdge());


	if(!check)return false;


	CVector3d mid=(pV1->GetCoordinate()+pV2->GetCoordinate())/2;
	pV2->SetCoordinate(mid);

	pSHE=pHE1->GetPrevEdge()->GetMate();
	pEHE=pHE2->GetNextEdge();

	pHE=pSHE;
	do 
	{
		pHE->SetVertex(pV2);
		pHE=pV1->NextOutHalfEdge(pHE);
	} while(pHE!=pEHE);


	pV2->SetOutHalfEdge(pSHE);
	pMHE1=pHE1->GetNextEdge()->GetMate();
	pV3->SetOutHalfEdge(pMHE1);
	pE1=pMHE1->GetEdge();
	pMHE1->SetMate(pSHE);
	pSHE->SetMate(pMHE1);
	pSHE->SetEdge(pE1);

	if(pE1->GetHe1()==pMHE1)
		pE1->SetHe2(pSHE);
	else
		pE1->SetHe1(pSHE);

	pMHE2=pHE2->GetPrevEdge()->GetMate();
	pE2=pMHE2->GetEdge();
	pEHE=pEHE->GetMate();
	pV4->SetOutHalfEdge(pEHE);
	pMHE2->SetMate(pEHE);
	pEHE->SetMate(pMHE2);
	pEHE->SetEdge(pE2);
	if(pE2->GetHe1()==pMHE2)
		pE2->SetHe2(pEHE);
	else
		pE2->SetHe1(pEHE);

	m_pEntity->DeleteEdge(pE);
	m_pEntity->DeleteFace(pHE1->GetFace());
	m_pEntity->DeleteFace(pHE2->GetFace());

	m_pEntity->DeleteVertex(pV1);


	pHE1=pV2->GetOutHalfEdge();
	do 
	{
		pF1=pHE1->GetFace();
		pF1->CalcNormal();
		pHE1=pV2->NextOutHalfEdge(pHE1);
	} while(pHE1!=pV2->GetOutHalfEdge());
	pV2->CalcNormal();

	pHE1=pV3->GetOutHalfEdge();
	do 
	{
		pF1=pHE1->GetFace();
		pF1->CalcNormal();
		pHE1=pV3->NextOutHalfEdge(pHE1);
	} while(pHE1!=pV3->GetOutHalfEdge());
	pV3->CalcNormal();

	pHE1=pV4->GetOutHalfEdge();
	do 
	{
		pF1=pHE1->GetFace();
		pF1->CalcNormal();
		pHE1=pV4->NextOutHalfEdge(pHE1);
	} while(pHE1!=pV4->GetOutHalfEdge());
	pV4->CalcNormal();



	return true;

}

bool CMeshOptimization::EdgeSwap(CEdge* pE)
{
	CFace *pF1,*pF2;

	CHalfEdge *pHE1,*pHE2,*pPrevHE1,*pPrevHE2,*pNxtHE1,*pNxtHE2,*pHE;
	CVertex *pV1,*pV2,*pV3,*pV4;
	bool check;

	pHE1=pE->GetHe1();
	pHE2=pE->GetHe2();

	//outfile<<"pHE1="<<pHE1<<"; pHE2="<<pHE2<<endl;

	pF1=pHE1->GetFace();
	pF2=pHE2->GetFace();

	pV1=pHE1->GetVertex();
	pV2=pHE2->GetVertex();

	pPrevHE1=pHE1->GetPrevEdge();
	pPrevHE2=pHE2->GetPrevEdge();
	//outfile<<"pPreHE1="<<pPrevHE1<<"; pPreHE2"<<pPrevHE2<<endl;

	pNxtHE1=pHE1->GetNextEdge();
	pNxtHE2=pHE2->GetNextEdge();
	//outfile<<"pNxtHE1="<<pNxtHE1<<"; pNxtHE2"<<pNxtHE2<<endl;


	pV3=pPrevHE1->GetVertex();
	pV4=pPrevHE2->GetVertex();

	check=true;
	pHE=pV3->GetOutHalfEdge();
	do 
	{
		if(pV4==pHE->GetNextEdge()->GetVertex())
		{
			check=false;
			break;
		}
		pHE=pV3->NextOutHalfEdge(pHE);
	} while(pHE!=pV3->GetOutHalfEdge());
	if(!check)return false;	

	pF1->ReplaceEdge(pHE1,pNxtHE2);
	pNxtHE2->SetFace(pF1);
	pF1->ReplaceEdge(pNxtHE1,pHE1);
	pF2->ReplaceEdge(pHE2,pNxtHE1);
	pNxtHE1->SetFace(pF2);
	pF2->ReplaceEdge(pNxtHE2,pHE2);

	pHE1->SetVertex(pV4);
	pHE2->SetVertex(pV3);

	pV1->SetOutHalfEdge(pNxtHE2);
	pV2->SetOutHalfEdge(pNxtHE1);

	pHE1=pV1->GetOutHalfEdge();
	do 
	{
		pF1=pHE1->GetFace();
		pF1->CalcNormal();
		pHE1=pV1->NextOutHalfEdge(pHE1);
	} while(pHE1!=pV1->GetOutHalfEdge());
	pV1->CalcNormal();

	pHE1=pV2->GetOutHalfEdge();
	do 
	{
		pF1=pHE1->GetFace();
		pF1->CalcNormal();
		pHE1=pV2->NextOutHalfEdge(pHE1);
	} while(pHE1!=pV2->GetOutHalfEdge());
	pV2->CalcNormal();

	pHE1=pV3->GetOutHalfEdge();
	do 
	{
		pF1=pHE1->GetFace();
		pF1->CalcNormal();
		pHE1=pV3->NextOutHalfEdge(pHE1);
	} while(pHE1!=pV3->GetOutHalfEdge());
	pV3->CalcNormal();

	pHE1=pV4->GetOutHalfEdge();
	do 
	{
		pF1=pHE1->GetFace();
		pF1->CalcNormal();
		pHE1=pV4->NextOutHalfEdge(pHE1);
	} while(pHE1!=pV4->GetOutHalfEdge());
	pV4->CalcNormal();

	return true;

}

void CMeshOptimization::SetEntity(CEntity* pEntity)
{
	m_pEntity=pEntity;
}
