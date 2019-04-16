#pragma once
#include "Box3.h"
#include "..\OpenGLBasic\Arcball\Vector3d.h"
#include "Triangle3.h"
#include "Point3.h"
#include "Plane.h"
class CIntersectionTest
{
public:
	CIntersectionTest(void);
	~CIntersectionTest(void);
	static bool Box_LineSegment(CBox3 box, const CPoint3& ls,const CPoint3& le,CVector3d org,double dim);
	static bool Box_Triangle(CBox3& box, CTriangle3& tri,CVector3d org,double dim);
	static bool Box_Plane(CBox3 box, CPlane& plane,CVector3d org,double dim);
};
