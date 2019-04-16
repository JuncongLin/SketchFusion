#pragma once
#include "heapnode.h"
#include <functional>

#define HEAPNODE_MAXEDGENUM		10

class CWEHeapNode :
	public CHeapNode
{
public:
	CWEHeapNode(void);
	~CWEHeapNode(void);
	int qIndex,pIndex;

	CWEHeapNode* edgeList[HEAPNODE_MAXEDGENUM];	// pointers to next node
	double edgeWeights[HEAPNODE_MAXEDGENUM];		// the weights of each edge
	int edgeNum;

	CWEHeapNode* previousNode;	// this pointer is utilized to store the shortest path

};
//class GreaterHeapNode : public std::binary_function<CWEHeapNode*,CWEHeapNode*,bool>
//{
//public:
//	bool operator () ( CWEHeapNode* a,CWEHeapNode* b )
//	{
//		return a->GetValue()>b->GetValue();
//	}
//
//};
//class SmallerHeapNode : public std::binary_function<CWEHeapNode*,CWEHeapNode*,bool>
//{
//public:
//	bool operator () ( CWEHeapNode* a,CWEHeapNode* b )
//	{
//		return a->GetValue()<b->GetValue();
//	}
//
//};
