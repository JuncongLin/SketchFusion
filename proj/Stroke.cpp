#include "StdAfx.h"
#include ".\stroke.h"
#include <math.h>
static const double EPSILON_LARGE = 1.0E-5 ;
static const double EPSILON_SMALL = 1.0E-7 ;

CStroke::CStroke(void)
: m_iDisplayStyle(0)
, m_bSelected(false)
{
	m_fLineColor[0]=1.0;
	m_fLineColor[1]=0.0;
	m_fLineColor[2]=0.0;
	m_fPointColor[0]=1.0;
	m_fPointColor[1]=0.0;
	m_fPointColor[2]=0.0;
	m_Material.SetProperty("Green");
}
CStroke::CStroke(int num)
{
	m_iDisplayStyle=0;
	m_vecPoints.resize(num);
	m_fLineColor[0]=1.0;
	m_fLineColor[1]=0.0;
	m_fLineColor[2]=0.0;
	m_fPointColor[0]=1.0;
	m_fPointColor[1]=0.0;
	m_fPointColor[2]=0.0;
	m_Material.SetProperty("Green");
}
CStroke::~CStroke(void)
{
	int i;
}

void CStroke::AddPoint(double x, double y, double z)
{
	CVector3d V(x,y,z);

	m_vecPoints.push_back(V);
}

void CStroke::Display()
{
	switch(m_iDisplayStyle)
	{
	case 0:
		{
			glDisable(GL_LIGHTING);
			glEnable(GL_POINT_SMOOTH);
			glDepthRange(0.0,0.99997);

			glPointSize(3.0);
			glColor3f(0.0,0.0,1.0);
			glBegin(GL_POINTS);
			for(unsigned int i=0;i<m_vecCriticalPoints.size();i++)
				glVertex3dv(m_vecPoints[m_vecCriticalPoints[i]].GetArray());
			glEnd();	


			if (m_bSelected)
				glColor3f(1.0,1.0,0.0);
			else
				glColor3f(1.0,0.0,0.0);

			
			glPointSize(2.0);
			glBegin(GL_POINTS);
			for(unsigned int i=0;i<m_vecPoints.size();i++)
				glVertex3dv(m_vecPoints[i].GetArray());
			glEnd();

			glLineWidth(2.0);
			glBegin(GL_LINE_STRIP);
			for(unsigned int i=0;i<m_vecPoints.size();i++)
				glVertex3dv(m_vecPoints[i].GetArray());
			glEnd();
			glEnable(GL_LIGHTING);
			glLineWidth(1.0);
			//glBegin(GL_LINES);
			//for(unsigned int i=0;i<m_vecPoints.size();i++)
			//{
			//	glVertex3dv(m_vecPoints[i].GetArray());
			//	glVertex3dv((m_vecPoints[i]+0.05*m_vecNormals[i]).GetArray());
			//}
			//glEnd();


			glDepthRange(0.0,1.0);

		}
		break;
	case 1:
		{

			CVector3d pt1,pt2,dir,dir1,dir2;
			double len;
			int i;
			m_Material.glDraw();
			for (i=0;i<m_vecPoints.size()-1;i++)
			{
				pt1=m_vecPoints[i];
				pt2=m_vecPoints[i+1];
				dir=pt2-pt1;
				len=dir.Length();
				dir.Normalize();
				dir1=Ortho3(dir);
				dir1.Normalize();
				dir2=dir.Cross(dir1);
				dir2.Normalize();


				glPushMatrix ();
				glTranslated(pt1[0],pt1[1],pt1[2]);
				gluSphere(gluNewQuadric(),0.005,50,50);

				double mx[]={
					dir1[0],dir1[1],dir1[2],0,
						dir2[0],dir2[1],dir2[2],0,
						dir[0],dir[1],dir[2],0,
						0,0,0,1
				};
				glMultMatrixd(mx);
				gluCylinder(gluNewQuadric(),0.005,0.005,len,50,50);
				glPopMatrix();
			}
			pt1=m_vecPoints[i];
			glPushMatrix ();
			glTranslated(pt1[0],pt1[1],pt1[2]);
			gluSphere(gluNewQuadric(),0.005,50,50);
			glPopMatrix();

		}
		break;
	}
}

