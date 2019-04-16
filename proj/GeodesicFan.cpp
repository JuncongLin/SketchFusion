#include "StdAfx.h"
#include ".\geodesicfan.h"
#include "..\OpenGLBasic\HalfEdge.h"
#include "Plane.h"
#include "global.h"
#include <limits>
#include <fstream>
extern ofstream outfile;
using namespace std;
CGeodesicFan::CGeodesicFan(void)
{
	for (int i=0;i<SAMPLES_PER_SPOKE;i++)
	{
		for (int j=0;j<8;j++)
		{
			Mask[i][j]=0;
		}
	}
}

CGeodesicFan::~CGeodesicFan(void)
{

	for (int i=0;i<8;i++)
	{
		for (int j=0;j<intersectP[i].size();j++)
		{
			delete intersectP[i][j];
		}

	}

}

bool CGeodesicFan::Construct(CVertex* pVertex,double FanLength)
{
	Spindles(pVertex);
	if(!Iterate(pVertex,FanLength))return false;
	if(!Sampling(FanLength))return false;
	return true;

}

void CGeodesicFan::Spindles(CVertex* pVertex)
{
	CVector3d v1, v2,norm;
	int ok = 0;
	CHalfEdge *pHe;
	CFace* pFace;
	CVertex* pV;



	pFace=pVertex->GetOutHalfEdge()->GetFace();
	pHe=pFace->GetFirstEdge();
	do
	{
		pV=pHe->GetVertex();
		if(pV!=pVertex)
		{
			if(ok == 0) //if this is the first vertex you found
			{    
				v1=pV->GetCoordinate();
				ok++;
			}
			else if(ok == 1) //if this is the second vertex you found
			{
				v2=pV->GetCoordinate();
				ok++;
			}
		}
		pHe=pFace->GetNextEdge(pHe);
	}while(pHe!=pFace->GetFirstEdge());


	norm=(v1+v2)/2-pVertex->GetCoordinate();


	v1=pVertex->GetNormal();

	v2=norm-v1*(v1.Dot(norm));
	v2.Normalize();

	littleTPoints[0]=v2;
	//End V0

	//V180
	littleTPoints[4]=-1*littleTPoints[0];

	//Begin V90
	v1=pVertex->GetNormal();

	v2=littleTPoints[0];

	norm=v1.Cross(v2);
	norm.Normalize();

	littleTPoints[2] = norm;
	//End V90

	//V270
	littleTPoints[6] = -1 * littleTPoints[2];

	//Begin V45
	norm=(littleTPoints[0]+littleTPoints[2])/2;
	norm.Normalize();
	littleTPoints[1]=norm;
	//End V45

	//V225
	littleTPoints[5]=-1*littleTPoints[1];

	//Begin V135
	norm = ((littleTPoints[4] + littleTPoints[2])/2);
	norm.Normalize();
	littleTPoints[3]=norm;
	//End V135

	//V315
	littleTPoints[7]=-1*littleTPoints[3];


}

bool CGeodesicFan::Iterate(CVertex* pVertex,double FanLength)
{

	int iteration;
	bool tf = false;
	double Length;
	for(int tindex = 0; tindex < 8; tindex++) //for 8 spindles
	{
		iteration = 2;

		tf = planeIntersect(pVertex,tindex,FanLength,Length); //find first intersection point of current spindle

		if(tf == false) 
			return false;

		while((iteration < MAX_ADJ_TRIS) && (Length < FanLength-0.0001) && tf) //find intersections until spindle reaches desired length	
		{
			tf = planeIntersectN(tindex, iteration,FanLength,Length); //find intersection N (specified by variable iteration
			if(tf == false) 
				return false;

			iteration++;                                    //increment iteration
		}

	}



}

