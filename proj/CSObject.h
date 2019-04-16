#pragma once


typedef CRITICAL_SECTION CriticalSection;
typedef HANDLE PrimitiveSemaphore;
typedef HANDLE PrimitiveThread;

class CCSObject
{
public:
	CCSObject(void);
	~CCSObject(void);
	void Enter(void);
	void Leave(void);
private:
	CriticalSection m_cs;
};
class CCS
{
public:
	CCS(CCSObject& csobject);
	~CCS();

	CCSObject& m_csobject;
};
