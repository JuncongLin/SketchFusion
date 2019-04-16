#pragma  once
#include "ImplicitSurface.h"
class CRBFSurface :
	public CImplicitSurface
{
public:
	CRBFSurface(void);
	~CRBFSurface(void);
private:
	double* h;//AX=B 的矩阵B  k+4 个
	double* phi;//
	double p[4];//线性多项式的系数 4个
	double* c;//RBF Center的几何坐标
	double* d;//基函数的系数k个
	double* lambda;
	int cur;//当前加入Center数
	int k;//RBF Center个数k个
	int m_iInitNum;

public:
	double GetValueAt(CVector3d point);
	double GetValueAt(double point[]);
	CVector3d GradientAt(CVector3d point);
	void Initialize(int num);
	void Solve(void);
	void AddConstraint(const double* coord, double value, double lam);
};
