#include "StdAfx.h"
#include ".\transitionprimitive.h"

CTransitionPrimitive::CTransitionPrimitive(void)
: m_bLocked(true)
,m_iLocatingStyle(0)
{
	m_Material.SetProperty("GlassBlue");
}

CTransitionPrimitive::~CTransitionPrimitive(void)
{
}

void CTransitionPrimitive::Display(void)
{

}


void CTransitionPrimitive::SetCenter(CVector3d c)
{
	m_vCenter=c;
}

CVector3d CTransitionPrimitive::GetCenter(void)
{
	return m_vCenter;
}

void CTransitionPrimitive::Lock(void)
{
	m_bLocked=true;
}

void CTransitionPrimitive::Unlock(void)
{
	m_bLocked=false;
}

bool CTransitionPrimitive::IsLocked(void)
{
	return m_bLocked;
}

void CTransitionPrimitive::SetCurLocator(double x, double y, double z)
{
	m_vCurLocator.Set(x,y,z);
}


int CTransitionPrimitive::LocatingStyle(void)
{
	return m_iLocatingStyle;
}

CVector3d CTransitionPrimitive::GetCurLocator(void)
{
	return m_vCurLocator;
}

void CTransitionPrimitive::SaveLocator(void)
{
	m_vecLocators.push_back(m_vCurLocator);
}

CVector3d CTransitionPrimitive::GetLocatorByIndex(int index)
{
	return m_vecLocators.at(index);
}

void CTransitionPrimitive::SetDefLocator(void)
{
}

void CTransitionPrimitive::SetAutoLocator(CVector3d& dir)
{
}

CVector3d CTransitionPrimitive::GetCurDirection(void)
{
	return CVector3d();
}

void CTransitionPrimitive::SetLocatingStyle(int style)
{
	m_iLocatingStyle=style;
}

int CTransitionPrimitive::GetLocatingStyle(void)
{
	return m_iLocatingStyle;
}

void CTransitionPrimitive::SetDirection(CVector3d& dir)
{
	m_vDirection=dir;
}

CVector3d& CTransitionPrimitive::GetDirection(void)
{
	return m_vDirection;
}

int CTransitionPrimitive::GetType(void)
{
	return m_iType;
}

void CTransitionPrimitive::SetLocator(int index, CVector3d& loc)
{
	*(m_vecLocators.begin()+index)=loc;
}

int CTransitionPrimitive::GetLocatorNum(void)
{
	return m_vecLocators.size();
}

void CTransitionPrimitive::AddLocator(CVector3d loc)
{
	m_vecLocators.push_back(loc);
}



void CTransitionPrimitive::OffsetLocator(CVector3d& raw)
{
}

void CTransitionPrimitive::ModifyLocator(int index,CVector3d& nLoc)
{
	vector<CVector3d>::iterator it=m_vecLocators.begin()+index;

    *(it)=nLoc;

}
