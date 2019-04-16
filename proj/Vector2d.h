#pragma once

class CVector2d
{
public:
	CVector2d(void);
	CVector2d(const CVector2d& ivec);
	CVector2d(double x, double y);
	~CVector2d(void);

protected:
	double vec[2];  // Storage for the vector components
public:
	void Set(const double x, const double y){ vec[0] = x; vec[1] = y;}

	// Data access using indices
	double&       operator[](int i)       { return (vec[i]); }
	const double& operator[](int i) const { return (vec[i]); }

	// Operators
	CVector2d& operator+=(const CVector2d& rVector);
	CVector2d& operator+=(const CVector2d* pVector);
	CVector2d& operator-=(const CVector2d& rVector);
	CVector2d& operator-=(const CVector2d* pVector);
	CVector2d& operator*=(double d);
	CVector2d& operator/=(double d)
	{ return *this *= (1.f/d); }

	// Per coordinate (explicit inline functions)
	void x(double newX) { vec[0] = newX; }
	void y(double newY) { vec[1] = newY; }

	// Data access (explicit inline functions)
	double x() const { return (vec[0]); }
	double y() const { return (vec[1]); }

};
