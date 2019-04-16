#include "StdAfx.h"
#include ".\detailpatch.h"
#include "BasicGeometryOpeartion.h"
#include <limits>
#include "nl.h"
#include "global.h"
#include <fstream>
extern ofstream outfile;
using namespace std;
CDetailPatch::CDetailPatch(void)
: m_bDisplayStyle(true)
{
	m_dXYRatio=1.0;
}

CDetailPatch::~CDetailPatch(void)
{
	delete m_pPatchI;
	delete m_pPatchO;
}

void CDetailPatch::SetIPatch(CPatch* pPatch)
{
	m_pPatchI=pPatch;
}

void CDetailPatch::SetOPatch(CPatch* pPatch)
{
	m_pPatchO=pPatch;
}

void CDetailPatch::SetFrame(CVector3d& x, CVector3d& y, CVector3d& z)
{
	m_Frame.SetAxises(x,y,z);
}

void CDetailPatch::DetermineCorner(CVector3d& c1,CVector3d& c2, CVector3d& c3, CVector3d& c4)
{


	double min1,min2,min3,min4,dst;
	min1=min2=min3=min4=DBL_MAX;

	CVector3d coord;
	CVertex *pV;

	int i;

	for (i=0;i<m_pBoundary->GetVertexNum();i++)
	{
		pV=m_pBoundary->GetVertexByIndex(i);
		coord=pV->GetCoordinate();

		dst=(coord-c1).Length();
		if(dst<min1)
		{
			min1=dst;
			m_pCorner1=pV;
		}
	
		dst=(coord-c2).Length();
		if(dst<min2)
		{
			min2=dst;
			m_pCorner2=pV;
		}

		dst=(coord-c3).Length();
		if(dst<min3)
		{
			min3=dst;
			m_pCorner3=pV;
		}

		dst=(coord-c4).Length();
		if(dst<min4)
		{
			min4=dst;
			m_pCorner4=pV;
		}

	}
}

void CDetailPatch::ConvexMapping(void)
{

	ResetIndex();

	m_pParaCoord=new CVector2d[m_pPatchO->GetVertexNum()];
	m_ppLambda=new double* [m_pPatchO->GetVertexNum()];
	outfile<<"bound mapping"<<endl;
	BoundaryMapping();
	outfile<<"internal mapping"<<endl;
	InternalMapping();

	m_pBoundary->ClearFlag();

}

