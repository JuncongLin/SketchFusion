// MyUnit.cpp: implementation of the MyUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "MyUnit.h"
#include "math.h"
#include "RBFSolver.h"
#include <fstream>
using namespace std;
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
typedef struct intlist {	   /* list of integers */
	int i;			   /* an integer */
	struct intlist *next;	   /* remaining elements */
} INTEGERLIST;

typedef struct intlists {	   /* list of list of integers */
	INTEGERLIST *list;		   /* a list of integers */
	struct intlists *next;	   /* remaining elements */
} INTEGERLISTS;

 extern TRIS gtris;
 extern VERTICES gvertices;  
 extern RBFSolver solver;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyUnit::MyUnit()
{
	/*清空存放数据的文件*/
	DeleteFile("data.txt");
}

MyUnit::~MyUnit()
{

}

////////////////////////////////////////////////
//
////////////////////////////////////////////////

/**** Cubical Polygonization (optional) ****/

#define LB	0  /* left bottom edge	*/
#define LT	1  /* left top edge	*/
#define LN	2  /* left near edge	*/
#define LF	3  /* left far edge	*/
#define RB	4  /* right bottom edge */
#define RT	5  /* right top edge	*/
#define RN	6  /* right near edge	*/
#define RF	7  /* right far edge	*/
#define BN	8  /* bottom near edge	*/
#define BF	9  /* bottom far edge	*/
#define TN	10 /* top near edge	*/
#define TF	11 /* top far edge	*/

static INTEGERLISTS *cubetable[256];
//edge: LB, LT, LN, LF, RB, RT, RN, RF, BN, BF, TN, TF
static int corner1[12]={LBN,LTN,LBN,LBF,RBN,RTN,RBN,RBF,LBN,LBF,LTN,LTF};
static int corner2[12] = {LBF,LTF,LTN,LTF,RBF,RTF,RTN,RTF,RBN,RBF,RTN,RTF};
// face on right when going corner1 to corner2 
static int leftface[12] = {B,  L,  L,  F,  R,  T,  N,  R,  N,  B,  T,  F};
// face on right when going corner1 to corner2 
static int rightface[12]= {L,  T,  N,  L,  B,  R,  R,  F,  B,  F,  N,  T};

/**** A Test Program ****/
// torus: a torus with major, minor radii = 0.5, 0.1
/*float function(float x, float y, float z)
{
	double x2 = x*x, y2 =y*y, z2 = z*z;
    double a = x2+y2+z2+(0.5*0.5)-(0.1*0.1);
    return (float)(a*a-4.0*(0.5*0.5)*(y2+z2));
}*/

// sphere: an inverse square function (always positive)
double sphere ( float x, float y, float z)
{
    double rsq = x*x+y*y+z*z;
    return 1.0/(rsq < 0.00001? 0.00001 : rsq);
}

// blob: a three-pole blend function, try size = .1 
float function ( float x, float y, float z)
{
//	double coord[3];
//	coord[0]=x;
//	coord[1]=y;
//	coord[2]=z;
  //  return solver.GetValueAt (coord);
	float r,Rad ;
	r = 1.85f ;
	Rad = 4 ;
	return ( ( x*x + y*y + z*z + Rad*Rad - r*r ) * ( x*x + y*y + z*z + Rad*Rad - r*r ) - 4 * Rad*Rad * ( x*x + y*y ) ) *
		( ( x*x + (y+Rad)*(y+Rad) + z*z + Rad*Rad - r*r ) * ( x*x + (y+Rad)*(y+Rad) + z*z + Rad*Rad - r*r ) - 4 * Rad*Rad * ( (y+Rad)*(y+Rad) + z*z ) ) ;
}


/* triangle: called by polygonize() for each triangle; write to stdout */
int triproc(int i1, int i2, int i3, VERTICES vertices)
{
	gvertices.count = vertices.count;
	gvertices.max=vertices.max;
	gvertices.ptr=vertices.ptr;
	TRI *ltri=&gtris.tria[gtris.count++];
	ltri->i=i1; ltri->j=i2; ltri->k=i3;
	return 1;
}

