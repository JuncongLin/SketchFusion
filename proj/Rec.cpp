#include "StdAfx.h"
#include ".\rec.h"
#include <math.h>
#include <stdlib.h>
#include <fstream>


template <class T> void DeleteLnk(T **phead);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRec::CRec()
{
	m_pBdyEdgeHead = NULL;
	m_pTrueBdyEdgeHead=NULL;
	m_iFaceSer = 0;

	m_iVertNum=0;

	m_fColor[0]=0.5;
	m_fColor[0]=0.5;
	m_fColor[0]=1.0;

	int i,l,m,n;
	for(l = 0; l < XMAX; l++)
		for( m = 0; m < YMAX; m++)
			for(n = 0; n < ZMAX; n++)
			{
				m_pVoxel[l][m][n] = NULL;
				m_iSgnVoxel[l][m][n] = 0;
			}
	for(i = 0; i < MAXFACE;	i++)
		m_pFaceHead[i] = NULL; // the head of the face list
}

CRec::~CRec()
{

	DeleteLnk<EDGEPNT>(&m_pBdyEdgeHead);
	DeleteLnk<EDGEPNT>(&m_pTrueBdyEdgeHead);
	int i,j,k;
	for(i = 0; i < XMAX; i++)
		for(j = 0; j < YMAX; j++)
			for(k = 0; k < ZMAX; k++)
				DeleteLnk<PNTLST>(&m_pVoxel[i][j][k]);
	for(i = 0; i < MAXFACE;	i++)
		delete m_pFaceHead[i]; // the head of the face list

}


/////////////////////////////////////////////////////////////////
//FUNCTION: Get the bounding box of the graphical model
void CRec::GetBoundBox()
{
	int i;
	double xmin = 1000, xmax = -1000;
	double ymin = 1000, ymax = -1000;
	double zmin = 1000, zmax = -1000;

	//get the bounding box
	for(i = 0; i < m_iVertNum; i++)
	{
		if(xmin > m_ptPoint[i].coord.x)
			xmin = m_ptPoint[i].coord.x;
		if(xmax < m_ptPoint[i].coord.x)
			xmax = m_ptPoint[i].coord.x;
		if(ymin > m_ptPoint[i].coord.y)
			ymin = m_ptPoint[i].coord.y;
		if(ymax < m_ptPoint[i].coord.y)
			ymax = m_ptPoint[i].coord.y;
		if(zmin > m_ptPoint[i].coord.z)
			zmin = m_ptPoint[i].coord.z;
		if(zmax < m_ptPoint[i].coord.z)
			zmax = m_ptPoint[i].coord.z;
	}

	//adjust the bounding box to cube
	double longs,x,y,z,m,little;
	longs = (m=((z=zmax-zmin)>(y=ymax-ymin)?z:y))>(x=xmax-xmin)?m:x;
	little = longs / (100*max(max(XMAX,YMAX),ZMAX));
	longs = longs + little;

	m_bbBoundBox.xmin = xmin - (longs-(xmax-xmin))/2;
	m_bbBoundBox.xmax = xmax + (longs-(xmax-xmin))/2;
	m_bbBoundBox.ymin = ymin - (longs-(ymax-ymin))/2;
	m_bbBoundBox.ymax = ymax + (longs-(ymax-ymin))/2;
	m_bbBoundBox.zmin = zmin - (longs-(zmax-zmin))/2;
	m_bbBoundBox.zmax = zmax + (longs-(zmax-zmin))/2;

}


/////////////////////////////////////////////////////////////////
//FUNCTION: Divide the bounding box
//    divnum: the number of division portion along each side
void CRec::DivBoundBox()
{
	double xstep,ystep,zstep; // the division step along each side
	xstep = (m_bbBoundBox.xmax-m_bbBoundBox.xmin) / XMAX;
	ystep = (m_bbBoundBox.ymax-m_bbBoundBox.ymin) / YMAX;
	zstep = (m_bbBoundBox.zmax-m_bbBoundBox.zmin) / ZMAX;

	m_ptStep.x = xstep; m_ptStep.y = ystep; m_ptStep.z = zstep;

	//initialize the voxel array
	int l,m,n;
	for(l = 0; l < XMAX; l++)
		for( m = 0; m < YMAX; m++)
			for(n = 0; n < ZMAX; n++)
				m_pVoxel[l][m][n] = NULL;

	int i;
	PNTLST *c,*w;
	CString str;
	PNT pt;
	for(i = 0; i < m_iVertNum; i++)
	{
		l = (m_ptPoint[i].coord.x-m_bbBoundBox.xmin)/xstep;
		m = (m_ptPoint[i].coord.y-m_bbBoundBox.ymin)/ystep;
		n = (m_ptPoint[i].coord.z-m_bbBoundBox.zmin)/zstep;
		if(l >= 20 || m >= 20 || n >= 20)
		{
			pt = m_ptPoint[i].coord;
			str.Format("DivBoundBox(), The lable exceed 20, l=%d,m=%d,n=%d,\
					   and x=%f,y=%f,z=%f",l,m,n,pt.x,pt.y,pt.z);
			AfxMessageBox(str);
		}

		w = new PNTLST;
		w->pntno = i;
		w->nxt = NULL;
		if(m_pVoxel[l][m][n] == NULL)
			m_pVoxel[l][m][n] = w;
		else 
		{
			c = LookforLastElmt(m_pVoxel[l][m][n]);
			c->nxt = w;
		}
	}

}


////////////////////////////////////////////////////////////////
//FUNCTION: Look for the seed triangular
void CRec::LookforSeedTriangle()
{
	int i,j,k;
	double maxz = -1000000;
	// look for the point whose z coordinates is maximum
	for(i = 0; i < m_iVertNum; i++)
		if(maxz < m_ptPoint[i].coord.z)
		{
			maxz = m_ptPoint[i].coord.z;
			m_iZMaxPnt = i;
		}

		//look for the voxel the point m_iZMaxPnt contains in
		int l,m,n;
		PNT pt,pv;
		pt = m_ptPoint[m_iZMaxPnt].coord;
		l = (pt.x-m_bbBoundBox.xmin)/m_ptStep.x;
		m = (pt.y-m_bbBoundBox.ymin)/m_ptStep.y;
		n = (pt.z-m_bbBoundBox.zmin)/m_ptStep.z;

		//look for the point which the distance from point m_iZMaxPnt
		//to is shortest
		int imin,imax,jmin,jmax,kmin,kmax;
		PNTLST *cu;
		double dis, mindis = 1000000;
		int minpntno = -1;
		imin = max(l-2,0); imax = min(l+2,XMAX-1);
		jmin = max(m-2,0); jmax = min(m+2,YMAX-1);
		kmin = max(n-2,0); kmax = min(n+2,ZMAX-1);

		for(k = kmin; k <= kmax; k++)
			for(j = jmin; j <= jmax; j++)
				for(i = imin; i <= imax; i++)
				{
					cu = m_pVoxel[i][j][k];
					while(cu)
					{
						if(cu->pntno != m_iZMaxPnt)
						{
							pv = m_ptPoint[cu->pntno].coord;
							dis = pow(pt.x-pv.x,2)+pow(pt.y-pv.y,2)+pow(pt.z-pv.z,2);
							if(dis < mindis)
							{
								mindis = dis;
								minpntno = cu->pntno;
							}
						}
						cu = cu->nxt;
					}
				}
				if(minpntno == -1)
				{
					AfxMessageBox("LookforSeedTriangle, No shortest vertext");
					exit(0);
				}
				PNT temp;
				temp=m_ptPoint[minpntno].coord;
				EDGELST *el = new EDGELST;
				el->pntno[0] = m_iZMaxPnt;
				el->pntno[1] = minpntno;

				//look for the shortest point nearest to the first edge
				int num = 0, sign[6];
				LABEL ptl;
				PNT norml[6], point[6]; //the four boundary faces
				double r = sqrt(mindis), runr = 0, vecnorm;
				PNT orthv1,orthv2, midv, midpnt;
				VOXLST *pvhead;
				//orthv1, orthv2: the vector perpendicular to vector p1p2
				PNT p1, p2, vec;
				p1 = m_ptPoint[m_iZMaxPnt].coord;
				p2 = m_ptPoint[minpntno].coord;
				
				vec = p1 - p2;
				vecnorm = sqrt(pow(vec.x,2)+pow(vec.y,2)+pow(vec.z,2));
				vec = vec / vecnorm;
				midv = GetMidVect(vec);
				orthv1 = CrossNorml(vec,midv);
				orthv2 = CrossNorml(orthv1,vec);
				midpnt = (p1 + p2)/2;
				ptl.i = (midpnt.x-m_bbBoundBox.xmin)/m_ptStep.x;
				ptl.j = (midpnt.y-m_bbBoundBox.ymin)/m_ptStep.y;
				ptl.k = (midpnt.z-m_bbBoundBox.zmin)/m_ptStep.z;
				int ser;
				while(1)
				{
					//construct the six boundary faces of the domain polygon
					num++;
					runr = runr + num*r;

					norml[0] = orthv1;
					point[0] = midpnt + runr*orthv1;

					norml[1] = orthv1;
					point[1] = midpnt - runr*orthv1;

					norml[2] = orthv2;
					point[2] = midpnt + runr*orthv2;

					norml[3] = orthv2;
					point[3] = midpnt - runr*orthv2;

					norml[4] = vec;
					point[4] = p1 + runr*vec;

					norml[5] = vec;
					point[5] = p2 - runr*vec;

					for(i = 0; i < 6; i++)
						sign[i] = Sign(FaceEquation(norml[i],point[i],midpnt));

					pvhead = CreateVoxelList(6,sign,norml,point,midpnt,el,runr);
					ser = SearchSeedVertex(6,sign,norml,point,pvhead,el);
					if(ser != -1) break;
				}
				delete el;

                
				//adjust the normal of the face outwards
				int v1 = m_iZMaxPnt, v2 = minpntno, v3 = ser;
				PNT facen, farpn = {0,0,10000};
				PNT pv1,pv2,pv3,vec1,vec2;
				double cross;
				pv1 = m_ptPoint[v1].coord;
				pv2 = m_ptPoint[v2].coord;
				pv3 = m_ptPoint[v3].coord;

				vec1 = pv2 - pv1;
				vec2 = pv3 - pv1;
				facen = CrossNorml(vec1,vec2);
				cross = farpn * facen;
				m_iFaceSer = 0;
				if(cross > 0) //the normal is outwards
					CreateSeedTriangle(v1,v2,v3,facen);
				else // the normal is inwards
				{
					facen.x = -facen.x; facen.y = -facen.y; facen.z = -facen.z;
					CreateSeedTriangle(v1,v3,v2,facen);
				}

}