bool CGeodesicFan::planeIntersect(CVertex* pVertex,int tindex,double FanLength,double& Length)
{
	CVector3d v1, v2, v3, v4, v5, ans,intersectVec;
	double dotProduct = 0;
	int ok;
	CVector3d p0, p1;
	double t;

	//set first index in intersection variable to center vertex
	CVector3d *pCoord;
	pCoord=new CVector3d;
	*pCoord=pVertex->GetCoordinate();
	intersectP[tindex].push_back(pCoord);

	v1=littleTPoints[tindex];
	intersectN[0][tindex]=v2=pVertex->GetNormal();



	//cross product of little t and normal at vertex equals plane normal
	ans=v1.Cross(v2);

	//A, B, C, and D values for each plane normal

	CPlane plane(pVertex->GetCoordinate(),ans);

	CFace *pF1;
	CHalfEdge *pHe1,*pHe2;
	CVertex* pV;
	pHe1=pVertex->GetOutHalfEdge();
	do
	{
		pF1=pHe1->GetFace();
		AdjacentFace=pF1;
		ok = 0;
		pHe2=pF1->GetFirstEdge();
		do
		{
			pV=pHe2->GetVertex();
			if(pV!=pVertex)
			{
				if(ok == 0) //if this is the first vertex you found
				{            
					p1=pV->GetCoordinate();
					intersectEndpoint1 = pV;
					ok++;
				}
				else if(ok == 1) //if this is the second vertex you found
				{
					p0=pV->GetCoordinate();
					intersectEndpoint2 = pV;
					ok++;
				}
			}
			pHe2=pF1->GetNextEdge(pHe2);
		}while(pHe2!=pF1->GetFirstEdge());

		//vector created by adjacent edge

		v3=p1-p0;

		//little t

		v4=littleTPoints[tindex];

		if((plane.a()*v3[0]+plane.b()*v3[1]+plane.c()*v3[2])!=0)
		{
			t=-1*((plane.a()*p0[0]+plane.b()*p0[1]+plane.c()*p0[2]+plane.d())/(plane.a()*v3[0]+plane.b()*v3[1]+plane.c()*v3[2]));
	
			if(t >= 0 && t <= 1) //if t is between 0 and 1 you have found either the correct intersection or it's opposite
			{
				//intersection point
				v5=p0+v3*t;

				//vector from intersection point
				intersectVec=v5-pVertex->GetCoordinate();

				//update length of current spindle

				double temp=intersectVec.Length();
				Length=temp;
				intersectVec.Normalize();



				//dot product with vector and little t
				dotProduct=intersectVec.Dot(v4);

				if(dotProduct > 0) //another check to make sure intersection point is in right direction 
				{
					pCoord=new CVector3d;
					if(Length>FanLength)
					{
						double r=(Length-FanLength)/temp;
						*pCoord=r*pVertex->GetCoordinate()+(1-r)*v5;
						Length = FanLength;
					}
					else
					{
						*pCoord=v5;
					}
					intersectP[tindex].push_back(pCoord);

					intersectF[tindex].push_back(pF1);
					return true;
				}
			}

		}			
		pHe1=pVertex->NextOutHalfEdge(pHe1);
	}while(pHe1!=pVertex->GetOutHalfEdge());
	return false;
}