void CDetailPatch::BoundaryMapping(void)
{
	int i,j,size,num;
	double TotLen,l;
	CVector3d u;
	CVertex *pV1, *pV2;
	vector<double> vecLen;
	size=m_pPatchO->GetVertexNum();
	num=size-m_pBoundary->GetVertexNum();



	bool reversed=false;

	//outfile<<m_pCorner1->GetIndex()<<","<<m_pCorner2->GetIndex()<<","<<m_pCorner3->GetIndex()<<","<<m_pCorner4->GetIndex()<<endl;


	i=m_pCorner1->GetIndex();
	j=i+1;
	do
	{
		//outfile<<"j="<<j<<endl;
		j=(j<size)?j:num;
		//		j%=size;
		if(j==m_pCorner2->GetIndex()){break;}
		if(j==m_pCorner4->GetIndex()){reversed=true;break;}
		j++;		
	} while(j!=i);
	if (reversed)
	{
		outfile<<"reverse"<<endl;

		i=m_pCorner1->GetIndex();
		TotLen=0.0;
		do
		{
			i=(i<size)?i:num;
			j=((i+1)<size)?(i+1):num;
			pV1=m_pBoundary->GetVertexByIndex(i-num);
			pV2=m_pBoundary->GetVertexByIndex(j-num);

			l=(pV1->GetCoordinate()-pV2->GetCoordinate()).Length();

			TotLen+=l;
			vecLen.push_back(TotLen);
			i++;		
		} while(j!=m_pCorner4->GetIndex());

		m_pParaCoord[m_pCorner1->GetIndex()].Set(0.0,0.0);
		i=1;
		j=m_pCorner1->GetIndex()+i;
		j=(j<size)?j:num;
		do
		{
			pV1=m_pBoundary->GetVertexByIndex(j-num);
			m_pParaCoord[pV1->GetIndex()].Set(vecLen[i-1]/TotLen,0.0);
			i++;		
			j=m_pCorner1->GetIndex()+i;
			j=(j<size)?j:j-size+num;
		} while(j!=m_pCorner4->GetIndex());

		vecLen.clear();
		i=m_pCorner4->GetIndex();
		TotLen=0.0;
		do
		{
			i=(i<size)?i:num;
			j=((i+1)<size)?(i+1):num;
			pV1=m_pBoundary->GetVertexByIndex(i-num);
			pV2=m_pBoundary->GetVertexByIndex(j-num);

			l=(pV1->GetCoordinate()-pV2->GetCoordinate()).Length();
			TotLen+=l;
			vecLen.push_back(TotLen);
			i++;		
		} while(j!=m_pCorner3->GetIndex());
		i=1;
		m_pParaCoord[m_pCorner4->GetIndex()].Set(1.0,0.0);
		j=m_pCorner4->GetIndex()+i;
		j=(j<size)?j:num;
		do
		{
			pV1=m_pBoundary->GetVertexByIndex(j-num);
			m_pParaCoord[pV1->GetIndex()].Set(1.0,m_dXYRatio*vecLen[i-1]/TotLen);

			i++;		
			j=m_pCorner4->GetIndex()+i;
			j=(j<size)?j:j-size+num;
		} while(j!=m_pCorner3->GetIndex());

		vecLen.clear();
		i=m_pCorner3->GetIndex();
		TotLen=0.0;
		do
		{
			i=(i<size)?i:num;
			j=((i+1)<size)?(i+1):num;

			pV1=m_pBoundary->GetVertexByIndex(i-num);
			pV2=m_pBoundary->GetVertexByIndex(j-num);

			l=(pV1->GetCoordinate()-pV2->GetCoordinate()).Length();
			TotLen+=l;
			vecLen.push_back(TotLen);
			i++;		
		} while(j!=m_pCorner2->GetIndex());
		i=1;
		m_pParaCoord[m_pCorner3->GetIndex()].Set(1.0,m_dXYRatio);
		j=m_pCorner3->GetIndex()+i;
		j=(j<size)?j:j-size+num;
		do
		{
			pV1=m_pBoundary->GetVertexByIndex(j-num);
			m_pParaCoord[pV1->GetIndex()].Set(1.0-vecLen[i-1]/TotLen,m_dXYRatio);

			i++;		
			j=m_pCorner3->GetIndex()+i;
			j=(j<size)?j:j-size+num;
		} while(j!=m_pCorner2->GetIndex());

		vecLen.clear();
		i=m_pCorner2->GetIndex();
		TotLen=0.0;
		do
		{
			i=(i<size)?i:num;
			j=((i+1)<size)?(i+1):num;

			pV1=m_pBoundary->GetVertexByIndex(i-num);
			pV2=m_pBoundary->GetVertexByIndex(j-num);

			l=(pV1->GetCoordinate()-pV2->GetCoordinate()).Length();
			TotLen+=l;
			vecLen.push_back(TotLen);
			i++;		
		} while(j!=m_pCorner1->GetIndex());
		i=1;
		m_pParaCoord[m_pCorner2->GetIndex()].Set(0.0,1.0);
		j=m_pCorner2->GetIndex()+i;
		j=(j<size)?j:num;
		do
		{

			pV1=m_pBoundary->GetVertexByIndex(j-num);
			//			pV1->SetParaCoord(1.0,1.0-pLenVect->at(i-1)/TotLen);
			m_pParaCoord[pV1->GetIndex()].Set(0.0,m_dXYRatio*(1.0-vecLen[i-1]/TotLen));

			i++;		
			j=m_pCorner2->GetIndex()+i;
			j=(j<size)?j:j-size+num;
		} while(j!=m_pCorner1->GetIndex());
	}
	else
	{
		outfile<<"unreverse"<<endl;
		outfile<<"1.1"<<endl;
		i=m_pCorner1->GetIndex();
		TotLen=0.0;
		do
		{
			i=(i<size)?i:num;
			j=((i+1)<size)?(i+1):num;
			pV1=m_pBoundary->GetVertexByIndex(i-num);
			pV2=m_pBoundary->GetVertexByIndex(j-num);

			l=(pV1->GetCoordinate()-pV2->GetCoordinate()).Length();

			TotLen+=l;
			vecLen.push_back(TotLen);
			i++;		
		} while(j!=m_pCorner2->GetIndex());
		outfile<<"1.2"<<endl;
		m_pParaCoord[m_pCorner1->GetIndex()].Set(0.0,0.0);
		i=1;
		j=m_pCorner1->GetIndex()+i;
		j=(j<size)?j:num;
		do
		{
			pV1=m_pBoundary->GetVertexByIndex(j-num);
			m_pParaCoord[pV1->GetIndex()].Set(0.0,m_dXYRatio*vecLen[i-1]/TotLen);
			i++;		
			j=m_pCorner1->GetIndex()+i;
			j=(j<size)?j:j-size+num;
		} while(j!=m_pCorner2->GetIndex());

		outfile<<"2.1"<<endl;
		vecLen.clear();
		i=m_pCorner2->GetIndex();
		TotLen=0.0;
		do
		{
			i=(i<size)?i:num;
			j=((i+1)<size)?(i+1):num;

			pV1=m_pBoundary->GetVertexByIndex(i-num);
			pV2=m_pBoundary->GetVertexByIndex(j-num);

			l=(pV1->GetCoordinate()-pV2->GetCoordinate()).Length();
			TotLen+=l;
			vecLen.push_back(TotLen);
			i++;		
		} while(j!=m_pCorner3->GetIndex());
		i=1;

		outfile<<"2.2"<<endl;
		m_pParaCoord[m_pCorner2->GetIndex()].Set(0.0,m_dXYRatio);
		j=m_pCorner2->GetIndex()+i;
		j=(j<size)?j:num;
		do
		{
			pV1=m_pBoundary->GetVertexByIndex(j-num);
			m_pParaCoord[pV1->GetIndex()].Set(vecLen[i-1]/TotLen,m_dXYRatio);

			i++;		
			j=m_pCorner2->GetIndex()+i;
			j=(j<size)?j:j-size+num;
		} while(j!=m_pCorner3->GetIndex());

		outfile<<"3.1"<<endl;
		vecLen.clear();
		i=m_pCorner3->GetIndex();
		TotLen=0.0;
		do
		{
			i=(i<size)?i:num;
			j=((i+1)<size)?(i+1):num;

			pV1=m_pBoundary->GetVertexByIndex(i-num);
			pV2=m_pBoundary->GetVertexByIndex(j-num);

			l=(pV1->GetCoordinate()-pV2->GetCoordinate()).Length();
			TotLen+=l;
			vecLen.push_back(TotLen);
			i++;		
		} while(j!=m_pCorner4->GetIndex());
		outfile<<"3.2"<<endl;
		i=1;
		m_pParaCoord[m_pCorner3->GetIndex()].Set(1.0,m_dXYRatio);
		j=m_pCorner3->GetIndex()+i;
		j=(j<size)?j:num;
		do
		{
			pV1=m_pBoundary->GetVertexByIndex(j-num);
			//			pV1->SetParaCoord(1.0,1.0-pLenVect->at(i-1)/TotLen);
			m_pParaCoord[pV1->GetIndex()].Set(1.0,1.0-vecLen[i-1]/TotLen);

			i++;		
			j=m_pCorner3->GetIndex()+i;
			j=(j<size)?j:j-size+num;
		} while(j!=m_pCorner4->GetIndex());

		outfile<<"4.1"<<endl;
		vecLen.clear();
		i=m_pCorner4->GetIndex();
		TotLen=0.0;
		do
		{
			i=(i<size)?i:num;
			j=((i+1)<size)?(i+1):num;

			pV1=m_pBoundary->GetVertexByIndex(i-num);
			pV2=m_pBoundary->GetVertexByIndex(j-num);

			l=(pV1->GetCoordinate()-pV2->GetCoordinate()).Length();
			TotLen+=l;
			vecLen.push_back(TotLen);
			i++;		
		} while(j!=m_pCorner1->GetIndex());
		outfile<<"4.2"<<endl;
		i=1;
		m_pParaCoord[m_pCorner4->GetIndex()].Set(1.0,0.0);
		j=m_pCorner4->GetIndex()+i;
		j=(j<size)?j:num;
		do
		{
			pV1=m_pBoundary->GetVertexByIndex(j-num);
			//			pV1->SetParaCoord(1.0,1.0-pLenVect->at(i-1)/TotLen);
			m_pParaCoord[pV1->GetIndex()].Set(1.0-vecLen[i-1]/TotLen,0.0);

			i++;		
			j=m_pCorner4->GetIndex()+i;
			j=(j<size)?j:j-size+num;
		} while(j!=m_pCorner1->GetIndex());

	}

}