/* writef data to file*/
int writef()
{
	int i;
	ofstream outfile("data.txt",ios::app );
	if (!outfile) {
		AfxMessageBox("can't write to file"); return 0; }
	else {
	    outfile<<gtris.count<<endl;
		outfile<<gvertices.count<<endl<<endl;
		
		for (i = 0; i < gtris.count; i++) {
			TRI t;
			t = gtris.tria[i];
			outfile<<t.i<<' '<<t.j<<' '<<t.k<<endl;
		}
		
		outfile<<endl;

		for (i = 0; i < gvertices.count; i++) {
			VERTEX v;
			v = gvertices.ptr[i];
			outfile<<v.position.x<<' '<<v.position.y<<' '<<v.position.z
				<<' '<<v.normal.x<<' '<<v.normal.y<<' '<<v.normal.z<<endl;
		}
		return 1;
	}
}


/* makecubetable: create the 256 entry table for cubical polygonization */
void MyUnit::makecubetable()
{
   int i, e, c, done[12], pos[8];
    for (i = 0; i < 256; i++) {
	for (e = 0; e < 12; e++) done[e] = 0;
	for (c = 0; c < 8; c++) pos[c] = BIT(i, c);
	for (e = 0; e < 12; e++)
	    if (!done[e] && (pos[corner1[e]] != pos[corner2[e]])) {
		INTEGERLIST *ints = 0;
		INTEGERLISTS *lists = (INTEGERLISTS *) calloc(1, sizeof(INTEGERLISTS));
		int start = e, edge = e;
		/* get face that is to right of edge from pos to neg corner: */
		int face = pos[corner1[e]]? rightface[e] : leftface[e];
		while (1) {
		    edge = nextcwedge(edge, face);
		    done[edge] = 1;
		    if (pos[corner1[edge]] != pos[corner2[edge]]) {
			INTEGERLIST *tmp = ints;
			ints = (INTEGERLIST *) calloc(1, sizeof(INTEGERLIST));
			ints->i = edge;
			ints->next = tmp; /* add edge to head of list */
			if (edge == start) break;
			face = otherface(edge, face);
		    }
		}
		lists->list = ints; /* add ints to head of table entry */
		lists->next = cubetable[i];
		cubetable[i] = lists;
	    }
    }
}

/* myfind: search for point with value of given sign (0: neg, 1: pos) */
TEST MyUnit::myfind(int sign, PROCESS *p, float x, float y, float z)
{
    int i;
    TEST test;
    float range = p->size;
    test.ok = 1;
    for (i = 0; i < 10000; i++) {
	test.p.x = x+range*(RAND()-0.5);
	test.p.y = y+range*(RAND()-0.5);
	test.p.z = z+range*(RAND()-0.5);
	test.value = p->function(test.p.x, test.p.y, test.p.z);
	if (sign == (test.value > 0.0)) return test;
	range = range*1.0005; /* slowly expand search outwards */
    }
    test.ok = 0;
    return test;
}


/* converge: from two points of differing sign, converge to zero crossing */
void MyUnit::converge(myPOINT *p1, myPOINT *p2, float v, myPOINT *p3,PROCESS *p)
{
    int i = 0;
    myPOINT pos, neg;
    if (v < 0) {
	pos.x = p2->x; pos.y = p2->y; pos.z = p2->z;
	neg.x = p1->x; neg.y = p1->y; neg.z = p1->z;
    }
    else {
	pos.x = p1->x; pos.y = p1->y; pos.z = p1->z;
	neg.x = p2->x; neg.y = p2->y; neg.z = p2->z;
    }
    while (1) {
	p3->x = 0.5*(pos.x + neg.x);
	p3->y = 0.5*(pos.y + neg.y);
	p3->z = 0.5*(pos.z + neg.z);
	if (i++ == RES) return;
	if ((p->function(p3->x, p3->y, p3->z)) > 0.0)
	     {pos.x = p3->x; pos.y = p3->y; pos.z = p3->z;}
	else {neg.x = p3->x; neg.y = p3->y; neg.z = p3->z;}
    }
}

