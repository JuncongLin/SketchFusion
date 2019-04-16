#include "StdAfx.h"
#include ".\sirfilemanager.h"
#include <fstream>
using namespace std;

CSIRFileManager::CSIRFileManager(void)
: m_pModel(NULL)
{
}

CSIRFileManager::~CSIRFileManager(void)
{
}

void CSIRFileManager::RegisterModel(CSolid* pModel)
{
	m_pModel=pModel;
}

void CSIRFileManager::Write(string path)
{
	ofstream ofile(path.c_str());

	if(!ofile)
	{
		return;
	}

	CFace *pF;
	CVertex *pV;
	CHalfEdge *pHE;
	CBoundary *pBound;
	int vindex[3];
	CVector3d coord;
	unsigned int j,k;
	ofile<<m_pModel->GetVertexNum()<<" "<<m_pModel->GetFaceNum()<<" "<<m_pModel->GetBoundaryNum()<<endl;
	for (j=0;j<m_pModel->GetVertexNum();j++)
	{
		pV=m_pModel->GetVertexByIndex(j);
		pV->SetIndex(j);
		coord=pV->GetCoordinate();
		ofile<<coord[0]<<" "<<coord[1]<<" "<<coord[2]<<endl;
	}
	for (j=0;j<m_pModel->GetFaceNum();j++)
	{
		pF=m_pModel->GetFaceByIndex(j);

		pHE=pF->GetFirstEdge();
		vindex[0]=pHE->GetVertex()->GetIndex();

		pHE=pF->GetNextEdge(pHE);
		vindex[1]=pHE->GetVertex()->GetIndex();

		pHE=pF->GetNextEdge(pHE);
		vindex[2]=pHE->GetVertex()->GetIndex();

		ofile<<vindex[0]<<" "<<vindex[1]<<" "<<vindex[2]<<endl;
	}
	for (j=0;j<m_pModel->GetBoundaryNum();j++)
	{
		pBound=m_pModel->GetBoundaryByIndex(j);
		ofile<<pBound->GetVertexNum();
		for (k=0;k<pBound->GetVertexNum();k++)
		{

			ofile<<" "<<pBound->GetVertexByIndex(k)->GetIndex();
		}
		ofile<<endl;
	}


}

void CSIRFileManager::Read(string path)
{
	ifstream ifile(path.c_str());


	int numV,numF,numB,numBV;

	double coord[3],normal[3];
	int vindex[3],bvindex,svindex,findex;

	vector<CVertex*> vecVertices;
	vector<CFace*> vecFaces;

	int j,k;

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

		CFace *pF=new CFace(m_pModel);
		pF->Init(p1,p2,p3,j);
		vecFaces.push_back(pF);
	}
	m_pModel=new CSolid;
	m_pModel->ImportFromList(vecVertices,vecFaces);
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
		m_pModel->AddBoundary(pBound);

	}

}

CSolid* CSIRFileManager::GetModel(void)
{
	return m_pModel;
}
