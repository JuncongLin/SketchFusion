#include "StdAfx.h"
#include ".\plane.h"
#include "global.h"
CPlane::CPlane(void)
{
	m_vColor.Set(0.0,0.0,1.0);
}
CPlane::CPlane(CVector3d& point, CVector3d& normal)
{
	m_a=normal[0];
	m_b=normal[1];
	m_c=normal[2];
	m_d=-1*(m_a*point.x()+m_b*point.y()+m_c*point.z());
	m_vColor.Set(0.0,0.0,1.0);
}
CPlane::CPlane(const CVector3d& normal, double d)
{
	m_a=normal[0];
	m_b=normal[1];
	m_c=normal[2];
	m_d=d;
}

CPlane::CPlane(const CVector3d& A,const CVector3d& B, const CVector3d& C)
{
	ASSERT(!(B-C).IsCollinear(A-C));
	CVector3d normal=(B-C).Cross(A-C);
	m_a=normal[0];
	m_b=normal[1];
	m_c=normal[2];
	m_d=-normal.Dot(A);
}

CPlane::~CPlane(void)
{

}
double CPlane::a()
{
	return m_a;
}
double CPlane::b()
{
	return m_b;
}
double CPlane::c()
{
	return m_c;
}
double CPlane::d()
{
	return m_d;
}

CVector3d CPlane::Normal(void)
{
	return CVector3d(m_a,m_b,m_c);
}

void CPlane::Display(bool bComplex)
{
	int i;
	if(bComplex)
	{
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		CVector3d dir,corner1,corner2,dir1,dir2;
		double len;
		CMaterial mat("Brass");
		mat.glDraw();

		for (i=0;i<m_vecCorners.size()-1;i++)
		{
			corner1=m_vecCorners.at(i);
			corner2=m_vecCorners.at(i+1);
			dir=corner2-corner1;
			len=dir.Length();
			dir.Normalize();
			dir1=Ortho3(dir);
			dir1.Normalize();
			dir2=dir.Cross(dir1);
			dir2.Normalize();


			glPushMatrix ();
			glTranslated(corner1[0],corner1[1],corner1[2]);
			gluSphere(gluNewQuadric(),0.0005,50,50);

			double mx[]={
					dir1[0],dir1[1],dir1[2],0,
					dir2[0],dir2[1],dir2[2],0,
					dir[0],dir[1],dir[2],0,
					0,0,0,1
			};
			glMultMatrixd(mx);
			gluCylinder(gluNewQuadric(),0.0005,0.0005,len,50,50);
			glPopMatrix();
		}

		corner1=m_vecCorners.at(i);
		corner2=m_vecCorners.at(0);
		dir=corner2-corner1;
		len=dir.Length();
		dir.Normalize();
		dir1=Ortho3(dir);
		dir1.Normalize();
		dir2=dir.Cross(dir1);
		dir2.Normalize();

		glPushMatrix ();
		glTranslated(corner1[0],corner1[1],corner1[2]);
		gluSphere(gluNewQuadric(),0.0005,50,50);

		double mx[]={
			dir1[0],dir1[1],dir1[2],0,
				dir2[0],dir2[1],dir2[2],0,
				dir[0],dir[1],dir[2],0,
				0,0,0,1
		};
		glMultMatrixd(mx);
		gluCylinder(gluNewQuadric(),0.0005,0.0005,len,50,50);
		glPopMatrix();


	}
	else
	{
		//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		//glLineWidth(2.0);
		//glColor3d(0.0,0.0,1.0);
		//glBegin(GL_POLYGON);
		//for (i=0;i<m_vecCorners.size();i++)
		//	glVertex3dv((m_vecCorners.at(i)).GetArray());
		//glEnd();
		//glLineWidth(2.0);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		CVector3d dir,corner1,corner2,dir1,dir2;
		double len;
		CMaterial mat("Brass");
		mat.glDraw();

		for (i=0;i<m_vecCorners.size()-1;i++)
		{
			corner1=m_vecCorners.at(i);
			corner2=m_vecCorners.at(i+1);
			dir=corner2-corner1;
			len=dir.Length();
			dir.Normalize();
			dir1=Ortho3(dir);
			dir1.Normalize();
			dir2=dir.Cross(dir1);
			dir2.Normalize();


			glPushMatrix ();
			glTranslated(corner1[0],corner1[1],corner1[2]);
			gluSphere(gluNewQuadric(),0.01,50,50);

			double mx[]={
				dir1[0],dir1[1],dir1[2],0,
					dir2[0],dir2[1],dir2[2],0,
					dir[0],dir[1],dir[2],0,
					0,0,0,1
			};
			glMultMatrixd(mx);
			gluCylinder(gluNewQuadric(),0.01,0.01,len,50,50);
			glPopMatrix();
		}

		corner1=m_vecCorners.at(i);
		corner2=m_vecCorners.at(0);
		dir=corner2-corner1;
		len=dir.Length();
		dir.Normalize();
		dir1=Ortho3(dir);
		dir1.Normalize();
		dir2=dir.Cross(dir1);
		dir2.Normalize();

		glPushMatrix ();
		glTranslated(corner1[0],corner1[1],corner1[2]);
		gluSphere(gluNewQuadric(),0.01,50,50);

		double mx[]={
			dir1[0],dir1[1],dir1[2],0,
				dir2[0],dir2[1],dir2[2],0,
				dir[0],dir[1],dir[2],0,
				0,0,0,1
		};
		glMultMatrixd(mx);
		gluCylinder(gluNewQuadric(),0.01,0.01,len,50,50);
		glPopMatrix();

	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glColor4d(m_vColor[0],m_vColor[1],m_vColor[2],0.3);
	glBegin(GL_POLYGON);
	for (i=0;i<m_vecCorners.size();i++)
		glVertex3dv((m_vecCorners.at(i)).GetArray());
	glEnd();

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);

	glDisable(GL_BLEND);
	glPopAttrib();

}

void CPlane::AddCorner(CVector3d pCorner)
{
	m_vecCorners.push_back(pCorner);
}

void CPlane::SetCorner(int index, CVector3d& vcorner)
{
	std::vector<CVector3d>::iterator it=m_vecCorners.begin();

	*(it+index)=vcorner;
}

CVector3d CPlane::GetCorner(int index)
{
	return m_vecCorners.at(index);
}



void CPlane::SetColor(double r, double g, double b)
{
	m_vColor.Set(r,g,b);
}

double CPlane::CalcValue(CVector3d& point)
{
	return m_a*point[0]+m_b*point[1]+m_c*point[2]+m_d;
}

int CPlane::GetCornerNum(void)
{
	return m_vecCorners.size();
}