////////////////////////////////////////////////////////////////
//FUNCTION: Create the seed triangle
void CRec::CreateSeedTriangle(int s1,int s2,int s3,PNT norml)
{
	FACELST* pf;

	//create triangle face
	pf = new FACELST;
	pf->faceno = m_iFaceNo++;
	pf->pntno[0] = s1; pf->pntno[1] = s2; pf->pntno[2] = s3;
	pf->norml = norml;
	pf->nxt = NULL;
	m_pFaceHead[m_iFaceSer] = pf;
	m_pfCurFacePtr = pf;

	//Add face to point field
	AddFaceToPntField(s1,pf);
	AddFaceToPntField(s2,pf);
	AddFaceToPntField(s3,pf);

	//create edges
	CreateEdge(s1,s2,pf); 
	CreateEdge(s2,s3,pf); 
	CreateEdge(s3,s1,pf);
}



///////////////////////////////////////////////////////////////////
//FUNCTION: Create the voxel list adjacent to the edge
VOXLST* CRec::CreateVoxelList(EDGELST* edge, double r)
{
	//calculate the radius of the cylinder
	EDGELST *e = edge;
	int s1, s2;   //the serial number of the end points of the 
	//boundary edge 
	s1 = e->pntno[0]; 
	s2 = e->pntno[1];

	double xstep, ystep, zstep, im;
	double mins;
	xstep = (m_bbBoundBox.xmax - m_bbBoundBox.xmin) / XMAX;
	ystep = (m_bbBoundBox.ymax - m_bbBoundBox.ymin) / YMAX;
	zstep = (m_bbBoundBox.zmax - m_bbBoundBox.zmin) / ZMAX;
	mins = (im=(xstep<ystep?xstep:ystep))<zstep?im:zstep;

	//caculate how many voxels are there on the boundary edge
	VOXLST *voxhead = NULL, *cu, *od;
	PNT pcu;
	PNT p1 = m_ptPoint[s1].coord, p2 = m_ptPoint[s2].coord;
	int ii,jj,kk;
	double dlt = mins / sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2)+pow(p1.z-p2.z,2));
	double t = 0;
	int endsgn = 1;
	while(endsgn)
	{
		if(t > 1)  { t = 1; endsgn = 0;}

		pcu.x = (1-t)*p1.x + t*p2.x;
		pcu.y = (1-t)*p1.y + t*p2.y;
		pcu.z = (1-t)*p1.z + t*p2.z;
		ii = (pcu.x - m_bbBoundBox.xmin) / xstep;
		jj = (pcu.y - m_bbBoundBox.ymin) / ystep;
		kk = (pcu.z - m_bbBoundBox.zmin) / zstep;
		if(voxhead == NULL)
		{
			cu = new VOXLST;
			cu->i = ii; cu->j = jj; cu->k = kk;
			cu->nxt = NULL;
			voxhead = cu;
			od = cu;
		}
		else if(TheVoxelIsNotExist(ii,jj,kk,voxhead))
		{
			cu = new VOXLST;
			cu->i = ii; cu->j = jj; cu->k = kk;
			cu->nxt = NULL;
			od->nxt = cu;
			od = cu;
		}
		t = t + dlt;
	}

	//create the voxel list, the distance between the voxels
	//and the edge is less than maxs
	int step; 
	step = r / mins;
	int imin,imax,jmin,jmax,kmin,kmax,s;
	int i,j,k;
	VOXLST *head = NULL, *c;
	cu = voxhead;
	while(cu)
	{
		imin = (s=(cu->i)-step)>0?s:0;
		imax = (s=(cu->i)+step)<XMAX-1?s:XMAX-1;
		jmin = (s=(cu->j)-step)>0?s:0;
		jmax = (s=(cu->j)+step)<YMAX-1?s:YMAX-1;
		kmin = (s=(cu->k)-step)>0?s:0;
		kmax = (s=(cu->k)+step)<ZMAX-1?s:ZMAX-1;
		for(k = kmin; k <= kmax; k++)
			for(j = jmin; j <= jmax; j++)
				for(i = imin; i <= imax; i++)
				{
					if(head == NULL)
					{
						c = new VOXLST;
						c->i = i; c->j = j; c->k = k;
						c->nxt = NULL;
						head = c;
						od = c;
					}
					else if(TheVoxelIsNotExist(i,j,k,head))
					{
						c = new VOXLST;
						c->i = i; c->j = j; c->k = k;
						c->nxt = NULL;
						od->nxt = c;
						od = c;
					}
				}
				cu = cu->nxt;
	}

	return head;

}


///////////////////////////////////////////////////////////////////
//FUCNTION: Look for the longest edge adjacent to points s1 and s2
double CRec::CalcRadius(int s1, int s2)
{
	double maxrat1 = -100000, maxrat2 = -100000, ratio, longe = -10000;
	double maxrat = 0, len1, len2, len;
	PNT pe;
	int i = 0;

	maxrat1 = GetLengthRatio(s1);
	maxrat2 = GetLengthRatio(s2);
	maxrat = max(maxrat1,maxrat2);

	len1 = GetMinEdgeLen(s1);
	len2 = GetMinEdgeLen(s2);
	len = (len1+len2) / 2;

	longe = maxrat*len;
	return longe;
}


///////////////////////////////////////////////////////////////////
//FUCNTION: Determine whether the voxel (i,j,k) is in the link head
BOOL CRec::TheVoxelIsNotExist(int i, int j, int k, VOXLST *head)
{
	VOXLST *cu;
	cu = head;
	while(cu)
	{
		if(i==cu->i && j==cu->j && k==cu->k)
			return 0; // the voxel is in the voxel link
		cu = cu->nxt;
	}
	return 1; // the voxel is not in the voxel link

}


/////////////////////////////////////////////////////////////////
//FUNCTION: Look for the last element of the link head
PNTLST* CRec::LookforLastElmt(PNTLST *head)
{
	PNTLST *cu;
	cu = head;
	while(cu->nxt)
		cu = cu->nxt;

	return cu;
}


//////////////////////////////////////////////////////////////////
//FUNCTION: Look for the last element of the link head
VOXLST* CRec::LookforLastElmt(VOXLST *head)
{
	VOXLST *cu;
	cu = head;
	while(cu->nxt)
		cu = cu->nxt;

	return cu;
}


///////////////////////////////////////////////////////////////////
//FUCNTION: Look for a vertex
//     pedge: the current boundary edge
//     pvoxh: the head of the voxel adjacent to the boundary edge
int CRec::LookforNewVertex(EDGELST *pedge, double r)
{
	//calculate the barycenter of the triangle, the middle point of
	//the boundary edge
	int s1, s2, oppv, i, fnum;
	FACELST *pf;
	PNT norl;
	PNT p1, p2, p3, bacent; // barycenter
	PNT middpnt; // the middle point of the edge
	s1 = pedge->pntno[0];  s2 =pedge->pntno[1];
	pf = pedge->face[0];
	for(i=0; i<=2; i++)
	{
		oppv = pf->pntno[i];
		if(oppv!=s1 && oppv!=s2)
			break;
	}

	norl = GetMeanNormal(pf);
	p1 = m_ptPoint[s1].coord;
	p2 = m_ptPoint[s2].coord;
	p3 = m_ptPoint[oppv].coord;
	middpnt = (p1 + p2) / 2;
	bacent = (p1+p2+p3)/3;

	//double len;
	//len = sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2)+pow(p1.z-p2.z,2));

	//calculate the normal of the six boundary faces and one point
	//on each face
	PNT norm[7], point[7];
	PNT v1, v2;
	double offs = r;

	norm[0] = norl;
	point[0] = middpnt+offs*norm[0];

	norm[1] = norl;
	point[1] = middpnt-offs*norm[1];

	v1 = norl; v2 = bacent - p1; 
	norm[2] = CrossNorml(v1,v2);
	point[2] = p1;

	v1 = norl; v2 = bacent - p2;
	norm[3] = CrossNorml(v1,v2);
	point[3] = p2;

	v1 = norl; v2 =p2 - p1;
	norm[4] = CrossNorml(v1,v2);

	//adjust the direction of the normal
	double sgn;
	v1 = bacent - middpnt; 
	sgn = v1.x*norm[4].x + v1.y*norm[4].y + v1.z*norm[4].z;
	if(sgn > 0)
		norm[4]= (-1)*norm[4]; 
	point[4] = middpnt + r*norm[4];

	fnum = 5;

	EDGELST *ano1, *ano2;
	PNT ed1,ed2;
	ano1 = SearchAdjEdge(s1,s2,oppv,pf->norml,point[4]);
	if(ano1 != NULL)
	{
		v1 = norl;
		ed1 = m_ptPoint[ano1->pntno[0]].coord;
		ed2 = m_ptPoint[ano1->pntno[1]].coord;
		v2 = ed2 - ed1;
		norm[fnum] = CrossNorml(v1,v2);
		point[fnum] = ed1;
		fnum = fnum + 1;
	}

	ano2 = SearchAdjEdge(s2,s1,oppv,pf->norml,point[4]);
	if(ano2 != NULL)
	{
		v1 = norl;
		ed1 = m_ptPoint[ano2->pntno[0]].coord;
		ed2 = m_ptPoint[ano2->pntno[1]].coord;
		v2 = ed2 - ed1;
		norm[fnum] = CrossNorml(v1,v2);
		point[fnum] = ed1;
		fnum = fnum + 1;
	}

	//calculate the sign array
	int sign[7];
	for(i = 0; i < fnum; i++)
		sign[i] = Sign(FaceEquation(norm[i],point[i],middpnt));

	VOXLST *pvoxh;
	LABEL ptl;
	ptl.i = (middpnt.x - m_bbBoundBox.xmin) / m_ptStep.x;
	ptl.j = (middpnt.y - m_bbBoundBox.ymin) / m_ptStep.y;
	ptl.k = (middpnt.z - m_bbBoundBox.zmin) / m_ptStep.z;
	pvoxh = CreateVoxelList(fnum,sign,norm,point,middpnt,pedge,r);

	//search new vertex
	int ser;
	ser = SearchNewVertex(fnum,sign,norm,point,pvoxh,pedge);
	//if(ser == -1) 
	//{ser = -1; ano1 = 0; ano2 = 0;}
	DeleteLnk<VOXLST>(&pvoxh);
	//if ser == -1, the edge is boundary edge
	return ser;
}


