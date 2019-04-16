#pragma once

class CHeapNode
{
public:
	CHeapNode(void);
	~CHeapNode(void);
private:
	float v;

public:
	void SetValue(float value);
	float GetValue(void);
};
bool SmallerHeapNode(CHeapNode* a ,CHeapNode* b);
bool GreaterHeapNode(CHeapNode* a ,CHeapNode* b);
