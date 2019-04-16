#include "StdAfx.h"
#include ".\vector2d.h"

CVector2d::CVector2d(void)
{
}
CVector2d::CVector2d(double x,double y)
{
	Set(x,y);
}
CVector2d::CVector2d(const CVector2d& ivec)
{
	vec[0]=ivec.x();
	vec[1]=ivec.y();

}
CVector2d::~CVector2d(void)
{
}

//********************************************
// Operator +=
//********************************************
CVector2d&
CVector2d::operator+=(const CVector2d& rVector)
{
	vec[0] += rVector.x();
	vec[1] += rVector.y();
	return *this;
}

//********************************************
// Operator +=
//********************************************
CVector2d&
CVector2d::operator+=(const CVector2d* pVector)
{
	vec[0] += pVector->x();
	vec[1] += pVector->y();
	return *this;
}

//********************************************
// Operator -=
//********************************************
CVector2d&
CVector2d::operator-=(const CVector2d& rVector)
{
	vec[0] -= rVector.x();
	vec[1] -= rVector.y();
	return *this;
}

//********************************************
// Operator -=
//********************************************
CVector2d&
CVector2d::operator-=(const CVector2d* pVector)
{
	vec[0] -= pVector->x();
	vec[1] -= pVector->y();
	return *this;
}

//********************************************
// Operator *=
//********************************************
CVector2d&
CVector2d::operator*=(double d)
{
	vec[0] *= d;
	vec[1] *= d;
	return *this;
}
