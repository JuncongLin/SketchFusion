#pragma once
#include "..\OpenGLBasic\Arcball\Vector3d.h"
class CImplicitSurface
{
public:
	CImplicitSurface(void);
	~CImplicitSurface(void);
	virtual double GetValueAt(CVector3d point);
	virtual CVector3d GradientAt(CVector3d point);
	virtual CVector3d Project(CVector3d& coord);
};
