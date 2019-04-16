#pragma once
#include "TransitionPrimitive.h"
class CTransitionCylinder: public CTransitionPrimitive
{
public:
	CTransitionCylinder(void);
	~CTransitionCylinder(void);
private:
	double m_dHeight;
	double m_dBRadius;
	double m_dTRadius;
	CVector3d m_vLimit;
public:
	void SetBottomRadius(double radius);
	void Display(void);
	void SetDefLocator(void);
	void SetAutoLocator(CVector3d& dir);
	void SetManualLocator(CRay3& ray);
	double GetHeight(void);
	double GetBottomRadius(void);
	double GetTopRadius(void);
	void SetHeight(double h);
	void SetTopRadius(double radius);
	void CalcLimit(void);
	CVector3d GetCurDirection(void);
	CVector3d GetLocDirectionByIndex(int index);
private:
	bool m_bDefault;
public:
	void OffsetLocator(CVector3d& raw);
};
