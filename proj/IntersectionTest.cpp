#include "StdAfx.h"
#include ".\intersectiontest.h"

#include <math.h>
#define FABS(a) ((a>=0)?a:-1*a)
#include "global.h"
#include <fstream>
using namespace std;
extern ofstream outfile;
CIntersectionTest::CIntersectionTest(void)
{
}

CIntersectionTest::~CIntersectionTest(void)
{
}

bool CIntersectionTest::Box_LineSegment(CBox3 box,const CPoint3& ls,const CPoint3& le,CVector3d org,double dim)
{
	long LATTICE_RESOLUTION=(long)pow((int)2,(int)20);
	double unitLen=dim/LATTICE_RESOLUTION;
	double temp;


	CVector3d vmin=box.GetMinCorner();
	CVector3d vmax=box.GetMaxCorner();
	CVector3d vcob=(vmin+vmax)/2;
	long long int cob[3],lcnt[3],ldiw[3],fabw[3],HalfSize[3];

	temp=(vcob[0]-org[0])/unitLen;
	cob[0]=(long long int)temp;
	temp=(vcob[1]-org[1])/unitLen;
	cob[1]=(long long int)temp;
	temp=(vcob[2]-org[2])/unitLen;
	cob[2]=(long long int)temp;

	CVector3d vls(ls[0],ls[1],ls[2]);
	CVector3d vle(le[0],le[1],le[2]);
	CVector3d vlcnt=(vls+vle)/2;
	temp=(vlcnt[0]-org[0])/unitLen;
	lcnt[0]=(long long int)temp;
	temp=(vlcnt[1]-org[1])/unitLen;
	lcnt[1]=(long long int)temp;
	temp=(vlcnt[2]-org[2])/unitLen;
	lcnt[2]=(long long int)temp;
	lcnt[0]-=cob[0];
	lcnt[1]-=cob[1];
	lcnt[2]-=cob[2];
	//outfile<<"LCNT: <"<<lcnt[0]<<","<<lcnt[1]<<","<<lcnt[2]<<">"<<endl;

	CVector3d vldiw=(vle-vls)/2;
	temp=(vldiw[0])/unitLen;
	ldiw[0]=(long long int)temp+1;
	temp=(vldiw[1])/unitLen;
	ldiw[1]=(long long int)temp+1;
	temp=(vldiw[2])/unitLen;
	ldiw[2]=(long long int)temp+1;
	//outfile<<"LDIW: <"<<ldiw[0]<<","<<ldiw[1]<<","<<ldiw[2]<<">"<<endl;

	//fabw[0]=fabs((long double)ldiw[0]);
	//fabw[1]=fabs((long double)ldiw[1]);
	//fabw[2]=fabs((long double)ldiw[2]);

	fabw[0]=FABS(ldiw[0]);
	fabw[1]=FABS(ldiw[1]);
	fabw[2]=FABS(ldiw[2]);
	//outfile<<"FABW: <"<<fabw[0]<<","<<fabw[1]<<","<<fabw[2]<<">"<<endl;

	temp=box.XLength()/(2*unitLen);
	HalfSize[0]=(long long int)temp+1;
	temp=box.YLength()/(2*unitLen);
	HalfSize[1]=(long long int)temp+1;
	temp=box.ZLength()/(2*unitLen);
	HalfSize[2]=(long long int)temp+1;
	//outfile<<"HALFSIZE: <"<<HalfSize[0]<<","<<HalfSize[1]<<","<<HalfSize[2]<<">"<<endl;


	//outfile<<"Test 1"<<endl;
	//outfile<<lcnt[0]<<" Left ="<<fabs((long double)lcnt[0])<<"; Right="<<(fabw[0]+HalfSize[0])<<endl;
	//if(fabs((long double)lcnt[0])>(fabw[0]+HalfSize[0]))return false;
	//outfile<<"Test 2"<<endl;
	//outfile<<lcnt[1]<<" Left ="<<fabs((long double)lcnt[1])<<"; Right="<<(fabw[1]+HalfSize[1])<<endl;
	//if(fabs((long double)lcnt[1])>(fabw[1]+HalfSize[1]))return false;
	//outfile<<"Test 3"<<endl;
	//outfile<<lcnt[2]<<" Left ="<<fabs((long double)lcnt[2])<<"; Right="<<(fabw[2]+HalfSize[2])<<endl;
	//if(fabs((long double)lcnt[2])>(fabw[2]+HalfSize[2]))return false;

	//outfile<<"Test 4"<<endl;
	//outfile<<lcnt[1]*ldiw[2]-lcnt[2]*ldiw[1]<<" Left ="<<fabs((long double)(lcnt[1]*ldiw[2]-lcnt[2]*ldiw[1]))<<"; Right="<<(HalfSize[1]*fabw[2]+HalfSize[2]*fabw[1])<<endl;
	//if(fabs((long double)(lcnt[1]*ldiw[2]-lcnt[2]*ldiw[1]))>(HalfSize[1]*fabw[2]+HalfSize[2]*fabw[1]))return false;
	//outfile<<"Test 5"<<endl;
	//outfile<<lcnt[0]*ldiw[2]-lcnt[2]*ldiw[0]<<" Left ="<<fabs((long double)(lcnt[0]*ldiw[2]-lcnt[2]*ldiw[0]))<<"; Right="<<(HalfSize[0]*fabw[2]+HalfSize[2]*fabw[0])<<endl;
	//if(fabs((long double)(lcnt[0]*ldiw[2]-lcnt[2]*ldiw[0]))>(HalfSize[0]*fabw[2]+HalfSize[2]*fabw[0]))return false;
	//outfile<<"Test 6"<<endl;
	//outfile<<lcnt[0]*ldiw[1]-lcnt[1]*ldiw[0]<<" Left ="<<fabs((long double)(lcnt[0]*ldiw[1]-lcnt[1]*ldiw[0]))<<"; Right="<<(HalfSize[0]*fabw[1]+HalfSize[1]*fabw[0])<<endl;
	//if(fabs((long double)(lcnt[0]*ldiw[1]-lcnt[1]*ldiw[0]))>(HalfSize[0]*fabw[1]+HalfSize[1]*fabw[0]))return false;


	//outfile<<"Test 1"<<endl;
	//outfile<<lcnt[0]<<" Left ="<<FABS(lcnt[0])<<"; Right="<<(fabw[0]+HalfSize[0])<<endl;
	if(FABS(lcnt[0])>(fabw[0]+HalfSize[0]))return false;
	//outfile<<"Test 2"<<endl;
	//outfile<<lcnt[1]<<" Left ="<<FABS(lcnt[1])<<"; Right="<<(fabw[1]+HalfSize[1])<<endl;
	if(FABS(lcnt[1])>(fabw[1]+HalfSize[1]))return false;
	//outfile<<"Test 3"<<endl;
	//outfile<<lcnt[2]<<" Left ="<<FABS(lcnt[2])<<"; Right="<<(fabw[2]+HalfSize[2])<<endl;
	if(FABS(lcnt[2])>(fabw[2]+HalfSize[2]))return false;

	//outfile<<"Test 4"<<endl;
	//outfile<<lcnt[1]*ldiw[2]-lcnt[2]*ldiw[1]<<" Left ="<<FABS((lcnt[1]*ldiw[2]-lcnt[2]*ldiw[1]))<<"; Right="<<(HalfSize[1]*fabw[2]+HalfSize[2]*fabw[1])<<endl;
	if(FABS((lcnt[1]*ldiw[2]-lcnt[2]*ldiw[1]))>(HalfSize[1]*fabw[2]+HalfSize[2]*fabw[1]))return false;
	//outfile<<"Test 5"<<endl;
	//outfile<<lcnt[0]*ldiw[2]-lcnt[2]*ldiw[0]<<" Left ="<<FABS((lcnt[0]*ldiw[2]-lcnt[2]*ldiw[0]))<<"; Right="<<(HalfSize[0]*fabw[2]+HalfSize[2]*fabw[0])<<endl;
	if(FABS((lcnt[0]*ldiw[2]-lcnt[2]*ldiw[0]))>(HalfSize[0]*fabw[2]+HalfSize[2]*fabw[0]))return false;
	//outfile<<"Test 6"<<endl;
	//outfile<<lcnt[0]*ldiw[1]-lcnt[1]*ldiw[0]<<" Left ="<<FABS((lcnt[0]*ldiw[1]-lcnt[1]*ldiw[0]))<<"; Right="<<(HalfSize[0]*fabw[1]+HalfSize[1]*fabw[0])<<endl;
	if(FABS((lcnt[0]*ldiw[1]-lcnt[1]*ldiw[0]))>(HalfSize[0]*fabw[1]+HalfSize[1]*fabw[0]))return false;

	return true;
}

