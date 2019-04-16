#pragma once
#include "..\openglbasic\arcball\vector3d.h"
#include "..\PoissonShapeRepresentation\Material.h"

#include <vector>
class CPlane
{
public:
	CPlane(void);
	CPlane(CVector3d& point, CVector3d& normal);
	CPlane(const CVector3d& normal,double d);
	CPlane(const CVector3d& A,const CVector3d& B, const CVector3d& C);
	~CPlane(void);
	double a();
	double b();
	double c();
	double d();
private:
	double m_a;
	double m_b;
	double m_c;
	double m_d;
	std::vector<CVector3d> m_vecCorners;
	CVector3d m_vColor;
public:
	CVector3d Normal(void);
	void Display(bool bComplex=false);
	void AddCorner(CVector3d pCorner);
	void SetCorner(int index, CVector3d& vcorner);
	CVector3d GetCorner(int index);
	void SetColor(double r, double g, double b);
	double CalcValue(CVector3d& point);
	int GetCornerNum(void);
};