/* setcorner: return corner with the given lattice location
   set (and cache) its function value */
CORNER * MyUnit::setcorner(PROCESS *p, int i, int j, int k)
{
    /* for speed, do corner value caching here */
    CORNER *c = (CORNER *) calloc(1, sizeof(CORNER));
    int index = HASH(i, j, k);
    CORNERLIST *l = p->corners[index];
    c->i = i; c->x = p->start.x+((float)i-.5)*p->size;
    c->j = j; c->y = p->start.y+((float)j-.5)*p->size;
    c->k = k; c->z = p->start.z+((float)k-.5)*p->size;
    for (; l != NULL; l = l->next)
	if (l->i == i && l->j == j && l->k == k) {
	    c->value = l->value;
	    return c;
	    }
    l = (CORNERLIST *) calloc(1, sizeof(CORNERLIST));
    l->i = i; l->j = j; l->k = k;
    l->value = c->value = p->function(c->x, c->y, c->z);
    l->next = p->corners[index];
    p->corners[index] = l;
    return c;
}

/* setcenter: set (i,j,k) entry of table[]
 * return 1 if already set; otherwise, set and return 0 */
int MyUnit::setcenter(CENTERLIST *table[], int i, int j, int k)
{
    int index = HASH(i, j, k);
    CENTERLIST *newlist, *l, *q = table[index];
    for (l = q; l != NULL; l = l->next)
	if (l->i == i && l->j == j && l->k == k) return 1;
    newlist = (CENTERLIST *) calloc(1, sizeof(CENTERLIST));
    newlist->i = i; newlist->j = j; newlist->k = k; newlist->next = q;
    table[index] = newlist;
    return 0;
}

/* dotet: triangulate the tetrahedron
 * b, c, d should appear clockwise when viewed from a
 * return 0 if client aborts, 1 otherwise */
int MyUnit::dotet(CUBE *cube, int c1, int c2, int c3, int c4, PROCESS *p)
{
    CORNER *a = cube->corners[c1];
    CORNER *b = cube->corners[c2];
    CORNER *c = cube->corners[c3];
    CORNER *d = cube->corners[c4];
    int index = 0, apos, bpos, cpos, dpos, e1, e2, e3, e4, e5, e6;
    if (apos = (a->value > 0.0)) index += 8;
    if (bpos = (b->value > 0.0)) index += 4;
    if (cpos = (c->value > 0.0)) index += 2;
    if (dpos = (d->value > 0.0)) index += 1;
    /* index is now 4-bit number representing one of the 16 possible cases */
    if (apos != bpos) e1 = vertid(a, b, p);
    if (apos != cpos) e2 = vertid(a, c, p);
    if (apos != dpos) e3 = vertid(a, d, p);
    if (bpos != cpos) e4 = vertid(b, c, p);
    if (bpos != dpos) e5 = vertid(b, d, p);
    if (cpos != dpos) e6 = vertid(c, d, p);
    /* 14 productive tetrahedral cases (0000 and 1111 do not yield polygons */
    switch (index) {
	case 1:	 return p->triproc(e5, e6, e3, p->vertices);
	case 2:	 return p->triproc(e2, e6, e4, p->vertices);
	case 3:	 return p->triproc(e3, e5, e4, p->vertices) &&
			p->triproc(e3, e4, e2, p->vertices);
	case 4:	 return p->triproc(e1, e4, e5, p->vertices);
	case 5:	 return p->triproc(e3, e1, e4, p->vertices) &&
			p->triproc(e3, e4, e6, p->vertices);
	case 6:	 return p->triproc(e1, e2, e6, p->vertices) &&
			p->triproc(e1, e6, e5, p->vertices);
	case 7:	 return p->triproc(e1, e2, e3, p->vertices);
	case 8:	 return p->triproc(e1, e3, e2, p->vertices);
	case 9:	 return p->triproc(e1, e5, e6, p->vertices) &&
			p->triproc(e1, e6, e2, p->vertices);
	case 10: return p->triproc(e1, e3, e6, p->vertices) &&
			p->triproc(e1, e6, e4, p->vertices);
	case 11: return p->triproc(e1, e5, e4, p->vertices);
	case 12: return p->triproc(e3, e2, e4, p->vertices) &&
			p->triproc(e3, e4, e5, p->vertices);
	case 13: return p->triproc(e6, e2, e4, p->vertices);
	case 14: return p->triproc(e5, e3, e6, p->vertices);
    }
    return 1;
}

