#pragma once
#include "..\OpenGLBasic\Arcball\Vector3d.h"
#include "..\OpenGLBasic\Arcball\Matrix44.h"
class CFrame
{
public:
	CFrame(void);
	CFrame(const CVector3d& v0, const CVector3d& v1,const CVector3d& v2, const CVector3d& o);
	CFrame(const CFrame& frame);
	~CFrame(void);
private:
	CVector3d Axis[3];
	CVector3d Orig;

public:

public:

	void Display(void);
	void SetAxis(int index, double a, double b, double c);
	CVector3d& operator[](int i)
	{
		return Axis[i];
	}
public:
	static CFrame Identity(void);
	void SetOrigin(CVector3d& o);
	void SetAxises(CVector3d& x, CVector3d& y, CVector3d& z);
	CVector3d GetOrigin(void);
	CMatrix44 TransformMatrix(CFrame& frame);
	const CVector3d& GetX()const{return Axis[0];}
	const CVector3d& GetY()const{return Axis[1];}
	const CVector3d& GetZ()const{return Axis[2];}
	const CVector3d& GetO()const{return Orig;}

};
