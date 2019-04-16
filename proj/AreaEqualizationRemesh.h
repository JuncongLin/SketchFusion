#pragma once
#include "MeshOptimization.h"

class CAreaEqualizationRemesh: public CMeshOptimization
{
public:
	CAreaEqualizationRemesh(void);
	~CAreaEqualizationRemesh(void);
	int Run(double TLength);
	int EdgeSplitByLength(double criterion);
public:
	void EdgeCollapseByLength(double criterion);
	void EdgeSwapByValence(void);
	void VertexRelocationUniform(double dampingFactor, int nSteps);
	void SetEntity(CEntity* pEntity);
};
