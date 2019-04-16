#include "StdAfx.h"
#include ".\wrinkle.h"
#include "WEHeapNode.h"
#include "Stopwatch.hpp"
#include "Plane.h"
#include <limits>
#include <list>
#include "global.h"
#include <fstream>
extern ofstream outfile;
using namespace std;
CWrinkle::CWrinkle(void)
{
	m_pCurveOne=NULL;
	m_pCurveTwo=NULL;
	m_curveOnePntNum=0;
	m_curveTwoPntNum=0;
	m_validEdgeFlags=NULL;
}

CWrinkle::~CWrinkle(void)
{
	if ((m_curveOnePntNum>0) && (m_curveTwoPntNum>0)) {
		for(int i=0;i<m_curveOnePntNum;i++)	delete [](bool*)m_validEdgeFlags[i];
		delete [](bool**)m_validEdgeFlags;
	}
}

CPatch* CWrinkle::GlobalMinimumBendingBridgeTriangulation(void)
{
	CPatch *mesh=NULL;
	int i,j;
	int qNodeNum,pNodeNum;
	CWEHeapNode ***pTrglNodeArray;
	CWEHeapNode ***qTrglNodeArray;
	CWEHeapNode *sourceNode,*targetNode;
	CWEHeapNode *currentNode;

	m_levelOfLocalConvexity=2;
	BuildEdgeValidityMatrix();

	pNodeNum=m_curveOnePntNum;	qNodeNum=m_curveTwoPntNum;

	//------------------------------------------------------------------------------
	//	Step 1: create the nodes
	pTrglNodeArray=(CWEHeapNode ***)new long[pNodeNum];
	qTrglNodeArray=(CWEHeapNode ***)new long[pNodeNum];

	for(i=0;i<pNodeNum;i++) {
		pTrglNodeArray[i]=(CWEHeapNode **)new long[qNodeNum];
		qTrglNodeArray[i]=(CWEHeapNode **)new long[qNodeNum];
	}

	for(i=1;i<pNodeNum;i++) {
		for(j=0;j<qNodeNum;j++) {
			pTrglNodeArray[i][j]=new CWEHeapNode;
			pTrglNodeArray[i][j]->pIndex=i;
			pTrglNodeArray[i][j]->qIndex=j;
			pTrglNodeArray[i][j]->SetValue(1.0e+10);
		}
	}
	for(i=0;i<pNodeNum;i++) {
		for(j=1;j<qNodeNum;j++) {
			qTrglNodeArray[i][j]=new CWEHeapNode;
			qTrglNodeArray[i][j]->pIndex=i;
			qTrglNodeArray[i][j]->qIndex=j;
			qTrglNodeArray[i][j]->SetValue(1.0e+10);
		}
	}
	sourceNode=new CWEHeapNode;
	sourceNode->pIndex=0;
	sourceNode->qIndex=0;
	sourceNode->SetValue(0);

	//------------------------------------------------------------------------------
	//	Step 2: computing the weights of edges linking the nodes and linking the edges
	//------------------------------------------------------------------------------
	//	for edges starting from sourceNode
	//outfile<<"Step 2: computing the weights of edges linking the nodes and linking the edges"<<endl;
	sourceNode->edgeList[0]=pTrglNodeArray[1][0];
	sourceNode->edgeWeights[0]=0.0;
	sourceNode->edgeList[1]=qTrglNodeArray[0][1];
	sourceNode->edgeWeights[1]=0.0;
	sourceNode->edgeNum=2;
	//------------------------------------------------------------------------------
	//	for edges starting from p-TrglNode
	for(i=1;i<pNodeNum;i++) {
		for(j=0;j<qNodeNum;j++) {
			currentNode=pTrglNodeArray[i][j];
			if ((i+1)<pNodeNum) {
				currentNode->edgeList[currentNode->edgeNum]=pTrglNodeArray[i+1][j];
				currentNode->edgeWeights[currentNode->edgeNum]=
					ComputeEdgeBendingEnergy(i,j,true,true);
				(currentNode->edgeNum)++;
			}
			if ((j+1)<qNodeNum) {
				currentNode->edgeList[currentNode->edgeNum]=qTrglNodeArray[i][j+1];
				currentNode->edgeWeights[currentNode->edgeNum]=
					ComputeEdgeBendingEnergy(i,j,true,false);
				(currentNode->edgeNum)++;
			}
		}
	}

	//------------------------------------------------------------------------------
	//	for edges starting from q-TrglNode
	for(i=0;i<pNodeNum;i++) {
		for(j=1;j<qNodeNum;j++) {
			currentNode=qTrglNodeArray[i][j];
			if ((i+1)<pNodeNum) {
				currentNode->edgeList[currentNode->edgeNum]=pTrglNodeArray[i+1][j];
				currentNode->edgeWeights[currentNode->edgeNum]=
					ComputeEdgeBendingEnergy(i,j,false,true);
				(currentNode->edgeNum)++;
			}
			if ((j+1)<qNodeNum) {
				currentNode->edgeList[currentNode->edgeNum]=qTrglNodeArray[i][j+1];
				currentNode->edgeWeights[currentNode->edgeNum]=
					ComputeEdgeBendingEnergy(i,j,false,false);
				(currentNode->edgeNum)++;
			}
		}
	}
	//------------------------------------------------------------------------------
	//	Step 3: using the Dijkstra algorithm to extract the shortest path
	//------------------------------------------------------------------------------
	//	Initialize the minimum heap
	list<CWEHeapNode*>::iterator pos;
	list<CWEHeapNode*> *minHeap=new list<CWEHeapNode*>;
	minHeap->push_back(sourceNode);
	for(i=1;i<pNodeNum;i++)
		for(j=0;j<qNodeNum;j++)
		{
			pos=minHeap->begin();
			while(pos!=minHeap->end())
			{
				if(SmallerHeapNode(pTrglNodeArray[i][j],(*pos)))
				{
					minHeap->insert(pos,pTrglNodeArray[i][j]);
					break;
				}
				else
					pos++;
			}
			if(pos==minHeap->end())
				minHeap->push_back(pTrglNodeArray[i][j]);

		}
	for(i=0;i<pNodeNum;i++)
		for(j=1;j<qNodeNum;j++)
		{
			pos=minHeap->begin();
			while(pos!=minHeap->end())
			{
				if(SmallerHeapNode(qTrglNodeArray[i][j],(*pos)))
				{
					minHeap->insert(pos,qTrglNodeArray[i][j]);
					break;
				}
				else
					pos++;
			}
			if(pos==minHeap->end())
				minHeap->push_back(qTrglNodeArray[i][j]);

		}

	//------------------------------------------------------------------------------
	//	remove the minimum node from the heap one by one
	Stopwatch time;
	time.Start();
	int count=0;
	while(!minHeap->empty())
	{
		//outfile<<"count="<<count<<endl;
		count++;
		//outfile<<"size="<<minHeap->size()<<endl;
		//currentNode=*(minHeap->begin());
		//minHeap->erase(minHeap->begin());
		currentNode=minHeap->front();
		minHeap->pop_front();
		for(i=0;i<currentNode->edgeNum;i++) {
			//outfile<<"Left: "<<(currentNode->GetValue()+currentNode->edgeWeights[i])<<"; ";
			//outfile<<"Right:"<<(currentNode->edgeList[i]->GetValue())<<endl;
			if ((currentNode->GetValue()+currentNode->edgeWeights[i])
				<(currentNode->edgeList[i]->GetValue()))
			{
				//outfile<<"update"<<endl;
				currentNode->edgeList[i]->
					SetValue((float)(currentNode->GetValue()+currentNode->edgeWeights[i]));
				currentNode->edgeList[i]->previousNode=currentNode;
				pos=minHeap->begin();
				while(pos!=minHeap->end())
				{
					if(SmallerHeapNode(currentNode->edgeList[i],(*pos)))
					{
						minHeap->insert(pos,currentNode->edgeList[i]);
						break;
					}
					else
						pos++;
				}
				if(pos==minHeap->end())
					minHeap->push_back(currentNode->edgeList[i]);

			}
		}

	}
	time.Stop();
	outfile<<"Dijkstra Time: "<<time.GetTime()<<endl;

	//------------------------------------------------------------------------------
	delete minHeap;
	//------------------------------------------------------------------------------
	//	determine the target node
	if ((pTrglNodeArray[pNodeNum-1][qNodeNum-1]->GetValue())
		<(qTrglNodeArray[pNodeNum-1][qNodeNum-1]->GetValue()))
		targetNode=pTrglNodeArray[pNodeNum-1][qNodeNum-1];
	else
		targetNode=qTrglNodeArray[pNodeNum-1][qNodeNum-1];
	//------------------------------------------------------------------------------
	//	backward tracking the shortest path
	m_pList.RemoveAll();	m_qList.RemoveAll();
	currentNode=targetNode;
	while(currentNode) {
		m_pList.InsertAt(0,currentNode->pIndex);
		m_qList.InsertAt(0,currentNode->qIndex);
		currentNode=currentNode->previousNode;
	}

	------------------------------------------------------------------------------
//		Step 4: construct the WW_Shell by the shortest path
	mesh=MeshGenerationByShortestPath();

	//------------------------------------------------------------------------------
	//	Step 5: free the memory
	for(i=1;i<pNodeNum;i++) {
		for(j=0;j<qNodeNum;j++) {
			delete (CWEHeapNode *)(pTrglNodeArray[i][j]);
		}
	}
	for(i=0;i<pNodeNum;i++) {
		for(j=1;j<qNodeNum;j++) {
			delete (CWEHeapNode *)(qTrglNodeArray[i][j]);
		}
	}
	for(i=0;i<pNodeNum;i++) {
		delete [](CWEHeapNode **)(pTrglNodeArray[i]);
		delete [](CWEHeapNode **)(qTrglNodeArray[i]);
	}
	delete [](CWEHeapNode ***)(pTrglNodeArray);
	delete [](CWEHeapNode ***)(qTrglNodeArray);
	delete sourceNode;

	//------------------------------------------------------------------------------
	//	Step 6: generate report
//	generateComputationalStatisticsReport();

	return mesh;


}

