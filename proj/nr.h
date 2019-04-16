#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "stdafx.h"
#include <math.h>
#define NRANSI
#define ROTATE(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau);\
	a[k][l]=h+s*(g-h*tau);
#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}
float *fvector(long nl, long nh);
double *dvector(long nl, long nh);
int *ivector(long nl, long nh);

void free_ivector(int *v, long nl, long nh);
void free_fvector(float *v, long nl, long nh);
void free_dvector(double *v, long nl, long nh);

void nrerror(char error_text[]);
void linbcg(unsigned long n, double b[], double x[], int itol, double tol,
			int itmax, int *iter, double *err);
double snrm(unsigned long n, double sx[], int itol);
void asolve(unsigned long n, double b[], double x[], int itrnsp);
void atimes(unsigned long n, double x[], double r[], int itrnsp);
void dsprsax(double sa[], unsigned long ija[], double x[], double b[],
			 unsigned long n);
void dsprstx(double sa[], unsigned long ija[], double x[], double b[],
			 unsigned long n);
void eigsrt(float d[], float **v, int n);
void jacobi(float **a, int n, float d[], float **v, int *nrot);
void gauss_jordan(float **a, int n, float ** b, int m);

float **matrix(long nrl, long nrh, long ncl, long nch);
void free_matrix(float **m, long nrl, long nrh, long ncl, long nch);
