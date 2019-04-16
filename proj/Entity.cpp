#include "StdAfx.h"
#include ".\entity.h"
using namespace std;
CEntity::CEntity(void)
{
}

CEntity::~CEntity(void)
{

}
void CEntity::ClearMemory()
{
	int i;
	for(i=0;i<m_vecBoundaries.size();i++)
		delete m_vecBoundaries.at(i);
	m_vecBoundaries.clear();

	for(i=0;i<m_vecFaces.size();i++)
	{
		delete m_vecFaces.at(i);
	}
	m_vecFaces.clear();

	for(i=0;i<m_vecEdges.size();i++)
	{
		delete m_vecEdges.at(i);
	}
	m_vecEdges.clear();

	for(i=0;i<m_vecVertices.size();i++)
	{
		delete m_vecVertices.at(i);
	}
	m_vecVertices.clear();
}
void CEntity::AddVertex(CVertex* pVertex)
{
	m_vecVertices.push_back(pVertex);
}

void CEntity::AddFace(CFace* pF)
{
	m_vecFaces.push_back(pF);
}

void CEntity::AddEdge(CEdge* pE)
{
	m_vecEdges.push_back(pE);
}

void CEntity::DeleteEdge(CEdge* pE)
{
	vector<CEdge*>::iterator eit;
	CEdge *pEdge;
	eit=find(m_vecEdges.begin(),m_vecEdges.end(),pE);
	if(eit!=m_vecEdges.end())
	{
		pEdge=*eit;
		m_vecEdges.erase(eit);
		delete pEdge;
	}
}

void CEntity::DeleteVertex(CVertex* pV)
{
	vector<CVertex*>::iterator vit;
	CVertex *pVertex;
	vit=find(m_vecVertices.begin(),m_vecVertices.end(),pV);
	if(vit!=m_vecVertices.end())
	{
		pVertex=*vit;
		m_vecVertices.erase(vit);
		delete pVertex;
	}
}

void CEntity::DeleteFace(CFace* pF)
{
	vector<CFace*>::iterator fit;
	fit=find(m_vecFaces.begin(),m_vecFaces.end(),pF);
	CFace *pFace;
	if(fit!=m_vecFaces.end())
	{
		pFace=*fit;
		m_vecFaces.erase(fit);
		delete pFace;
	}
}
int CEntity::GetEdgeNum(void)
{
	return m_vecEdges.size();
}

CEdge* CEntity::GetEdgeByIndex(int index)
{
	return m_vecEdges.at(index);
}

int CEntity::GetFaceNum(void)
{
	return m_vecFaces.size();
}

CFace* CEntity::GetFaceByIndex(int index)
{
	return m_vecFaces.at(index);
}
int CEntity::GetVertexNum(void)
{
	return m_vecVertices.size();
}

CVertex* CEntity::GetVertexByIndex(int index)
{
	return m_vecVertices.at(index);
}

void CEntity::Merge(CEntity* pEntity)
{
	int i;
	CFace *pF;
	CEdge *pE;
	CVertex *pV;

	for (i=0;i<pEntity->GetFaceNum();i++)
	{
		pF=pEntity->GetFaceByIndex(i);
		pF->SetInPatch(false);
	}
	for (i=0;i<pEntity->GetEdgeNum();i++)
	{
		pE=pEntity->GetEdgeByIndex(i);
		pE->SetInPatch(false);
	}
	for (i=0;i<pEntity->GetVertexNum();i++)
	{
		pV=pEntity->GetVertexByIndex(i);
		pV->SetInPatch(false);
	}

	for (i=0;i<m_vecVertices.size();i++)
		m_vecVertices[i]->SetInPatch(true);	
	for (i=0;i<m_vecFaces.size();i++)
		m_vecFaces[i]->SetInPatch(true);
	for (i=0;i<m_vecEdges.size();i++)
		m_vecEdges[i]->SetInPatch(true);

	for (i=0;i<pEntity->GetFaceNum();i++)
	{
		pF=pEntity->GetFaceByIndex(i);
		if(!pF->IsInPatch())
			m_vecFaces.push_back(pF);
	}
	for (i=0;i<pEntity->GetEdgeNum();i++)
	{
		pE=pEntity->GetEdgeByIndex(i);
		if(!pE->IsInPatch())
			m_vecEdges.push_back(pE);
	}
	for (i=0;i<pEntity->GetVertexNum();i++)
	{
		pV=pEntity->GetVertexByIndex(i);
		if(!pV->IsInPatch())
			m_vecVertices.push_back(pV);
	}
}

CMaterial& CEntity::GetMaterial(void)
{
	return m_FrontMaterial;
}

int CEntity::GetBoundaryNum(void)
{
	return m_vecBoundaries.size();
}

CBoundary* CEntity::GetBoundaryByIndex(int index)
{
	return m_vecBoundaries.at(index);
}


