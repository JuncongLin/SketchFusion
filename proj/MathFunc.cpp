#include "stdafx.h"
#include "matlib.h"
#include "MathFunc.h"
#include <fstream>
using namespace std;
static const double ZERO=1.0E-10;
void Normalize(double* pVector,int num){
	int i;
	double total=0.0;
	for(i=0;i<num;++i){
		total+=pow(pVector[i],2);
	}
	total=sqrt(total);
    for(i=0;i<num;++i){
		pVector[i]/=total;
	}
}

void getMulti(double* a,int row_a,int col_a,double* b,int row_b,int col_b,double* c){
	Mm A=zeros(row_a,col_a);
	memcpy(A.addr(),a,row_a*col_a*sizeof(double));
	Mm B=zeros(row_b,col_b);
	memcpy(B.addr(),b,row_b*col_b*sizeof(double));
	Mm C=mtimes(A,B);
	memcpy(c,C.addr(),row_a*col_b*sizeof(double));
}

void   GetRotate(double* norm,double* vec,double* t){
	// norm 4*1
	// vec 4*1
	// t   4*4;
	Mm A =zeros(4,1);
	memcpy(A.addr(),norm,4*sizeof(double));
	Mm B =zeros(4,1);
	memcpy(B.addr(),vec,4*sizeof(double));
	Mm C = mrdivide(A,B);
	memcpy(t,C.addr(),4*4*sizeof(double));
}

void getCross(double* x,double* y,double* z){
	Mm X=zeros(3,1);
	memcpy(X.addr(),x,3*1*sizeof(double));
	Mm Y=zeros(3,1);
	memcpy(Y.addr(),y,3*1*sizeof(double));
	Mm Z=cross(X,Y);
	memcpy(z,Z.addr(),3*1*sizeof(double));
    Normalize(z,3);
}

void GetR(double* q1,double* q2,int num,double* R){
	//Qi1,Qi2   3*1矩阵
	//H         3*3矩阵  H=U*S*Vt; 
	//R         3*3
    Mm H=zeros(3,3);
	Mm U,S,V,Ut;
	Mm Qi1=zeros(3,1);
	Mm Qi2=zeros(3,1);
    Mm Qi2t=zeros(1,3);
	//Mm tmp;
	int i;
	for(i=0;i<num;++i){
		Qi1.r(1,1)=q1[i*3];
		Qi1.r(2,1)=q1[i*3+1];
		Qi1.r(3,1)=q1[i*3+2];

		Qi2.r(1,1)=q2[i*3];
		Qi2.r(2,1)=q2[i*3+1];
		Qi2.r(3,1)=q2[i*3+2];

		Qi2t=transpose(Qi2);
		H=H+mtimes(Qi1,Qi2t);
		
	}

	S=svd_S(H);
	U=svd_U(H);
	V=svd_W(H);
	Ut=transpose(U);

	Mm X=mtimes(V,Ut);

	Mm result=det(X);

	double y=result.r(1,1);
	if((y<1+ZERO)&&(y>1-ZERO)){
	   	memcpy(R,X.addr(),3*3*sizeof(double));  
		return;
	}
	else{
		Mm s=svd(H);
		double ss[3];
		memcpy(ss,s.addr(),3*sizeof(double));  
		for(i=1;i<=3;++i){
			if(s.r(i,1)==0.0){
				break;
			}
		}
		if(i>3){
			AfxMessageBox("SVD奇异");
			ASSERT(false);
			exit(-1);
		}
		else{
			V.r(i,1)*=-1;
			V.r(i,2)*=-1;
			V.r(i,3)*=-1;
			X=mtimes(V,Ut);
			memcpy(R,X.addr(),3*3*sizeof(double));  
			return;
		}
	}
}

void GetT(double* p1,double* p2,double* R,double* T){
	Mm Mp1=zeros(3,1);
	memcpy(Mp1.addr(),p1,3*1*sizeof(double));
	Mm Mp2=zeros(3,1);
	memcpy(Mp2.addr(),p2,3*1*sizeof(double));

	Mm MR=zeros(3,3);
	memcpy(MR.addr(),R,3*3*sizeof(double));

	Mm MT=Mp2-mtimes(MR,Mp1);
	memcpy(T,MT.addr(),3*1*sizeof(double));
}

void Trans(double* coord,double* R,double *T){
	Mm MR=zeros(3,3);
	memcpy(MR.addr(),R,3*3*sizeof(double));
	Mm MT=zeros(3,1);
	memcpy(MT.addr(),T,3*1*sizeof(double));
	Mm MP=zeros(3,1);
	memcpy(MP.addr(),coord,3*1*sizeof(double));
	Mm MP2=mtimes(MR,MP)+MT;
	memcpy(coord,MP2.addr(),3*1*sizeof(double));
}

float CalcLen(float* pCoord1,float* pCoord2){
	float delx=pCoord1[0]-pCoord2[0];
	float dely=pCoord1[1]-pCoord2[1];
	float delz=pCoord1[2]-pCoord2[2];
	return sqrtf(delx*delx+dely*dely+delz*delz);
}

double CalcLen(double* pCoord1,double* pCoord2){
	double delx=pCoord1[0]-pCoord2[0];
	double dely=pCoord1[1]-pCoord2[1];
	double delz=pCoord1[2]-pCoord2[2];
	return sqrt(delx*delx+dely*dely+delz*delz);
}

