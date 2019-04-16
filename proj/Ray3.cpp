#include "StdAfx.h"
#include ".\ray3.h"
#include <math.h>
CRay3::CRay3(void)
{
}
CRay3::CRay3(const CPoint3 &p, const CVector3d &d)
{
	m_ptSource=p;
	m_vDirection=d;
}
CRay3::CRay3(const CPoint3 &p, const CPoint3 &q)
{
	m_ptSource=p;
	m_vDirection=q-p;
	m_vDirection.Normalize();
}

CRay3::~CRay3(void)
{
}

bool CRay3::Intersect(CSphere sphere, double& t1, double& t2)
{
	CVector3d oc=m_ptSource-sphere.GetCenter();
	double b=m_vDirection.Dot(oc);
	double c=oc.Dot(oc)-sphere.GetRadius()*sphere.GetRadius();
	double delta=b*b-c;
	if(delta<0)
		return false;
	else
	{
		t1=-b-sqrt(delta);
		t2=-b+sqrt(delta);
	}
	return true;
}


CPoint3 CRay3::GetSource(void)
{
	return m_ptSource;
}

CVector3d& CRay3::GetDirection(void)
{
	return m_vDirection;
}

bool CRay3::Intersect(CCone& cone, double& t1, double& t2)
{

    CVector3d n=m_vDirection.Cross(cone.GetVertex()-m_ptSource);
	if(n.Length()==0)
	{
		t1=t2=((cone.GetVertex())[0]-m_ptSource[0])/m_vDirection[0];
		return true;
	}
	n.Normalize();
	CVector3d v=cone.GetDirection();

	double dot=v.Dot(n);
	if(dot>0)
		n*=-1;
	double cosTheta=sqrt(1.0-dot*dot);
	double cosAngle=cos(cone.GetAngle());
	if(cosTheta<cosAngle)
		return false;
	else
	{
		CVector3d u=v.Cross(n);
		CVector3d w=u.Cross(v);
		double tanTheta=sqrt(1-cosTheta*cosTheta)/cosTheta;
		double tanAngle=sqrt(1-cosAngle*cosAngle)/cosAngle;
		double coeffU=sqrt(tanAngle*tanAngle-tanTheta*tanTheta);
		CVector3d DirA=v+w*tanTheta+u*coeffU;
		DirA.Normalize();
		CVector3d DirB=v+w*tanTheta-u*coeffU;
		DirB.Normalize();
		if(m_vDirection.Dot(DirA)==1)
		{
			if(m_vDirection.Dot(DirB)==1)
				return false;
			else
				t1=t2=((cone.GetVertex()-m_ptSource).Cross(DirB))[0]/(m_vDirection.Cross(DirB))[0];
		}
		else
		{
			t1=((cone.GetVertex()-m_ptSource).Cross(DirA))[0]/(m_vDirection.Cross(DirA))[0];
			if(m_vDirection.Dot(DirB)==1)
			{
				t2=t1;
			}
			else
			{
				t2=((cone.GetVertex()-m_ptSource).Cross(DirB))[0]/(m_vDirection.Cross(DirB))[0];
				if(t1>t2)
				{
					double tmp=t1;
					t1=t2;
					t2=tmp;
				}
			}
		}

	}
	return true;
}

bool CRay3::Intersect(CCylinder& cylinder, double& t1, double& t2)
{
	CVector3d u=m_vDirection;
	CVector3d v=cylinder.GetDirection();
	CVector3d w=u.Cross(v);

	CVector3d A(m_ptSource[0],m_ptSource[1],m_ptSource[2]);
	CPoint3 ptB=cylinder.GetCenter();
	CVector3d B(ptB[0],ptB[1],ptB[2]);

	double d=(B-A).Dot(w);

	double r=((B-A-w*d).Cross(v)).Dot(w)/w.LengthSquared();
	CVector3d P=A+u*r;
	double dot=u.Dot(v);
	double deltaT=sqrt((r*r-d*d)/(1-dot*dot));
	t1=r-deltaT;
	t2=r+deltaT;
	return true;

}