bool CEntity::RemoveBoundary(CBoundary* pBound)
{
	vector<CBoundary*>::iterator bit=find(m_vecBoundaries.begin(),m_vecBoundaries.end(),pBound);
	if(bit!=m_vecBoundaries.end())
	{
		m_vecBoundaries.erase(bit);
		delete pBound;
		return true;
	}
	else
		return false;
}
bool CEntity::AddBoundary(CBoundary* pBound)
{
	vector<CBoundary*>::iterator bit=find(m_vecBoundaries.begin(),m_vecBoundaries.end(),pBound);
	if(bit!=m_vecBoundaries.end())
		return false;
	else
	{
		m_vecBoundaries.push_back(pBound);
		return true;
	}
}


void CEntity::ClearBoundary(void)
{
	for (int i=0;i<m_vecBoundaries.size();i++)
	{
		delete m_vecBoundaries.at(i);
	}
	m_vecBoundaries.clear();
}

void CEntity::SetMaterial(CString name)
{
	m_FrontMaterial.SetProperty(name);
}

void CEntity::Subtract(CEntity* pEntity)
{
	int i;
	CFace *pF;
	CEdge *pE;
	CVertex *pV;

	vector<CVertex*>::iterator vit=m_vecVertices.begin();
	vector<CFace*>::iterator fit=m_vecFaces.begin();
	vector<CEdge*>::iterator eit=m_vecEdges.begin();


	while (vit!=m_vecVertices.end())
	{
		(*vit)->SetInPatch(false);
		vit++;
	}
	while (fit!=m_vecFaces.end())
	{
		(*fit)->SetInPatch(false);
		fit++;
	}
	while (eit!=m_vecEdges.end())
	{
		(*eit)->SetInPatch(false);
		eit++;
	}

	for (i=0;i<pEntity->GetVertexNum();i++)
		pEntity->GetVertexByIndex(i)->SetInPatch(true);
	for (i=0;i<pEntity->GetFaceNum();i++)
		pEntity->GetFaceByIndex(i)->SetInPatch(true);
	for (i=0;i<pEntity->GetEdgeNum();i++)
		pEntity->GetEdgeByIndex(i)->SetInPatch(true);

	vector<CVertex*> vecVertices;
	vit=m_vecVertices.begin();
	while(vit!=m_vecVertices.end())
	{
		if(!(*vit)->IsInPatch())	
			vecVertices.push_back((*vit));
		vit++;
	}
	m_vecVertices.clear();
	m_vecVertices.resize(vecVertices.size());
	copy(vecVertices.begin(),vecVertices.end(),m_vecVertices.begin());

	vector<CFace*> vecFaces;
	fit=m_vecFaces.begin();
	while (fit!=m_vecFaces.end())
	{
		if(!(*fit)->IsInPatch())
			vecFaces.push_back((*fit));
		fit++;
	}
	m_vecFaces.clear();
	m_vecFaces.resize(vecFaces.size());
	copy(vecFaces.begin(),vecFaces.end(),m_vecFaces.begin());

	vector<CEdge*> vecEdges;
	eit=m_vecEdges.begin();
	while (eit!=m_vecEdges.end())
	{
		if(!(*eit)->IsInPatch())
			vecEdges.push_back((*eit));
		eit++;
	}
	m_vecEdges.clear();
	m_vecEdges.resize(vecEdges.size());
	copy(vecEdges.begin(),vecEdges.end(),m_vecEdges.begin());

}

bool CEntity::Find(CVertex* pVertex)
{
	return find(m_vecVertices.begin(),m_vecVertices.end(),pVertex)!=m_vecVertices.end();
}

bool CEntity::Find(CFace* pFace)
{
	return find(m_vecFaces.begin(),m_vecFaces.end(),pFace)!=m_vecFaces.end();
}

bool CEntity::Find(CEdge* pEdge)
{
	return find(m_vecEdges.begin(),m_vecEdges.end(),pEdge)!=m_vecEdges.end();
}

bool CEntity::CheckTopology(void)
{
	CFace *pF;
	CHalfEdge *pHe;
	int i;
	for (i=0;i<m_vecFaces.size();i++)
	{
        pF=m_vecFaces[i];
		pHe=pF->GetFirstEdge();
		do 
		{
			if(pHe->GetMate()==NULL)
			{
				m_vecStrangeFaces.push_back(pF);
				break;
			}
			pHe=pF->GetNextEdge(pHe);
		} while(pHe!=pF->GetFirstEdge());

	}
	if(m_vecStrangeFaces.size())
	{
		AfxMessageBox("Strange Faces exist, Not a Closed Manifold");
		return false;
	}
	else
		return true;

}

