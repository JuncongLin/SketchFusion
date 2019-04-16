#include "StdAfx.h"
#include "global.h"
#include ".\transitioncylinder.h"
#include <math.h>
CTransitionCylinder::CTransitionCylinder(void)
: m_bDefault(true)
{
	m_iType=1;

}

CTransitionCylinder::~CTransitionCylinder(void)
{
}

void CTransitionCylinder::SetBottomRadius(double radius)
{
	m_dBRadius=radius;
	//m_dHeight=0.8*m_dBRadius;
	//m_dTRadius=0.5*m_dBRadius;
}

void CTransitionCylinder::Display(void)
{
	double  matrix[16];

	CVector3d u=Ortho3(m_vDirection);
	u.Normalize();
	CVector3d v=m_vDirection.Cross(u);
	v.Normalize();

	matrix[0]=u[0];
	matrix[1]=u[1];
	matrix[2]=u[2];
	matrix[3]=0.0;

	matrix[4*1+0]=v[0];
	matrix[4*1+1]=v[1];
	matrix[4*1+2]=v[2];
	matrix[4*1+3]=0.0;

	matrix[4*2+0]=m_vDirection[0];
	matrix[4*2+1]=m_vDirection[1];
	matrix[4*2+2]=m_vDirection[2];
	matrix[4*2+3]=0.0;

	matrix[4*3+0]=0.0;
	matrix[4*3+1]=0.0;
	matrix[4*3+2]=0.0;
	matrix[4*3+3]=1.0;

	double ang=3.1415926/18;
	int i;
	glPolygonMode(GL_FRONT,GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_COLOR_MATERIAL);
	glColor4d(0.0,0.0,0.8,0.4);

	m_Material.glDraw();

	glPushMatrix ();
	glTranslated(m_vCenter[0],m_vCenter[1],m_vCenter[2]);
	glMultMatrixd(matrix);
	glBegin(GL_TRIANGLES);
	for(i=0;i<35;i++)
	{
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(m_dBRadius*cos((i+1)*ang),m_dBRadius*sin((i+1)*ang),0.0);
		glVertex3f(m_dBRadius*cos(i*ang),m_dBRadius*sin(i*ang),0.0);
	}
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(m_dBRadius,0.0,0.0);
	glVertex3f(m_dBRadius*cos(i*ang),m_dBRadius*sin(i*ang),0.0);
	glEnd();
	gluCylinder(gluNewQuadric(),m_dBRadius,m_dTRadius,m_dHeight,50,50);
	glBegin(GL_TRIANGLES);
	for(i=0;i<35;i++)
	{
		glVertex3f(0.0,0.0,m_dHeight);
		glVertex3f(m_dTRadius*cos(i*ang),m_dTRadius*sin(i*ang),m_dHeight);
		glVertex3f(m_dTRadius*cos((i+1)*ang),m_dTRadius*sin((i+1)*ang),m_dHeight);
	}
	glVertex3f(0.0,0.0,m_dHeight);
	glVertex3f(m_dTRadius*cos(i*ang),m_dTRadius*sin(i*ang),m_dHeight);
	glVertex3f(m_dTRadius,0.0,m_dHeight);
	glEnd();
	glPopMatrix ();
	glDisable(GL_BLEND);
	if(m_iLocatingStyle!=-1)
	{
		glColor3f(1.0,0.0,0.0);
		glPushMatrix ();
		glTranslated(m_vCurLocator.x(),m_vCurLocator.y(),m_vCurLocator.z());
		glColor3f(1.0,0.0,0.0);
		gluSphere(gluNewQuadric(),0.02,50,50);
		glPopMatrix ();
	}


}

void CTransitionCylinder::SetDefLocator(void)
{
	m_vCurLocator=m_vCenter+m_vDirection*m_dHeight;
}

void CTransitionCylinder::SetAutoLocator(CVector3d& dir)
{
}

