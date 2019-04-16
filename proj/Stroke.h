#pragma once
#include "..\OpenGLBasic\Arcball\Vector3d.h"
#include "global.h"
#include <vector>
#include "..\poissonshaperepresentation\material.h"
#include "..\OpenGLBasic\Arcball\Matrix44.h"
struct Point3D{
	float x,y,z;
};
struct Point2D{
	float x,y;
};
struct BOX
{
	Point2D pt[4];
};

using namespace std;

class CStroke
{
public:
	CStroke(void);
	CStroke(int num);
	~CStroke(void);
protected:
	vector <CVector3d> m_vecPoints;
	vector<CVector3d> m_vecNormals;
	vector<int> m_vecCriticalPoints;
	float m_fLineColor[3];
	float m_fPointColor[3];

public:
	void AddPoint(double x, double y, double z);
	virtual void Display();
	int GetPointsNum(void);
	void GetPoint(int index, double* x, double* y, double* z);
public:
	void SetLineColor(float r, float g, float b);
	void SetPointColor(float r, float g, float b);
	CVector3d GetPoint(int index);
	void AddPoint(CVector3d &pt);
	void ModifyPoint(int index, CVector3d& ncoord);
private:
	CMaterial m_Material;
public:
	void Clear(void);
	double Length(void);
	void AddNormal(CVector3d& norm);
	CVector3d GetNormal(int index);
	void GenerateNormalByInterpolation(CVector3d& norm1,CVector3d& norm2);
	void Optimization(void);
	void Transform(CMatrix44& mat);
private:
	int m_iDisplayStyle;
public:
	void SetDisplayStyle(int style);
	void AddCriticalPoint(int index);
	void ClearCriticalPoints(void);
	int GetCriticalPointNum(void);
	CVector3d GetCriticalPointByIndex(int index);
private:
	bool m_bSelected;
public:
	void SetSelected(bool flag);
};
