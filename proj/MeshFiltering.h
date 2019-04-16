#pragma once
#include "..\OpenGLBasic\Solid.h"
class CMeshFiltering
{
public:
	CMeshFiltering(void);
	~CMeshFiltering(void);
	static void Uniform(CSolid* pModel, vector<CVector3d*>& ResCoords);
	static void CurvFlowLapImplicit(CSolid* pModel, vector<CVector3d*>& ResCoords,int iter);
};
