#pragma once

#include "global.h"
#include "ImplicitSurface.h"
#include "SpatialSubdivision.h"
#define CLOSEST_CACHE_SIZE 6 
#define CLOSEST_CACHE_TRIANGLESIZE 6        

#define GRADIENT_EPSILON .001  // EPSILON used for some calculations

#define WH_ALPHA     6.      // If this value works, we are happy 
#define WH_BETA     10.      // Used in computation of delta_rad 
#define WH_GAMMA     4.      // Equilibrium speed 
#define WH_DELTA      .7     // .7 * nominal radius is minimum radius 
#define WH_RHO       10.     // Coeff. for radius term 
#define WH_PHI       10.     // Coeff. for gradient feedback term 
#define WH_NY         .5
#define WH_DELTA_T    .1     // Time step 
#define WH_SIGMA_MAX 1.5

#define FLOATER_RES     12    // Number of triangles in floater 

#define MAX_PARTICLES 10000     // Max. particles per surface

#define SET_FLAG     1
#define NOT_SET_FLAG 0

#define PT_BOUNDARY 0
#define PT_VIRTUAL  1
#define PT_NORMAL   2

class CParticleSamplingSys
{
public:
	CParticleSamplingSys(CImplicitSurface* pSolver,float ratio, float rad);
	CParticleSamplingSys();
	~CParticleSamplingSys(void);
private:

	double maxx,maxy,maxz; //theshold of x,y,z

	int size;              // Total Number of particles

	int ci[MAX_PARTICLES];  
	int particles_poll[MAX_PARTICLES];



	int pt[MAX_PARTICLES];
	CVector3d p[MAX_PARTICLES];  // List of particles 

	CVector3d n[MAX_PARTICLES];  // List of normals/gradients at each particle

	float turk_radius[MAX_PARTICLES], init_turk_radius;

	int particles_poll_size;// Number of particles in poll

	int initial_particles;  // Number of particles to begin with

	float time_step;       // Step size for numerical computations

	float turk_radius_factor; // Global user-specified parameter. When this is changed, the turk_radius array above is modified accordingly 
	char zombie_flag[MAX_PARTICLES]; // Set if a particle is deleted and put in the particles_poll so that deleted particles do not repel others 

	CImplicitSurface *ImpFunc;   

	float cos_table[FLOATER_RES],sin_table[FLOATER_RES];

	CSpatialSubdivision *subd;

	int closest_cache[MAX_PARTICLES][CLOSEST_CACHE_SIZE];
	int closest_cache_size[MAX_PARTICLES];
	float closest_cache_lgt[MAX_PARTICLES][CLOSEST_CACHE_SIZE];

public:
	void SplitParticle(int);    // Split a particle into two new slighly perturbed ones

	void UpdateParticle(int);   // Update one particle

	void ComputeWHStuff(int,CVector3d *, float *,char *,char *);// Compute delta repulsion force delta_P (projected to tangent plane) and delta repulsion radius delta_rad.Last two parameters are flags that specify whether or not the particle is to be split or to die

	void InsertParticle(CVector3d *,float, int);  

private:
	void PrecomputeMathTable(void);
public:
	void DetermineClosestNeighborsFromNeighbourhood(int id,vector<int> neighbours);
	void ComputeNormal(int id);
	void DeleteParticle(int id);
	void UpdateParticles(void);
	void Display(void);
	void DisplayNI(int index);
	int GetParticleNum(void);
	void GetParticleByIndex(int index,CVector3d& coord, int& PntT);
	void GetParticleByIndex(int index, CVector3d& coord, CVector3d& normal, float& rad, int& PntT);
	void InsertParticle(CVector3d& coord, CVector3d& normal, float rad, int PntT);
};