///////////////////////////////////////////////////////////////////
//FUCNTION: Calculate the cross product of v1 and v2 and 
//          normalize it
inline PNT CRec::CrossNorml(PNT v1, PNT v2)
{
	PNT norl;
	norl.x = v1.y*v2.z - v1.z*v2.y;
	norl.y = v1.z*v2.x - v1.x*v2.z;
	norl.z = v1.x*v2.y - v1.y*v2.x;


	double norm;
	norm = sqrt(pow(norl.x,2)+pow(norl.y,2)+pow(norl.z,2));

	if(norm != 0)
		norl = norl / norm;

	return norl;
}


///////////////////////////////////////////////////////////////////
//FUNCTION: the plane equation, decide which side the point pt lie 
inline double CRec::FaceEquation(PNT norm, PNT point, PNT pt)
{
	double sgn;

	sgn = (pt.x-point.x)*norm.x + (pt.y-point.y)*norm.y + 
		(pt.z-point.z)*norm.z;

	return sgn;
}


///////////////////////////////////////////////////////////////////
//FUNCTION: Search a new vertex
//    fnum: the face number of the domain polygon
int CRec::SearchNewVertex(int fnum, int sign[], PNT norm[], PNT pnt[], VOXLST *phead, EDGELST *pedge)
{
	VOXLST *cu, *ccu;
	PNTLST *cp, *ccp;
	int i,j,k,ser,notinpoly,minpntser;
	double mind = 1000000, dis;
	int pinequ;
	FACELST *pf;
	FACEPNT *cpf;

	PNT pt,p1,p2;
	p1 = m_ptPoint[pedge->pntno[0]].coord;
	p2 = m_ptPoint[pedge->pntno[1]].coord;
	//s1 = sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2)+pow(p1.z-p2.z,2));

	pf = pedge->face[0];

	//Calculate coefficient for harmonic map
	double lij,lik1,ljk1,aijk1;
	PNT p3;
	lij = pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2)+pow(p1.z-p2.z,2);
	for(i=0; i<3; i++)
	{
		if((pf->pntno[i]!=pedge->pntno[0]) && (pf->pntno[i]!=pedge->pntno[1]))
		{
			p3 = m_ptPoint[pf->pntno[i]].coord; 
			break;
		}
	}
	lik1 = pow(p1.x-p3.x,2)+pow(p1.y-p3.y,2)+pow(p1.z-p3.z,2);
	ljk1 = pow(p2.x-p3.x,2)+pow(p2.y-p3.y,2)+pow(p2.z-p3.z,2);
	double l1,l2,l3,s;
	l1 = sqrt(lij); l2 = sqrt(lik1); l3 = sqrt(ljk1);
	s = (l1 + l2 + l3) / 2;
	aijk1 = sqrt(s*(s-l1)*(s-l2)*(s-l3));
	double co1;
	co1 = (lik1 + ljk1 - lij) / aijk1;

	int unsearched = 1;
	while(unsearched)
	{
		cu = phead;
		minpntser = -1; //if minpntser == -1, the point is boundary point
		while(cu)
		{
			i = cu->i; j = cu->j; k = cu->k;
			cp = m_pVoxel[i][j][k];
			while(cp)
			{
				ser = cp->pntno;
				if(ser==pf->pntno[0]||ser==pf->pntno[1]||ser==pf->pntno[2])
				{
					cp = cp->nxt;
					continue;
				}
				pt = m_ptPoint[ser].coord;
				notinpoly = 0;
				for(i = 0; i < fnum; i++)
				{
					pinequ = Sign(FaceEquation(norm[i],pnt[i],pt));
					if((sign[i] != pinequ) && (pinequ != 0))
					{
						notinpoly = 1;
						break;
					}
				}
				if((!notinpoly) && (m_ptPoint[ser].pcsdsgn == 0)) 
				{
					//the point is in the polygon and it is not a 
					//fixed point
					//dis = CalcHigh(pt,pedge);
					//dis = DiffMinimum(p1,p2,pt,s1);
					//dis = CalcR(pt,pedge,s1);
					//dis = CalcSum(pt,pedge);
					dis = CalcWeightSum(pt,pedge,lij,co1);

					if(mind > dis)
					{
						mind = dis;
						minpntser = ser;
					}
				}
				cp = cp->nxt;
			}
			cu = cu->nxt;
		}

		if(minpntser == -1)  unsearched = 0;
		else  //check whether there is intersection among the new 
			//triangle and existing triangles 
		{
			ccu = phead;
			while(ccu && unsearched)
			{
				i = ccu->i; j = ccu->j; k = ccu->k;
				ccp = m_pVoxel[i][j][k];
				while(ccp && unsearched)
				{
					ser = ccp->pntno;
					if(m_ptPoint[ser].pface)
					{
						cpf = m_ptPoint[ser].pface;
						if(PntIsVertex(minpntser,pedge->pntno[0],pedge->pntno[1],cpf))
							unsearched = 0;
					}
					ccp = ccp->nxt;
				}
				ccu = ccu->nxt;
			}
		}
		if(unsearched && minpntser != -1)
			m_ptPoint[minpntser].pcsdsgn = -1;
	}

	//recover the process sign of the points
	cu = phead;
	while(cu)
	{
		i = cu->i; j = cu->j; k = cu->k;
		cp = m_pVoxel[i][j][k];
		while(cp)
		{
			ser = cp->pntno;
			if(m_ptPoint[ser].pcsdsgn == -1)
				m_ptPoint[ser].pcsdsgn == 0;
			cp = cp->nxt;
		}
		cu = cu->nxt;
	}

	return minpntser;
}


///////////////////////////////////////////////////////////////////
//FUNCTION: Calculate the distance from point pt to edge 
inline double CRec::CalcHigh(PNT pt, EDGELST *edge)
{
	PNT p1,p2,v1,v2;

	p1 = m_ptPoint[edge->pntno[0]].coord;
	p2 = m_ptPoint[edge->pntno[1]].coord;

	v1.x = pt.x - p1.x; v1.y = pt.y - p1.y; v1.z = pt.z - p1.z;
	v2.x = p2.x - p1.x; v2.y = p2.y - p1.y; v2.z = p2.z - p1.z;

	double dis1,dis2,cosalf,sinalf,high;
	dis1 = sqrt(pow(v1.x,2)+pow(v1.y,2)+pow(v1.z,2));
	dis2 = sqrt(pow(v2.x,2)+pow(v2.y,2)+pow(v2.z,2));
	cosalf = (v1.x*v2.x+v1.y*v2.y+v1.z*v2.z) / (dis1*dis2);
	sinalf = sqrt(1-pow(cosalf,2));

	high = dis1*sinalf;

	return high;
}


//////////////////////////////////////////////////////////////////
//FUNCTION: get the sign of num
inline int CRec::Sign(double num)
{
	if(fabs(num) < 10e-8)
		return 0;
	else if(num > 0)
		return 1;
	else
		return -1;
}


//////////////////////////////////////////////////////////////////
//FUNCTION: Add a triangular face
void CRec::AddTriangle(int ser, EDGELST *pedge)
{
	//create a new triangle
	FACELST *pf,*newpf;
	PNT facen;
	int snew, sold1, sold2;
	snew = ser;
	sold1 = pedge->pntno[0]; sold2 = pedge->pntno[1];
	pf = pedge->face[0];
	facen = pf->norml;
	newpf = CreateNewTriangle(sold2,sold1,snew);

	//add the face to face link
	m_pfCurFacePtr->nxt = newpf;
	m_pfCurFacePtr = newpf;

	//add the face to point field
	AddFaceToPntField(snew,newpf);
	AddFaceToPntField(sold1,newpf);
	AddFaceToPntField(sold2,newpf);

	// Create edges and adjust edge field
	CreateEdge(sold1,snew,newpf);
	CreateEdge(snew,sold2,newpf);

	if(pedge->pcsdsgn == BNDRYEDGE)
		m_iBndryEdgeNum--;
	SetEdgeToFixed(pedge,newpf,FIXEDGE);
	//SoftDeleteElmt(pedge,m_pBdyEdgeHead);

	//adjust the state of three vertexes of the new triangle
	AdjustPntState(snew);
	AdjustPntState(sold1);
	AdjustPntState(sold2);
}