void CWrinkle::BuildEdgeValidityMatrix(void)
{
	int i,j;
	double hh,ll;
	CVector3d p1,p2;

	hh=HausdorffDistance()*1.5;

	for(i=0;i<m_curveOnePntNum;i++) {
		p1=m_pCurveOne->GetVertexByIndex(i)->GetCoordinate();
		for(j=0;j<m_curveTwoPntNum;j++) {
			//outfile<<"<"<<i<<","<<j<<">"<<endl;
			p2=m_pCurveTwo->GetVertexByIndex(j)->GetCoordinate();
			ll=p1.Dot(p2);
			if (ll>=hh) {m_validEdgeFlags[i][j]=false;continue;}
			//outfile<<"Edge Validity Check"<<endl;
			m_validEdgeFlags[i][j]=EdgeValidityCheck(i,j);
		}
	}

}

double CWrinkle::HausdorffDistance(void)
{
	double min,max,ll;
	CVector3d p1,p2;
	int i,j;

	max=-1.0e+8;
	for(i=0;i<m_curveOnePntNum;i++) {
		min=1.0e+8;
		p1=m_pCurveOne->GetVertexByIndex(i)->GetCoordinate();
		for(j=0;j<m_curveTwoPntNum;j++) {
			p2=m_pCurveTwo->GetVertexByIndex(j)->GetCoordinate();
			ll=p1.Dot(p2);
			if (ll<min) min=ll;
		}
		if (min>max) max=min;
	}
	return max;
}

