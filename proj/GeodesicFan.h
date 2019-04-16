#pragma  once
#include "..\OpenGLBasic\Arcball\Vector3d.h"
#include "..\OpenGLBasic\Vertex.h"
#include "..\OpenGLBasic\Face.h"
#include <vector>
#define MAX_ADJ_TRIS			20
#define SAMPLES_PER_SPOKE		2

class CGeodesicFan
{
public:
	CGeodesicFan(void);
	~CGeodesicFan(void);
private:
	CVector3d littleTPoints[8];

	vector<CVector3d*> intersectP[8];

	vector<CFace*> intersectF[8];

	CVector3d intersectN[MAX_ADJ_TRIS][8];

	CVertex* intersectEndpoint1;
	CVertex* intersectEndpoint2;

	CFace* AdjacentFace;

	CVector3d SampleLC[SAMPLES_PER_SPOKE][8];

	CFace* SampleF[SAMPLES_PER_SPOKE][8];
	int Mask[SAMPLES_PER_SPOKE][8];

public:
	bool Construct(CVertex* pVertex,double FanLength);
private:
	void Spindles(CVertex *pVertex);
	bool Iterate(CVertex* pVertex,double FanLength);
	bool planeIntersect(CVertex* pVertex,int tindex,double FanLength,double& Length);
	bool planeIntersectN(int tindex, int iteration, double FanLength,double& Length);
public:
	double CompareScalar(CGeodesicFan* pGeoFan);
	double CompareVector(CGeodesicFan* pGeoFan,const std::vector<CVector3d*>& vMetrics);
	void TransformMatrix(double* matrix,int spin);
	bool Sampling(double FanLen);
	CVector3d GetSampleLC(int layer, int spin);
	CFace* GetSampleF(int layer, int spin);
	void Display(void);
	int NumOfKnownSample(void);
};