void CDetailPatch::InternalMapping(void)
{
	int num,i,j,k,count,nnz;
	CVertex *pV,*pNV;

	CHalfEdge *pHe1,*pHe2,*pHe3;
	vector<CVertex*> vecNeigh;
	vector<double> vecTheta;
	vector<CVector2d*> vecProj;
	CVertex *pV1,*pV2;
	double theta,TotAng;
	CVector3d u,v;
	double l,ang;
	int valency;
	CVector2d* pt;
	double ratio;

	CVector2d raw;	double* pA,*bu,*bv,*coord;

	num=m_pPatchO->GetVertexNum()-m_pBoundary->GetVertexNum();
	count=0;

	outfile<<m_pPatchO->GetVertexNum()<<endl;
	outfile<<m_pBoundary->GetVertexNum()<<endl;


	for(k=0;k<m_pPatchO->GetVertexNum();k++)
	{


		pV=m_pPatchO->GetVertexByIndex(k);
		if(pV->IsBoundary())continue;


		theta=0.0;
		valency=0;
		raw.Set(0,0);

		vecTheta.clear();
		vecProj.clear();
		vecNeigh.clear();


		pHe1=pV->GetOutHalfEdge();
		do
		{
			pHe2=pV->NextOutHalfEdge(pHe1);
			pV1=pHe1->GetMate()->GetVertex();
			pV2=pHe2->GetMate()->GetVertex();

			u=pV1->GetCoordinate()-pV->GetCoordinate();
			u.Normalize();

			v=pV2->GetCoordinate()-pV->GetCoordinate();
			v.Normalize();


			ang=acos(u.Dot(v));
			theta+=ang;
			valency++;


			vecTheta.push_back(ang);
			vecNeigh.push_back(pV1);
			pHe1=pV->NextOutHalfEdge(pHe1);
		} while(pHe1!=pV->GetOutHalfEdge());


		ratio=2*M_PI/theta;

		pV1=vecNeigh[0];

		l=(pV1->GetCoordinate()-pV->GetCoordinate()).Length();
		l*=10;
		pt=new CVector2d;
		pt->Set(l,0);
		vecProj.push_back(pt);
		TotAng=0;


		for(i=1;i<valency;i++)
		{

			pV1=vecNeigh[i];
			TotAng+=vecTheta[i-1]*ratio;
			l=(pV1->GetCoordinate()-pV->GetCoordinate()).Length();
			l*=10;

			pt=new CVector2d;
			pt->Set(l*cos(TotAng),l*sin(TotAng));
			vecProj.push_back(pt);
		}

		m_ppLambda[pV->GetIndex()]=new double[valency];
		if ( valency > 3 )
		{

			for ( i = 0; i < valency; ++i ) m_ppLambda[pV->GetIndex()][i] = 0.0f;
			for ( i = 0; i < valency; ++i )
			{
				j = 0;
				do {
					int v1 = j;
					int v2 = (j != valency-1) ? j+1 : 0;
					if ( (v1 != i) && (v2 != i) )
					{

						outfile<<CBasicGeometryOperation::IsLeftSide(*vecProj[i], raw, *vecProj[v1] )<<endl;
						outfile<<CBasicGeometryOperation::IsLeftSide( *vecProj[i], raw, *vecProj[v2] )<<endl;
						if ( CBasicGeometryOperation::IsLeftSide(*vecProj[i], raw, *vecProj[v1] ) != 
							CBasicGeometryOperation::IsLeftSide( *vecProj[i], raw, *vecProj[v2] ) )
						{
							double area=CBasicGeometryOperation::TriangleArea(*vecProj[i],*vecProj[v1],*vecProj[v2]);

							m_ppLambda[pV->GetIndex()][i]  -= ( CBasicGeometryOperation::TriangleArea(raw, *vecProj[v1], *vecProj[v2])/area );

							m_ppLambda[pV->GetIndex()][v1]-=( CBasicGeometryOperation::TriangleArea( *vecProj[i], raw,  *vecProj[v2] )/area );

							m_ppLambda[pV->GetIndex()][v2] -= ( CBasicGeometryOperation::TriangleArea( *vecProj[i], *vecProj[v1], raw )/area );
							break;
						}
					}
					++j;
				}while ( j < valency );

			}
			for ( i = 0; i <valency; ++i )
				m_ppLambda[pV->GetIndex()][i] /= (double) valency;

			double TotLambda=0;
			for(i=0;i<valency;i++)
			{
			TotLambda-=m_ppLambda[pV->GetIndex()][i];
			}

		}
		else if ( valency == 3 )
		{
			double area = CBasicGeometryOperation::TriangleArea( *vecProj[0],*vecProj[1],*vecProj[2]);
			m_ppLambda[pV->GetIndex()][0] = -1.0f* CBasicGeometryOperation::TriangleArea( raw, *vecProj[1], *vecProj[2]) / area;
			m_ppLambda[pV->GetIndex()][1] = -1.0f* CBasicGeometryOperation::TriangleArea( *vecProj[0], raw, *vecProj[2])  / area;
			m_ppLambda[pV->GetIndex()][2] = -1.0f* CBasicGeometryOperation::TriangleArea( *vecProj[0], *vecProj[1], raw ) / area;
		}

	}


	nlNewContext() ;
	if(nlInitExtension("SUPERLU")) {
		nlSolverParameteri(NL_SOLVER, NL_PERM_SUPERLU_EXT) ;
	} else {
		nlSolverParameteri(NL_SOLVER, NL_CG) ;
		nlSolverParameteri(NL_PRECONDITIONER, NL_PRECOND_JACOBI) ;
	}
	nlSolverParameteri(NL_NB_VARIABLES, 2*num) ;
	nlSolverParameteri(NL_LEAST_SQUARES, NL_FALSE) ;
	nlSolverParameteri(NL_MAX_ITERATIONS, 10*num) ;
	nlSolverParameterd(NL_THRESHOLD, 1e-10) ;

	nlBegin(NL_SYSTEM);


	for(i=0; i<2*num; i++) 
	{
		nlSetVariable(i,0.0);
	}

	double b;

	outfile<<"matrix"<<endl;

	nlBegin(NL_MATRIX);

	for (k=0;k<2;k++)
	{
		for(i=0;i<m_pPatchO->GetVertexNum();i++)
		{
			pV=m_pPatchO->GetVertexByIndex(i);
			if(pV->IsBoundary())continue;
			outfile<<"vi"<<pV->GetIndex()<<endl;
			count=0;
			b=0;
			pHe1=pV->GetOutHalfEdge();
			do
			{
				pNV=pHe1->GetMate()->GetVertex();
				if(pNV->IsBoundary())
				{

					outfile<<"vni: "<<pNV->GetIndex()<<endl;

					b-=m_ppLambda[pV->GetIndex()][count]*m_pParaCoord[pNV->GetIndex()][k];
				}
				count++;
				pHe1=pV->NextOutHalfEdge(pHe1);
			} while(pHe1!=pV->GetOutHalfEdge());


			count=0;

			nlRowParameterd(NL_RIGHT_HAND_SIDE,-b);
			nlBegin(NL_ROW);
			pHe1=pV->GetOutHalfEdge();
			do
			{
				pNV=pHe1->GetMate()->GetVertex();
				if(!pNV->IsBoundary())
					nlCoefficient(k*num+pNV->GetIndex(),m_ppLambda[pV->GetIndex()][count]);
				count++;
				pHe1=pV->NextOutHalfEdge(pHe1);
			} while(pHe1!=pV->GetOutHalfEdge());
			nlCoefficient(k*num+pV->GetIndex(),1.0);
			nlEnd(NL_ROW);
		}

	}

	nlEnd(NL_MATRIX);

	nlEnd(NL_SYSTEM);


	outfile<<"solving"<<endl;

	nlSolve() ;



	outfile<<"export"<<endl;

	count=0;

	for(i=0;i<m_pPatchO->GetVertexNum();i++)
	{
		pV=m_pPatchO->GetVertexByIndex(i);
		if(pV->IsBoundary())continue;

		m_pParaCoord[pV->GetIndex()].Set(nlGetVariable(pV->GetIndex()),nlGetVariable(num+pV->GetIndex()));

	}
	nlDeleteContext(nlGetCurrent());

	outfile<<"done"<<endl;

}