/* docube: triangulate the cube directly, without decomposition */
int MyUnit::docube(CUBE *cube, PROCESS *p)
{
    INTEGERLISTS *polys;
    int i, index = 0;
    for (i = 0; i < 8; i++) if (cube->corners[i]->value > 0.0) index += (1<<i);
    for (polys = cubetable[index]; polys; polys = polys->next) {
	INTEGERLIST *edges;
	int a = -1, b = -1, count = 0;
	for (edges = polys->list; edges; edges = edges->next) {
	    CORNER *c1 = cube->corners[corner1[edges->i]];
	    CORNER *c2 = cube->corners[corner2[edges->i]];
	    int c = vertid(c1, c2, p);
	    if (++count > 2 && ! p->triproc(a, b, c, p->vertices)) return 0;
	    if (count < 3) a = b;
	    b = c;
	}
    }
    return 1;
}

/* testface: given cube at lattice (i, j, k), and four corners of face,
 * if surface crosses face, compute other four corners of adjacent cube
 * and add new cube to cube stack */
void MyUnit::testface(int i, int j, int k, CUBE *old, int face, int c1, int c2, int c3, int c4, PROCESS *p)
{
    CUBE newcube;
    CUBES *oldcubes = p->cubes;
    CORNER *setcorner();
    static int facebit[6] = {2, 2, 1, 1, 0, 0};
    int n, pos = old->corners[c1]->value > 0.0 ? 1 : 0, bit = facebit[face];

    /* test if no surface crossing, cube out of bounds, or already visited: */
    if ((old->corners[c2]->value > 0) == pos &&
	(old->corners[c3]->value > 0) == pos &&
	(old->corners[c4]->value > 0) == pos) return;
    if (abs(i) > p->bounds || abs(j) > p->bounds || abs(k) > p->bounds) return;
    if (setcenter(p->centers, i, j, k)) return;

    /* create new cube: */
    newcube.i = i;
    newcube.j = j;
    newcube.k = k;
    for (n = 0; n < 8; n++) newcube.corners[n] = NULL;
    newcube.corners[FLIP(c1, bit)] = old->corners[c1];
    newcube.corners[FLIP(c2, bit)] = old->corners[c2];
    newcube.corners[FLIP(c3, bit)] = old->corners[c3];
    newcube.corners[FLIP(c4, bit)] = old->corners[c4];
    for (n = 0; n < 8; n++)
	if (newcube.corners[n] == NULL)
		newcube.corners[n] = MyUnit::setcorner(p, i+BIT(n,2), j+BIT(n,1), k+BIT(n,0));

    /*add cube to top of stack: */
    p->cubes = (CUBES *) calloc(1, sizeof(CUBES));
    p->cubes->cube = newcube;
    p->cubes->next = oldcubes;
}

/* nextcwedge: return next clockwise edge from given edge around given face */
int MyUnit::nextcwedge(int edge, int face)
{
    switch (edge) {
	case LB: return (face == L)? LF : BN;
	case LT: return (face == L)? LN : TF;
	case LN: return (face == L)? LB : TN;
	case LF: return (face == L)? LT : BF;
	case RB: return (face == R)? RN : BF;
	case RT: return (face == R)? RF : TN;
	case RN: return (face == R)? RT : BN;
	case RF: return (face == R)? RB : TF;
	case BN: return (face == B)? RB : LN;
	case BF: return (face == B)? LB : RF;
	case TN: return (face == T)? LT : RN;
	case TF: return (face == T)? RT : LF;
	default: return 0;
    }
}

