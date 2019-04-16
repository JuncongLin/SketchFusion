#include "stdafx.h"
#include ".\rbfsurface.h"
#include "nl.h"
#include "matlib.h"
#include "global.h"
#include <fstream>
using namespace std;
extern ofstream outfile;
using namespace std;

//#include "..\taucs\taucsaddon.h"
CRBFSurface::CRBFSurface(void)
{
	k = 0;
	cur = 0;
	c = NULL;
	d = NULL;
	h = NULL;
	phi = NULL;
	lambda=NULL;

	p[0]=p[1]=p[2]=p[3]=0.0;

}

CRBFSurface::~CRBFSurface(void)
{
	delete []c;
	c = NULL;
	delete []d;
	d = NULL;
	delete []h;
	h=NULL;
	delete []phi;
	phi=NULL;
	delete []lambda;
	lambda=NULL;

}

double CRBFSurface::GetValueAt(CVector3d point)
{
	float value = p[0]+point.x()*p[1]+point.y()*p[2]+point.z()*p[3];
	float tmp=0.0;
	int i=0;
	CVector3d vc;
	for(;i<k;++i){
		vc.Set(&c[3*i]);
		tmp=(point-vc).Length();
		value+=d[i]*tmp;
	}
	return value;

}

double CRBFSurface::GetValueAt(double point[])
{
	CVector3d vpoint(point);
	return GetValueAt(vpoint);
}


CVector3d CRBFSurface::GradientAt(CVector3d point)
{
	static CVector3d result;
	CVector3d tmp;

	tmp.Set(point.x()+0.001,point.y(),point.z());
	result.x((GetValueAt(tmp)-GetValueAt(point))*1000);

	tmp.Set(point.x(),point.y()+0.001,point.z());
	result.y((GetValueAt(tmp)-GetValueAt(point))*1000);

	tmp.Set(point.x(),point.y(),point.z()+0.001);
	result.z((GetValueAt(tmp)-GetValueAt(point))*1000);

	return &result;
}

void CRBFSurface::Initialize(int num)
{

	cur=0;
	k = num;
	c = new double[3*k];
	ZeroMemory(c,3*k*sizeof(double));
	d = new double[k];
	lambda = new double[k];
	h = new double[k+4];
	h[k]=h[k+1]=h[k+2]=h[k+3]=0.0;
	phi = new double[k*k];

}

void CRBFSurface::Solve(void)
{
	ASSERT(cur==k);
	CVector3d v1,v2;
	double val;

	//求phi
	int i,j;
	for(i=0;i<k;++i)
	{
			phi[i*k+i] = lambda[i];  
	}
	for(i=0;i<k;++i)
	{
		for(j=0;j<i;++j)
		{
			v1.Set(&c[3*i]);
			v2.Set(&c[3*j]);
			val=(v1-v2).Length();
			phi[i*k+j] = val;
		}
	}
	for(i=0;i<k;++i)
	{
		for(j=k-1;j>i;--j){
			phi[i*k+j] = phi [j*k+i];
		}
	}

	nlNewContext() ;
	if(nlInitExtension("SUPERLU")) {
		nlSolverParameteri(NL_SOLVER, NL_PERM_SUPERLU_EXT) ;
	} else {
		nlSolverParameteri(NL_SOLVER, NL_CG) ;
		nlSolverParameteri(NL_PRECONDITIONER, NL_PRECOND_JACOBI) ;
	}
	nlSolverParameteri(NL_NB_VARIABLES, (k+4)) ;
	nlSolverParameteri(NL_LEAST_SQUARES, NL_FALSE) ;
	nlSolverParameteri(NL_MAX_ITERATIONS, 5*(k+4)) ;
	nlSolverParameterd(NL_THRESHOLD, 1e-10) ;

	nlBegin(NL_SYSTEM) ;

	for(i=0; i<(k+4); i++) 
		nlSetVariable(i,0.0) ;

	nlBegin(NL_MATRIX);

	for(i=0;i<(k+4);i++)
	{
		nlRowParameterd(NL_RIGHT_HAND_SIDE,-1*h[i]);
		nlBegin(NL_ROW) ;
		if(i<k)
		{
			for(j=0;j<(k+4);j++)
			{
				if(j<k)
					nlCoefficient(j,phi[i*k+j]);
				else if(j==k)
					nlCoefficient(j,1.0);
				else
					nlCoefficient(j,c[3*i+j-k-1]);
			}

		}
		else if(i==k)
		{
			for(j=0;j<(k+4);j++)
			{
				if(j<k)
					nlCoefficient(j,1.0);
				else
					nlCoefficient(j,0.0);
			}

		}
		else
		{
			for(j=0;j<(k+4);j++)
			{
				if(j<k)
					nlCoefficient(j,c[3*j+i-k-1]);
				else
					nlCoefficient(j,0.0);
			}

		}
		nlEnd(NL_ROW) ;

	}

	nlEnd(NL_MATRIX) ;

	nlEnd(NL_SYSTEM) ;

	//outfile<<"solve"<<endl;
	nlSolve() ;
	//outfile<<"copy back"<<endl;
	for(i=0;i<k;i++)
		d[i]=nlGetVariable(i);
	for(;i<(k+4);i++)
		p[i-k]=nlGetVariable(i);
	nlDeleteContext(nlGetCurrent()) ;



	//int num=k;
	//Mm CT = zeros(4,num);
	//Mm CT0 = zeros(3,num);
	//memcpy(CT0.addr(),c,3*num*sizeof(double));
	//Mm I = zeros(1,num);
	//i;
	//for(i=1;i<=num;++i){
	//	I.r(1,i) = 1;
	//}
	//CT = vertcat(I,CT0);
	//Mm C = transpose(CT);
	//Mm corner = zeros(4,4);
	//Mm PHI = zeros(num,num);
	//memcpy(PHI.addr(),phi,num*num*sizeof(double));  //phi是对称的，不需要调整
	//Mm A = vertcat(horzcat(PHI,C),horzcat(CT,corner));
	//Mm B = zeros(num+4,1);
	//memcpy(B.addr(),h,(num+4)*sizeof(double));
	//Mm X = mldivide(A,B);
	//memcpy(d,X.addr(),num*sizeof(double));
	//memcpy(p,X.addr(num+1),4*sizeof(double));


}

void CRBFSurface::AddConstraint(const double* coord, double value, double lam)
{
	ASSERT(cur<k);
	memcpy(&c[3*cur],coord,3*sizeof(double));
	h[cur] = value;
	lambda[cur] = lam;
	++cur;
}