bool CIntersectionTest::Box_Triangle(CBox3& box,  CTriangle3& tri,CVector3d org,double dim)
{
	long LATTICE_RESOLUTION=(long)pow((int)2,(int)20);
	double unitLen=dim/LATTICE_RESOLUTION;
	double temp;

	long long int cob[3],v0[3],v1[3],v2[3],HalfSize[3],f0[3],f1[3],f2[3],ff[3],normal[3],cmin[3],cmax[3];
	long long int  p0,p1,p2,rad,min,max,d;

	CVector3d vmin=box.GetMinCorner();
	CVector3d vmax=box.GetMaxCorner();
	CVector3d vcob=(vmin+vmax)/2;

	temp=(vcob[0]-org[0])/unitLen;
	cob[0]=(long long int)temp;
	temp=(vcob[1]-org[1])/unitLen;
	cob[1]=(long long int)temp;
	temp=(vcob[2]-org[2])/unitLen;
	cob[2]=(long long int)temp;

	temp=(tri[0][0]-org[0])/unitLen;
	v0[0]=(long long int)temp;
	temp=(tri[0][1]-org[1])/unitLen;
	v0[1]=(long long int)temp;
	temp=(tri[0][2]-org[2])/unitLen;
	v0[2]=(long long int)temp;
	v0[0]-=cob[0];
	v0[1]-=cob[1];
	v0[2]-=cob[2];

	temp=(tri[1][0]-org[0])/unitLen;
	v1[0]=(long long int)temp;
	temp=(tri[1][1]-org[1])/unitLen;
	v1[1]=(long long int)temp;
	temp=(tri[1][2]-org[2])/unitLen;
	v1[2]=(long long int)temp;
	v1[0]-=cob[0];
	v1[1]-=cob[1];
	v1[2]-=cob[2];

	temp=(tri[2][0]-org[0])/unitLen;
	v2[0]=(long long int)temp;
	temp=(tri[2][1]-org[1])/unitLen;
	v2[1]=(long long int)temp;
	temp=(tri[2][2]-org[2])/unitLen;
	v2[2]=(long long int)temp;
	v2[0]-=cob[0];
	v2[1]-=cob[1];
	v2[2]-=cob[2];

	f0[0]=v1[0]-v0[0];
	f0[1]=v1[1]-v0[1];
	f0[2]=v1[2]-v0[2];

	f1[0]=v2[0]-v1[0];
	f1[1]=v2[1]-v1[1];
	f1[2]=v2[2]-v1[2];

	f2[0]=v0[0]-v2[0];
	f2[1]=v0[1]-v2[1];
	f2[2]=v0[2]-v2[2];

	temp=box.XLength()/(2*unitLen);
	HalfSize[0]=(long long int)temp+50;
	temp=box.YLength()/(2*unitLen);
	HalfSize[1]=(long long int)temp+50;
	temp=box.ZLength()/(2*unitLen);
	HalfSize[2]=(long long int)temp+50;



	//9 Test

	ff[0]=FABS(f0[0]);
	ff[1]=FABS(f0[1]);
	ff[2]=FABS(f0[2]);

	//9.1
	p0=f0[2]*v0[1]-f0[1]*v0[2];
	p2=f0[2]*v2[1]-f0[1]*v2[2];
    
	if(p0<p2) 
	{
		min=p0; max=p2;
	} 
	else 
	{
		min=p2; max=p0;
	} 

	rad = ff[2] * HalfSize[1] + ff[1]*HalfSize[2];   

	if(min>rad || max<-rad) return false;

	//9.2
	p0 = -f0[2]*v0[0] + f0[0]*v0[2];                   
	p2 = -f0[2]*v2[0] + f0[0]*v2[2];                      
	if(p0<p2) 
	{
		min=p0; max=p2;
	} else 
	{
		min=p2; max=p0;
	} 

	rad = ff[2] * HalfSize[0] + ff[0] * HalfSize[2];   
	if(min>rad || max<-rad) return false;

	//9.3
	p1 = f0[1]*v1[0] - f0[0]*v1[1];                    
	p2 = f0[1]*v2[0] - f0[0]*v2[1];                    
	if(p2<p1) 
	{
		min=p2; max=p1;
	} else 
	{
		min=p1; max=p2;
	} 
	rad = ff[1] * HalfSize[0] + ff[0] * HalfSize[1];   
	if(min>rad || max<-rad) return 0;


	ff[0]=FABS(f1[0]);
	ff[1]=FABS(f1[1]);
	ff[2]=FABS(f1[2]);

	//9.4
	p0=f1[2]*v0[1]-f1[1]*v0[2];
	p2=f1[2]*v2[1]-f1[1]*v2[2];

	if(p0<p2) 
	{
		min=p0; max=p2;
	} 
	else 
	{
		min=p2; max=p0;
	} 

	rad = ff[2] * HalfSize[1] + ff[1]*HalfSize[2];   

	if(min>rad || max<-rad) return false;

	//9.5
	p0 = -f1[2]*v0[0] + f1[0]*v0[2];                   
	p2 = -f1[2]*v2[0] + f1[0]*v2[2];                      
	if(p0<p2) 
	{
		min=p0; max=p2;
	} else 
	{
		min=p2; max=p0;
	} 

	rad = ff[2] * HalfSize[0] + ff[0] * HalfSize[2];   
	if(min>rad || max<-rad) return false;

	//9.6
	p0 = f1[1]*v0[0] - f1[0]*v0[1];                
	p1 = f1[1]*v1[0] - f1[0]*v1[1];                    
	if(p0<p1) 
	{
		min=p0; max=p1;
	} else 
	{
		min=p1; max=p0;
	} 
	rad = ff[1] * HalfSize[0] + ff[0] * HalfSize[1];   
	if(min>rad || max<-rad) return false;

	ff[0]=FABS(f2[0]);
	ff[1]=FABS(f2[1]);
	ff[2]=FABS(f2[2]);
	//9.7
	p0=f2[2]*v0[1]-f2[1]*v0[2];
	p2=f2[2]*v1[1]-f2[1]*v1[2];

	if(p0<p2) 
	{
		min=p0; max=p2;
	} 
	else 
	{
		min=p2; max=p0;
	} 

	rad = ff[2] * HalfSize[1] + ff[1]*HalfSize[2];   

	if(min>rad || max<-rad) return false;

	//9.8
	p0 = -f2[2]*v0[0] + f2[0]*v0[2];                   
	p2 = -f2[2]*v1[0] + f2[0]*v1[2];                      
	if(p0<p2) 
	{
		min=p0; max=p2;
	} else 
	{
		min=p2; max=p0;
	} 

	rad = ff[2] * HalfSize[0] + ff[0] * HalfSize[2];   
	if(min>rad || max<-rad) return false;

	//9.9
	p0 = f2[1]*v1[0] - f2[0]*v1[1];                
	p1 = f2[1]*v2[0] - f2[0]*v2[1];                    
	if(p0<p1) 
	{
		min=p0; max=p1;
	} else 
	{
		min=p1; max=p0;
	} 
	rad = ff[1] * HalfSize[0] + ff[0] * HalfSize[1];   
	if(min>rad || max<-rad) return false;




	//3 Test

	//3.1
	min = max = v0[0];   
	if(v1[0]<min) min=v1[0];
	if(v1[0]>max) max=v1[0];
	if(v2[0]<min) min=v2[0];
	if(v2[0]>max) max=v2[0];
	if(min>HalfSize[0] || max<-HalfSize[0]) return false;

	//3.2
	min = max = v0[1];   
	if(v1[1]<min) min=v1[1];
	if(v1[1]>max) max=v1[1];
	if(v2[1]<min) min=v2[1];
	if(v2[1]>max) max=v2[1];
	if(min>HalfSize[1]|| max<-HalfSize[1]) return false;

	//3.3
	min = max = v0[2];   
	if(v1[2]<min) min=v1[2];
	if(v1[2]>max) max=v1[2];
	if(v2[2]<min) min=v2[2];
	if(v2[2]>max) max=v2[2];
	if(min>HalfSize[2] || max<-HalfSize[2]) return false;

	//1 Test

	normal[0]=f0[1]*f1[2]-f0[2]*f1[1];
	normal[1]=f0[2]*f1[0]-f0[0]*f1[2];
	normal[2]=f0[0]*f1[1]-f0[1]*f1[0];

	d=-(normal[0]*v0[0]+normal[1]*v0[1]+normal[2]*v0[2]);

	for(int q=0;q<=2;q++)
	{
		if(normal[q]>0)
		{
			cmin[q]=-HalfSize[q];
			cmax[q]=HalfSize[q];
		}
		else
		{
			cmin[q]=HalfSize[q];
			cmax[q]=-HalfSize[q];
		}
	}
	if((normal[0]*cmin[0]+normal[1]*cmin[1]+normal[2]*cmin[2]+d)>0) return false;
	if((normal[0]*cmax[0]+normal[1]*cmax[1]+normal[2]*cmax[2]+d)>0) return true;

	return true;
}

bool CIntersectionTest::Box_Plane(CBox3 box, CPlane& plane,CVector3d org,double dim)
{
	int q;

    CVector3d normal(plane.a(),plane.b(),plane.c());
	CVector3d BoxHalfSize;
	BoxHalfSize.Set(box.XLength()/2,box.YLength()/2,box.ZLength()/2);

	CVector3d vmin,vmax;
	for(q=0;q<=2;q++)
	{
		if(normal[q]>0)
		{
			vmin[q]=-BoxHalfSize[q];
			vmax[q]=BoxHalfSize[q];
		}
		else
		{
			vmin[q]=BoxHalfSize[q];
			vmax[q]=-BoxHalfSize[q];
		}
	}
	if(normal.Dot(vmin)+plane.d()>0) return false;
	if(normal.Dot(vmax)+plane.d()>=0) return true;

	return false;
}
