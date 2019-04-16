#include "StdAfx.h"
#include ".\cone.h"

CCone::CCone(void)
{
}
CCone::CCone(CPoint3& vertex,CVector3d& dir,double angle)
{
	m_ptVertex=vertex;
	m_vDirection=dir;
	m_dAngle=angle;
}

CCone::~CCone(void)
{
}

CPoint3 CCone::GetVertex(void)
{
	return m_ptVertex;
}

CVector3d CCone::GetDirection(void)
{
	return m_vDirection;
}

double CCone::GetAngle(void)
{
	return m_dAngle;
}
