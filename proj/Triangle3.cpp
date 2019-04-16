#include "StdAfx.h"
#include ".\triangle3.h"

CTriangle3::CTriangle3(void)
{
}
CTriangle3::CTriangle3(const CTriangle3& tri)
{
	m_A=tri.GetA();
	m_B=tri.GetB();
	m_C=tri.GetC();
}
CTriangle3::~CTriangle3(void)
{
}

CPoint3& CTriangle3::operator [](int i)
{
	//assert(i>=0 && i<3);

	switch (i) {
	case 0:	return m_A; break;
	case 1:	return m_B; break;
	case 2:	return m_C; break;
	}

	return m_A;
}

void CTriangle3::Set_A(const CVector3d& pos)
{
	m_A.Set(pos);
	
}

void CTriangle3::Set_B(const CVector3d& pos)
{
	m_B.Set(pos);
}

void CTriangle3::Set_C(const CVector3d& pos)
{
	m_C.Set(pos);
}
