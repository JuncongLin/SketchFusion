#pragma once
#include "..\OpenGLBasic\Arcball\Vector3d.h"
#include "..\PoissonShapeRepresentation\Material.h"
class CPoint3
{
public:
	CPoint3(void);
	CPoint3(double px, double py, double pz);
	CPoint3(const CPoint3& pt){coord[0]=pt[0];coord[1]=pt[1];coord[2]=pt[2];}
	~CPoint3(void);
private:
	CVector3d coord;
	CMaterial m_Material;
public:
	double operator[](unsigned i) const;
	double &operator[](unsigned i);

	friend CVector3d operator+(const CPoint3 &p, const CPoint3 &v);
	friend CVector3d operator-(const CPoint3 &p, const CPoint3 &q);


	void Set(const CVector3d& pos);
	bool Collinear(const CPoint3& A, const CPoint3& B, const CPoint3& C);
	void Display(void);
	CVector3d& GetCoordinate(void);
	void SetMaterial(CString material);
private:
	bool m_bSelected;
public:
	void SetSelected(bool flag);
	bool IsSelected(void);
};