//////////////////////////////////////////////////////////////////
//FUNCTION: Create a new edge of new face pf
void CRec::CreateEdge(int s1, int s2, FACELST *pf)
{
	//check that whether the egde is existed
	EDGELST *ce,*od,*cep;
	int edgeisexist = 0;
	ce = m_ptPoint[s1].pedge;
	od = ce;
	while(ce)
	{
		if(ce->pntno[1] == s2)
		{
			edgeisexist = 1;
			break;
		}
		od = ce;
		ce = ce->nxt;
	}

	if(edgeisexist) //the edge is existed, use glue operation
	{
		if(ce->pcsdsgn == BNDRYEDGE) //the edge is boundary edge
		{
			(m_iBndryEdgeNum)--;
			//DeleteElmt(ce,&(m_pDoc->m_pTrueBdyEdgeHead));
		}
		SetEdgeToFixed(ce,pf,FIXEDGE);
	}
	else   //the edge is not existed, use join operation
	{
		//create the edge adjacent to vertext s1
		ce = new EDGELST;
		ce->edgeno = m_iEdgeNo++;
		ce->face[0] = pf;
		ce->face[1] = NULL;
		ce->pcsdsgn = 0;
		ce->pntno[0] = s1; ce->pntno[1] = s2;
		ce->nxt = NULL;
		if(m_ptPoint[s1].pedge == NULL)
			m_ptPoint[s1].pedge = ce;
		else
			od->nxt = ce;
		AddElmtToLnk(ce,&m_pBdyEdgeHead);

		//create the edge adjacent to vertext s2
		ce = new EDGELST;
		ce->edgeno = m_iEdgeNo++;
		ce->face[0] = pf;
		ce->face[1] = NULL;
		ce->pcsdsgn = 0;
		ce->pntno[0] = s2; ce->pntno[1] = s1;
		ce->nxt = NULL;
		if(m_ptPoint[s2].pedge == NULL)
			m_ptPoint[s2].pedge = ce;
		else
		{
			cep = m_ptPoint[s2].pedge;
			while(cep->nxt)
				cep = cep->nxt;
			cep->nxt = ce;
		}


	}

}


//////////////////////////////////////////////////////////////////
//FUCNTION: Set the edge to fixed edge or boundary edge
void CRec::SetEdgeToFixed(EDGELST *pe, FACELST *pf, int type)
{
	int edpnt[2];
	edpnt[0] = pe->pntno[0]; edpnt[1] = pe->pntno[1];

	//the edge pe adjacent to edpnt[0]
	pe->pcsdsgn = type;
	pe->face[1] = pf;

	//look for the edge adjacent to edpnt[1]
	EDGELST *ce;
	ce = m_ptPoint[edpnt[1]].pedge;
	while(ce)
	{
		if(ce->pntno[1] == edpnt[0])
			break;
		ce = ce->nxt;
	}
	ce->pcsdsgn = type;
	ce->face[1] = pf;
}


//////////////////////////////////////////////////////////////////
//FUNCTION: Delete the element from link phead, but do not delete
//          it truely.
void CRec::SoftDeleteElmt(EDGELST *pe, EDGEPNT *phead)
{
	int edpnt[2], edwk[2];
	edpnt[0] = pe->pntno[0]; edpnt[1] = pe->pntno[1];

	EDGEPNT *cep,*cepod;
	EDGELST *cel;
	BOOL c1,c2;
	cep = phead;
	cepod = cep;
	while(cep)
	{
		cel = cep->pedge;
		edwk[0] = cel->pntno[0]; edwk[1] = cel->pntno[1];
		c1 = (edwk[0] == edpnt[0]) && (edwk[1] == edpnt[1]);
		c2 = (edwk[1] == edpnt[0]) && (edwk[0] == edpnt[1]);
		if(c1 || c2) //find the element 
		{
			/*if(cep == *phead) // the element is the head of link
			*phead = cep->nxt;
			else
			cepod->nxt = cepp->nxt;
			delete cep;*/
			cel->pcsdsgn = 1;
			break;
		}
		cepod = cep;
		cep = cep->nxt;
	}

}


//////////////////////////////////////////////////////////////////
//FUNCTION: If there is not the edge in boundary edge link, add it
void CRec::AddElmtToLnk(EDGELST *pe, EDGEPNT **phead)
{
	EDGEPNT *cep, *cepc;

	cep = new EDGEPNT;
	cep->pedge = pe;
	cep->nxt = NULL;
	if(*phead == NULL)
		*phead = cep;
	else
	{
		cepc = *phead;
		while(cepc->nxt)
			cepc = cepc->nxt;
		cepc->nxt = cep;
	}
}


//////////////////////////////////////////////////////////////////
//FUNCTION: Create a new triangle face
FACELST* CRec::CreateNewTriangle(int s1, int s2, int s3, PNT nrl)
{
	//generate the face
	FACELST *cf;
	cf = new FACELST;
	cf->faceno = m_iFaceNo++;
	cf->nxt = NULL;

	//generate the normal and adjust it outwards
	PNT vec1, vec2, p1, p2, p3, nrml;
	double dot;
	p1 = m_ptPoint[s1].coord;
	p2 = m_ptPoint[s2].coord;
	p3 = m_ptPoint[s3].coord;
	vec1 = p2 - p1;
	vec2 = p3 - p1;
	nrml = CrossNorml(vec1,vec2);
	dot = nrml * nrl;
	if(dot >= 0)
	{
		//nrml = (-1) * nrml;
		cf->pntno[0]=s1; cf->pntno[1]=s2; cf->pntno[2]=s3;
		cf->norml = nrml;
	}

	else if(dot < 0)
	{
		nrml = (-1) * nrml;
		cf->pntno[0]=s1; cf->pntno[1]=s3; cf->pntno[2]=s2;
		cf->norml = nrml;
	}

	return cf;
}


//////////////////////////////////////////////////////////////////
//FUNCTION: Adjust the state of the point
void CRec::AdjustPntState(int ser)
{
	EDGELST *ce;
	int isfixed = 1;
	ce = m_ptPoint[ser].pedge;
	while(ce)
	{
		if(ce->pcsdsgn == 0) 
		{
			isfixed = 0;
			break;
		}
		ce = ce->nxt;
	}
	if(isfixed)
		m_ptPoint[ser].pcsdsgn = 1;
	else
		m_ptPoint[ser].pcsdsgn = 0;
}


//////////////////////////////////////////////////////////////////
//FUNCTION: The main program for CRec
void CRec::Run()
{
	DeleteLink();  //delete link in this CRec and CGLproDoc class


	GetBoundBox();
	DivBoundBox();
	m_iFaceNo = m_iEdgeNo = 0;

	LookforSeedTriangle();
	GenerateTopology();



	double maxrat, minrat, step, minr, maxr;
	int rto[10];
	CalcPntRatio(maxrat,minrat);
	step = (maxrat - minrat) / 10;
	int i;
	maxr = maxrat; minr = maxr - step;
	for(i = 0; i < 10; i++)
	{
		rto[i] = StatsPntNumBtwRtos(minr,maxr);
		minr = minr - step;
		maxr = maxr - step;
	}
	rto[9] = rto[9] + 1;

	//AfxMessageBox("Reconstruction end");
	//CString msg, each;
	//msg.Format("Tris: %d, Verts: %d, Time: %d",m_iFaceNo,
	//	m_iVertNum,time);
	//AfxMessageBox(msg);
	/*msg.Format("maxratio: %f, minratio: %f,",maxrat,minrat);
	for(i = 0; i < 10; i++)
	{
	each.Format("The %d iv num: %d, ", i,rto[i]);
	msg = msg + each;
	}
	AfxMessageBox(msg);*/


	FACELST *fh;
	int j,k;
	i = -1;
	bool bound[3];
	for(k = 0; k <= m_iFaceSer; k++)
	{
		fh = m_pFaceHead[k];
		while(fh)
		{
			i++;
			m_faceFace[i].vnum = 3;
			for(j = 0; j < 3; j++)
			{
				m_faceFace[i].v[j] = fh->pntno[j];
				m_pntVertNorm[fh->pntno[j]] = CalcNrml(fh->pntno[j]);
				m_faceFace[i].vn[j] = fh->pntno[j];
				bound[j]=m_ptPoint[m_faceFace[i].v[j]].pVertex!=NULL;
			}
			if(bound[0]&&bound[1]&&bound[2])i--;
			fh = fh->nxt;
		}
	}

	m_iFaceNum = i+1;


}


//////////////////////////////////////////////////////////////////
//FUNCTION: Generate the topology structure
void CRec::GenerateTopology()
{
	EDGEPNT *cep, *cod;
	int s1,s2,ser;
	EDGELST *e;
	double r;
	cep = m_pBdyEdgeHead;
	cod = cep;
	int progress = 0;
	CString sprog;
	m_iBndryEdgeNum = 0;
	while(cep)
	{
		if(cep->pedge->pcsdsgn == 0)
		{
			e = cep->pedge;
			s1 = e->pntno[0]; 
			s2 = e->pntno[1];	
			if(m_ptPoint[s1].pVertex!=NULL&&m_ptPoint[s2].pVertex!=NULL)
			{
				ser=-1;
			}
			else
			{
				r = CalcRadius(s1,s2);
				ser = LookforNewVertex(e,r);
			}
			if(ser == -1) //the edge is boundary edge
			{
				//sprog.Format("Boundary edge, vertex %d,%d",s1,s2);
				//AfxMessageBox(sprog);
				m_iBndryEdgeNum++;
				SetEdgeToFixed(e,NULL,BNDRYEDGE);
				//AddElmtToLnk(e,&(m_pDoc->m_pTrueBdyEdgeHead));
			}
			else
			{
				AddTriangle(ser,e);
				progress++;
/*				if(progress%5000 == 0)
					m_pView->SendMessage(WM_DISPLAY_POLYGON);
*/			}
		}
		cod = cep;
		cep = cep->nxt;
		m_pBdyEdgeHead = cep;
		delete cod;
	}

	//sprog.Format("Reconstruction end, the number is %d\n",m_iFaceNo);
	//AfxMessageBox(sprog);

}