double CDetailPatch::Compare(CDetailPatch* pDPatch)
{

	int i,j,count;
	CVertex *pV;
	double diff;
	CVector3d detail1,detail2,detailTmp;
	vector<int> vecIndices;
	ResetIndex();
	for (i=0;i<m_pPatchI->GetVertexNum();i++)
	{
		vecIndices.push_back(m_pPatchI->GetVertexByIndex(i)->GetIndex());
	}
	CMatrix44 mat=m_Frame.TransformMatrix(pDPatch->GetFrame());
	diff=0.0;
	count=0;
	for(i=0;i<m_pPatchI->GetVertexNum();i++)
	{
		pV=m_pPatchI->GetVertexByIndex(i);
		if(!pV->IsKnown())continue;
		count++;
		detail1=*(m_vecDetails[vecIndices[i]]);



		detailTmp=pDPatch->DetailFromParaCoord(m_pParaCoord[vecIndices[i]]);

		detail2=mat.MultMatVec(detailTmp);
		diff+=(detail2-detail1).Length();

	}
	outfile<<count<<endl;
	diff/=count;
	m_pBoundary->ClearFlag();
	return diff;
}

void CDetailPatch::FillUnknownFromSample(CDetailPatch* pDPatch)
{
	CMatrix44 mat=m_Frame.TransformMatrix(pDPatch->GetFrame());

	int i,j;
	CVertex *pV;
	CVector3d detail,detailTmp,*pDetail;
	ResetIndex();
	vector<int> vecIndices;
	for (i=0;i<m_pPatchI->GetVertexNum();i++)
	{
		vecIndices.push_back(m_pPatchI->GetVertexByIndex(i)->GetIndex());
	}
	for(i=0;i<m_pPatchI->GetVertexNum();i++)
	{
		pV=m_pPatchI->GetVertexByIndex(i);
		if(pV->IsKnown())continue;

		detailTmp=pDPatch->DetailFromParaCoord(m_pParaCoord[vecIndices[i]]);

		detail=mat.MultMatVec(detailTmp);

		pDetail=*(m_vecDetails.begin()+vecIndices[i]);
		*pDetail=detail;

		pV->SetKnown(true);

	}
	m_pBoundary->ClearFlag();

}

