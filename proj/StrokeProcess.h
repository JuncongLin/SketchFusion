#pragma once
#include "Stroke.h"
class CStrokeProcess
{
public:
	CStrokeProcess(void);
	~CStrokeProcess(void);
	void BSplineFitting(CStroke* pStroke);
};