//////////////////////////////////////////////////////////////////
//FUNCTION: Delete some links
void CRec::DeleteLink()
{
	m_iEdgeNo = m_iFaceNo = 0;
	DeleteLnk<EDGEPNT>(&m_pBdyEdgeHead);
	int i,j,k;
	for(i = 0; i < XMAX; i++)
		for(j = 0; j < YMAX; j++)
			for(k = 0; k < ZMAX; k++)
			{
				DeleteLnk<PNTLST>(&m_pVoxel[i][j][k]);
				m_iSgnVoxel[i][j][k] = 0;
			}
			for(i = 0; i <= m_iFaceSer; i++)
				DeleteLnk<FACELST>(&m_pFaceHead[m_iFaceSer]);
			for(i = 0; i < m_iVertNum; i++)
			{
				DeleteLnk<EDGELST>(&(m_ptPoint[i].pedge));
				DeleteLnk<FACEPNT>(&(m_ptPoint[i].pface));
			}
			DeleteLnk<EDGEPNT>(&(m_pTrueBdyEdgeHead));
}



//////////////////////////////////////////////////////////////////
//FUNCTION: Add face pf to the point ser field
inline void CRec::AddFaceToPntField(int ser, FACELST *pf)
{
	FACEPNT *phead, *cf, *cn;

	cn = new FACEPNT;
	cn->pface = pf;
	cn->nxt = NULL;

	phead = m_ptPoint[ser].pface;
	if(phead == NULL)
		m_ptPoint[ser].pface = cn;
	else
	{
		cf = phead;
		while(cf->nxt)
			cf = cf->nxt;
		cf->nxt = cn;
	}

}


///////////////////////////////////////////////////////////////////
//FUNCTION: Create the voxel list adjacent to the edge with seed
//          algorithm
//  fnum: the face number of the polygon
VOXLST* CRec::CreateVoxelList(int fnum, int sign[], PNT norl[], PNT pnt[],PNT midpt,EDGELST *e,double r)
{
	LABEL lw;
	PNT bom;
	PNT vertex[8];
	int sgninpoly,num;
	int i,j,sgne,sgcs;
	VOXLST *phead = NULL, *cv;
	LABEL pt;
	pt.i = (midpt.x - m_bbBoundBox.xmin) / m_ptStep.x;
	pt.j = (midpt.y - m_bbBoundBox.ymin) / m_ptStep.y;
	pt.k = (midpt.z - m_bbBoundBox.zmin) / m_ptStep.z;
	//mdstep.x=m_ptStep.x/2; mdstep.y=m_ptStep.y/2; mdstep.z=m_ptStep.z/2;
	m_iStackPtr = -1;
	m_iSgnVoxel[pt.i][pt.j][pt.k] = 1;
	Push(pt);
	while(m_iStackPtr != -1)
	{
		lw = Pop();
		bom.x = m_bbBoundBox.xmin + m_ptStep.x*lw.i;
		bom.y = m_bbBoundBox.ymin + m_ptStep.y*lw.j;
		bom.z = m_bbBoundBox.zmin + m_ptStep.z*lw.k;
		GetCubeVertex(vertex,bom);

		sgninpoly = 1;
		for(i = 0; i < fnum; i++)
		{
			num = 0;
			sgcs = (-1)*Sign(FaceEquation(norl[i],pnt[i],midpt));
			for(j = 0; j < 8; j++)
			{
				sgne = Sign(FaceEquation(norl[i],pnt[i],vertex[j]));
				if(sgcs == sgne) num++;
			}
			if(num == 8) { sgninpoly = 0; break;}
		}

		/*sgninpoly = 0;
		for(i = 0; i < disinnum; i++)
		{
		if(PntInPoly(fnum,sign,norl,pnt,disinpnt[i]))
		{
		sgninpoly = 1;
		break;
		}
		}*/

		if(sgninpoly)
		{
			cv = new VOXLST;
			cv->i = lw.i; cv->j = lw.j; cv->k = lw.k;
			cv->nxt = NULL;
			AddElmtToLnk(cv,&phead);
			PushAdjacentVoxel(lw);
		}
	}

	if(phead)
		RestoreSign(phead);

	return phead;
}


//////////////////////////////////////////////////////////////////
//FUNCTION: Push 27 voxels adjacent to voxel whose label is lw 
void CRec::PushAdjacentVoxel(LABEL lw)
{
	int imin,imax,jmin,jmax,kmin,kmax;
	imin = max(lw.i-1,0); imax = min(lw.i+1,XMAX-1);
	jmin = max(lw.j-1,0); jmax = min(lw.j+1,YMAX-1);
	kmin = max(lw.k-1,0); kmax = min(lw.k+1,ZMAX-1);

	int i,j,k;
	LABEL cl;
	for(i=imin; i<=imax; i++)
		for(j=jmin; j<=jmax; j++)
			for(k=kmin; k<=kmax; k++)
			{
				if(m_iSgnVoxel[i][j][k] == 0)
				{
					m_iSgnVoxel[i][j][k] = 1;
					cl.i = i; cl.j = j; cl.k = k;
					Push(cl);
				}
			}
}


//////////////////////////////////////////////////////////////////
//FUNCTION: Restore the sign of the voxel
void CRec::RestoreSign(VOXLST *ph)
{
	/*VOXLST *cv;
	int imin,imax,jmin,jmax,kmin,kmax;
	int i,j,k;
	cv = ph;
	while(cv)
	{
	imin = max(cv->i-1,0); imax = min(cv->i+1,XMAX-1);
	jmin = max(cv->j-1,0); jmax = min(cv->j+1,YMAX-1);
	kmin = max(cv->k-1,0); kmax = min(cv->k+1,ZMAX-1);

	for(i=imin; i<=imax; i++)
	{
	for(j=jmin; j<=jmax; j++)
	{
	for(k=kmin; k<=kmax; k++)
	m_iSgnVoxel[i][j][k] = 0;
	}
	}
	cv = cv->nxt;
	}*/

	int i,j,k;
	for(k = 0; k < ZMAX; k++)
		for(j = 0; j < YMAX; j++)
			for(i = 0; i < XMAX; i++)
				m_iSgnVoxel[i][j][k] = 0;

}


//////////////////////////////////////////////////////////////////
//FUNCTION: Push an element to the stack
void CRec::Push(LABEL lw)
{
	m_iStackPtr++;
	if(m_iStackPtr >= STACKMAX-1) 
		AfxMessageBox("Stack overflow");
	else
		m_lStack[m_iStackPtr] = lw;

}


//////////////////////////////////////////////////////////////////
//FUNCTION: Pop an element from the stack
LABEL CRec::Pop()
{
	if(m_iStackPtr <= -1)
		AfxMessageBox("Stack downflow");
	else
	{
		return m_lStack[m_iStackPtr--];
	}

}



//////////////////////////////////////////////////////////////////////////
//FUNCTION: Decide whether the point pt is in the polygon
BOOL CRec::PntInPoly(int fnum, int sign[], PNT norml[], PNT pnt[], PNT pt)
{
	int i;
	for(i = 0; i < fnum; i++)
	{
		if(sign[i]!=Sign(FaceEquation(norml[i],pnt[i],pt)))
			return 0;
	}
	return 1;

}


//////////////////////////////////////////////////////////////////
//FUNCTION: Get the vector for calculating two vectors perpecular
//          to the first edge
//     vect: the directional vector of the first edge
PNT CRec::GetMidVect(PNT vect)
{
	PNT rev;
	if(fabs(vect.x-vect.y) > 10e-3)
	{  rev.x = vect.y; rev.y = vect.x; rev.z = vect.z; }
	else if(fabs(vect.x-vect.z) > 10e-3)
	{  rev.x = vect.z; rev.y = vect.y; rev.z = vect.x; }
	else if(fabs(vect.y-vect.z) > 10e-3)
	{  rev.x = vect.x; rev.y = vect.z; rev.z = vect.y; }
	else
	{  rev.x = -vect.x; rev.y = vect.y; rev.z = vect.z;}

	return rev;
}


//////////////////////////////////////////////////////////////////
//FUNCTION: Get the eighte vertexes of the cube, the vertex whose
//          three coordinates is miminum is bom
void CRec::GetCubeVertex(PNT vert[], PNT bom)
{
	vert[0] = bom;
	vert[1].x=bom.x+m_ptStep.x; vert[1].y=bom.y; vert[1].z=bom.z;
	vert[2].x=bom.x+m_ptStep.x; vert[2].y=bom.y; 
	vert[2].z=bom.z+m_ptStep.z;
	vert[3].x=bom.x; vert[3].y=bom.y; vert[3].z=bom.z+m_ptStep.z;

	vert[4].x = bom.x; vert[4].y=bom.y+m_ptStep.y; vert[4].z=bom.z;
	vert[5].x=bom.x+m_ptStep.x; vert[5].y=bom.y+m_ptStep.y; 
	vert[5].z=bom.z;
	vert[6].x=bom.x+m_ptStep.x; vert[6].y=bom.y+m_ptStep.y; 
	vert[6].z=bom.z+m_ptStep.z;
	vert[7].x=bom.x; vert[7].y=bom.y+m_ptStep.y; 
	vert[7].z=bom.z+m_ptStep.z;


}


//////////////////////////////////////////////////////////////////
// FUNCTION: Add the element cv to link with head phead
void CRec::AddElmtToLnk(VOXLST *cv, VOXLST **phead)
{
	VOXLST *cvl;

	if(*phead == NULL)
		*phead = cv;
	else
	{
		cvl = *phead;
		while(cvl->nxt)
			cvl = cvl->nxt;
		cvl->nxt = cv;
	}

}


