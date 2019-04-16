#include "StdAfx.h"
#include ".\heapnode.h"

CHeapNode::CHeapNode(void)
{
}

CHeapNode::~CHeapNode(void)
{
}

void CHeapNode::SetValue(float value)
{
	v=value;
}

float CHeapNode::GetValue(void)
{
	return v;
}
bool SmallerHeapNode(CHeapNode* a ,CHeapNode* b)
{
	return a->GetValue()<b->GetValue();
}
bool GreaterHeapNode(CHeapNode* a ,CHeapNode* b)
{
	return a->GetValue()>b->GetValue();
}