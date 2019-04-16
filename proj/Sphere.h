#pragma once
#include "Point3.h"
class CSphere
{
public:
	CSphere(void);
	CSphere(CPoint3 center,double radius);
	~CSphere(void);
private:
	CPoint3 m_ptCenter;
	double m_dRadius;
public:
	CPoint3 GetCenter(void);
	double GetRadius(void);
};