/* otherface: return face adjoining edge that is not the given face */
int MyUnit::otherface(int edge, int face)
{
    int other = leftface[edge];
    return face == other? rightface[edge] : other;
}

/* vertid: return index for vertex on edge:
 * c1->value and c2->value are presumed of different sign
 * return saved index if any; else compute vertex and save */
int MyUnit::vertid(CORNER *c1, CORNER *c2, PROCESS *p)
{
    VERTEX v;
    myPOINT a, b;
    int vid = getedge(p->edges, c1->i, c1->j, c1->k, c2->i, c2->j, c2->k);
    if (vid != -1) return vid;			     /* previously computed */
    a.x = c1->x; a.y = c1->y; a.z = c1->z;
    b.x = c2->x; b.y = c2->y; b.z = c2->z;
    converge(&a,&b,c1->value,&v.position,p);
	vnormal(&v.position, p, &v.normal);			   /* normal */
    addtovertices(&p->vertices, v);			   /* save vertex */
    vid = p->vertices.count-1;
    setedge(p->edges, c1->i, c1->j, c1->k, c2->i, c2->j, c2->k, vid);
    return vid;
}

/* getedge: return vertex id for edge; return -1 if not set */
int MyUnit::getedge(EDGELIST *table[], int i1, int j1, int k1, int i2, int j2, int k2)
{
    EDGELIST *q;
    if (i1>i2 || (i1==i2 && (j1>j2 || (j1==j2 && k1>k2)))) {
	int t=i1; i1=i2; i2=t; t=j1; j1=j2; j2=t; t=k1; k1=k2; k2=t;
    };
    q = table[HASH(i1, j1, k1)+HASH(i2, j2, k2)];
    for (; q != NULL; q = q->next)
	if (q->i1 == i1 && q->j1 == j1 && q->k1 == k1 &&
	    q->i2 == i2 && q->j2 == j2 && q->k2 == k2)
	    return q->vid;
    return -1;
}

/* vnormal: compute unit length surface normal at point */
void MyUnit::vnormal(myPOINT *point, PROCESS *p, myPOINT *v)
{
    float f = p->function(point->x, point->y, point->z);
    v->x = p->function(point->x+p->delta, point->y, point->z)-f;
    v->y = p->function(point->x, point->y+p->delta, point->z)-f;
    v->z = p->function(point->x, point->y, point->z+p->delta)-f;
    f = (float)(sqrt(v->x*v->x + v->y*v->y + v->z*v->z));
    if (f != 0.0) {v->x /= f; v->y /= f; v->z /= f;}
}

/* addtovertices: add v to sequence of vertices */
void MyUnit::addtovertices(VERTICES *vertices, VERTEX v)
{
    if (vertices->count == vertices->max) {
	int i;
	VERTEX *newvertex;
	vertices->max = vertices->count == 0 ? 10 : 2*vertices->count;
	newvertex = (VERTEX *) calloc(vertices->max, sizeof(VERTEX));
	for (i = 0; i < vertices->count; i++) newvertex[i] = vertices->ptr[i];
	if (vertices->ptr != NULL) free((char *) vertices->ptr);
	vertices->ptr = newvertex;
    }
    vertices->ptr[vertices->count++] = v;
}

/* setedge: set vertex id for edge */
void MyUnit::setedge(EDGELIST *table[], int i1, int j1, int k1, int i2, int j2, int k2, int vid)
{
    unsigned int index;
    EDGELIST *newedgelist;
    if (i1>i2 || (i1==i2 && (j1>j2 || (j1==j2 && k1>k2)))) {
	int t=i1; i1=i2; i2=t; t=j1; j1=j2; j2=t; t=k1; k1=k2; k2=t;
    }
    index = HASH(i1, j1, k1) + HASH(i2, j2, k2);
    newedgelist = (EDGELIST *) calloc(1, sizeof(EDGELIST));
    newedgelist->i1 = i1; newedgelist->j1 = j1; newedgelist->k1 = k1;
    newedgelist->i2 = i2; newedgelist->j2 = j2; newedgelist->k2 = k2;
    newedgelist->vid = vid;
    newedgelist->next = table[index];
    table[index] = newedgelist;
}