bool CGeodesicFan::planeIntersectN(int tindex, int iteration, double FanLength,double& Length)
{
	CVector3d n0, n1, p0, p1;
	CVector3d v0, v1, v2, v3, v4, v5, ans;
	CVector3d adj0, adj1;
	double dotProductTerm;
	CVector3d InterNorm;
	CVector3d intersectVec;
	CVertex * lineVertex1;
	CVertex * lineVertex2;
	CVertex * oppVertex;
	CVertex * pVertex;
	int ok = 0;
	double t, dotProduct;
	double planeEq[4];

	//first endpoint of most recent triangle edge intersected by spindle
	lineVertex1 = intersectEndpoint1;

	//first endpoint of intersection edge			
	p1=lineVertex1->GetCoordinate();
	//normal of endpoint

	n1=lineVertex1->GetNormal();

	//second endpoint of most recent triangle edge intersected by spindle
	lineVertex2 = intersectEndpoint2;

	//second endpoint of intersection edge
	p0=lineVertex2->GetCoordinate();
	//normal of first endpoint
	n0=lineVertex2->GetNormal();

	v0=p1-p0;

	//vector from intersection point to end point
	v1=intersectP[tindex][iteration-1]-p0;

	//end vector dot product divided by start vector dot product
	dotProductTerm = v1.Length()/v0.Length();

	//normal values for intersection point
	intersectN[iteration-1][tindex]=InterNorm=(dotProductTerm * n1) + ( (1 - dotProductTerm) * n0);
	InterNorm.Normalize();


	CHalfEdge *pHe;
	CFace* pFace;
	pHe=lineVertex1->GetOutHalfEdge(lineVertex2);
	if(pHe->GetFace()==AdjacentFace)
		pHe=pHe->GetMate();
	pFace=AdjacentFace=pHe->GetFace();
	oppVertex=pHe->GetPrevEdge()->GetVertex();
	pHe=pFace->GetFirstEdge();
	do
	{
		pVertex=pHe->GetVertex();
		if(pVertex!=oppVertex)
		{
			v2=*(intersectP[tindex][iteration-1])-*(intersectP[tindex][iteration-2]);

			//intersection point normal
			v3=InterNorm;

			ans=v2.Cross(v3);

			//A, B, C, and D values for each plane normal

			CPlane plane(*(intersectP[tindex][iteration-1]),ans);

			//first endpoint of adjacent edge
			adj0=pVertex->GetCoordinate();


			//second endpoint of adjacent edge
			adj1=oppVertex->GetCoordinate();


			//vector created by adjacent edge

			v4=adj1-adj0;

            if((plane.a()*v4[0]+plane.b()*v4[1]+plane.c()*v4[2])!=0)
			{
				//solve for t
				t=-1*((plane.a()*adj0[0]+plane.b()*adj0[1]+plane.c()*adj0[2]+plane.d())/(plane.a()*v4[0]+plane.b()*v4[1]+plane.c()*v4[2]));

				if(t >= 0 && t <= 1.00001)
				{
					if(t>1)t=0.99999;

					//intersection point
					v5[0] = (adj0[0] + (v4[0] * t));
					v5[1] = (adj0[1] + (v4[1] * t));
					v5[2] = (adj0[2] + (v4[2] * t));


					//vector from intersection point
					intersectVec=v5-intersectP[tindex][iteration-1];


					//update length of spindle
					double temp=intersectVec.Length();
					Length += temp;
					CVector3d *pCoord=new CVector3d;
					if(Length>FanLength)
					{
						double r=(Length-FanLength)/temp;
						*pCoord=r*(*intersectP[tindex][iteration-1])+(1-r)*v5;
						intersectF[tindex].push_back(pFace);
						intersectF[tindex].push_back(pFace);
						Length = FanLength ;

					}
					else
					{
						*pCoord=v5;
						intersectF[tindex].push_back(pFace);

						intersectEndpoint1 = pVertex;
						intersectEndpoint2 = oppVertex;

					}
					intersectP[tindex].push_back(pCoord);

					//set values for intersection point just found
					return true;

				}
			}

		}
		pHe=pFace->GetNextEdge(pHe);
	}while(pHe!=pFace->GetFirstEdge());
	return false;

}

double CGeodesicFan::CompareScalar(CGeodesicFan* pGeoFan)
{
	return 0;
}

