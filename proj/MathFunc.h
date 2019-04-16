void Normalize(double* pVector,int num);

void getMulti(double* a,int row_a,int col_a,double* b,int row_b,int col_b,double* c);

void getCross(double* x,double* y,double* z);

void GetR(double* q1,double* q2,int num,double* R);

void GetT(double* p1,double* p2,double* R,double* T);

void Trans(double* coord,double* R,double *T);

float CalcLen(float* pCoord1,float* pCoord2);
double CalcLen(double* pCoord1,double* pCoord2);

void  getU(double *pLCS,float* pV,float* pCV,double* u,double t);

void  getV(double *pLCS,double* u,float* pCV,double* v,double wi);

void  GetRBFCoeff(int num,double* c,double* phi,double* h,double* d,double* p);

float getProjectLen(float* pt0,float* pt1,float* pt2);
float getProjectDist(float* pt0,float* pt1,float* pt2);

void   GetRotate(double* norm,double* vec,double* t);

void   AdjusttoProject(float* pt,float* start,float* end);