bool CWrinkle::EdgeValidityCheck(int pIndex, int qIndex)
{
	int pntNum,n,i;
	double *x,*y,*z;

	pntNum=(m_levelOfLocalConvexity*2+1)*2;
	if ((pIndex-m_levelOfLocalConvexity)<0) pntNum=pntNum+(pIndex-m_levelOfLocalConvexity);
	if ((qIndex-m_levelOfLocalConvexity)<0) pntNum=pntNum+(qIndex-m_levelOfLocalConvexity);
	if ((pIndex+m_levelOfLocalConvexity)>(m_curveOnePntNum-1))
		pntNum=pntNum-((pIndex+m_levelOfLocalConvexity)-(m_curveOnePntNum-1));
	if ((qIndex+m_levelOfLocalConvexity)>(m_curveTwoPntNum-1))
		pntNum=pntNum-((qIndex+m_levelOfLocalConvexity)-(m_curveTwoPntNum-1));

	x=new double[pntNum];	y=new double[pntNum];	z=new double[pntNum];

	CVector3d *pPt=new CVector3d[pntNum];

	pPt[0]=m_pCurveOne->GetVertexByIndex(pIndex)->GetCoordinate();
	pPt[1]=m_pCurveTwo->GetVertexByIndex(qIndex)->GetCoordinate();


	n=2;
	//outfile<<"push Curve one vertex"<<endl;
	for(i=pIndex-m_levelOfLocalConvexity;i<=pIndex+m_levelOfLocalConvexity;i++) {
		if (i==pIndex) continue;
		if (i<0) continue;
		if (i>(m_curveOnePntNum-1)) continue;
		pPt[n]=m_pCurveOne->GetVertexByIndex(i)->GetCoordinate();
		n++;
	}
	//outfile<<"push Curve two vertex"<<endl;
	for(i=qIndex-m_levelOfLocalConvexity;i<=qIndex+m_levelOfLocalConvexity;i++) {
		if (i==qIndex) continue;
		if (i<0) continue;
		if (i>(m_curveTwoPntNum-1)) continue;
		pPt[n]=m_pCurveTwo->GetVertexByIndex(i)->GetCoordinate();
		n++;
	}

	//outfile<<"ConvexEdgeDetection"<<endl;
	bool bFlag=ConvexEdgeDetection(pntNum,pPt);

	delete [] pPt;

	return bFlag;
}

bool CWrinkle::ConvexEdgeDetection(int pntNum, CVector3d* pPt)
{
	CVector3d p1,p2,p3;

	double aa,bb,cc,dd,func;
	int num=0,dirFlag;
	bool bOneSide;
	int i,j;

	p1=pPt[0];
	p2=pPt[1];


	for(i=2;i<pntNum;i++) {
		p3=pPt[i];
		if((p2-p3).IsCollinear(p1-p3))continue;
		CPlane plane(p1,p2,p3);

		aa=plane.a();
		bb=plane.b();
		cc=plane.c();
		dd=plane.d();

		dirFlag=0;	bOneSide=true;

		for(j=2;j<pntNum;j++) {
			if (j==i) continue;
			if (dirFlag==0) {
				func=aa*pPt[j][0]+bb*pPt[j][1]+cc*pPt[j][2]+dd;
				if (func<0) dirFlag=-1;
				if (func>0) dirFlag=1;
			}
			else {
				func=aa*pPt[j][0]+bb*pPt[j][1]+cc*pPt[j][2]+dd;
				if ((func<0) && (dirFlag==1)) {bOneSide=false;break;}
				if ((func>0) && (dirFlag==-1)) {bOneSide=false;break;}
			}
		}

		if (bOneSide) num++;
	}

	if (num>=2) return true;

	return false;
}

