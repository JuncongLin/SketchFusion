#include "StdAfx.h"
#include ".\principleanalysis.h"
#include "nr.h"		// jacobi(), eigsrt()

unsigned long* ija;
double* sa;
int NSIZE;
int NP;


void atimes(unsigned long n, double x[], double r[], int itrnsp)
{
	if (itrnsp) dsprstx(sa,ija,x,r,n);
	else dsprsax(sa,ija,x,r,n);
}
void asolve(unsigned long n, double b[], double x[], int itrnsp)
{
	unsigned long i;

	for(i=1;i<=n;i++) x[i]=(sa[i] != 0.0 ? b[i]/sa[i] : b[i]);
}


CPrincipleAnalysis::CPrincipleAnalysis(void)
{
}

CPrincipleAnalysis::~CPrincipleAnalysis(void)
{
}

void CPrincipleAnalysis::Execute(vector<CVector3d>& points,double weight,CFrame* pFrame, double eimag[3])
{
	int i,j,k;
	double tot;

	float* d=fvector(1,3);
	float** v=matrix(1,3,1,3);

	float** mcovar=matrix(1,3,1,3);
	for(i=0;i<3;i++)
	{
		for(j=0;j<=i;j++)
		{
			tot=0;
			for(k=0;k<points.size();k++)
			{
				tot+=points[k][i]*points[k][j];
			}
			tot/=weight;
			mcovar[1+i][1+j]=tot;
			if (j<i) mcovar[1+j][1+i]=tot;

		}
	}

	int nrot;
	jacobi(mcovar,3,d,v,&nrot);
	eigsrt(d,v,3);
	for (i=0;i<3;i++) {
		float a=d[1+i];
		if (a<0) a=0;	// for numerics
		a=sqrt(a);
		eimag[i]=a;
		if (!a) a=1e-15; // very small but non-zero vector
		pFrame->SetAxis(i,(double)v[1][1+i]*a,(double)v[2][1+i]*a,(double)v[3][1+i]*a);
	}
	free_matrix(mcovar,1,3,1,3);
	free_fvector(d,1,3);
	free_matrix(v,1,3,1,3);

}
