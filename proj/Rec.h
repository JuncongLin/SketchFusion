#pragma once
#include "..\OpenGLBasic\Vertex.h"


#define MAXN 350000
#define FMAXN 500000
#define FNUM 3
#define XMAX 20
#define YMAX 20
#define ZMAX 20
#define MAXFACE 10



struct LABEL
{
	int i;
	int j;
	int k;
};

struct PNT
{
	double x;
	double y;
	double z;
public:
	friend PNT operator *(double n,PNT& p);
	friend PNT operator /(PNT &p, double d);
	PNT operator -(PNT p);
	PNT operator + (PNT p);
	double operator * (PNT p);
};

struct NORMAL
{
	double x;
	double y;
	double z;
	int num;
};

struct FACE
{
	int vnum;      // the number of vertexes belonging to the face
	int v[FNUM];   // the coordinate of the vertex
	int vn[FNUM];  // the normal at the vertex
};

struct EDGELST;
struct FACELST;

struct PNTLST
{
	int pntno;
	PNTLST* nxt;
};


struct EDGELST
{
	int edgeno;  // the serial number of the edge
	int pcsdsgn; // 0: boundary edge; 1: fixed edge
	int pntno[2]; // the end points of the edge
	FACELST* face[2]; // the faces adjacent to the edge
	EDGELST* nxt;
};

struct FACELST
{
	int faceno; // the serial number of the face
	int pntno[3]; // the vertexes of the triangular face
	PNT norml; // the normalized normal of the triangular face
	FACELST* nxt;
};

struct EDGEPNT
{
	EDGELST *pedge;
	EDGEPNT *nxt;
};

struct FACEPNT
{
	FACELST *pface;
	FACEPNT *nxt;
};

struct RCNPNT
{
	PNT coord;   // the coordinates of the point
	int pcsdsgn; // Whether the point is processed
	// 0: nonfixed point 1: fixed point
	EDGELST *pedge; // the pointer of the edges adjacent to the point
	FACEPNT *pface; //the faces adjacent to the point
	CVertex* pVertex;
};

struct VOXLST
{
	int i;
	int j;
	int k;
	VOXLST* nxt;
};

//template <class T> void DeleteLnk(T **phead);

struct BOUNDBOX
{
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	double zmin;
	double zmax;
};

#define STACKMAX 8001
#define FIXEDGE 1
#define BNDRYEDGE 2

#define WM_DISPLAY_POLYGON  WM_USER+30

template<class T> void DeleteLnk(T **phead)
{
	T *ph, *od;
	ph = *phead;
	od = ph;
	while(ph)
	{
		od = ph;
		ph = ph->nxt;
		delete od;
	}
	*phead = NULL;
}

