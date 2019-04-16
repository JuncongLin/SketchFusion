#include "StdAfx.h"
#include ".\box3.h"
#include <limits>

const unsigned _vertex_indices[6][4] = {
	{0, 1, 3, 2},
	{4, 6, 7, 5},
	{0, 4, 5, 1},
	{2, 3, 7, 6},
	{0, 2, 6, 4},
	{1, 5, 7, 3}
};

CBox3::CBox3(void)
{
	double value=DBL_MAX;
	m_vMinCorner.Set(value,value,value);
	m_vMaxCorner.Set(-value,-value,-value);
}
CBox3::CBox3(CVector3d vmin, CVector3d vmax)
{
	m_vMinCorner.Set(vmin);
	m_vMaxCorner.Set(vmax);
}
CBox3::CBox3(double xmin,double ymin,double zmin,double xmax, double ymax, double zmax)
{
	m_vMinCorner.Set(xmin,ymin,zmin);
	m_vMaxCorner.Set(xmax,ymax,zmax);
}

CBox3::~CBox3(void)
{
}

void CBox3::Reset(double xmin,double ymin,double zmin,double xmax, double ymax, double zmax)
{
	m_vMinCorner.Set(xmin,ymin,zmin);
	m_vMaxCorner.Set(xmax,ymax,zmax);

}
void CBox3::Reset(const CVector3d& vmin,const CVector3d& vmax)
{
	m_vMinCorner=vmin;
	m_vMaxCorner=vmax;
}

void CBox3::Update(const CVector3d& vpt)
{
	for (int i=0;i<3;i++)
	{
		m_vMaxCorner[i]=m_vMaxCorner[i]>vpt[i]?m_vMaxCorner[i]:vpt[i];
		m_vMinCorner[i]=m_vMinCorner[i]<vpt[i]?m_vMinCorner[i]:vpt[i];
	}
}

void CBox3::Display(void)
{
	glBegin(GL_QUADS);
	for (unsigned i = 0; i < 6; i++) {
		glVertex3dv(Vertex(i,0).GetArray());
		glVertex3dv(Vertex(i,1).GetArray());
		glVertex3dv(Vertex(i,2).GetArray());
		glVertex3dv(Vertex(i,3).GetArray());
	}
	glEnd();

}
CVector3d CBox3::Vertex(int fi, int vi)
{
	ASSERT(fi < 6);
	ASSERT(vi < 4);
	return Vertex(_vertex_indices[fi][vi]);

}


CVector3d CBox3::Vertex(int vi)
{
	ASSERT(vi < 8);
	double x = (vi & RIGHT_MASK) ? m_vMaxCorner.x() : m_vMinCorner.x();
	double y = (vi & UP_MASK) ?    m_vMaxCorner.y() : m_vMinCorner.y();
	double z = (vi & FRONT_MASK) ? m_vMaxCorner.z() : m_vMinCorner.z();
	return CVector3d(x, y, z);

}

double CBox3::MaxAxisLength(void)
{
	double dx = XLength();
	double dy = YLength();
	double dz = ZLength();
	return max(max(dx, dy), dz);
}
