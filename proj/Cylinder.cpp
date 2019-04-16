#include "StdAfx.h"
#include ".\cylinder.h"

CCylinder::CCylinder(void)
{
}
CCylinder::CCylinder(CPoint3 center,CVector3d direction, double radius)
{
	m_ptCenter=center;
	m_vDirection=direction;
	m_dRadius=radius;
}

CCylinder::~CCylinder(void)
{
}

CPoint3 CCylinder::GetCenter(void)
{
	return m_ptCenter;
}

CVector3d CCylinder::GetDirection(void)
{
	return m_vDirection;
}