//////////////////////////////////////////////////////////////////
//FUNCTION: The rule of the difference of the maximum and minimum
//      edges is the minimum
inline double CRec::DiffMinimum(PNT p1, PNT p2, PNT pt, double s1)
{
	double s2, s3, minlen, maxlen;
	s2 = sqrt(pow(pt.x-p1.x,2)+pow(pt.y-p1.y,2)+pow(pt.z-p1.z,2));
	s3 = sqrt(pow(pt.x-p2.x,2)+pow(pt.y-p2.y,2)+pow(pt.z-p2.z,2));
	minlen = min(min(s1,s2),s3);
	maxlen = max(max(s1,s2),s3);

	return (maxlen-minlen);

}


///////////////////////////////////////////////////////////////////
//FUNCTION: Search the third vertex of the seed triangle
int CRec::SearchSeedVertex(int fnum, int sign[], PNT norm[], PNT pnt[], VOXLST *phead, EDGELST *pedge)
{
	VOXLST *cu;
	PNTLST *cp;
	int i,j,k,ser,notinpoly,minpntser;
	double mind = 1000000, dis;
	int pinequ;

	PNT pt,p1,p2;
	PNT v21,v12,v2,v3;
	double s1,s2,s3,alf1,alf2,alf3;
	double cosminalf = cos(30*3.1415926 / 180);
	p1 = m_ptPoint[pedge->pntno[0]].coord;
	p2 = m_ptPoint[pedge->pntno[1]].coord;
	v21 = p1 - p2;
	s1 = sqrt(pow(v21.x,2)+pow(v21.y,2)+pow(v21.z,2));
	v21 = v21 / s1;
	v12 = (p2 - p1) / s1;

	cu = phead;
	minpntser = -1; //if minpntser == -1, the point is boundary point
	while(cu)
	{
		i = cu->i; j = cu->j; k = cu->k;
		cp = m_pVoxel[i][j][k];
		while(cp)
		{
			ser = cp->pntno;
			if(ser == pedge->pntno[0] || ser == pedge->pntno[1])
			{
				cp = cp->nxt;
				continue;
			}
			pt = m_ptPoint[ser].coord;
			notinpoly = 0;
			for(i = 0; i < fnum; i++)
			{
				pinequ = Sign(FaceEquation(norm[i],pnt[i],pt));
				if((sign[i] != pinequ) && (pinequ != 0))
				{
					notinpoly = 1;
					break;
				}
			}
			if((!notinpoly) && (m_ptPoint[ser].pcsdsgn == 0)) 
			{
				//the point is in the polygon and it is not a 
				//fixed point
				//dis = CalcHigh(pt,pedge);
				//dis = DiffMinimum(p1,p2,pt,s1);
				dis = CalcSum(pt,pedge);
				if(mind > dis)
				{
					v2 = pt - p1;
					s2 = sqrt(pow(v2.x,2)+pow(v2.y,2)+pow(v2.z,2));
					v2 = v2 / s2;
					v3 = pt - p2;
					s3 = sqrt(pow(v3.x,2)+pow(v3.y,2)+pow(v3.z,2));
					v3 = v3 / s3;
					alf1 = v2.x*v12.x + v2.y*v12.y + v2.z*v12.z;
					alf2 = v3.x*v21.x + v3.y*v21.y + v3.z*v21.z;
					alf3 = max(alf1,alf2);
					if(alf3 < cosminalf)
					{
						mind = dis;
						minpntser = ser;
					}
				}
			}
			cp = cp->nxt;
		}
		cu = cu->nxt;
	}
	return minpntser;

}


///////////////////////////////////////////////////////////////////
//FUNCTION: Search the edge whose angle with edge s1s2 is smallest
EDGELST* CRec::SearchAdjEdge(int s1, int s2, int oppv, PNT norl, PNT pt)
{
	PNT p1,p2,v,normal,opp, vs1s2,p3;
	double norm, maxdot;
	int sgn,sgnv;

	p1 = m_ptPoint[s1].coord;
	p2 = m_ptPoint[s2].coord;
	p3 = m_ptPoint[oppv].coord;

	opp = (p1 + p2 + p3) / 3;



	vs1s2 = p2 - p1;
	norm = sqrt(pow(vs1s2.x,2) + pow(vs1s2.y,2) + pow(vs1s2.z,2));
	vs1s2 = vs1s2 / norm;

	v = p1 - opp;
	norm = sqrt(pow(v.x,2) + pow(v.y,2) + pow(v.z,2));
	v = v / norm;
	maxdot = v.x*vs1s2.x + v.y*vs1s2.y + v.z*vs1s2.z;

	normal = CrossNorml(vs1s2,norl);
	sgn = Sign(FaceEquation(normal,p1,pt));


	EDGELST *cu, *ae = NULL;
	BOOL dc1;
	double ang;
	cu = m_ptPoint[s1].pedge;
	while(cu)
	{
		dc1 = (cu->pntno[1] == s2);
		//dc2 = m_pDoc->m_ptPoint[cu->pntno[1]].pcsdsgn == FIXEDGE;
		if(dc1)
		{   cu = cu->nxt;  continue; }
		p2 = m_ptPoint[cu->pntno[1]].coord;
		sgnv = Sign(FaceEquation(normal,p1,p2));
		if(sgnv == sgn)
		{
			v = p2 - p1;
			norm = sqrt(pow(v.x,2)+pow(v.y,2)+pow(v.z,2));
			v = v / norm;
			ang = v.x*vs1s2.x + v.y*vs1s2.y + v.z*vs1s2.z;
			if(ang > maxdot)
			{
				maxdot = ang;
				ae = cu;
			}
		}
		cu = cu->nxt;
	}
	return ae;
}



//////////////////////////////////////////////////////////////////
//FUNCTION: Decide whether the point and the edge in same side
BOOL CRec::PntAndEdgeInSameSide(int ser, EDGELST *pedge)
{

	return 0;

}


//////////////////////////////////////////////////////////////////
//FUNCTION: Calculate the sum of the egde length of the triangle
double CRec::CalcSum(PNT pt, EDGELST *pedge)
{
	double sum, len1, len2;
	PNT p1, p2;

	p1 = m_ptPoint[pedge->pntno[0]].coord;
	p2 = m_ptPoint[pedge->pntno[1]].coord;

	len1 = sqrt(pow(p1.x-pt.x,2)+pow(p1.y-pt.y,2)+pow(p1.z-pt.z,2));
	len2 = sqrt(pow(p2.x-pt.x,2)+pow(p2.y-pt.y,2)+pow(p2.z-pt.z,2));

	sum = len1 + len2;
	return sum;
}

//////////////////////////////////////////////////////////////////
//FUNCTION: Delete the element from the line whose head is *ph
void CRec::DeleteElmt(EDGELST *ce, EDGEPNT **ph)
{
	EDGEPNT *cd, *od;

	if((*ph)->pedge == ce)
	{
		delete (*ph);
		*ph = NULL;
		return;
	}

	od = cd = *ph;
	while(cd)
	{
		if(cd->pedge == ce)
		{
			od->nxt = cd->nxt;
			delete cd;
		}
		od = cd;
		cd = cd->nxt;
	}


}


///////////////////////////////////////////////////////////////////
//FUNCTION: Look for seed triangle taking ser as vertex
void CRec::LookforSeedTriangle(int ser)
{
	int i,j,k;

	//look for the voxel the point ser contains in
	int l,m,n;
	PNT pt,pv;
	pt = m_ptPoint[ser].coord;
	l = (pt.x-m_bbBoundBox.xmin)/m_ptStep.x;
	m = (pt.y-m_bbBoundBox.ymin)/m_ptStep.y;
	n = (pt.z-m_bbBoundBox.zmin)/m_ptStep.z;

	//look for the point which the distance from point m_iZMaxPnt
	//to is shortest
	int imin,imax,jmin,jmax,kmin,kmax;
	PNTLST *cu;
	double dis, mindis = 1000000;
	int minpntno = -1;
	imin = max(l-2,0); imax = min(l+2,XMAX-1);
	jmin = max(m-2,0); jmax = min(m+2,YMAX-1);
	kmin = max(n-2,0); kmax = min(n+2,ZMAX-1);

	for(k = kmin; k <= kmax; k++)
		for(j = jmin; j <= jmax; j++)
			for(i = imin; i <= imax; i++)
			{
				cu = m_pVoxel[i][j][k];
				while(cu)
				{
					if((cu->pntno != ser) && (m_ptPoint[cu->pntno].pcsdsgn == 0))
					{
						pv = m_ptPoint[cu->pntno].coord;
						dis = pow(pt.x-pv.x,2)+pow(pt.y-pv.y,2)+pow(pt.z-pv.z,2);
						if(dis < mindis)
						{
							mindis = dis;
							minpntno = cu->pntno;
						}
					}
					cu = cu->nxt;
				}
			}
			if(minpntno == -1)
				AfxMessageBox("LookforSeedTriangle, No shortest vertext");
			EDGELST *el = new EDGELST;
			el->pntno[0] = ser;
			el->pntno[1] = minpntno;
			el->edgeno = 0; el->face[0] = NULL; el->face[1] = NULL;
			el->pcsdsgn = 0; el->nxt = NULL;

			//look for the shortest point nearest to the first edge
			int num = 0, sign[6];
			LABEL ptl;
			PNT norml[6], point[6]; //the four boundary faces
			double r = sqrt(mindis), runr = 0, vecnorm;
			PNT orthv1,orthv2, midv, midpnt;
			VOXLST *pvhead;
			//orthv1, orthv2: the vector perpendicular to vector p1p2
			PNT p1, p2, vec;
			p1 = m_ptPoint[ser].coord;
			p2 = m_ptPoint[minpntno].coord;
			vec.x = p1.x-p2.x; vec.y = p1.y-p2.y; vec.z = p1.z-p2.z;
			vecnorm = sqrt(pow(vec.x,2)+pow(vec.y,2)+pow(vec.z,2));
			vec.x=vec.x/vecnorm; vec.y=vec.y/vecnorm; vec.z=vec.z/vecnorm;
			midv = GetMidVect(vec);
			orthv1 = CrossNorml(vec,midv);
			orthv2 = CrossNorml(orthv1,vec);
			midpnt.x=(p1.x+p2.x)/2; midpnt.y=(p1.y+p2.y)/2; 
			midpnt.z=(p1.z+p2.z)/2;
			ptl.i = (midpnt.x-m_bbBoundBox.xmin)/m_ptStep.x;
			ptl.j = (midpnt.y-m_bbBoundBox.ymin)/m_ptStep.y;
			ptl.k = (midpnt.z-m_bbBoundBox.zmin)/m_ptStep.z;
			int serc;
			while(1)
			{
				//construct the six boundary faces of the domain polygon
				num++;
				runr = runr + num*r;

				norml[0] = orthv1;
				point[0] = midpnt + runr*orthv1;

				norml[1] = orthv1;
				point[1] = midpnt - runr*orthv1;

				norml[2] = orthv2;
				point[2] = midpnt + runr*orthv2;

				norml[3] = orthv2;
				point[3] = midpnt - runr*orthv2;

				norml[4] = vec;
				point[4] = p1 + runr*vec;
				//point[4] = p1;

				norml[5] = vec;
				point[5] = p2 - runr*vec;
				//point[5] = p2;

				for(i = 0; i < 6; i++)
					sign[i] = Sign(FaceEquation(norml[i],point[i],midpnt));

				pvhead = CreateVoxelList(6,sign,norml,point,midpnt,el,runr);
				serc = SearchSeedVertex(6,sign,norml,point,pvhead,el);
				if(serc != -1) break;
			}
			delete el;

			int v1 = serc, v2 = minpntno, v3 = ser;
			PNT pv1,pv2,pv3,vec1,vec2,facen;
			pv1 = m_ptPoint[v1].coord;
			pv2 = m_ptPoint[v2].coord;
			pv3 = m_ptPoint[v3].coord;
			vec1.x=pv2.x-pv1.x; vec1.y=pv2.y-pv1.y; vec1.z=pv2.z-pv1.z;
			vec2.x=pv3.x-pv1.x; vec2.y=pv3.y-pv1.y; vec2.z=pv3.z-pv1.z;
			facen = CrossNorml(vec1,vec2);
			CreateSeedTriangle(v1,v2,v3,facen);

			//adjust the state of three vertexes of the new triangle
			AdjustPntState(v1);
			AdjustPntState(v2);
			AdjustPntState(v3);
}