CFrame& CDetailPatch::GetFrame(void)
{
	return m_Frame;
}

CVector3d CDetailPatch::DetailFromParaCoord(CVector2d& coord)
{
	CFace* pF;
	CVertex* pV1,*pV2,*pV3;
	CHalfEdge* pHe;
	CVector2d pt1,pt2,pt3,pt;
	CVector3d bc;
	CVector3d detail1,detail2,detail3,detail;

	int i;

	ResetIndex();


	for(i=0;i<m_pPatchO->GetFaceNum();i++)
	{
		pF=m_pPatchO->GetFaceByIndex(i);

		pHe = pF->GetFirstEdge();

		pV1 = pHe->GetVertex ();
		pt1=m_pParaCoord[pV1->GetIndex()];

		pHe =pF->GetNextEdge(pHe);
		pV2 = pHe->GetVertex ();
		pt2=m_pParaCoord[pV2->GetIndex()];

		pHe =pF->GetNextEdge(pHe);
		pV3 = pHe->GetVertex ();
		pt3=m_pParaCoord[pV3->GetIndex()];
		if(CBasicGeometryOperation::IsPointInFace(coord, pt1, pt2, pt3 ))break;
	}

	CBasicGeometryOperation::BarycentricCoordinate(coord, pt1, pt2, pt3, bc);

	//outfile<<pV1<<":"<<pV1->GetIndex()<<endl;
	//outfile<<pV2<<":"<<pV2->GetIndex()<<endl;
	//outfile<<pV3<<":"<<pV3->GetIndex()<<endl;

	detail1=*(m_vecDetails[pV1->GetIndex()]);
	detail2=*(m_vecDetails[pV2->GetIndex()]);
	detail3=*(m_vecDetails[pV3->GetIndex()]);

	detail=bc[0]*detail1+bc[1]*detail2+bc[2]*detail3;

	m_pBoundary->ClearFlag();

	return detail;


}

