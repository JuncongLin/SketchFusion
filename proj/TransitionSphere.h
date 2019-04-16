#pragma once

#include "TransitionPrimitive.h"
class CTransitionSphere : public CTransitionPrimitive
{
public:
	CTransitionSphere(void);
	~CTransitionSphere(void);

private:
	double m_dRadius;

public:
	void SetRadius(double r);
	double GetRadius(void);

	void Display(void);
	void SetDefLocator(void);
	void SetAutoLocator(CVector3d& dir);
	void SetManualLocator(CRay3& ray);
	void SetManualLocator(int index,CRay3& ray);
	CVector3d GetCurDirection(void);
	CVector3d GetLocDirectionByIndex(int index);
	void OffsetLocator(CVector3d& raw);
};
