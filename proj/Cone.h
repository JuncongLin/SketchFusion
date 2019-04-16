#pragma once
#include "Point3.h"
#include "..\OpenGLBasic\Arcball\Vector3d.h"
class CCone
{
public:
	CCone(void);
	CCone(CPoint3& vertex,CVector3d& dir,double angle);
	~CCone(void);
private:
	CPoint3 m_ptVertex;
	CVector3d m_vDirection;
	double m_dAngle;
public:
	CPoint3 GetVertex(void);
	CVector3d GetDirection(void);
	double GetAngle(void);
};