double CWrinkle::ComputeEdgeBendingEnergy(int pIndex, int qIndex, bool prev_pOrq, bool next_pOrq)
{
	double E=0.0;
	double dd,n1[3],n2[3],ll,aa,cs;
	CVector3d p1,p2,p3;

	if ((pIndex==0) && (qIndex==0)) return 0.0;

	p1=m_pCurveOne->GetVertexByIndex(pIndex)->GetCoordinate();
	p2=m_pCurveTwo->GetVertexByIndex(qIndex)->GetCoordinate();

	if (prev_pOrq) 
		p3=m_pCurveOne->GetVertexByIndex(pIndex-1)->GetCoordinate();
	else
		p3=m_pCurveTwo->GetVertexByIndex(qIndex-1)->GetCoordinate();

	if((p1-p3).IsCollinear(p2-p3))return 0.0;
	CPlane plane1(p1,p2,p3);

	n1[0]=plane1.a();
	n1[1]=plane1.b();
	n1[2]=plane1.c();
	dd=plane1.d();

	if (next_pOrq) 
		p3=m_pCurveOne->GetVertexByIndex(pIndex+1)->GetCoordinate();
	else
		p3=m_pCurveTwo->GetVertexByIndex(qIndex+1)->GetCoordinate();

	//ll=geo.Distance_to_LineSegment(p3,p1,p2);
	if(((p3-p1).Dot(p2-p1))<0)
	{
		ll=(p3-p1).Length();
	}
	else if(((p3-p2).Dot(p1-p2))<0)
	{
		ll=(p3-p2).Length();
	}
	else
	{
		ll=((p3-p1).Dot(p2-p1))/(p2-p1).Length();

	}

	if((p1-p3).IsCollinear(p2-p3))return 0.0;
	CPlane plane2(p1,p2,p3);
	n2[0]=plane2.a();
	n2[1]=plane2.b();
	n2[2]=plane2.c();
	dd=plane1.d();

	aa=((p2-p1).Cross(p3-p1)).Length();
	cs=n1[0]*n2[0]+n1[1]*n2[1]+n1[2]*n2[2];

	E=aa*(1.0-cs*cs)/(ll*ll);

	return E;
}


