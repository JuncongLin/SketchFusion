#pragma once
#include "Entity.h"

class CMeshOptimization
{
public:
	CMeshOptimization(void);
	~CMeshOptimization(void);
	bool EdgeSplit(CEdge* pE);
	bool EdgeCollapse(CEdge* pE);
	bool EdgeSwap(CEdge* pE);
	void SetEntity(CEntity* pEntity);
protected:
	CEntity *m_pEntity;
};
