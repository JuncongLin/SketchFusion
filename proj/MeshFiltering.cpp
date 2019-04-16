#include "StdAfx.h"
#include ".\meshfiltering.h"
#include "nl.h"

#include "global.h"
#include <fstream>
using namespace std;
extern ofstream outfile;
using namespace std;

CMeshFiltering::CMeshFiltering(void)
{
}

CMeshFiltering::~CMeshFiltering(void)
{
}

void CMeshFiltering::Uniform(CSolid* pModel, vector<CVector3d*>& ResCoords)
{
}

void CMeshFiltering::CurvFlowLapImplicit(CSolid* pModel, vector<CVector3d*>& ResCoords, int iter)
{
	CVertex *pVert1, * pVert2,*pVNxt,*pVPre;
	CHalfEdge *pHe1, *pHe2, *pHNxt,*pHPre;
	int i,j,valency,nnz=0,k;
	double dt=0.0009;
	int index,start;

	CVector3d u,v,w,coord;
	double tweight,weight,area,ASub,cotA,cotB,A;
	int ii;

	int num=pModel->GetVertexNum();
	unsigned long* ijL=new unsigned long[20*num];
	double *vL=new double[20*num];


	for(i=0;i<num;i++)
	{
		pVert1=pModel->GetVertexByIndex(i);
		pVert1->SetIndex(i);
		CVector3d* pCoord=new CVector3d;
		*pCoord=pVert1->GetCoordinate();
		ResCoords.push_back(pCoord);
	}    


	for(ii=0;ii<iter;ii++)
	{
		index=num+1;

		for(i=0;i<num;i++)
		{
			if(ii==0)
				ijL[i]=index;

			pVert1=pModel->GetVertexByIndex(i);
			A=0;
			tweight=0;
			pHe1=pVert1->GetOutHalfEdge();
			pHe2=pHe1;
			start=index;
			do
			{
				pVert2=pHe2->GetMate()->GetVertex();
				pVPre=pHe2->GetMate()->GetPrevEdge()->GetVertex();
				pVNxt=pHe2->GetNextEdge()->GetNextEdge()->GetVertex();

				u=*(ResCoords[pVert1->GetIndex()])-*(ResCoords[pVPre->GetIndex()]);
				
				v=*(ResCoords[pVert2->GetIndex()])-*(ResCoords[pVPre->GetIndex()]);

				w=u.Cross(v);

				ASub=w.Length()/2;

				cotA=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

				u=*(ResCoords[pVert1->GetIndex()])-*(ResCoords[pVNxt->GetIndex()]);

				v=*(ResCoords[pVert2->GetIndex()])-*(ResCoords[pVNxt->GetIndex()]);

				w=u.Cross(v);

				cotB=1/(tan(acos((u.Dot(v))/(u.Length()*v.Length()))));

				weight=cotA+cotB;
				tweight+=weight;
				A+=ASub;

				vL[index]=-1*weight;
				if(ii==0)
					ijL[index++]=pVert2->GetIndex();
				else
					index++;

				pHe2=pVert1->NextOutHalfEdge(pHe2); 

			} while(pHe1!=pHe2);
			for(j=start;j<index;j++)
				vL[j]*=dt/(4*A);
			vL[i]=1+(dt*tweight)/(4*A);
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
		nlSolverParameteri(NL_LEAST_SQUARES, NL_FALSE) ;
		nlSolverParameteri(NL_MAX_ITERATIONS, 15*num) ;
		nlSolverParameterd(NL_THRESHOLD, 1e-10) ;

		nlBegin(NL_SYSTEM);

		for(i=0; i<num; i++) 
		{
			pVert1=pModel->GetVertexByIndex(i);
			coord=*ResCoords[pVert1->GetIndex()];
			nlSetVariable(i,coord[0]);
			nlSetVariable(num+i,coord[1]);
			nlSetVariable(2*num+i,coord[2]);
		}

		nlBegin(NL_MATRIX);
		for (i=0;i<3;i++)
		{
			for (j=0;j<num;j++)
			{
				pVert1=pModel->GetVertexByIndex(j);
				coord=*ResCoords[pVert1->GetIndex()];
				nlRowParameterd(NL_RIGHT_HAND_SIDE,-1*coord[i]);
				nlBegin(NL_ROW) ;
				for (k=ijL[j];k<ijL[j+1];k++)
				{
					nlCoefficient(i*num+ijL[k],vL[k]);
				}
				nlCoefficient(i*num+j,vL[j]);
				nlEnd(NL_ROW) ;
			}
		}
		nlEnd(NL_MATRIX);

		nlEnd(NL_SYSTEM);

		nlSolve() ;


		for(i=0;i<num;i++)
		{
			CVector3d *pCoord=ResCoords[i];
			pCoord->Set(nlGetVariable(i),nlGetVariable(num+i),nlGetVariable(2*num+i));
		}
		nlDeleteContext(nlGetCurrent()) ;

	}
	delete []ijL;
	delete []vL;
}