void CTransitionCylinder::SetManualLocator(CRay3& ray)
{
	double t1,t2;
	CPoint3 source=ray.GetSource();
	CVector3d vsource(source[0],source[1],source[2]);
	if(m_dTRadius==0)
	{
		CVector3d vvertex=m_vCenter+m_vDirection*m_dHeight;
		CPoint3 cvertex(vvertex[0],vvertex[1],vvertex[2]);
		double angle=atan(m_dBRadius/m_dHeight);
		CCone cone(cvertex,-m_vDirection,angle);
		if(ray.Intersect(cone,t1,t2))	
		{
			m_vCurLocator=vsource+t1*ray.GetDirection();
			if((m_vCurLocator-vvertex).LengthSquared()>(m_dHeight*m_dHeight+m_dBRadius*m_dBRadius))
			{
				SetDefLocator();
				m_bDefault=true;
			}
			else
				m_bDefault=false;

		}
		else
		{
			SetDefLocator();
			m_bDefault=true;
		}
	}
	else if(m_dBRadius==m_dTRadius)
	{
		CPoint3 cvertex(m_vCenter[0],m_vCenter[1],m_vCenter[2]);
		if(ray.Intersect(CCylinder(cvertex,m_vDirection,m_dBRadius),t1,t2))
		{
			m_vCurLocator=vsource+t1*ray.GetDirection();
			double dot=(m_vCurLocator-m_vCenter).Dot(m_vDirection);
			if (dot<0||dot>m_dHeight)
			{
				SetDefLocator();
				m_bDefault=true;
			}
			else
				m_bDefault=false;
		}
		else
		{
			SetDefLocator();
			m_bDefault=true;
		}
	}
	else
	{
		double tanAngle=(m_dBRadius-m_dTRadius)/m_dHeight;
		double angle=atan(tanAngle);
		CVector3d vvertex=m_vCenter+m_vDirection*(m_dBRadius/tanAngle);
		CPoint3 cvertex(vvertex[0],vvertex[1],vvertex[2]);

		CCone cone(cvertex,-m_vDirection,angle);

		if(ray.Intersect(cone,t1,t2))	
		{
			m_vCurLocator=vsource+t1*ray.GetDirection();

			double LenMin=m_dTRadius/tanAngle;
			double LenMax=m_dBRadius/tanAngle;
			double Len=(m_vCurLocator-vvertex).Length();
			if(Len<LenMin||Len>LenMax)
			{
				SetDefLocator();
				m_bDefault=true;
			}
			else
				m_bDefault=false;

		}
		else
		{
			SetDefLocator();
			m_bDefault=true;
		}

	}
}

double CTransitionCylinder::GetHeight(void)
{
	return m_dHeight;
}

double CTransitionCylinder::GetBottomRadius(void)
{
	return m_dBRadius;
}

double CTransitionCylinder::GetTopRadius(void)
{
	return m_dTRadius;
}

void CTransitionCylinder::SetHeight(double h)
{
	m_dHeight=h;
}

void CTransitionCylinder::SetTopRadius(double radius)
{
	m_dTRadius=radius;
}

void CTransitionCylinder::CalcLimit(void)
{

	double d=m_dHeight/(1.0-(m_dTRadius/m_dBRadius));
	m_vLimit=m_vCenter+d*m_vDirection;

}

CVector3d CTransitionCylinder::GetCurDirection(void)
{
	CVector3d n,Dir,ODir,ctl;

	if((m_vCurLocator-m_vCenter).Dot(m_vDirection)==m_dHeight)
	{
		return -1*m_vDirection;
	}

	Dir=m_vCurLocator-m_vLimit;
	Dir.Normalize();
	ctl=m_vCenter-m_vLimit;
	ctl.Normalize();

	n=Dir.Cross(ctl);
	n.Normalize();

	ODir.Set(Dir[1],-1*Dir[0],0.0);
	ODir.Normalize();
	Dir=ODir-(n.Dot(ODir))*n;
	Dir*=Dir.Dot(m_vCenter-m_vCurLocator);
	Dir.Normalize();
	return Dir;
}

CVector3d CTransitionCylinder::GetLocDirectionByIndex(int index)
{
	CVector3d n,Dir,ODir,ctl;

	double temp=(m_vecLocators[index]-m_vCenter).Dot(m_vDirection);

	if(temp>m_dHeight-0.00001&&temp<m_dHeight+0.00001)
	{
		return m_vDirection;
	}


	Dir=m_vecLocators[index]-m_vLimit;
	Dir.Normalize();
	ctl=m_vCenter-m_vLimit;
	ctl.Normalize();

	n=Dir.Cross(ctl);
	n.Normalize();

	ODir.Set(Dir[1],-1*Dir[0],0.0);
	ODir.Normalize();
	Dir=ODir-(n.Dot(ODir))*n;
	Dir*=Dir.Dot(m_vCenter-m_vCurLocator);
	Dir.Normalize();
	return Dir;
}



void CTransitionCylinder::OffsetLocator(CVector3d& raw)
{
	raw+=CVector3d(0.2*my_random(),0.0,0.0);

	const double *cnt,*off;

	off=raw.GetArray();
	cnt=m_vCenter.GetArray();

	CRay3 ray(CPoint3(off[0],off[1],off[2]),CPoint3(cnt[0],cnt[1],cnt[2]));
	SetManualLocator(ray);

}
