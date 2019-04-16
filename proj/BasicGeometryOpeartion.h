#pragma once
#include "Vector2d.h"
#include "..\OpenGLBasic\Arcball\Vector3d.h"
#include <vector>
class CBasicGeometryOperation
{
public:
	CBasicGeometryOperation(void);
	~CBasicGeometryOperation(void);
	static bool IsLeftSide(CVector2d& pt1, CVector2d& pt2, CVector2d& pt3);
	static bool IsPointInFace(CVector2d& p, CVector2d& pt1, CVector2d& pt2, CVector2d& pt3);
	static double TriangleArea(CVector2d& p1, CVector2d& p2, CVector2d& p3);
	static void BarycentricCoordinate(CVector2d& p, CVector2d& p1, CVector2d& p2, CVector2d& p3, CVector3d& bc);
	static bool IsPointInPolygon(std::vector<CVector2d*>& poly, CVector2d& p);
};
