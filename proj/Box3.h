#pragma once
#include "..\openglbasic\arcball\vector3d.h"
#include "..\openglbasic\arcball\vector3d.h"

class CBox3
{
public:
	enum { INSIDE, INTERSECT, OUTSIDE };

	enum Face { L, R, D, U, B, F };

	enum Vertex { LDB,
		LDF,
		LUB,
		LUF,
		RDB,
		RDF,
		RUB,
		RUF
	};

	enum VertexMask {
		RIGHT_MASK =	1 << 2,
		UP_MASK =	1 << 1,
		FRONT_MASK =	1 << 0
	};


public:
	CBox3(void);
	CBox3(CVector3d vmin, CVector3d vmax);
	CBox3(double xmin,double ymin,double zmin,double xmax, double ymax, double zmax);
	~CBox3(void);
private:
	CVector3d m_vMaxCorner;
	CVector3d m_vMinCorner;
public:
	void Reset(double xmin,double ymin,double zmin,double xmax, double ymax, double zmax);
	void Reset(const CVector3d& vmin,const CVector3d& vmax);
	double XMin(void){return m_vMinCorner.x();}
	double YMin(void){return m_vMinCorner.y();}
	double ZMin(void){return m_vMinCorner.z();}

	double XMax(void){return m_vMaxCorner.x();}
	double YMax(void){return m_vMaxCorner.y();}
	double ZMax(void){return m_vMaxCorner.z();}


	const CVector3d& GetMinCorner(void){return m_vMinCorner;}
	const CVector3d& GetMaxCorner(void){return m_vMaxCorner;}
	void Update(const CVector3d & vpt);
	double XLength(void){return (m_vMaxCorner[0]-m_vMinCorner[0]);}
	double YLength(void){return (m_vMaxCorner[1]-m_vMinCorner[1]);}
	double ZLength(void){return (m_vMaxCorner[2]-m_vMinCorner[2]);}

	void Display(void);
	CVector3d Vertex(int fi, int vi);
	CVector3d Vertex(int vi);
	double MaxAxisLength(void);
};
