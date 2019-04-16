#include "StdAfx.h"
#include ".\basicgeometryopeartion.h"
#include <math.h>
#include "global.h"
using namespace std;
CBasicGeometryOperation::CBasicGeometryOperation(void)
{
}

CBasicGeometryOperation::~CBasicGeometryOperation(void)
{
}

bool CBasicGeometryOperation::IsLeftSide(CVector2d& pt1, CVector2d& pt2, CVector2d& pt3)
{
	if ( ((pt2[0] - pt1[0]) * (pt3[1]  - pt1[1]) -
		(pt3[0]  - pt1[0]) * (pt2[1] - pt1[1])) > 0.0f )
		return true;
	else
		return false;
}

bool CBasicGeometryOperation::IsPointInFace(CVector2d& p, CVector2d& pt1, CVector2d& pt2, CVector2d& pt3)
{
	if ( (IsLeftSide( pt1, pt2, p ) == true) &&
		(IsLeftSide( pt2, pt3, p ) == true) &&
		(IsLeftSide( pt3, pt1, p ) == true) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

double CBasicGeometryOperation::TriangleArea(CVector2d& p1, CVector2d& p2, CVector2d& p3)
{
	return fabs((p2[0] - p1[0]) * (p3[1] - p1[1]) - (p2[1] - p1[1]) * (p3[0] - p1[0])) / 2.0;  
}

void CBasicGeometryOperation::BarycentricCoordinate(CVector2d& p, CVector2d& p1, CVector2d& p2, CVector2d& p3, CVector3d& bc)
{
	double area = TriangleArea( p1, p2, p3 );
	//cout << area << endl;
	bc[0]=TriangleArea( p, p2, p3 ) / area;
	bc[1]=TriangleArea( p, p3, p1 ) / area;
	bc[2]=TriangleArea( p, p1, p2 ) / area;

}

bool CBasicGeometryOperation::IsPointInPolygon(vector<CVector2d*>& poly, CVector2d& p)
{
	int    counter = 0;
	int    i;
	double xinters;
	CVector2d  p1,p2;

	p1 = *(poly[0]);
	for (i=1;i<=poly.size();i++) {
		p2 = *(poly[i%poly.size()]);
		if (p[1] > MIN(p1[1],p2[1])) {
			if (p[1] <= MAX(p1[1],p2[1])) {
				if (p[0] <= MAX(p1[0],p2[0])) {
					if (p1[1] != p2[1]) {
						xinters = (p[1]-p1[1])*(p2[0]-p1[0])/(p2[1]-p1[1])+p1[0];
						if (p1[0] == p2[0] || p[0] <= xinters)
							counter++;
					}
				}
			}
		}
		p1 = p2;
	}

	if (counter % 2 == 0)
		return false;
	else
		return true;
}
