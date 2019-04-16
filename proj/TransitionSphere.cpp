#include "StdAfx.h"
#include ".\transitionsphere.h"

CTransitionSphere::CTransitionSphere(void)
:m_dRadius(0)
{
	m_iType=0;
}

CTransitionSphere::~CTransitionSphere(void)
{
}
void CTransitionSphere::SetRadius(double r)
{
	m_dRadius=r;
}

double CTransitionSphere::GetRadius(void)
{
	return m_dRadius;
}
void CTransitionSphere::Display(void)
{
	glPolygonMode(GL_FRONT,GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_COLOR_MATERIAL);
	glColor4d(0.0,0.0,0.8,0.4);

	m_Material.glDraw();


	glPushMatrix ();
	glTranslated(m_vCenter.x(),m_vCenter.y(),m_vCenter.z());
	gluSphere(gluNewQuadric(),m_dRadius,50,50);
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
	glDisable(GL_COLOR_MATERIAL);

}

void CTransitionSphere::SetDefLocator(void)
{
	m_vCurLocator=m_vCenter+m_vDirection*m_dRadius;
}

void CTransitionSphere::SetAutoLocator(CVector3d& dir)
{
	m_vCurLocator=m_vCenter-dir*m_dRadius;
}

void CTransitionSphere::SetManualLocator(CRay3& ray)
{
	double t1,t2;
	CPoint3 source=ray.GetSource();
	CVector3d vsource(source[0],source[1],source[2]);
	if(ray.Intersect(CSphere(CPoint3(m_vCenter[0],m_vCenter[1],m_vCenter[2]),m_dRadius),t1,t2))	
		m_vCurLocator=vsource+t1*ray.GetDirection();
	else
		SetDefLocator();

}

void CTransitionSphere::SetManualLocator(int index,CRay3& ray)
{
	double t1,t2;
	CPoint3 source=ray.GetSource();
	CVector3d vsource(source[0],source[1],source[2]);
	if(ray.Intersect(CSphere(CPoint3(m_vCenter[0],m_vCenter[1],m_vCenter[2]),m_dRadius),t1,t2))	
		*(m_vecLocators.begin()+index)=vsource+t1*ray.GetDirection();

}


CVector3d CTransitionSphere::GetCurDirection(void)
{
	CVector3d dir=m_vCenter-m_vCurLocator;
	dir.Normalize();
	return dir;
}

CVector3d CTransitionSphere::GetLocDirectionByIndex(int index)
{
	CVector3d dir= m_vCenter-m_vecLocators[index];
	dir.Normalize();
	return dir;
}

void CTransitionSphere::OffsetLocator(CVector3d& raw)
{
	raw+=CVector3d(0.2*my_random(),0.0,0.0);

	const double *cnt,*off;

	off=raw.GetArray();
	cnt=m_vCenter.GetArray();

	CRay3 ray(CPoint3(off[0],off[1],off[2]),CPoint3(cnt[0],cnt[1],cnt[2]));
	SetManualLocator(ray);

}
