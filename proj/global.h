#pragma once
#include "..\OpenGLBasic\Arcball\Vector3d.h"

#ifndef M_PI
#define M_PI 3.141592
#endif

#ifndef FLT_MAX
#define FLT_MAX 10000
#endif


#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define Max3(x,y,z) (x>y && x>z ? x : (y>z ? y : z))
#define Min3(x,y,z) (x<y && x<z ? x : (y<z ? y : z))
#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}

typedef enum{MO_NONE,MO_ROTATE,MO_TRANSLATE,MO_SCALE,MO_SILHOUETTE_SMOOTH,MO_SILHOUETTE_ADJUST,MO_STROKE_SILHOUETTE,MO_STROKE_CUT,MO_STROKE_SELECT,MO_STROKE_SEED,MO_STROKE_STRUCTURE,MO_SELECT_VERTEX,MO_SELECT_EDGE,MO_SELECT_FACE,MO_SELECT_BOUNDARY,MO_SELECT_STROKE,MO_SELECT_OBJECT,MO_SELECT_RECTPATCH,MO_SELECT_POLYPATCH,MO_ADD_FACE,MO_TPRIMITIVE_LOCATE}MouseOp;
typedef enum{OM_NONE,OM_SCISSOR,OM_FUSION}OperationMode;
typedef enum{SM_NONE,SM_PART,SM_WHOLE}SceneMode;
typedef enum{ODM_COMPOSED,ODM_SOLID,ODM_POINT,ODM_LINE,ODM_SILHOUETTE,ODM_METRIC}ObjectDisplayMode;
typedef enum{SPT_MODEL,SPT_TPRIMITIVE,SPT_STROKE,SPT_PLANE}ScenePrimitiveType;
typedef enum{SST_FULL,SST_PARTIAL,SST_ISOLATE}SilhoueteStrokeType;

CVector3d Ortho3(const CVector3d &arg);
double my_random(void);
void ClampInt(int *x,int max);
float fast_exp(float);         // Faster approx. to exp function
double CalcBernstein(int n,int i, double t);
