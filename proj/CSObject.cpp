#include "StdAfx.h"
#include ".\csobject.h"

CCSObject::CCSObject(void)
{
	InitializeCriticalSection(&m_cs);

}

CCSObject::~CCSObject(void)
{
	DeleteCriticalSection(&m_cs);
}

void CCSObject::Enter(void)
{
	EnterCriticalSection(&m_cs);
}

void CCSObject::Leave(void)
{
	LeaveCriticalSection(&m_cs);
}

CCS::CCS(CCSObject& csobject)
:m_csobject(csobject)
{
	m_csobject.Enter();
}
CCS::~CCS()
{
    m_csobject.Leave();
}