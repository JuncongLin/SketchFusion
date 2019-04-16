#pragma once
#include "Sphere.h"
#include "Cone.h"
#include "Cylinder.h"
class CRay3
{
public:
	CRay3(void);
	CRay3(const CPoint3 &p, const CVector3d &d);
	CRay3(const CPoint3 &p, const CPoint3 &q);
	~CRay3(void);
private:
	CPoint3 m_ptSource;
	CVector3d m_vDirection;

public:
	bool Intersect(CSphere sphere, double& t1, double& t2);
	CPoint3 GetSource(void);
	CVector3d& GetDirection(void);
	bool Intersect(CCone& cone, double& t1, double& t2);
	bool Intersect(CCylinder& cylinder, double& t1, double& t2);
};