double CGeodesicFan::CompareVector(CGeodesicFan* pGeoFan,const vector<CVector3d*>& vMetrics)
{

	int spin,start,i,j,k,m,n,cmask;
    double SampleMatrix[9],CompareMatrix[9],ComposeMatrix[9];
	double diff,mindiff=DBL_MAX,temp,area,a1,a2,a3;

	CVector3d CSampleLC,SampleMetric[SAMPLES_PER_SPOKE][8],CompMetric,tempMetric,Metric0,Metric1,Metric2;

	CFace *pFace;
	CHalfEdge *pHe;
	CVertex *pV;

	TransformMatrix(SampleMatrix,0);

	//outfile<<"TransformMatrix: "<<endl;
	//for (i=0;i<3;i++)
	//{
	//	for (j=0;j<3;j++)
	//	{
	//		outfile<<SampleMatrix[i*3+j]<<" ";
	//	}
	//	outfile<<endl;
	//}

	cmask=0;
	for(start=0;start<8;start++)
	{

		for (i=1;i<SAMPLES_PER_SPOKE+1;i++)
		{

            if(Mask[i-1][start])
			{
				pFace=SampleF[i-1][start];

				pHe=pFace->GetFirstEdge();
				pV=pHe->GetVertex();
				Metric0=*(vMetrics[pV->GetIndex()]);
				//outfile<<"metric0: <"<<Metric0[0]<<","<<Metric0[1]<<","<<Metric0[2]<<">"<<endl;

				pHe=pFace->GetNextEdge(pHe);
				pV=pHe->GetVertex();
				Metric1=*(vMetrics[pV->GetIndex()]);
				//outfile<<"metric1: <"<<Metric1[0]<<","<<Metric1[1]<<","<<Metric1[2]<<">"<<endl;

				pHe=pFace->GetNextEdge(pHe);
				pV=pHe->GetVertex();

				Metric2=*(vMetrics[pV->GetIndex()]);
				//outfile<<"metric2: <"<<Metric2[0]<<","<<Metric2[1]<<","<<Metric2[2]<<">"<<endl;
				//outfile<<"lc: "<<SampleLC[i-1][start][0]<<" "<<SampleLC[i-1][start][1]<<" "<<SampleLC[i-1][start][2]<<endl;

				SampleMetric[i-1][start]=SampleLC[i-1][start][0]*Metric0+SampleLC[i-1][start][1]*Metric1+SampleLC[i-1][start][2]*Metric2;
			}
			cmask+=Mask[i-1][start];
		}

	}

	//outfile<<"cmask: "<<cmask<<endl;

	for(spin=0;spin<8;spin++)
	{
		pGeoFan->TransformMatrix(CompareMatrix,spin);

		for (i=0;i<3;i++)
		{
			for (j=i+1;j<3;j++)
			{
				temp=CompareMatrix[i*3+j];
				CompareMatrix[i*3+j]=CompareMatrix[j*3+i];
				CompareMatrix[j*3+i]=temp;
			}
		}

		//outfile<<"Compose Matrix:"<<endl;
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				ComposeMatrix[i*3+j]=0.0;
				for(k=0;k<3;k++)
				{
					ComposeMatrix[i*3+j]+=SampleMatrix[i*3+k]*CompareMatrix[k*3+j];
				}
				//outfile<<ComposeMatrix[i*3+j]<<" ";
			}
			//outfile<<"endl"<<endl;
		}

		diff=0.0;

	

		for(start=0;start<8;start++)
		{

			for (i=1;i<SAMPLES_PER_SPOKE+1;i++)
			{

				CSampleLC=pGeoFan->GetSampleLC(i-1,(spin+start)%8);

				pFace=pGeoFan->GetSampleF(i-1,(spin+start)%8);


				pHe=pFace->GetFirstEdge();
				pV=pHe->GetVertex();
				Metric0=*(vMetrics[pV->GetIndex()]);
				//outfile<<"metric0: <"<<Metric0[0]<<","<<Metric0[1]<<","<<Metric0[2]<<">"<<endl;

				pHe=pFace->GetNextEdge(pHe);
				pV=pHe->GetVertex();
				Metric1=*(vMetrics[pV->GetIndex()]);
				//outfile<<"metric1: <"<<Metric1[0]<<","<<Metric1[1]<<","<<Metric1[2]<<">"<<endl;

				pHe=pFace->GetNextEdge(pHe);
				pV=pHe->GetVertex();

				Metric2=*(vMetrics[pV->GetIndex()]);
				//outfile<<"metric2: <"<<Metric2[0]<<","<<Metric2[1]<<","<<Metric2[2]<<">"<<endl;

				//outfile<<"CSampleLC: "<<CSampleLC[0]<<","<<CSampleLC[1]<<","<<CSampleLC[2]<<endl;
				tempMetric=CSampleLC[0]*Metric0+CSampleLC[1]*Metric1+CSampleLC[2]*Metric2;

				for(m=0;m<3;m++)
				{
					CompMetric[m]=0.0;
					for(n=0;n<3;n++)
					{
						CompMetric[m]+=ComposeMatrix[m*3+n]*tempMetric[n];
					}
				}
				//outfile<<"CompMetric: "<<CompMetric[0]<<","<<CompMetric[1]<<","<<CompMetric[2]<<endl;
				//outfile<<"SampleMetric: "<<SampleMetric[i-1][start][0]<<","<<SampleMetric[i-1][start][1]<<","<<SampleMetric[i-1][start][2]<<endl;

				if(Mask[i-1][start])
                    diff+=(CompMetric-SampleMetric[i-1][start]).Length();
			}
		}

		//outfile<<"cmask="<<cmask<<endl;
		diff/=cmask;

		if(diff<mindiff)
			mindiff=diff;
	}

	return mindiff;

}

