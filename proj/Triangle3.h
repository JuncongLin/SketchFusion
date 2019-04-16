#pragma once
#include "Point3.h"
class CTriangle3
{
public:
	CTriangle3(void);
	CTriangle3(const CTriangle3 &tri);  
	~CTriangle3(void);
protected:
	CPoint3 m_A, m_B, m_C;


public:
	CPoint3& operator [](int i);
	void Set_A(const CVector3d& pos);
	void Set_B(const CVector3d& pos);
	void Set_C(const CVector3d& pos);
	const CPoint3& GetA()const{return m_A;} 
	const CPoint3& GetB()const{return m_B;} 
	const CPoint3& GetC()const{return m_C;} 

};