void CDetailPatch::Display(void)
{
	int i;
	CFace *pFace;
	CHalfEdge *pHe;
	glPolygonMode(GL_FRONT,GL_FILL);
	if(m_bDisplayStyle)
	{
		glDepthRange(0.0,0.999);
		glBegin(GL_TRIANGLES);
		for (i=0;i<m_pPatchO->GetFaceNum();i++)
		{
			pFace=m_pPatchO->GetFaceByIndex(i);
			if(m_pPatchI->Find(pFace))
				glColor3f(1.0,0.0,0.0);
			else
				glColor3f(0.0,1.0,0.0);

			pHe=pFace->GetFirstEdge();
			glVertex3dv(pHe->GetVertex()->GetCoordinate().GetArray());
			pHe=pFace->GetNextEdge(pHe);
			glVertex3dv(pHe->GetVertex()->GetCoordinate().GetArray());
			pHe=pFace->GetNextEdge(pHe);
			glVertex3dv(pHe->GetVertex()->GetCoordinate().GetArray());

		}
		glEnd();
		glDepthRange(0.0,1.0);

	}
	else
	{

		for (i=0;i<m_pPatchO->GetVertexNum();i++)
		{
			m_pPatchO->GetVertexByIndex(i)->Display();
		}

	}
	m_pBoundary->Display();



	CVector3d pt1;

	glColor3f(1.0,0.0,0.0);
	pt1=m_pCorner1->GetCoordinate();
	glPushMatrix ();
	glTranslated(pt1[0],pt1[1],pt1[2]);
	gluSphere(gluNewQuadric(),0.01,50,50);
	glPopMatrix();

	glColor3f(1.0,1.0,0.0);
	pt1=m_pCorner2->GetCoordinate();
	glPushMatrix ();
	glTranslated(pt1[0],pt1[1],pt1[2]);
	gluSphere(gluNewQuadric(),0.01,50,50);
	glPopMatrix();

	glColor3f(0.0,1.0,0.0);
	pt1=m_pCorner3->GetCoordinate();
	glPushMatrix ();
	glTranslated(pt1[0],pt1[1],pt1[2]);
	gluSphere(gluNewQuadric(),0.01,50,50);
	glPopMatrix();

	glColor3f(0.0,1.0,1.0);
	pt1=m_pCorner4->GetCoordinate();
	glPushMatrix ();
	glTranslated(pt1[0],pt1[1],pt1[2]);
	gluSphere(gluNewQuadric(),0.01,50,50);
	glPopMatrix();

	m_Frame.Display();
}

