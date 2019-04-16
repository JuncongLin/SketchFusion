#include "StdAfx.h"
#include ".\point3.h"

CPoint3::CPoint3(void)
: m_bSelected(false)
{
	coord.Set(0.0,0.0,0.0);
	m_Material.SetProperty("Red");
}
CPoint3::CPoint3(double px, double py, double pz)
{
	m_bSelected=false;
	coord.Set(px,py,pz);
}

CPoint3::~CPoint3(void)
{
}

double CPoint3::operator[](unsigned i) const
{
	//assert(i < 3);
	return coord[i];
}

double &CPoint3::operator[](unsigned i)
{
	//assert(i < 3);
	return coord[i];
}

CVector3d operator+(const CPoint3 &p, const CPoint3 &v)
{
	return CVector3d(
		p[0] + v[0],
		p[1] + v[1],
		p[2] + v[2]
		);

}

CVector3d operator-(const CPoint3 &p, const CPoint3 &q)
{
	return CVector3d(
		p[0] - q[0],
		p[1] - q[1],
		p[2] - q[2]
		);

}


void CPoint3::Set(const CVector3d& pos)
{
	coord[0]=pos[0];
	coord[1]=pos[1];
	coord[2]=pos[2];
}

bool CPoint3::Collinear(const CPoint3& A, const CPoint3& B, const CPoint3& C)
{
	CVector3d vA(A[0],A[1],A[2]);
	CVector3d vB(B[0],B[1],B[2]);
	CVector3d vC(C[0],C[1],C[2]);

	return false;


}

void CPoint3::Display(void)
{
	glDisable(GL_COLOR_MATERIAL);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glPushMatrix ();
	glTranslated(coord[0],coord[1],coord[2]);

	if(m_bSelected)
	{
		CMaterial mat("Green");
		mat.glDraw();
	}
	else
	{
		m_Material.glDraw();
	}
	gluSphere(gluNewQuadric(),0.001,50,50);

	glPopMatrix();
}

CVector3d& CPoint3::GetCoordinate(void)
{
	return coord;
}

void CPoint3::SetMaterial(CString material)
{
	m_Material.SetProperty(material);
}

void CPoint3::SetSelected(bool flag)
{
	m_bSelected=flag;
}

bool CPoint3::IsSelected(void)
{
	return m_bSelected;
}
