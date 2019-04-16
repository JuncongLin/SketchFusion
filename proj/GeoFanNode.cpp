#include "StdAfx.h"
#include ".\geofannode.h"

CGeoFanNode::CGeoFanNode(void)
{
	m_pGeoFan=NULL;
	m_pVertex=NULL;
}

CGeoFanNode::~CGeoFanNode(void)
{
	delete m_pGeoFan;
}