void  getU(double *pLCS,float* pV,float* pCV,double* u,double t){
	Mm V=zeros(3,1);
	V.r(1,1)=pV[0];
	V.r(2,1)=pV[1];
	V.r(3,1)=pV[2];

	Mm CV=zeros(3,1);
	CV.r(1,1)=pCV[0];
	CV.r(2,1)=pCV[1];
	CV.r(3,1)=pCV[2];

	Mm LCS=zeros(3,3);
	memcpy(LCS.addr(),pLCS,3*3*sizeof(double));

	LCS=inv(LCS);

	Mm U=mtimes(LCS,(V-CV));
	U=times(U,t);
	memcpy(u,U.addr(),3*1*sizeof(double));
}

void  getV(double *pLCS,double* u,float* pCV,double* v,double wi)
{
    Mm CV=zeros(3,1);
	CV.r(1,1)=pCV[0];
	CV.r(2,1)=pCV[1];
	CV.r(3,1)=pCV[2];
    
	Mm LCS=zeros(3,3);
	memcpy(LCS.addr(),pLCS,3*3*sizeof(double));
	Mm U=zeros(3,1);
	memcpy(U.addr(),u,3*1*sizeof(double));
	Mm V=zeros(3,1);
	memcpy(V.addr(),v,3*1*sizeof(double));
	Mm tmp=mtimes(LCS,U)+CV;
	tmp=times(tmp,wi);
	V=V+tmp;
    memcpy(v,V.addr(),3*1*sizeof(double));
}

void DisplayMatrix(Mm& X,ofstream& outfile)
{
	for(int i=1;i<=X.rows();++i){
		for(int j=1;j<=X.cols();++j)
			outfile<<X.r(i,j)<<" ";
		outfile<<endl;
	}
}

void  GetRBFCoeff(int num,double* c,double* phi,double* h,double* d,double* p)
{
	Mm CT = zeros(4,num);
	Mm CT0 = zeros(3,num);
	memcpy(CT0.addr(),c,3*num*sizeof(double));
	Mm I = zeros(1,num);
	int i;
	for(i=1;i<=num;++i){
		I.r(1,i) = 1;
	}
	CT = vertcat(I,CT0);
//	ofstream outfile("D:\Solver.txt",ios_base::app);
	//outfile<<"CT.."<<endl;
    //DisplayMatrix(CT,outfile);
	Mm C = transpose(CT);
	//outfile<<"C.."<<endl;
	//DisplayMatrix(C,outfile);
	Mm corner = zeros(4,4);
	Mm PHI = zeros(num,num);
	memcpy(PHI.addr(),phi,num*num*sizeof(double));  //phi是对称的，不需要调整
	Mm A = vertcat(horzcat(PHI,C),horzcat(CT,corner));
	//outfile<<"A"<<endl;
	//DisplayMatrix(A,outfile);
	
	Mm B = zeros(num+4,1);
	memcpy(B.addr(),h,(num+4)*sizeof(double));
	//outfile<<"B.."<<endl;
	//DisplayMatrix(B,outfile);
	Mm X = mldivide(A,B);
	memcpy(d,X.addr(),num*sizeof(double));
	memcpy(p,X.addr(num+1),4*sizeof(double));
	//outfile<<"X.."<<endl;
	//DisplayMatrix(X,outfile);
}

float getProjectDist(float* pt0,float* pt1,float* pt2){
    double x[3];
	x[0] = pt0[0] - pt1[0];
	x[1] = pt0[1] - pt1[1];
	x[2] = pt0[2] - pt1[2];
	double y[3];
    y[0] = pt1[0] - pt2[0];
	y[1] = pt1[1] - pt2[1];
	y[2] = pt1[2] - pt2[2];
	
	Mm X=zeros(3,1);
	memcpy(X.addr(),x,3*1*sizeof(double));
	Mm Y=zeros(3,1);
	memcpy(Y.addr(),y,3*1*sizeof(double));
	Mm Z=cross(X,Y);
	double z[3];
	memcpy(z,Z.addr(),3*1*sizeof(double));
    double area = sqrt(z[0]*z[0]+z[1]*z[1]+z[2]*z[2]);
	double line = sqrt(y[0]*y[0]+y[1]*y[1]+y[2]*y[2]);
	return area/line;
}

float getProjectLen(float* pt0,float* pt1,float* pt2){
    double x[3];
	x[0] = pt0[0] - pt1[0];
	x[1] = pt0[1] - pt1[1];
	x[2] = pt0[2] - pt1[2];
	double y[3];
    y[0] = pt2[0] - pt1[0];
	y[1] = pt2[1] - pt1[1];
	y[2] = pt2[2] - pt1[2];
	/*
	Mm X=zeros(1,3);
	memcpy(X.addr(),x,3*1*sizeof(double));
	Mm Y=zeros(1,3);
	memcpy(Y.addr(),y,3*1*sizeof(double));
	Mm Z=dot(X,Y);
	double z;
	memcpy(&z,Z.addr(),sizeof(double));
	*/
	double z = x[0]*y[0]+x[1]*y[1]+x[2]*y[2];  //点积
    double line = CalcLen(pt1,pt2);
	return z/line;
}

void   AdjusttoProject(float* pt,float* start,float* end){
	float proLen =  getProjectLen(pt,start,end);
	double vec[3];
	//debug
	double vec2[3];

	vec[0] = end[0]-start[0];
	vec[1] = end[1]-start[1];
	vec[2] = end[2]-start[2];
	Normalize(vec,3);
	vec2[0] = proLen*vec[0];
	vec2[1] = proLen*vec[1];
	vec2[2] = proLen*vec[2];

	pt[0] = start[0]+vec2[0];
	pt[1] = start[1]+vec2[1];
	pt[2] = start[2]+vec2[2];
/*	double i,j,k;
	i = vec2[0]/vec[0];
	j = vec2[1]/vec[1];
	k = vec2[2]/vec[2];
	*/
}