///////////////////////////////////////////////////////////////////
// FUNCTION: Look for a unprocessed vertex
int CRec::LookforUnprcsVertx()
{
	int i, sgn = 0;
	for(i = 0; i < m_iVertNum; i++)
	{
		if(m_ptPoint[i].pedge == NULL)
		{
			sgn = 1;
			return i;
		}
	}

	if(sgn == 0)
		return -1;

}


//////////////////////////////////////////////////////////////////
//FUNCTION: Search the new vertex opposite to edge pedge
int CRec::SearchNewVertex(VOXLST *phead, EDGELST *pedge)
{
	VOXLST *cu;
	PNTLST *cp;
	int i,j,k,ser,minpntser;
	double mind = 1000000, dis;
	FACELST *pf;

	PNT pt,p1,p2;
	double s1;
	BOOL cond;
	p1 = m_ptPoint[pedge->pntno[0]].coord;
	p2 = m_ptPoint[pedge->pntno[1]].coord;
	s1 = sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2)+pow(p1.z-p2.z,2));

	pf = pedge->face[0];

	cu = phead;
	minpntser = -1; //if minpntser == -1, the point is boundary point
	while(cu)
	{
		i = cu->i; j = cu->j; k = cu->k;
		cp = m_pVoxel[i][j][k];
		while(cp)
		{
			ser = cp->pntno;
			cond = (pf!=NULL)&&(ser==pf->pntno[0]||ser==pf->pntno[1]||ser==pf->pntno[2]);
			if(cond)
			{
				cp = cp->nxt;
				continue;
			}
			pt = m_ptPoint[ser].coord;
			if(m_ptPoint[ser].pcsdsgn != 1) 
			{
				//the point is in the polygon and it is not a 
				//fixed point
				//dis = CalcHigh(pt,pedge);
				//dis = DiffMinimum(p1,p2,pt,s1);
				dis = CalcSum(pt,pedge);
				if(mind > dis)
				{
					mind = dis;
					minpntser = ser;
				}
			}
			cp = cp->nxt;
		}
		cu = cu->nxt;
	}
	return minpntser;
}



//////////////////////////////////////////////////////////////////
//FUNCTION: Get the max length edge adjacent to the vertex s
double CRec::GetMaxEdgeLen(int s)
{
	EDGELST *cu;
	PNT ps, pe;
	double lg, longe = -100000;
	int num = 0;
	double len = 0;
	ps = (m_ptPoint[s]).coord;
	cu = m_ptPoint[s].pedge;
	while(cu)
	{
		pe = (m_ptPoint[cu->pntno[1]]).coord;
		lg = pow(ps.x-pe.x,2)+pow(ps.y-pe.y,2)+pow(ps.z-pe.z,2);
		//len = len + sqrt(lg);
		//num++;
		if(longe < lg)   longe = lg;
		cu = cu->nxt;
	}

	return (sqrt(longe));
}


//////////////////////////////////////////////////////////////////
//FUNCTION: Get the min length edge adjacent to the vertex s
double CRec::GetMinEdgeLen(int s)
{
	EDGELST *cu;
	PNT ps, pe;
	double lg, longe = 100000;
	int num = 0;
	double len = 0;
	ps = (m_ptPoint[s]).coord;
	cu = m_ptPoint[s].pedge;
	while(cu)
	{
		pe = (m_ptPoint[cu->pntno[1]]).coord;
		lg = pow(ps.x-pe.x,2)+pow(ps.y-pe.y,2)+pow(ps.z-pe.z,2);
		//len = len + sqrt(lg);
		//num++;
		if(longe > lg)   longe = lg;
		cu = cu->nxt;
	}

	return (sqrt(longe));
}


//////////////////////////////////////////////////////////////////
//FUNCTION: Get the mean normal of faces adjacent to the three
//          vertexes of the triangle pf
PNT CRec::GetMeanNormal(FACELST *pf)
{
	FACEPNT *pfp;
	int i, num = 0;
	PNT norl = {0,0,0};

	for(i = 0; i < 3; i++)
	{
		pfp = m_ptPoint[pf->pntno[i]].pface;
		while(pfp)
		{
			norl = norl + pfp->pface->norml;
			num++;
			pfp = pfp->nxt;
		}
	}
	norl = norl / num;

	double norm;
	norm = sqrt(pow(norl.x,2)+pow(norl.y,2)+pow(norl.z,2));
	norl = norl / norm;

	return norl;
}


///////////////////////////////////////////////////////////////
//FUNCTION: Create the new triangle
FACELST* CRec::CreateNewTriangle(int s1, int s2, int s3)
{
	//generate the face
	FACELST *cf;
	cf = new FACELST;
	cf->faceno = m_iFaceNo++;
	cf->nxt = NULL;

	//generate the normal and adjust it outwards
	PNT vec1, vec2, p1, p2, p3, nrml;
	p1 = m_ptPoint[s1].coord;
	p2 = m_ptPoint[s2].coord;
	p3 = m_ptPoint[s3].coord;
	vec1.x=p2.x-p1.x; vec1.y=p2.y-p1.y; vec1.z=p2.z-p1.z;
	vec2.x=p3.x-p1.x; vec2.y=p3.y-p1.y; vec2.z=p3.z-p1.z;
	nrml = CrossNorml(vec1,vec2);
	cf->pntno[0]=s1; cf->pntno[1]=s2; cf->pntno[2]=s3;
	cf->norml = nrml;

	return cf;
}


///////////////////////////////////////////////////////////////
//FUNTION: Get the ratio of the length adjacent to ser
double CRec::GetLengthRatio(int s)
{
	EDGELST *cu;
	PNT ps, pe;
	double lg, longe = -1000000, shorte = 1000000;
	ps = (m_ptPoint[s]).coord;
	cu = m_ptPoint[s].pedge;
	while(cu)
	{
		pe = (m_ptPoint[cu->pntno[1]]).coord;
		lg = pow(ps.x-pe.x,2)+pow(ps.y-pe.y,2)+pow(ps.z-pe.z,2);
		if(longe < lg)   longe = lg;
		if(shorte > lg) shorte = lg;
		cu = cu->nxt;
	}

	//if(longe > 0)
	return (sqrt(longe/shorte));
	//else
	//return -1;

}



/////////////////////////////////////////////////////////////////
//FUNCTION: Calculate the radius of circumscribed circle of the 
//          triangle
double CRec::CalcR(PNT pt, EDGELST *pedge, double len)
{
	PNT p0,p1,v0,v1,v;
	p0 = m_ptPoint[pedge->pntno[0]].coord;
	p1 = m_ptPoint[pedge->pntno[1]].coord;
	v0 = p0 - pt;
	v1 = p1 - pt;
	v.x = v0.y*v1.z - v0.z*v1.y;
	v.y = v0.z*v1.x - v0.x*v1.z;
	v.z = v0.x*v1.y - v0.y*v1.x;

	double l0, l1,lv0v1,sina;

	l0 = pow(v0.x,2)+pow(v0.y,2)+pow(v0.z,2);
	l1 = pow(v1.x,2)+pow(v1.y,2)+pow(v1.z,2);
	lv0v1 = pow(v.x,2)+pow(v.y,2)+pow(v.z,2);
	sina = sqrt(lv0v1 / (l0*l1));

	return (len / (2*sina));



}


