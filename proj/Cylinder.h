#pragma once
#include "Point3.h"
#include "..\OpenGLBasic\Arcball\Vector3d.h"
class CCylinder
{
public:
	CCylinder(void);
	CCylinder(CPoint3 center,CVector3d direction, double radius);
	~CCylinder(void);
private:
	CPoint3 m_ptCenter;
	CVector3d m_vDirection;
	double m_dRadius;
public:
	CPoint3 GetCenter(void);
	CVector3d GetDirection(void);
};
