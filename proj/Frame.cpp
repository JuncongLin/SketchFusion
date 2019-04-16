#include "StdAfx.h"
#include ".\frame.h"

CFrame::CFrame(void)
{
}
CFrame::CFrame(const CFrame& frame)
{
	Axis[0]=frame.GetX();
	Axis[1]=frame.GetY();
	Axis[2]=frame.GetZ();
	Orig=frame.GetO();
}

CFrame::CFrame(const CVector3d& v0, const CVector3d& v1,const CVector3d& v2, const CVector3d& o)
{
	Axis[0]=v0;
	Axis[1]=v1;
	Axis[2]=v2;
	Orig=o;
}

CFrame::~CFrame(void)
{
}

CFrame CFrame::Identity(void)
{
	return CFrame(CVector3d(1,0,0),CVector3d(0,1,0),CVector3d(0,0,1),CVector3d(0,0,0));
}

void CFrame::Display(void)
{

	float matSpecular[4];
	float matDiffuse[4];
	float matAmbient[4];

	glEnable(GL_COLOR_MATERIAL);

	//matSpecular[0]=0.6;
	//matSpecular[1]=0.6;
	//matSpecular[2]=0.6;
	//matSpecular[3]=1.0;

	//matAmbient[0]=0.3;
	//matAmbient[1]=0.3;
	//matAmbient[2]=0.3;
	//matAmbient[3]=1.0;
	//glMaterialfv( GL_FRONT, GL_AMBIENT,   matAmbient);
	//glMaterialf(GL_FRONT,GL_SHININESS,100);
	//glMaterialfv(GL_FRONT,GL_SPECULAR,matSpecular);

	glPushMatrix ();
	glTranslatef(Orig.x(),Orig.y(),Orig.z());


	glColor3f(0.6,0.6,0.6);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,matDiffuse);
	gluSphere(gluNewQuadric(),0.01,50,50);

//	glPushMatrix ();
//	glTranslatef(3.0,3.0,3.0);
//	gluSphere(gluNewQuadric(),0.5,50,50);
//	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	float mx[]={
		Axis[1].x(),Axis[1].y(),Axis[1].z(),0,
		Axis[2].x(),Axis[2].y(),Axis[2].z(),0,
		Axis[0].x(),Axis[0].y(),Axis[0].z(),0,
		0,0,0,1
	};
	glMultMatrixf(mx);
	gluCylinder(gluNewQuadric(),0.01,0.01,0.3,50,50);
	glPushMatrix ();
	glTranslatef(0.0,0.0,0.3);
	gluCylinder(gluNewQuadric(),0.02,0.0,0.1,50,50);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0,1.0,0.0);
	float my[]={
		Axis[2].x(),Axis[2].y(),Axis[2].z(),0,
		Axis[0].x(),Axis[0].y(),Axis[0].z(),0,
		Axis[1].x(),Axis[1].y(),Axis[1].z(),0,
		0,0,0,1
	};
	glMultMatrixf(my);

	gluCylinder(gluNewQuadric(),0.01,0.01,0.3,50,50);
	glPushMatrix ();
	glTranslatef(0.0,0.0,0.3);
	gluCylinder(gluNewQuadric(),0.02,0.0,0.1,50,50);
	glPopMatrix();
	glPopMatrix();

	glColor3f(0.0,0.0,1.0);
	float mz[]={
		Axis[0].x(),Axis[0].y(),Axis[0].z(),0,
		Axis[1].x(),Axis[1].y(),Axis[1].z(),0,
		Axis[2].x(),Axis[2].y(),Axis[2].z(),0,
		0,0,0,1
	};
	glMultMatrixf(mz);
	gluCylinder(gluNewQuadric(),0.01,0.01,0.3,50,50);
	glPushMatrix ();
	glTranslatef(0.0,0.0,0.3);
	gluCylinder(gluNewQuadric(),0.02,0.0,0.1,50,50);
	glPopMatrix();

	glPopMatrix ();
	glDisable(GL_COLOR_MATERIAL);

}

void CFrame::SetAxis(int index, double a, double b, double c)
{
	Axis[index].Set(a,b,c);
	Axis[index].Normalize();
	
}

void CFrame::SetOrigin(CVector3d& o)
{
	Orig=o;
}

void CFrame::SetAxises(CVector3d& x, CVector3d& y, CVector3d& z)
{
	Axis[0]=x;
	Axis[1]=y;
	Axis[2]=z;
}

CVector3d CFrame::GetOrigin(void)
{
	return Orig;
}

CMatrix44 CFrame::TransformMatrix(CFrame& frame)
{
	CMatrix44 i2gmat,g2smat,commat;
	double data[16];

	CVector3d ix=(frame)[0];
	CVector3d iy=(frame)[1];
	CVector3d iz=(frame)[2];

	data[0]=ix[0];data[1]=ix[1];data[2]=ix[2];data[3]=0.0;
	data[4]=iy[0];data[5]=iy[1];data[6]=iy[2];data[7]=0.0;
	data[8]=iz[0];data[9]=iz[1];data[10]=iz[2];data[11]=0.0;
	data[12]=0.0;data[13]=0.0;data[14]=0.0;data[15]=1.0;
	i2gmat.Set(data);

	data[0]=Axis[0][0];data[1]=Axis[1][0];data[2]=Axis[2][0];data[3]=0.0;
	data[4]=Axis[0][1];data[5]=Axis[1][1];data[6]=Axis[2][1];data[7]=0.0;
	data[8]=Axis[0][2];data[9]=Axis[1][2];data[10]=Axis[2][2];data[11]=0.0;
	data[12]=0.0;data[13]=0.0;data[14]=0.0;data[15]=1.0;

	g2smat.Set(data);

	commat=i2gmat.MultRight(g2smat);
	return commat;


}