void CEntity::RemoveFace(CFace* pFace)
{
	vector<CFace*>::iterator it,it2;
	vector<CEdge*>::iterator eit;
	vector<CVertex*>::iterator vit;
	CHalfEdge *pMHe,*pNHe,*pSHe;
	CFace *pF;
	CEdge *pE;
	CVertex *pV;
	it=find(m_vecFaces.begin(),m_vecFaces.end(),pFace);
	if(it==m_vecFaces.end())
		return;
	CHalfEdge *pHe=pFace->GetFirstEdge();
	do 
	{
		pMHe=pHe->GetMate();
		pE=pHe->GetEdge();
		if(pE!=NULL)
		{
			if(pMHe==NULL)
			{
				eit=find(m_vecEdges.begin(),m_vecEdges.end(),pE);
				m_vecEdges.erase(eit);
				delete pE;
			}
			else
			{
				pMHe->SetMate(NULL);
				if(pE->GetHe1()==pHe)
				{
					pE->SetHe1(pMHe);
				}
				pE->SetHe2(NULL);
			}

		}
		pV=pHe->GetVertex();
		if(pV->GetOutHalfEdge()==pHe)
		{
			pNHe=pV->NextOutHalfEdge(pHe);
			if(pNHe==NULL)
			{
				for (int i=0;i<m_vecFaces.size();i++)
				{
					if(pF==pFace)continue;
					pF=m_vecFaces[i];
					pSHe=pF->GetFirstEdge();
					do 
					{
						if(pSHe->GetVertex()==pV)break;
					} while(pSHe!=pF->GetFirstEdge());
					if(pSHe->GetVertex()==pV)
					{
						pV->SetOutHalfEdge(pSHe);			
						break;
					}

				}

			}
			else
                pV->SetOutHalfEdge(pNHe);
		}
		
		pHe=pFace->GetNextEdge(pHe);
	} while(pHe!=pFace->GetFirstEdge());

	it2=find(m_vecStrangeFaces.begin(),m_vecStrangeFaces.end(),pFace);
	if(it2!=m_vecStrangeFaces.end())
		m_vecStrangeFaces.erase(it2);
	m_vecFaces.erase(it);
	delete pFace;

}

void CEntity::AddFace(vector<CVertex*>& vecVertices)
{
	CVertex *pV1=vecVertices[0];
	CVertex *pV2=vecVertices[1];
	CVertex *pV3=vecVertices[2];


	CFace *pFace=new CFace;
	CHalfEdge *pHe,*pMHe;
	CEdge *pE;

	if(pV1->GetOutHalfEdge(pV2)==NULL)
	{
		pMHe=pV2->GetOutHalfEdge(pV1);
		pE=pMHe->GetEdge();
		pHe=new CHalfEdge(pFace,pV1);
		pMHe->SetMate(pHe);
		pHe->SetMate(pMHe);
		pHe->SetEdge(pE);
		if(pE->GetHe1()==pMHe)
			pE->SetHe2(pHe);
		else
			pE->SetHe1(pMHe);
		pFace->AddEdge(pHe);


		pMHe=pV3->GetOutHalfEdge(pV2);
		pE=pMHe->GetEdge();
		pHe=new CHalfEdge(pFace,pV2);
		pMHe->SetMate(pHe);
		pHe->SetMate(pMHe);
		pHe->SetEdge(pE);
		if(pE->GetHe1()==pMHe)
			pE->SetHe2(pHe);
		else
			pE->SetHe1(pMHe);
		pFace->AddEdge(pHe);

		pMHe=pV1->GetOutHalfEdge(pV3);
		pE=pMHe->GetEdge();
		pHe=new CHalfEdge(pFace,pV3);
		pMHe->SetMate(pHe);
		pHe->SetMate(pMHe);
		pHe->SetEdge(pE);
		if(pE->GetHe1()==pMHe)
			pE->SetHe2(pHe);
		else
			pE->SetHe1(pMHe);
		pFace->AddEdge(pHe);

	}
	else
	{
		pMHe=pV2->GetOutHalfEdge(pV3);
		pE=pMHe->GetEdge();
		pHe=new CHalfEdge(pFace,pV3);
		pMHe->SetMate(pHe);
		pHe->SetMate(pMHe);
		pHe->SetEdge(pE);
		if(pE->GetHe1()==pMHe)
			pE->SetHe2(pHe);
		else
			pE->SetHe1(pMHe);
		pFace->AddEdge(pHe);

		pMHe=pV1->GetOutHalfEdge(pV2);
		pE=pMHe->GetEdge();
		pHe=new CHalfEdge(pFace,pV2);
		pMHe->SetMate(pHe);
		pHe->SetMate(pMHe);
		pHe->SetEdge(pE);
		if(pE->GetHe1()==pMHe)
			pE->SetHe2(pHe);
		else
			pE->SetHe1(pMHe);
		pFace->AddEdge(pHe);

		pMHe=pV3->GetOutHalfEdge(pV1);
		pE=pMHe->GetEdge();
		pHe=new CHalfEdge(pFace,pV1);
		pMHe->SetMate(pHe);
		pHe->SetMate(pMHe);
		pHe->SetEdge(pE);
		if(pE->GetHe1()==pMHe)
			pE->SetHe2(pHe);
		else
			pE->SetHe1(pMHe);
		pFace->AddEdge(pHe);

	}

    m_vecFaces.push_back(pFace);

}
