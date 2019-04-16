
#define MAX_ADJ_TRIS			20
#define SAMPLES_PER_SPOKE		2

typedef struct 
{
	double littleTxpoints[8];
	double littleTypoints[8];
	double littleTzpoints[8];

	double ** intersectx;
	double ** intersecty;
	double ** intersectz;

//	CFace *** intersectF;

	double intersectNormX[MAX_ADJ_TRIS][8];
	double intersectNormY[MAX_ADJ_TRIS][8];
	double intersectNormZ[MAX_ADJ_TRIS][8];

//	CVertex* intersectEndpoint1;
//	CVertex* intersectEndpoint2;

	int IntersectionCount[8];

//	CFace* AdjacentFace;

	double Length;

	double SampleX[SAMPLES_PER_SPOKE][8];
	double SampleY[SAMPLES_PER_SPOKE][8];
	double SampleZ[SAMPLES_PER_SPOKE][8];

//	CFace* SampleF[SAMPLES_PER_SPOKE][8];

}GeodesicFan;

class CLocalSimilarityOperator
{
public:
	CLocalSimilarityOperator(void);
	~CLocalSimilarityOperator(void);
};