/**** An Implicit Surface Polygonizer ****/
char *MyUnit::polygonize(float size, int bounds, float x, float y, float z, int mode)
{
    PROCESS p;
    int n, noabort;
    CORNER *setcorner();
    TEST in, out, find();

    p.function = function;
    p.triproc = triproc;
    p.size = size;
    p.bounds = bounds;
    p.delta = size/(float)(RES*RES);

    /* allocate hash tables and build cube polygon table: */
    p.centers = (CENTERLIST **) calloc(HASHSIZE,sizeof(CENTERLIST *));
    p.corners = (CORNERLIST **) calloc(HASHSIZE,sizeof(CORNERLIST *));
    p.edges =	(EDGELIST   **) calloc(2*HASHSIZE,sizeof(EDGELIST *));
    makecubetable();

    /* find point on surface, beginning search at (x, y, z): */
    srand(1);
    in = myfind(1, &p, x, y, z);
    out = myfind(0, &p, x, y, z);
    if (!in.ok || !out.ok) return "can't find starting point";
    //converge(&in.p, &out.p, in.value, p.function, &p.start);
	converge(&in.p,&out.p,in.value,&p.start,&p);

    /* push initial cube on stack: */
    p.cubes = (CUBES *) calloc(1, sizeof(CUBES)); /* list of 1 */
    p.cubes->cube.i = p.cubes->cube.j = p.cubes->cube.k = 0;
    p.cubes->next = NULL;

    /* set corners of initial cube: */
    for (n = 0; n < 8; n++)
		p.cubes->cube.corners[n] = MyUnit::setcorner(&p, BIT(n,2), BIT(n,1), BIT(n,0));

    p.vertices.count = p.vertices.max = 0; /* no vertices yet */
    p.vertices.ptr = NULL;

    setcenter(p.centers, 0, 0, 0);

    while (p.cubes != NULL) { /* process active cubes till none left */
	CUBE c;
	CUBES *temp = p.cubes;
	c = p.cubes->cube;

	noabort = mode == TET?
	       /* either decompose into tetrahedra and polygonize: */
	       dotet(&c, LBN, LTN, RBN, LBF, &p) &&
	       dotet(&c, RTN, LTN, LBF, RBN, &p) &&
	       dotet(&c, RTN, LTN, LTF, LBF, &p) &&
	       dotet(&c, RTN, RBN, LBF, RBF, &p) &&
	       dotet(&c, RTN, LBF, LTF, RBF, &p) &&
	       dotet(&c, RTN, LTF, RTF, RBF, &p)
	       :
	       /* or polygonize the cube directly: */
	       docube(&c, &p);
	if (! noabort) return "aborted";

	/* pop current cube from stack */
	p.cubes = p.cubes->next;
	free((char *) temp);
	/* test six face directions, maybe add to stack: */
	testface(c.i-1, c.j, c.k, &c, L, LBN, LBF, LTN, LTF, &p);
	testface(c.i+1, c.j, c.k, &c, R, RBN, RBF, RTN, RTF, &p);
	testface(c.i, c.j-1, c.k, &c, B, LBN, LBF, RBN, RBF, &p);
	testface(c.i, c.j+1, c.k, &c, T, LTN, LTF, RTN, RTF, &p);
	testface(c.i, c.j, c.k-1, &c, N, LBN, LTN, RBN, RTN, &p);
	testface(c.i, c.j, c.k+1, &c, F, LBF, LTF, RBF, RTF, &p);
    }
    
	/* output to file*/
	writef();
	
	return NULL;
}