void CGeodesicFan::TransformMatrix(double *matrix,int spin)
{
	CVector3d DirX,DirY,DirZ;

	int i;

	DirX=littleTPoints[spin];
	DirY=littleTPoints[(spin+2)%8];
	DirZ=DirX.Cross(DirY);

	for(i=0;i<3;i++)
	{
		matrix[i*3]=DirX[i];
		matrix[i*3+1]=DirY[i];
		matrix[i*3+2]=DirZ[i];
	}

}

bool CGeodesicFan::Sampling(double FanLen)
{
	int iteration = 1;
	CVector3d intersectVector,sampleCoord,v0,v1,v2,u0,u1;
	double t;
	double len=0.0, sample_len = 0.0, incr_len = 0.0,area,a0,a1,a2;

	CHalfEdge *pHe;
	CVertex *pV;
	int i,j;

	incr_len = FanLen/(double)SAMPLES_PER_SPOKE;
	for(i = 0; i < 8; i++)                              //and spindle
	{
		iteration  = 1;   //intitialize spindle section to 1

		len = 0;          //intialize spindle length to 0

		sample_len = 0.0;
		//for(int y = 1; y < 4; y++)//this determines the length of the sampling
		for(j = 1; j < SAMPLES_PER_SPOKE; j++)//this determines the length of the sampling
		{
			// Check to see if we are beyond the size of the spoke (spoke could be at an edge)
			if (iteration > intersectP[i].size()-1) {
				return false;
			} 

			// increment sample length
			sample_len += incr_len;

			intersectVector=*(intersectP[i][iteration])-*(intersectP[i][iteration-1]);


			//check to see that y is less than length of current vector
			//while(((double)y/100.0) > (len + (sqrt(dot(intersectVector, intersectVector)))))
			while(sample_len > (len + intersectVector.Length()))
			{
				//add length of vector before going to next one
				len += intersectVector.Length();

				iteration++;  //increment spindle section
				if (iteration > intersectP[i].size()-1) break;

				intersectVector=*(intersectP[i][iteration])-*(intersectP[i][iteration-1]);
			}
			// Check to see if we are beyond the size of the spoke (spoke could be at an edge)
			if (iteration > intersectP[i].size()-1) {
				return false;
			} 

			t = (sample_len - len)/(intersectVector.Length());

			sampleCoord=intersectP[i][iteration-1]+t*intersectVector;

			SampleF[j-1][i] = intersectF[i][iteration-1];

			if(SampleF[j-1][i]->IsKnown())
				Mask[j-1][i]=1;

			pHe=SampleF[j-1][i]->GetFirstEdge();
			pV=pHe->GetVertex();
			v0=pV->GetCoordinate();

			pHe=SampleF[j-1][i]->GetNextEdge(pHe);
			pV=pHe->GetVertex();
			v1=pV->GetCoordinate();

			pHe=SampleF[j-1][i]->GetNextEdge(pHe);
			pV=pHe->GetVertex();
			v2=pV->GetCoordinate();

			u0=v1-v0;

			u1=v2-v0;

			area=(u0.Cross(u1)).Length();

			u0=v1-sampleCoord;

			u1=v2-sampleCoord;

			a0=(u0.Cross(u1)).Length()/area;

			u0=v0-sampleCoord;

			u1=v2-sampleCoord;

			a1=(u0.Cross(u1)).Length()/area;


			u0=v0-sampleCoord;

			u1=v1-sampleCoord;

			a2=(u0.Cross(u1)).Length()/area;

			SampleLC[j-1][i].Set(a0,a1,a2);	
		}
		sampleCoord=(intersectP[i].back());
		SampleF[j-1][i] = (intersectF[i].back());

		pHe=SampleF[j-1][i]->GetFirstEdge();
		pV=pHe->GetVertex();
		v0=pV->GetCoordinate();

		pHe=SampleF[j-1][i]->GetNextEdge(pHe);
		pV=pHe->GetVertex();
		v1=pV->GetCoordinate();

		pHe=SampleF[j-1][i]->GetNextEdge(pHe);
		pV=pHe->GetVertex();
		v2=pV->GetCoordinate();

		u0=v1-v0;

		u1=v2-v0;

		area=(u0.Cross(u1)).Length();

		u0=v1-sampleCoord;

		u1=v2-sampleCoord;

		a0=(u0.Cross(u1)).Length()/area;

		u0=v0-sampleCoord;

		u1=v2-sampleCoord;

		a1=(u0.Cross(u1)).Length()/area;


		u0=v0-sampleCoord;

		u1=v1-sampleCoord;

		a2=(u0.Cross(u1)).Length()/area;

		SampleLC[j-1][i].Set(a0,a1,a2);	

	}


}