int CStroke::GetPointsNum(void)
{
	return m_vecPoints.size();
}

void CStroke::GetPoint(int index, double* x, double* y, double* z)
{
	CVector3d V=m_vecPoints[index];
	*x=V.x();
	*y=V.y();
	*z=V.z();

}


void CStroke::SetLineColor(float r, float g, float b)
{
	m_fLineColor[0]=r;
	m_fLineColor[1]=g;
	m_fLineColor[2]=b;
}

void CStroke::SetPointColor(float r, float g, float b)
{
	m_fPointColor[0]=r;
	m_fPointColor[1]=g;
	m_fPointColor[2]=b;
}

CVector3d CStroke::GetPoint(int index)
{
	return m_vecPoints[index];
}

void CStroke::AddPoint(CVector3d& pt)
{
	m_vecPoints.push_back(pt);

}

void CStroke::ModifyPoint(int index, CVector3d& ncoord)
{
	vector<CVector3d>::iterator it=m_vecPoints.begin()+index;
	(*it)=ncoord;
}

void CStroke::Clear(void)
{
	m_vecPoints.clear();
}

double CStroke::Length(void)
{
	double sum=0.0;
	for (int i=0;i<m_vecPoints.size()-1;i++)
	{
		sum+=(m_vecPoints[i+1]-m_vecPoints[i]).Length();
	}
	return sum;
}

void CStroke::AddNormal(CVector3d& norm)
{
	m_vecNormals.push_back(norm);
}

CVector3d CStroke::GetNormal(int index)
{
	return m_vecNormals[index];
}

void CStroke::GenerateNormalByInterpolation(CVector3d& norm1,CVector3d& norm2)
{
	double t,l,len=0.0;
	CVector3d coord1,coord2,norm;
	int i;

	len=Length();

	m_vecNormals.push_back(norm1);

	l=0.0;
	for (i=1;i<m_vecPoints.size()-1;i++)
	{
		coord1=m_vecPoints[i-1];
		coord2=m_vecPoints[i];
		l+=(coord2-coord1).Length();

		t=l/len;
		norm=(1-t)*norm1+t*norm2;
		norm.Normalize();
		m_vecNormals.push_back(norm);        
	}
	m_vecNormals.push_back(norm2);

}

void CStroke::Optimization(void)
{
	vector<CVector3d> vecCoords;

	CVector3d coord1,coord2,coord3,offset;
	double e1,e2;
	vector<CVector3d>::iterator it=m_vecPoints.begin();

	int i,k;


	int num=m_vecPoints.size();

	for (k=0;k<10;k++)
	{
		for (i=1;i<num-1;i++)
		{
			coord1=m_vecPoints[i-1];
			coord2=m_vecPoints[i];
			coord3=m_vecPoints[i+1];
			e1=(coord2-coord1).Length();
			e2=(coord2-coord3).Length();

			offset=2*(e1*coord1+e2*coord3-(e1+e2)*coord2)/(e1+e2);
			vecCoords.push_back(coord2+0.001*offset);
		}
		for (i=1;i<num-1;i++)
		{
			*(it+i)=vecCoords[i-1];
		}
		vecCoords.clear();
	}

}

void CStroke::Transform(CMatrix44& mat)
{
	vector<CVector3d>::iterator it=m_vecPoints.begin();
	CVector3d coord;

	while (it!=m_vecPoints.end())
	{
		coord=mat.MultMatVec((*it));
		(*it)=coord;
		it++;
	}


}

void CStroke::SetDisplayStyle(int style)
{
	m_iDisplayStyle=style;
}

void CStroke::AddCriticalPoint(int index)
{
	m_vecCriticalPoints.push_back(index);
}

void CStroke::ClearCriticalPoints(void)
{
	m_vecCriticalPoints.clear();
}

int CStroke::GetCriticalPointNum(void)
{
	return m_vecCriticalPoints.size();
}

CVector3d CStroke::GetCriticalPointByIndex(int index)
{
	return m_vecPoints[m_vecCriticalPoints[index]];
}

void CStroke::SetSelected(bool flag)
{
	m_bSelected=flag;
}