CPatch* CWrinkle::MeshGenerationByShortestPath(void)
{
	CPatch *mesh=NULL;
	int i;
	int qNodeNum,pNodeNum;
	int pIndex,qIndex;
	CEdge *currentEdge,*nextEdge,*pBoundEdge,*firstEdge;
	CVertex *pVertex1,*pVertex2,*pVertex3;
	CFace *pFace;
	CHalfEdge *pMHE,*pHE,*pTMHE;

	pNodeNum=m_curveOnePntNum;	
	qNodeNum=m_curveTwoPntNum;

	//------------------------------------------------------------------------------
	//	Step 4: construct the WW_Shell by the shortest path
	mesh=new CPatch;
	//------------------------------------------------------------------------------
	//	create nodes
	for(i=0;i<pNodeNum;i++) {

		pVertex1=m_pCurveOne->GetVertexByIndex(i);
		mesh->AddVertex(pVertex1);
	}
	for(i=0;i<qNodeNum;i++) {
		pVertex1=m_pCurveTwo->GetVertexByIndex(i);
		mesh->AddVertex(pVertex1);
	}
	//-----------------------------------------------------------------------
	//	incrementally construct the triangulation
	int num=m_pList.GetSize();
	currentEdge=new CEdge;
	mesh->AddEdge(currentEdge);
	firstEdge=currentEdge;
	bool prev_pOrq=true;
	int pIndex2,qIndex2;
	for(i=1;i<num;i++) {
		//outfile<<"i="<<i<<endl;
		pIndex=m_pList[i-1];	qIndex=m_qList[i-1];
		pIndex2=m_pList[i];		qIndex2=m_qList[i];
		//outfile<<"<"<<pIndex<<","<<pIndex2<<","<<qIndex<<","<<qIndex2<<">"<<endl;
		if (pIndex!=m_pList[i]) {	//	p-succeed
			pVertex1=m_pCurveOne->GetVertexByIndex(pIndex);
			pVertex2=m_pCurveOne->GetVertexByIndex(pIndex2);
			pVertex3=m_pCurveTwo->GetVertexByIndex(qIndex);

			//outfile<<"p"<<endl;

			//outfile<<"1: "<<pVertex1<<endl;
			//outfile<<"2: "<<pVertex2<<endl;
			//outfile<<"3: "<<pVertex3<<endl;


			pMHE=pVertex1->GetOutHalfEdge();
			if(pMHE->GetNextEdge()->GetVertex()!=pVertex2)
			{
				//outfile<<"c1"<<endl;
				pMHE=pVertex2->GetOutHalfEdge();
				pFace=new CFace;
				pHE=new CHalfEdge(pFace,pVertex1);
				pHE->SetMate(pMHE);
				pMHE->SetMate(pHE);
				pBoundEdge=pMHE->GetEdge();
				pBoundEdge->SetHe2(pHE);
				pHE->SetEdge(pBoundEdge);
				pFace->AddEdge(pHE);

				nextEdge=new CEdge;
				pHE=new CHalfEdge(pFace,pVertex2);
				nextEdge->SetHe1(pHE);
				pHE->SetEdge(nextEdge);
				pFace->AddEdge(pHE);

				pHE=new CHalfEdge(pFace,pVertex3);
				pTMHE=currentEdge->GetHe1();
				if(pTMHE!=NULL)
				{
					pTMHE->SetMate(pHE);
					pHE->SetMate(pTMHE);
				}
				currentEdge->SetHe2(pHE);
				pHE->SetEdge(currentEdge);
				pFace->AddEdge(pHE);
	    	}
			else
			{
				//outfile<<"c2"<<endl;
				pFace=new CFace;
				pHE=new CHalfEdge(pFace,pVertex2);
				pHE->SetMate(pMHE);
				pMHE->SetMate(pHE);
				pBoundEdge=pMHE->GetEdge();
				pBoundEdge->SetHe2(pHE);
				pHE->SetEdge(pBoundEdge);
				pFace->AddEdge(pHE);

				pHE=new CHalfEdge(pFace,pVertex1);
				pTMHE=currentEdge->GetHe1();
				if(pTMHE!=NULL)
				{
					pTMHE->SetMate(pHE);
					pHE->SetMate(pTMHE);
				}
				currentEdge->SetHe2(pHE);
				pHE->SetEdge(currentEdge);
				pFace->AddEdge(pHE);

				nextEdge=new CEdge;
				pHE=new CHalfEdge(pFace,pVertex3);
				nextEdge->SetHe1(pHE);
				pHE->SetEdge(nextEdge);
				pFace->AddEdge(pHE);

			}
			mesh->AddFace(pFace);
			currentEdge=nextEdge;			
			mesh->AddEdge(currentEdge);
			prev_pOrq=true;
		}
		else {	// q-succeed
			//outfile<<"q"<<endl;
			pVertex1=m_pCurveOne->GetVertexByIndex(pIndex);
			pVertex2=m_pCurveTwo->GetVertexByIndex(qIndex);
			pVertex3=m_pCurveTwo->GetVertexByIndex(qIndex2);

			//outfile<<"1: "<<pVertex1<<endl;
			//outfile<<"2: "<<pVertex2<<endl;
			//outfile<<"3: "<<pVertex3<<endl;

			pMHE=pVertex2->GetOutHalfEdge();
			if(pMHE->GetNextEdge()->GetVertex()!=pVertex3)
			{
				//outfile<<"c1"<<endl;
				pMHE=pVertex3->GetOutHalfEdge();
				pFace=new CFace;
				pHE=new CHalfEdge(pFace,pVertex2);
				pHE->SetMate(pMHE);
				pMHE->SetMate(pHE);
				pBoundEdge=pMHE->GetEdge();
				pBoundEdge->SetHe2(pHE);
				pHE->SetEdge(pBoundEdge);
				pFace->AddEdge(pHE);

				nextEdge=new CEdge;
				pHE=new CHalfEdge(pFace,pVertex3);
				nextEdge->SetHe1(pHE);
				pHE->SetEdge(nextEdge);
				pFace->AddEdge(pHE);

				pHE=new CHalfEdge(pFace,pVertex1);
				pTMHE=currentEdge->GetHe1();
				if(pTMHE!=NULL)
				{
					pTMHE->SetMate(pHE);
					pHE->SetMate(pTMHE);
				}
				currentEdge->SetHe2(pHE);
				pHE->SetEdge(currentEdge);
				pFace->AddEdge(pHE);
			}
			else
			{
				//outfile<<"c2"<<endl;
				pFace=new CFace;
				pHE=new CHalfEdge(pFace,pVertex3);
				pHE->SetMate(pMHE);
				pMHE->SetMate(pHE);
				pBoundEdge=pMHE->GetEdge();
				pBoundEdge->SetHe2(pHE);
				pHE->SetEdge(pBoundEdge);
				pFace->AddEdge(pHE);


				pHE=new CHalfEdge(pFace,pVertex2);
				pTMHE=currentEdge->GetHe1();
				if(pTMHE!=NULL)
				{
					pTMHE->SetMate(pHE);
					pHE->SetMate(pTMHE);
				}
				currentEdge->SetHe2(pHE);
				pHE->SetEdge(currentEdge);
				pFace->AddEdge(pHE);


				nextEdge=new CEdge;
				pHE=new CHalfEdge(pFace,pVertex1);
				nextEdge->SetHe1(pHE);
				pHE->SetEdge(nextEdge);
				pFace->AddEdge(pHE);


			}
			mesh->AddFace(pFace);
			currentEdge=nextEdge;		
			mesh->AddEdge(currentEdge);
			prev_pOrq=false;
		}
	}


	pVertex1=m_pCurveOne->GetVertexByIndex(m_curveOnePntNum-1);
	pVertex2=m_pCurveOne->GetVertexByIndex(0);
	pVertex3=m_pCurveTwo->GetVertexByIndex(m_curveTwoPntNum-1);

	pMHE=pVertex1->GetOutHalfEdge();
	if(pMHE->GetNextEdge()->GetVertex()!=pVertex2)
	{
		pMHE=pVertex2->GetOutHalfEdge();
		pFace=new CFace;
		pHE=new CHalfEdge(pFace,pVertex1);
		pHE->SetMate(pMHE);
		pMHE->SetMate(pHE);
		pBoundEdge=pMHE->GetEdge();
		pBoundEdge->SetHe2(pHE);
		pHE->SetEdge(pBoundEdge);
		pFace->AddEdge(pHE);

		nextEdge=new CEdge;
		pHE=new CHalfEdge(pFace,pVertex2);
		nextEdge->SetHe1(pHE);
		pHE->SetEdge(nextEdge);
		pFace->AddEdge(pHE);

		pHE=new CHalfEdge(pFace,pVertex3);
		pTMHE=currentEdge->GetHe1();
		if(pTMHE!=NULL)
		{
			pTMHE->SetMate(pHE);
			pHE->SetMate(pTMHE);
		}
		currentEdge->SetHe2(pHE);
		pHE->SetEdge(currentEdge);
		pFace->AddEdge(pHE);
	}
	else
	{
		pFace=new CFace;
		pHE=new CHalfEdge(pFace,pVertex2);
		pHE->SetMate(pMHE);
		pMHE->SetMate(pHE);
		pBoundEdge=pMHE->GetEdge();
		pBoundEdge->SetHe2(pHE);
		pHE->SetEdge(pBoundEdge);
		pFace->AddEdge(pHE);

		pHE=new CHalfEdge(pFace,pVertex1);
		pTMHE=currentEdge->GetHe1();
		if(pTMHE!=NULL)
		{
			pTMHE->SetMate(pHE);
			pHE->SetMate(pTMHE);
		}
		currentEdge->SetHe2(pHE);
		pHE->SetEdge(currentEdge);
		pFace->AddEdge(pHE);

		nextEdge=new CEdge;
		pHE=new CHalfEdge(pFace,pVertex3);
		nextEdge->SetHe1(pHE);
		pHE->SetEdge(nextEdge);
		pFace->AddEdge(pHE);

	}
	mesh->AddFace(pFace);
	currentEdge=nextEdge;			
	mesh->AddEdge(currentEdge);

	pVertex1=m_pCurveOne->GetVertexByIndex(0);
	pVertex2=m_pCurveTwo->GetVertexByIndex(m_curveTwoPntNum-1);
	pVertex3=m_pCurveTwo->GetVertexByIndex(0);

	pMHE=pVertex2->GetOutHalfEdge();
	if(pMHE->GetNextEdge()->GetVertex()!=pVertex3)
	{
		pMHE=pVertex3->GetOutHalfEdge();
		pFace=new CFace;
		pHE=new CHalfEdge(pFace,pVertex2);
		pHE->SetMate(pMHE);
		pMHE->SetMate(pHE);
		pBoundEdge=pMHE->GetEdge();
		if(pBoundEdge->GetHe1()==NULL)
			pBoundEdge->SetHe1(pHE);
		else
			pBoundEdge->SetHe2(pHE);
		pHE->SetEdge(pBoundEdge);
		pFace->AddEdge(pHE);

		pHE=new CHalfEdge(pFace,pVertex3);
		pTMHE=firstEdge->GetHe2();
		pTMHE->SetMate(pHE);
		pHE->SetMate(pTMHE);
		firstEdge->SetHe1(pHE);
		pHE->SetEdge(nextEdge);
		pFace->AddEdge(pHE);

		pHE=new CHalfEdge(pFace,pVertex1);
		pTMHE=currentEdge->GetHe1();
		if(pTMHE!=NULL)
		{
			pTMHE->SetMate(pHE);
			pHE->SetMate(pTMHE);
		}
		currentEdge->SetHe2(pHE);
		pHE->SetEdge(currentEdge);
		pFace->AddEdge(pHE);
	}
	else
	{
		pFace=new CFace;
		pHE=new CHalfEdge(pFace,pVertex3);
		pHE->SetMate(pMHE);
		pMHE->SetMate(pHE);
		pBoundEdge=pMHE->GetEdge();
		if(pBoundEdge->GetHe1()==NULL)
			pBoundEdge->SetHe1(pHE);
		else
			pBoundEdge->SetHe2(pHE);
		pHE->SetEdge(pBoundEdge);
		pFace->AddEdge(pHE);

		pHE=new CHalfEdge(pFace,pVertex2);
		pTMHE=currentEdge->GetHe1();
		if(pTMHE!=NULL)
		{
			pTMHE->SetMate(pHE);
			pHE->SetMate(pTMHE);
		}
		currentEdge->SetHe2(pHE);
		pHE->SetEdge(currentEdge);
		pFace->AddEdge(pHE);

		pHE=new CHalfEdge(pFace,pVertex1);
		pTMHE=firstEdge->GetHe2();
		pTMHE->SetMate(pHE);
		pHE->SetMate(pTMHE);
		firstEdge->SetHe1(pHE);
		pHE->SetEdge(nextEdge);
		pFace->AddEdge(pHE);

	}
	mesh->AddFace(pFace);

	return mesh;
}

