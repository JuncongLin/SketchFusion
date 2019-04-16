#pragma  once
#include "ImplicitSurface.h"
class CRBFSurface :
	public CImplicitSurface
{
public:
	CRBFSurface(void);
	~CRBFSurface(void);
private:
	double* h;//AX=B �ľ���B  k+4 ��
	double* phi;//
	double p[4];//���Զ���ʽ��ϵ�� 4��
	double* c;//RBF Center�ļ�������
	double* d;//��������ϵ��k��
	double* lambda;
	int cur;//��ǰ����Center��
	int k;//RBF Center����k��
	int m_iInitNum;

public:
	double GetValueAt(CVector3d point);
	double GetValueAt(double point[]);
	CVector3d GradientAt(CVector3d point);
	void Initialize(int num);
	void Solve(void);
	void AddConstraint(const double* coord, double value, double lam);
};