int CDetailPatch::GetEdgeNum(void)
{
	return m_pPatchO->GetEdgeNum();
}

CEdge* CDetailPatch::GetEdgeByIndex(int index)
{
	return m_pPatchO->GetEdgeByIndex(index);
}

double CDetailPatch::GetXYRatio(void)
{
	return m_dXYRatio;
}

CVector2d& CDetailPatch::GetParaCoord(int index)
{
	return m_pParaCoord[index];
}

bool CDetailPatch::IsFaceInside(CFace* pFace)
{
	return m_pPatchI->Find(pFace);
}



void CDetailPatch::SetXYRatio(double ratio)
{
	m_dXYRatio=ratio;
}

void CDetailPatch::SwitchDisplayStyle(void)
{
	m_bDisplayStyle=!m_bDisplayStyle;
}

void CDetailPatch::ClearFlag(void)
{
	m_pPatchI->ClearFlag();
	m_pPatchO->ClearFlag();
	m_pBoundary->ClearFlag();
}

void CDetailPatch::AddDetail(CVector3d* pDetail)
{
	m_vecDetails.push_back(pDetail);
}



void CDetailPatch::ImportDetails(vector<CVector3d*>& vDetails)
{
	CVertex *pV;
	int i, count=0;
	m_pBoundary=m_pPatchO->GetBoundaryByIndex(0);
	for (i=0;i<m_pPatchO->GetVertexNum();i++)
	{
		pV=m_pPatchO->GetVertexByIndex(i);
		if(pV->IsBoundary())continue;
		m_vecDetails.push_back(vDetails[count++]);
	}
	for (i=0;i<m_pBoundary->GetVertexNum();i++)
	{
		m_vecDetails.push_back(vDetails[count++]);
	}
}

void CDetailPatch::ApplyDetail(vector<CVector3d*>& vDetails)
{
	vector<CVector3d*>::iterator git=vDetails.begin();
	vector<CVector3d*>::iterator lit=m_vecDetails.begin();
	CVertex *pV;
	int i,count=0;
	m_pBoundary->SetFlag();
	for (i=0;i<m_pPatchO->GetVertexNum();i++)
	{
		pV=m_pPatchO->GetVertexByIndex(i);
		if(pV->IsBoundary())continue;

		if(pV->IsKnown())
		{
			*(*(git+pV->GetIndex()))=*(*(lit+count));
		}
		count++;
	}
	m_pBoundary->ClearFlag();
}
