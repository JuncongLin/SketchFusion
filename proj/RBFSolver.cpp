// RBFSolver.cpp: implementation of the RBFSolver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MathFunc.h"
#include <fstream>
#include <math.h>
#include ".\rbfsolver.h"
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern ofstream debugfile;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RBFSolver::RBFSolver()
{
   k = 0;
   cur = 0;
   c = NULL;
   d = NULL;
   h = NULL;
   
   phi = NULL;
   p[0]=p[1]=p[2]=p[3]=0.0;
   m_bSolve = false;
   m_iInitNum=0;

}

RBFSolver::~RBFSolver()
{

}

float RBFSolver::GetValueAt(double coord[])
{
	float value = p[0]+coord[0]*p[1]+coord[1]*p[2]+coord[2]*p[3];
	float tmp=0.0;
	int i=0;
	for(;i<k;++i){
		tmp=CalcLen(coord,&c[3*i]);
		value+=d[i]*tmp;
	}
	return value;

}
float RBFSolver::GetValueAt(CVector3d* v)
{
	double coord[3];
	coord[0]=v->x();
	coord[1]=v->y();
	coord[2]=v->z();
	return GetValueAt(coord);
}

void RBFSolver::AddRBFCenter(const double *Coord, double value)
{
   ASSERT(cur<k);
   memcpy(&c[3*cur],Coord,3*sizeof(double));
   h[cur] = value;
   ++cur;

}

void RBFSolver::InitSystem(int num)
{

   cur=0;
   k = num;
   c = new double[3*k];
   ZeroMemory(c,3*k*sizeof(double));
   d = new double[k];
   h = new double[k+4];
   h[k]=h[k+1]=h[k+2]=h[k+3]=0.0;
   phi = new double[k*k];
   m_iInitNum=0;

}

void RBFSolver::ResetSystem()
{
	if(k!=0){
		k = 0;
		cur =0;
		delete []c;
		c = NULL;
		delete []d;
		d = NULL;
		p[0]=p[1]=p[2]=p[3]=0.0;
		m_bSolve =false;
		m_iInitNum=0;
	}

}

void RBFSolver::Solve(int istart,float fPara)
{
   ASSERT(cur==k);
   ASSERT(!m_bSolve);

   //Çóphi
   int i,j;
   for(i=0;i<k;++i)
   {
	   if(i<istart)
		   phi[i*k+i] = 0;  
	   else
		   phi[i*k+i] = fPara;  
   }
   for(i=0;i<k;++i)
   {
   	   for(j=0;j<i;++j)
	   {
          double val=CalcLen(&c[3*i],&c[3*j]);
          phi[i*k+j] = val;
	   }
   }
   for(i=0;i<k;++i)
   {
	   for(j=k-1;j>i;--j){
		   phi[i*k+j] = phi [j*k+i];
	   }
   }
   GetRBFCoeff(k,c,phi,h,d,p);

   m_bSolve = true;

}
void RBFSolver::Test(double* coord, double value)
{
	double rel=GetValueAt(coord);
	
}

void RBFSolver::MEMRelease()
{
	if(phi!=NULL)
		delete []phi;
	if(h!=NULL)
		delete []h;
}

CVector3d* RBFSolver::InitialSample(int i)
{
	static CVector3d result;
	result.x(InitalParticle[i][0]);
	result.y(InitalParticle[i][1]);
	result.z(InitalParticle[i][2]);
	return &result;
}

CVector3d* RBFSolver::Grad(CVector3d* v)
{
	static CVector3d result;
	CVector3d tmp;

	tmp.x(v->x()+0.001);
	tmp.y(v->y());
	tmp.z(v->z());
	result.x((GetValueAt(&tmp)-GetValueAt(v))*1000);


	tmp.x(v->x());
	tmp.y(v->y()+0.001);
	tmp.z(v->z());
	result.y((GetValueAt(&tmp)-GetValueAt(v))*1000);

	tmp.x(v->x());
	tmp.y(v->y());
	tmp.z(v->z()+0.001);
	result.z((GetValueAt(&tmp)-GetValueAt(v))*1000);

	return &result;
}

void RBFSolver::SetInitialSample(double x,double y, double z)
{
	InitalParticle[0][0]=x;
	InitalParticle[0][1]=y;
	InitalParticle[0][2]=z;
	m_iInitNum=1;
}
void RBFSolver::AddInitialSample(double x, double y, double z)
{
	if(m_iInitNum<10)
	{
		InitalParticle[m_iInitNum][0]=x;
		InitalParticle[m_iInitNum][1]=y;
		InitalParticle[m_iInitNum][2]=z;
		m_iInitNum++;
	}
}

int RBFSolver::GetInitSampleNum(void)
{
	return m_iInitNum;
}


