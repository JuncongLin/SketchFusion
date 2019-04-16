#pragma once
#include <vector>
using namespace std;
#include "..\OpenGLBasic\Arcball\Vector3d.h"
#include ".\Frame.h"
class CPrincipleAnalysis
{
public:
	CPrincipleAnalysis(void);
	~CPrincipleAnalysis(void);
	void Execute(vector<CVector3d>& points,double weight,CFrame* pFrame, double eimag[3]);
};
