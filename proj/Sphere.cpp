#include "StdAfx.h"
#include ".\sphere.h"

CSphere::CSphere(void)
:m_ptCenter(0,0,0)
,m_dRadius(1.0)
{
}
CSphere::CSphere(CPoint3 center,double radius)
{
	m_ptCenter=center;
	m_dRadius=radius;
}


CSphere::~CSphere(void)
{
}

CPoint3 CSphere::GetCenter(void)
{
	return m_ptCenter;
}

double CSphere::GetRadius(void)
{
	return m_dRadius;
}
