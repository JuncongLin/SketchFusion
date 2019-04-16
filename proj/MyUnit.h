// MyUnit.h: interface for the MyUnit class.
//说明：这个类由 Jules Bloomenthal 的 implicit polygonizer 的 c 语言
//  程序移植而得。
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYUNIT_H__0E706A7C_57F6_4CBC_A811_08C255447D0F__INCLUDED_)
#define AFX_MYUNIT_H__0E706A7C_57F6_4CBC_A811_08C255447D0F__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TET	0  /* use tetrahedral decomposition */
#define NOTET	1  /* no tetrahedral decomposition  */

#define RES	10 /* # converge iterations    */

#define L	0  /* left direction:	-x, -i */
#define R	1  /* right direction:	+x, +i */
#define B	2  /* bottom direction: -y, -j */
#define T	3  /* top direction:	+y, +j */
#define N	4  /* near direction:	-z, -k */
#define F	5  /* far direction:	+z, +k */
#define LBN	0  /* left bottom near corner  */
#define LBF	1  /* left bottom far corner   */
#define LTN	2  /* left top near corner     */
#define LTF	3  /* left top far corner      */
#define RBN	4  /* right bottom near corner */
#define RBF	5  /* right bottom far corner  */
#define RTN	6  /* right top near corner    */
#define RTF	7  /* right top far corner     */

/* the LBN corner of cube (i, j, k), corresponds with location
 * (start.x+(i-.5)*size, start.y+(j-.5)*size, start.z+(k-.5)*size) */

#define RAND()	    ((rand()&32767)/32767.)    /* random number between 0 and 1 */
#define HASHBIT	    (5)
#define HASHSIZE    (size_t)(1<<(3*HASHBIT))   /* hash table size (32768) */
#define MASK	    ((1<<HASHBIT)-1)
#define HASH(i,j,k) ((((((i)&MASK)<<HASHBIT)|((j)&MASK))<<HASHBIT)|((k)&MASK))
#define BIT(i, bit) (((i)>>(bit))&1)
#define FLIP(i,bit) ((i)^1<<(bit)) /* flip the given bit of i */

typedef struct point {		   /* a three-dimensional point */
    float x, y, z;		   /* its coordinates */
} myPOINT;

typedef struct test {		   /* test the function for a signed value */
    myPOINT p;			   /* location of test */
    float value;		   /* function value at p */
    int ok;			   /* if value is of correct sign */
} TEST;

typedef struct vertex {		   /* surface vertex */
    myPOINT position, normal;	   /* position and surface normal */
} VERTEX;

typedef struct vertices {	   /* list of vertices in polygonization */
    int count, max;		   /* # vertices, max # allowed */
    VERTEX *ptr;		   /* dynamically allocated */
} VERTICES;

typedef struct tri{  
	int i,j,k;
} TRI;

#define MAX_TRI_NUM 50000
#define MAX_VER_NUM 50000

typedef struct tris { /* list of triangles in polygonization */
	int count;        /* # triangels */
	TRI tria[MAX_TRI_NUM];         
} TRIS;

typedef struct corner {		   /* corner of a cube */
    int i, j, k;		   /* (i, j, k) is index within lattice */
    float x, y, z, value;	   /* location and function value */
} CORNER;

typedef struct cube {		   /* partitioning cell (cube) */
    int i, j, k;		   /* lattice location of cube */
    CORNER *corners[8];		   /* eight corners */
} CUBE;

typedef struct cubes {		   /* linked list of cubes acting as stack */
    CUBE cube;			   /* a single cube */
    struct cubes *next;		   /* remaining elements */
} CUBES;

typedef struct centerlist {	   /* list of cube locations */
    int i, j, k;		   /* cube location */
    struct centerlist *next;	   /* remaining elements */
} CENTERLIST;

typedef struct cornerlist {	   /* list of corners */
    int i, j, k;		   /* corner id */
    float value;		   /* corner value */
    struct cornerlist *next;	   /* remaining elements */
} CORNERLIST;

typedef struct edgelist {	   /* list of edges */
    int i1, j1, k1, i2, j2, k2;	   /* edge corner ids */
    int vid;			   /* vertex id */
    struct edgelist *next;	   /* remaining elements */
} EDGELIST;


typedef struct process {	   /* parameters, function, storage */
    float (*function)(float x, float y, float z);	   /* implicit surface function */
    int (*triproc)(int i1,int i2,int i3, VERTICES vertices);		   /* triangle output function */
    float size, delta;		   /* cube size, normal delta */
    int bounds;			   /* cube range within lattice */
    myPOINT start;		   /* start point on surface */
    CUBES *cubes;		   /* active cubes */
    VERTICES vertices;		   /* surface vertices */
    CENTERLIST **centers;	   /* cube center hash table */
    CORNERLIST **corners;	   /* corner value hash table */
    EDGELIST **edges;		   /* edge and vertex id hash table */
} PROCESS;


float function(float x, float y, float z);//a torus with major, minor radii = 0.5, 0.1, try size = .05 
int triproc(int i1, int i2, int i3, VERTICES vertices);
int writef();
//void *calloc();
//char *mycalloc();

class MyUnit  
{
public:
	MyUnit();
	virtual ~MyUnit();

public:
	// An Implicit Surface Polygonizer 
	char * polygonize( float size, int bounds, float x,float y, float z, int mode);

protected:
	void converge( myPOINT *p1, myPOINT *p2, float v, myPOINT *p3,PROCESS *p);

	//set vertex id for edge
	void setedge(EDGELIST *table[], int i1, int j1, int k1, int i2, int j2, int k2, int vid);
	//add v to sequence of vertices
	void addtovertices(VERTICES *vertices, VERTEX v);
	//compute unit length surface normal at point 
	void vnormal(myPOINT *point, PROCESS *p, myPOINT *v);
	//return vertex id for edge; return -1 if not set 
	int getedge(EDGELIST *table[], int i1, int j1, int k1, int i2, int j2, int k2);
	int vertid(CORNER *c1, CORNER *c2,PROCESS *p);//return index for vertex on edge
	int otherface(int edge, int face);//return face adjoining edge that is not the given face
	int nextcwedge(int edge, int face);//return next clockwise edge from given edge around given face 
	//compute other four corners of adjacent cube and add new cube to cube stack
	void testface(int i, int j, int k, CUBE *old, int face, int c1, int c2, int c3, int c4, PROCESS *p);
	int docube(CUBE *cube, PROCESS *p);//triangulate the cube directly, without decomposition
	//triangulate the tetrahedron
	int dotet( CUBE *cube, int c1,int c2, int c3, int c4, PROCESS *p);
	int setcenter( CENTERLIST *table[], int i, int j, int k);//set (i,j,k) entry of table[]
	// return corner with the given lattice location set (and cache) its function value 
	CORNER* setcorner(PROCESS *p, int i, int j, int k);
	//search for point with value of given sign (0: neg, 1: pos)
	TEST myfind(int sign, PROCESS *p, float x, float y, float z);
	void makecubetable ();// create the 256 entry table for cubical polygonization

public:
};

#endif // !defined(AFX_MYUNIT_H__0E706A7C_57F6_4CBC_A811_08C255447D0F__INCLUDED_)
