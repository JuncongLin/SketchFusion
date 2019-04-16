// RBFSolver.h: interface for the RBFSolver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBFSOLVER_H__313FCD86_8589_44FD_9DBD_134493AB19E0__INCLUDED_)
#define AFX_RBFSOLVER_H__313FCD86_8589_44FD_9DBD_134493AB19E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\OpenGLBasic\Arcball\Vector3d.h"
class RBFSolver  
{
public:
	void MEMRelease();
	void Solve(int istart,float fPara);
	void Test(double* coord,double value);
	void ResetSystem();
	void InitSystem(int num);
	void AddRBFCenter(const double* Coord,double value);
	float GetValueAt(double coord[]);
	float GetValueAt(CVector3d* v);
	RBFSolver();
	virtual ~RBFSolver();

private:
	double InitalParticle[10][3];
	bool m_bSolve;
	double* h;//AX=B �ľ���B  k+4 ��
	double* phi;//
	double p[4];//���Զ���ʽ��ϵ�� 4��
	double* c;//RBF Center�ļ�������
	double* d;//��������ϵ��k��
	int cur;//��ǰ����Center��
	int k;//RBF Center����k��
	int m_iInitNum;
public:
	CVector3d* InitialSample(int i);
	CVector3d* Grad(CVector3d* v);
	void SetInitialSample(double x, double y, double z);
	void AddInitialSample(double x, double y, double z);
	int GetInitSampleNum(void);
};

#endif // !defined(AFX_RBFSOLVER_H__313FCD86_8589_44FD_9DBD_134493AB19E0__INCLUDED_)