void CWrinkle::ImportCurvePair(CBoundary* pBound1, CBoundary* pBound2)
{
	m_pCurveOne=pBound1;
	m_pCurveTwo=pBound2;

}

void CWrinkle::Initialize(void)
{

	int i,j,minI,minJ,span;
	CVertex *pV1,*pV1a,*pV1b,*pV2,*pV2a,*pV2b;
	bool reverse;
	m_curveOnePntNum=m_pCurveOne->GetVertexNum();
	m_curveTwoPntNum=m_pCurveTwo->GetVertexNum();

	m_validEdgeFlags=(bool**)new long[m_curveOnePntNum];
	for(int i=0;i<m_curveOnePntNum;i++) {
		m_validEdgeFlags[i]=new bool[m_curveTwoPntNum];
		for(int j=0;j<m_curveTwoPntNum;j++) m_validEdgeFlags[i][j]=false;
	}	
	
	
	double ntwist, elength, metric,MinMetrix=DBL_MAX;

	for (i=0;i<m_curveOnePntNum;i++)
	{
		pV1=m_pCurveOne->GetVertexByIndex(i);
		for (j=0;j<m_curveTwoPntNum;j++)
		{
			pV2=m_pCurveTwo->GetVertexByIndex(j);
			//ntwist=1-pV1->GetNormal().Dot(pV2->GetNormal());
			elength=(pV1->GetCoordinate()-pV2->GetCoordinate()).Length();
			metric=elength;
			if(MinMetrix>metric)
			{
				MinMetrix=metric;
				minI=i;
				minJ=j;
			}
		}
	}
	span=m_pCurveOne->GetVertexNum()/3;
	pV1=m_pCurveOne->GetVertexByIndex(minI);
	pV1a=m_pCurveOne->GetVertexByIndex((minI+span)%m_pCurveOne->GetVertexNum());
	pV1b=m_pCurveOne->GetVertexByIndex((minI+2*span)%m_pCurveOne->GetVertexNum());
	CVector3d dir1=(pV1a->GetCoordinate()-pV1->GetCoordinate()).Cross(pV1b->GetCoordinate()-pV1->GetCoordinate());
	dir1.Normalize();

	span=m_pCurveTwo->GetVertexNum()/3;
	pV2=m_pCurveTwo->GetVertexByIndex(minJ);
	pV2a=m_pCurveTwo->GetVertexByIndex((minJ+span)%m_pCurveTwo->GetVertexNum());
	pV2b=m_pCurveTwo->GetVertexByIndex((minJ+2*span)%m_pCurveTwo->GetVertexNum());
	CVector3d dir2=(pV2a->GetCoordinate()-pV2->GetCoordinate()).Cross(pV2b->GetCoordinate()-pV2->GetCoordinate());
	dir2.Normalize();

	if(dir1.Dot(dir2)>0)
		reverse=false;
	else
		reverse=true;

	//outfile<<"reverse="<<reverse<<endl;

	//outfile<<"Start Vertex pair: <"<<minI<<","<<minJ<<">"<<endl;

	m_pCurveOne->Reordering(minI,false);
	m_pCurveTwo->Reordering(minJ,reverse);

	outfile<<"Reordered Curve 1: "<<m_pCurveOne->GetVertexNum()<<endl;
	for (int i=0;i<m_pCurveOne->GetVertexNum();i++)
	{
		CVertex *pV=m_pCurveOne->GetVertexByIndex(i);
		//outfile<<"V["<<i<<"] "<<pV<<endl;
	}

	outfile<<"Reordered Curve 2:"<<m_pCurveTwo->GetVertexNum()<<endl;
	for (int i=0;i<m_pCurveTwo->GetVertexNum();i++)
	{
		CVertex *pV=m_pCurveTwo->GetVertexByIndex(i);
		//outfile<<"V["<<i<<"] "<<pV<<endl;
	}



}

