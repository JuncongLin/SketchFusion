#pragma once
#include "..\openglbasic\arcball\vector3d.h"
#include "..\PoissonShapeRepresentation\Material.h"
#include "global.h"
#include "Ray3.h"
#include <vector>
using namespace std;
class CTransitionPrimitive
{
public:
	CTransitionPrimitive(void);
	~CTransitionPrimitive(void);
	virtual void Display(void);
protected:
	int m_iType;  //0 Transition Sphere, 1 Transition Cylinder
	int m_iLocatingStyle;
	bool m_bLocked;
	CVector3d m_vCenter;
	CVector3d m_vCurLocator;
	CVector3d m_vDirection;
	vector<CVector3d> m_vecLocators;
	CMaterial m_Material;
public:
	void SetCenter(CVector3d c);
	CVector3d GetCenter(void);
	void Lock(void);
	void Unlock(void);
	bool IsLocked(void);
	void SetCurLocator(double x, double y, double z);
	int LocatingStyle(void);
	CVector3d GetCurLocator(void);
	void SaveLocator(void);
	CVector3d GetLocatorByIndex(int index);
	virtual void SetDefLocator(void);
	virtual void SetAutoLocator(CVector3d& dir);
	virtual void SetManualLocator(CRay3& ray){};
	virtual void SetManualLocator(int index,CRay3& ray){};
	virtual CVector3d GetCurDirection(void);
	void SetLocatingStyle(int style);
	int GetLocatingStyle(void);
	void SetDirection(CVector3d& dir);
	CVector3d& GetDirection(void);
	int GetType(void);
	void SetLocator(int index, CVector3d& loc);
	int GetLocatorNum(void);
	void AddLocator(CVector3d loc);
	virtual CVector3d GetLocDirectionByIndex(int index)=0;
	virtual void OffsetLocator(CVector3d& raw);
	void ModifyLocator(int index,CVector3d& nLoc);
};
