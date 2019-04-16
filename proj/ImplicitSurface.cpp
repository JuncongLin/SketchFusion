#include "stdafx.h"
#include ".\implicitsurface.h"

CImplicitSurface::CImplicitSurface(void)
{
}

CImplicitSurface::~CImplicitSurface(void)
{
}

double CImplicitSurface::GetValueAt(CVector3d point)
{
	return 0;
}

CVector3d CImplicitSurface::GradientAt(CVector3d point)
{
	return CVector3d();
}

CVector3d CImplicitSurface::Project(CVector3d& coord)
{
	int k=-1;
	CVector3d u[2],gradient;
	u[0]=coord;

	do 
	{
		k++;
		gradient=GradientAt(coord);
		u[(k+1)%2]=u[k%2]-gradient*(GetValueAt(coord)/gradient.LengthSquared());
	} while((u[(k+1)%2]-u[k%2]).Length()>1.0e-3);
	return u[(k+1)%2];
}