CVector3d CGeodesicFan::GetSampleLC(int layer, int spin)
{
	return SampleLC[layer][spin];
}

CFace* CGeodesicFan::GetSampleF(int layer, int spin)
{
	return SampleF[layer][spin];
}

void CGeodesicFan::Display(void)
{
	int i,j;
	CVector3d coord1,coord2,u,v,w;
	double len;

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glDepthRange(0.00003,1.0);

	for(i=0;i<8;i++)
	{
		for (j=0;j<intersectP[i].size()-1;j++)
		{
			coord1=intersectP[i][j];
			coord2=intersectP[i][j+1];

			w=coord2-coord1;
			len=w.Length();
			w.Normalize();
			u=Ortho3(w);
			u.Normalize();
			v=w.Cross(u);
			v.Normalize();
		
			glPushMatrix ();
			glTranslatef(coord2[0],coord2[1],coord2[2]);
			gluSphere(gluNewQuadric(),0.003,50,50);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(coord1[0],coord1[1],coord1[2]);

			float mx[]={
				u[0],u[1],u[2],0,
				v[0],v[1],v[2],0,
				w[0],w[1],w[2],0,
					0,0,0,1
			};
			glMultMatrixf(mx);
			gluCylinder(gluNewQuadric(),0.003,0.003,len,50,50);
			glPopMatrix();

		}
	}

	glDepthRange(0.0,1.0);

}

int CGeodesicFan::NumOfKnownSample(void)
{
	int kmask=0;
	for (int i=0;i<SAMPLES_PER_SPOKE;i++)
	{
		for (int j=0;j<8;j++)
		{
			kmask+=Mask[i][j];

		}
	}
	return kmask;
}