class CRec  
{
public:
	PNT m_ptStep;
	int m_iSgnVoxel[XMAX][YMAX][ZMAX];
	LABEL m_lStack[STACKMAX];
	int m_iStackPtr;
	EDGEPNT* m_pBdyEdgeHead;
	PNTLST* m_pVoxel[XMAX][YMAX][ZMAX];//storage the points contained in the voxel 
	BOUNDBOX m_bbBoundBox;

public:
	int StatsPntNumBtwRtos(double minr, double maxr);
	void CalcPntRatio(double& maxrat, double& minrat);
	bool HaveIntsctBtwLineTri(int ls1, int ls2, int ts1, int ts2, int ts3);
	bool HaveIntsctBtwTris(int s1, int s2, int s3, FACELST *fl);
	bool PntIsVertex(int nser, int ser1, int ser2, FACEPNT *pl);
	double CRec::GetMinEdgeLen(int s);
	double CalcWeightSum(PNT pt,EDGELST* pedge,double lij,double co1);
	double CalcR(PNT pt, EDGELST* pedge,double len);
	double GetLengthRatio(int ser);
	FACELST* CreateNewTriangle(int s1, int s2, int s3);
	PNT GetMeanNormal(FACELST *pf);
	double GetMaxEdgeLen(int s);
	int SearchNewVertex(VOXLST *phead, EDGELST *pedge);
	int LookforUnprcsVertx();
	void LookforSeedTriangle(int ser);
	void DeleteElmt(EDGELST *ce, EDGEPNT **ph);
	double CalcSum(PNT pt, EDGELST *pedge);
	BOOL PntAndEdgeInSameSide(int ser,EDGELST *pedge);
	EDGELST* SearchAdjEdge(int s1,int s2,int oppv,PNT norl,PNT pt);
	int SearchSeedVertex(int fnum,int sign[],PNT norm[],PNT pnt[], VOXLST* phead, EDGELST* pedge);
	double DiffMinimum(PNT p1, PNT p2, PNT pt, double s1);
	FACELST* m_pfCurFacePtr;
	void AddElmtToLnk(VOXLST *cv, VOXLST **phead);
	void GetCubeVertex(PNT vertex[],PNT bom);
	PNT GetMidVect(PNT vect);
	BOOL PntInPoly(int fnum,int sign[],PNT norml[], PNT pnt[],PNT pt);
	LABEL Pop();
	void Push(LABEL lw);
	void RestoreSign(VOXLST *ph);
	void PushAdjacentVoxel(LABEL lw);
	VOXLST* CreateVoxelList(int fnum,int sign[],PNT norl[],PNT pnt[],PNT midpt,EDGELST *e,double r);
	inline void AddFaceToPntField(int ser, FACELST *pf);
	void DeleteLink();
	void GenerateTopology();
	void Run();
	void AdjustPntState(int ser);
	FACELST* CreateNewTriangle(int s1,int s2,int s3,PNT nrl);
	void AddElmtToLnk(EDGELST *pe, EDGEPNT **phead);
	void SoftDeleteElmt(EDGELST *pe, EDGEPNT *phead);
	void SetEdgeToFixed(EDGELST *pe, FACELST *pf, int type);
	void CreateEdge(int s1, int s2, FACELST *pf);
	void AddTriangle(int ser, EDGELST *pedge);
	int Sign(double num);
	double CalcHigh(PNT pt, EDGELST *edge);
	int SearchNewVertex(int fnum,int sign[],PNT norm[],PNT pnt[], VOXLST* phead, EDGELST* pedge);
	double FaceEquation(PNT norm, PNT point, PNT pt);
	PNT CrossNorml(PNT v1, PNT v2);
	int LookforNewVertex(EDGELST* pedge, double r);
	VOXLST* LookforLastElmt(VOXLST *head);
	BOOL TheVoxelIsNotExist(int i,int j,int k,VOXLST *head);
	double CalcRadius(int s1, int s2);
	VOXLST* CreateVoxelList(EDGELST* edge, double r);
	int m_iEdgeNo;
	int m_iFaceNo;
	void CreateSeedTriangle(int s1,int s2,int s3,PNT norml);
	void LookforSeedTriangle();
	int m_iZMaxPnt;
	PNTLST* LookforLastElmt(PNTLST* head);
	void DivBoundBox();
	void GetBoundBox();

	CRec();
	virtual ~CRec();

	int m_iFaceSer;
	int m_iVertNum;
	RCNPNT m_ptPoint[MAXN]; // the points for reconstruction
	FACELST* m_pFaceHead[MAXFACE]; // the head of the face list
	int m_iBndryEdgeNum;
	EDGEPNT *m_pTrueBdyEdgeHead;
	double m_dPntRatio[MAXN];
	void Display(void);
	FACE m_faceFace[FMAXN];  //the face in the model
	int m_iFaceNum;         //the number of the face in the model
	PNT m_pntVertNorm[MAXN];//the normal at each vertex from file
	PNT CalcNrml(int ser);
private:
	float m_fColor[3];
public:
	void SetColor(float r, float g, float b);
	void Inverse(void);
	void AddPoint(CVector3d coord, CVertex* pVertex=NULL);
};

