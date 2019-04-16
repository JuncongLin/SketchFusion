#pragma once
#include "heapnode.h"
#include "..\OpenGLBasic\Vertex.h"
#include "GeodesicFan.h"
class CGeoFanNode :
	public CHeapNode
{
public:
	CGeoFanNode(void);
	~CGeoFanNode(void);
	CGeodesicFan *m_pGeoFan;
	CVertex *m_pVertex;

};
