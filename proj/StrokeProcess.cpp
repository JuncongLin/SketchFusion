#include "StdAfx.h"
#include ".\strokeprocess.h"
#include <math.h>
CStrokeProcess::CStrokeProcess(void)
{
}

CStrokeProcess::~CStrokeProcess(void)
{
}

static const double EPSILON_LARGE = 1.0E-5 ;
static const double EPSILON_SMALL = 1.0E-7 ;

int FindSpan(double t, int n, int p, double* U)
{
	if( fabs(t-U[n+1])<EPSILON_LARGE ) return n ;

	//Do binary search
	int low = p ;
	int high = n+1 ;
	int mid = (low+high)/2 ;
	while( t< U[mid]|| t >= U[mid+1] )
	{
		if(t<U[mid]) high = mid ;
		else low = mid ;
		mid = (low+high)/2 ;
	}
	return mid ;
}



void BasisFuns(int i, double t, int p, double* U, double* N)
{
	double * left = new double[p+1];
	double * right= new double[p+1];

	N[0] = 1.0 ;
	for( int j=1 ; j <= p ; j++ )
	{
		left[j]  = t - U[i+1-j] ;
		right[j] = U[i+j] - t ;
		double saved = 0.0 ;
		for(int r = 0 ; r < j ; r++ )
		{
			double temp = N[r]/(right[r+1]+left[j-r]) ;
			N[r] = saved + right[r+1]*temp ;
			saved = left[j-r]*temp ;
		}
		N[j] = saved ;
	}
	delete [] left ;
	delete [] right ;

}
void CurvePoint(double t , int n,double * U,CVector3d * P,CVector3d & coord)
{
	int span = FindSpan(t,n+2,3,U) ;

	double * N = new double[4];
	BasisFuns(span,t,3,U,N) ;

	coord.Set(0.0,0.0,0.0);

	for(int i = 0 ; i<=3; i++ )
	{

		coord+= N[i]*P[span-3+i];
	}
	delete [] N ;

}


void CStrokeProcess::BSplineFitting(CStroke* pStroke)
{
	CVector3d Pt1,Pt2,coord;
	double l,totLen=0.0;
	int size=pStroke->GetPointsNum();

	int num=0;
	int i,n;

	CVector3d *Q=new CVector3d[size];

	int a=0;
	int b=a+1;

	Pt1=pStroke->GetPoint(0);
	Q[num++]=Pt1;
	while(b<size)
	{
		Pt1=pStroke->GetPoint(a);
		do 
		{
			Pt2=pStroke->GetPoint(b);
			l=(Pt2-Pt1).Length();
			b++;
		} while(b<size&&l<EPSILON_SMALL);
		Q[num++]=Pt2;
		a = b-1;
	}
	n=num-1;
	double * uk=new double[n+1];
	for(i = 1 ; i<=n ; i++ )
	{
		uk[i]=(Q[i]-Q[i-1]).Length();
		totLen += uk[i] ;
	}
	uk[0] = 0.0  ;
	for( i = 1 ; i < n ; i++ )
		uk[i] = uk[i-1] + uk[i]/totLen ;
	uk[n] = 1.0  ;


	Q[n]=Pt2;
	double *U=new double[n+7];
	for( i = 0 ; i<= 3 ; i++ )
	{
		U[i] = 0.0 ;
	}
	for( i=4 ; i<n+3; i++ )
	{

		U[i] = uk[i-3] ;
	}
	for( i=n+3 ; i<=n+6 ; i++ )
	{
		U[i] = 1.0 ;
	}
	CVector3d *P=new CVector3d[n+3];

	{
		CVector3d*  R = new CVector3d[n+1] ;
		double* dd = new double[n+1]  ;

		P[0]=Q[0];

		P[1] = Q[1] ;
		P[n+1] = Q[n-1] ;
		P[n+2]=Q[n] ;

		double abc[4] ;
		for(i = 3 ; i<n  ; i++ ){
			R[i] = Q[i-1] ;
		}
		BasisFuns(4,U[4],3,U,abc) ;
		double den = abc[1] ;
		P[2]=(Q[1]-abc[0]*P[1])/den;
		for( i=3 ; i<n ; i++)
		{
			dd[i] = abc[2]/den ;
			BasisFuns(i+2,U[i+2],3,U,abc) ;
			den = abc[1] - abc[0]*dd[i] ;
			P[i] = (R[i]-abc[0]*P[i-1])*(1.0/den) ;
		}
		dd[n] = abc[2]/den ;
		BasisFuns(n+2,U[n+2],3,U,abc);
		den=abc[1]-abc[0]*dd[n];
		P[n]=(Q[n-1]-abc[2]*P[n+1]-abc[0]*P[n-1])/den;

		for( i = n-1 ; i>=2 ; i-- )
			P[i] = P[i] - P[i+1] * dd[i+1];

		delete []R ;
		delete [] dd ;
	}

	pStroke->Clear();

	double t=0.0,deltat;
	deltat=1.0/50.0;

	for(i=0;i<50;i++)
	{
		CurvePoint(t,n,U,P,coord);
		pStroke->AddPoint(coord);
		t+=deltat;
	}
}