CPatch* CWrinkle::GlobalMaximumTriRegularityBridgeTriangulation(void)
{

	CPatch *mesh=NULL;

	int i,j;

	int qNodeNum,pNodeNum;

	CWEHeapNode ***TrglNodeArray;

	CWEHeapNode *sourceNode,*targetNode, *currentNode;

	m_levelOfLocalConvexity=2;
	BuildEdgeValidityMatrix();

	pNodeNum=m_curveOnePntNum;	qNodeNum=m_curveTwoPntNum;

	//	Step 1: create the nodes
	TrglNodeArray=(CWEHeapNode ***)new long[pNodeNum];
	for(i=0;i<pNodeNum;i++) {
		TrglNodeArray[i]=(CWEHeapNode **)new long[qNodeNum];
		for(j=0;j<qNodeNum;j++) {
			TrglNodeArray[i][j]=new CWEHeapNode;
			TrglNodeArray[i][j]->pIndex=i;
			TrglNodeArray[i][j]->qIndex=j;
			TrglNodeArray[i][j]->SetValue(1.0e+10);
		}
	}
	sourceNode=TrglNodeArray[0][0];
	sourceNode->SetValue(0);
	//	Step 2: compute the weights of edges linking the nodes
	for(i=0;i<pNodeNum;i++) {
		for(j=0;j<qNodeNum;j++) {
			currentNode=TrglNodeArray[i][j];
			if ((i+1)<pNodeNum) {
				currentNode->edgeList[currentNode->edgeNum]=TrglNodeArray[i+1][j];
				currentNode->edgeWeights[currentNode->edgeNum]=ComputeTrglRegularity(i,j,true);
				(currentNode->edgeNum)++;
			}
			if ((j+1)<qNodeNum) {
				currentNode->edgeList[currentNode->edgeNum]=TrglNodeArray[i][j+1];
				currentNode->edgeWeights[currentNode->edgeNum]=ComputeTrglRegularity(i,j,false);
				(currentNode->edgeNum)++;
			}
		}
	}

	//------------------------------------------------------------------------------
	//	Step 3: using the Dijkstra algorithm to extract the shortest path
	//------------------------------------------------------------------------------
	//	Initialize the minimum heap

	list<CWEHeapNode*>::iterator pos;

	list<CWEHeapNode*> *minHeap=new list<CWEHeapNode*>;
	for(i=0;i<pNodeNum;i++)
		for(j=0;j<qNodeNum;j++)
		{
			pos=minHeap->begin();
			while(pos!=minHeap->end())
			{
				if(!GreaterHeapNode(TrglNodeArray[i][j],(*pos)))
				{
					minHeap->insert(pos,TrglNodeArray[i][j]);
					break;
				}
				else
					pos++;
			}
			if(pos==minHeap->end())
				minHeap->push_back(TrglNodeArray[i][j]);
		}

	while(!minHeap->empty())
	{
		currentNode=minHeap->front();
		minHeap->pop_front();
		for(i=0;i<currentNode->edgeNum;i++) {
			if ((currentNode->GetValue()+currentNode->edgeWeights[i])
				<(currentNode->edgeList[i]->GetValue()))
			{
				currentNode->edgeList[i]->
					SetValue((float)(currentNode->GetValue()+currentNode->edgeWeights[i]));
				currentNode->edgeList[i]->previousNode=currentNode;


				minHeap->erase(find(minHeap->begin(),minHeap->end(),currentNode->edgeList[i]));
                pos=minHeap->begin();
				while(pos!=minHeap->end())
				{
					if(!GreaterHeapNode(currentNode->edgeList[i],(*pos)))
					{
						minHeap->insert(pos,currentNode->edgeList[i]);
						break;
					}
					else
                        pos++;
				}
				if(pos==minHeap->end())
					minHeap->push_back(currentNode->edgeList[i]);

			}
		}

	}
	//------------------------------------------------------------------------------
	delete minHeap;
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	//	determine the target node
	targetNode=TrglNodeArray[pNodeNum-1][qNodeNum-1];

	//------------------------------------------------------------------------------
	//	backward tracking the shortest path
	m_pList.RemoveAll();	m_qList.RemoveAll();
	currentNode=targetNode;
	while(currentNode) {
		m_pList.InsertAt(0,currentNode->pIndex);
		m_qList.InsertAt(0,currentNode->qIndex);
		currentNode=currentNode->previousNode;
	}

	//		Step 4: construct the WW_Shell by the shortest path
	mesh=MeshGenerationByShortestPath();

	//------------------------------------------------------------------------------
	//	Step 5: free the memory
	for(i=0;i<pNodeNum;i++) {
		for(j=0;j<qNodeNum;j++) {
			delete (CWEHeapNode*)(TrglNodeArray[i][j]);
		}
		delete [](CWEHeapNode**)(TrglNodeArray[i]);
	}
	delete [](CWEHeapNode ***)(TrglNodeArray);

	return mesh;

}

double CWrinkle::ComputeTrglRegularity(int pIndex, int qIndex, bool next_pOrq)
{
	CVector3d p1,p2,p3;

	p1=m_pCurveOne->GetVertexByIndex(pIndex)->GetCoordinate();
	p2=m_pCurveTwo->GetVertexByIndex(qIndex)->GetCoordinate();

	if (next_pOrq) 
		p3=m_pCurveOne->GetVertexByIndex(pIndex+1)->GetCoordinate();
	else
		p3=m_pCurveTwo->GetVertexByIndex(qIndex+1)->GetCoordinate();

	double area=((p1-p2).Cross(p3-p2)).Length();

	double l1,l2,l3,maxl;
	l1=(p1-p2).Length();
	l2=(p3-p2).Length();
	l3=(p1-p3).Length();
	maxl=l1>l2?l1:l2;
	if(l3>maxl)maxl=l3;

	double r=area/(l1+l2+l3);

	return fabs(3.464102*r/maxl-1.0);
}