/////////////////////////////////////////////////////////////////
//FUNCTION: Calculate the weight sum of the new triangle
double CRec::CalcWeightSum(PNT pt, EDGELST* pedge, double lij, double co1)
{
	//Calculate coefficient for harmonic map
	double lik1,ljk1,aijk1;
	PNT p1,p2;
	double co2,co3,co4;
	p1 = m_ptPoint[pedge->pntno[0]].coord;
	p2 = m_ptPoint[pedge->pntno[1]].coord;
	lik1 = pow(p1.x-pt.x,2)+pow(p1.y-pt.y,2)+pow(p1.z-pt.z,2);
	ljk1 = pow(p2.x-pt.x,2)+pow(p2.y-pt.y,2)+pow(p2.z-pt.z,2);
	double l1,l2,l3,s;
	l1 = sqrt(lij); l2 = sqrt(lik1); l3 = sqrt(ljk1);
	s = (l1 + l2 + l3) / 2;
	aijk1 = sqrt(s*(s-l1)*(s-l2)*(s-l3));

	co2 = (lik1 + ljk1 - lij) / aijk1;
	co3 = (lij + ljk1 - lik1) / aijk1;
	co4 = (lij + lik1 - ljk1) / aijk1;

	double rev;
	rev = (co1+co2)*lij + 2*co2*lik1 + 2*co2*ljk1;
	//rev = (co1+co2)*lij + lik1 + ljk1;
	//rev = co1 + co2;
	//rev = (co1+co2)*lij + co3*lik1 + co4*ljk1;
	//rev = l2+l3;
	return rev;

}

//-----------------------------------------------------------------------
//Function: Check whether the point nser is the correct vertex of new
//          triangle
bool CRec::PntIsVertex(int nser, int ser1, int ser2, FACEPNT* pl)
{
	FACEPNT *fp;
	fp = pl;
	if(fp == NULL) return 1;

	FACELST *fl;
	while(fp)
	{
		fl = fp->pface;
		if(HaveIntsctBtwTris(nser,ser1,ser2,fl))
			return 0;
		fp = fp->nxt;
	}

	return 1;
}


//-----------------------------------------------------------------------
//Function: Check whether there is intersection between two triangles,
//          one with (s1, s2, s3), the other fl
bool CRec::HaveIntsctBtwTris(int s1, int s2, int s3, FACELST *fl)
{
	int es1, es2, es3;
	es1 = fl->pntno[0]; es2 = fl->pntno[1]; es3 = fl->pntno[2];

	if(HaveIntsctBtwLineTri(s1,s2,es1,es2,es3))
		return 1;
	else if(HaveIntsctBtwLineTri(s2,s3,es1,es2,es3))
		return 1;
	else if(HaveIntsctBtwLineTri(s3,s1,es1,es2,es3))
		return 1;
	else if(HaveIntsctBtwLineTri(es1,es2,s1,s2,s3))
		return 1;
	else if(HaveIntsctBtwLineTri(es2,es3,s1,s2,s3))
		return 1;
	else if(HaveIntsctBtwLineTri(es3,es1,s1,s2,s3))
		return 1;

	return 0;
}


//-----------------------------------------------------------------------
//Function: Check whether there is intersection between line with
//          (ls1,ls2) and triangle with(ts1,ts2,ts3)
bool CRec::HaveIntsctBtwLineTri(int ls1, int ls2, int ts1, int ts2, int ts3)
{
	//calculate the normal of the triangle with (ts1,ts2,ts3)
	PNT p1, p2, p3, v1, v2, norl;
	p1 = m_ptPoint[ts1].coord;
	p2 = m_ptPoint[ts2].coord;
	p3 = m_ptPoint[ts3].coord;

	v1 = p2 - p1;
	v2 = p3 - p1;

	norl = CrossNorml(v1,v2);

	//Calculate the intersecting point between line and triangle
	double t;
	PNT lp1,lp2, intsp;
	lp1 = m_ptPoint[ls1].coord;
	lp2 = m_ptPoint[ls2].coord;
	if(norl*(lp1-lp2) == 0) return 0;
	t = (norl*(p1-lp1)) / (norl*(lp1-lp2));
	if(t<=0 || t>=1 || fabs(t) < 10e-10 || fabs(t-1) < 10e-10) return 0;
	intsp = lp1 + t*(lp1-lp2);

	//check whether intersecting point intsp is the inner point of 
	//triangle with(ts1,ts2,ts3)
	PNT cue, ote, che;
	PNT cros1, cros2;
	double chn;
	//edge (ts1,ts2)
	che = intsp - p1;
	cue = p2 - p1; ote = p3 - p1;
	cros1 = CrossNorml(cue,che);
	cros2 = CrossNorml(cue,ote);
	chn = cros1 * cros2;
	if(chn < 0) return 0;

	//edge (ts2,ts3)
	che = intsp - p2;
	cue = p3 - p2; ote = p1 - p2;
	cros1 = CrossNorml(cue,che);
	cros2 = CrossNorml(cue,ote);
	chn = cros1 * cros2;
	if(chn < 0) return 0;

	//edge (ts3,ts1)
	che = intsp - p3;
	cue = p1 - p3; ote = p2 - p3;
	cros1 = CrossNorml(cue,che);
	cros2 = CrossNorml(cue,ote);
	chn = cros1 * cros2;
	if(chn < 0) return 0;

	//the intersecting point is in the new and old triangles
	return 1;

}



//-----------------------------------------------------------------------
//Function: Calculate the length ratio at the point
void CRec::CalcPntRatio(double& maxrat, double& minrat)
{
	int i;
	maxrat = -10000; minrat = 10000;

	for(i = 0; i < m_iVertNum; i++)
	{
		m_dPntRatio[i] = GetLengthRatio(i);
		if(maxrat < m_dPntRatio[i])
			maxrat = m_dPntRatio[i];
		if(minrat > m_dPntRatio[i])
			minrat = m_dPntRatio[i];
	}

}


//-----------------------------------------------------------------------
//Function: Statistic the number of points whose length ratioes are 
//          between minr and maxr
int CRec::StatsPntNumBtwRtos(double minr, double maxr)
{
	int i, num = 0;
	for(i = 0; i < m_iVertNum; i++)
	{
		if(m_dPntRatio[i] > minr && m_dPntRatio[i] <= maxr)
			num++;
	}
	return num;
}


PNT PNT::operator +(PNT p)
{
	PNT r;
	r.x = x + p.x;
	r.y = y + p.y;
	r.z = z + p.z;

	return r;

}

double PNT::operator *(PNT p)
{
	double r;

	r = x*p.x + y*p.y + z*p.z;

	return r;

}

PNT PNT::operator -(PNT p)
{
	PNT r;
	r.x = x - p.x;
	r.y = y - p.y;
	r.z = z - p.z;

	return r;

}

PNT operator *(double n,PNT &p)
{
	PNT r;
	r.x = n*p.x; r.y = n*p.y; r.z = n*p.z;
	return r;

}

PNT operator /(PNT &p, double d)
{
	PNT r;
	r.x = p.x / d; r.y = p.y / d; r.z = p.z / d;
	return r;
}

void CRec::Display(void)
{

	int i,j;
	double vert[3];
	double norm[3];
	PNT vertex,normal;
	
	glEnable(GL_CULL_FACE);
	glColor3f(m_fColor[0],m_fColor[1],m_fColor[2]);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_TRIANGLES);
	for(i = 0; i < m_iFaceNum; i++)
	{
		for(j = 0; j < m_faceFace[i].vnum; j++)
		{
			vertex = m_ptPoint[m_faceFace[i].v[j]].coord;
			vert[0] = vertex.x; 
			vert[1] = vertex.y;
			vert[2] = vertex.z;
			normal = m_pntVertNorm[m_faceFace[i].vn[j]];
			norm[0] = (float)normal.x;
			norm[1] = (float)normal.y;
			norm[2] = (float)normal.z;

			glNormal3dv(norm);
			glVertex3dv(vert);
		}
	}
	glEnd();
}


PNT CRec::CalcNrml(int ser)
{
	FACELST *cf;
	FACEPNT *cfp;
	int num = 0;
	PNT nrml = {0,0,0};
	cfp = m_ptPoint[ser].pface;
	while(cfp)
	{
		cf = cfp->pface;

		nrml.x = nrml.x + cf->norml.x;
		nrml.y = nrml.y + cf->norml.y;
		nrml.z = nrml.z + cf->norml.z;

		++num;
		cfp = cfp->nxt;
	}

	nrml.x=nrml.x/num; nrml.y=nrml.y/num; nrml.z=nrml.z/num;

	double norm;
	norm = sqrt(pow(nrml.x,2)+pow(nrml.y,2)+pow(nrml.z,2));
	nrml.x=nrml.x/norm; nrml.y=nrml.y/norm; nrml.z=nrml.z/norm;

	return nrml;
}

void CRec::SetColor(float r, float g, float b)
{
	m_fColor[0]=r;
	m_fColor[1]=g;
	m_fColor[2]=b;
}

void CRec::Inverse(void)
{
	int i,tmp;

	for(i = 0; i < m_iFaceNum; i++)
	{
		tmp = m_faceFace[i].v[0];
		m_faceFace[i].v[0]=m_faceFace[i].v[1];
		m_faceFace[i].v[1]=tmp;
	}
}

void CRec::AddPoint(CVector3d coord, CVertex* pVertex)
{
	PNT pntTempVertex;
	pntTempVertex.x=coord[0];
	pntTempVertex.y=coord[1];
	pntTempVertex.z=coord[2];
	m_ptPoint[m_iVertNum].coord = pntTempVertex;
	m_ptPoint[m_iVertNum].pcsdsgn = 0;
	m_ptPoint[m_iVertNum].pedge = NULL;
	m_ptPoint[m_iVertNum].pface = NULL;
	m_ptPoint[m_iVertNum].pVertex=pVertex;
	m_iVertNum++;

}
