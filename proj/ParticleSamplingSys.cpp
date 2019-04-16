#include "StdAfx.h"
#include ".\particlesamplingsys.h"
#include <math.h>
#include <fstream>
extern ofstream outfile;
using namespace std;
CParticleSamplingSys::CParticleSamplingSys(CImplicitSurface* pSolver,float r, float rad)
{

	maxx = maxy = maxz = 100.0;

	size = 0;

//	radii = 0.0;	

	particles_poll_size = 0;      // No deleted particles in poll

	// Init repulsion radius stuff

	initial_particles = 10;       // Approx. k*N particles initially
	// sounds reasonable

	turk_radius_factor = 0.6;      // Default
	init_turk_radius =0.6*rad;



	time_step = 1.;               // Time step for numerical stuff

//	ratio=r;
	PrecomputeMathTable();
	subd = new CSpatialSubdivision();
	subd->Initialize();
	// Create spatial subdivision

	// Initialize repulsion radius for each particle
//	init_turk_radius = ratio*rad; 

	for ( int i = 0; i < MAX_PARTICLES; i++)
		turk_radius[i] = init_turk_radius;

	// Start with just one particle on the primitive

	ImpFunc = pSolver;


}

CParticleSamplingSys::CParticleSamplingSys()
{

}
CParticleSamplingSys::~CParticleSamplingSys()
{

}
void CParticleSamplingSys::SplitParticle(int id)
{
	CVector3d new_loc,tgt1,tgt2;
	float pert = .5*init_turk_radius*turk_radius_factor;

	tgt1 = Ortho3(n[id]);
    tgt1.Normalize();
	tgt2 = n[id].Cross(tgt1);

	tgt1 *= pert*my_random();
	tgt2 *= pert*my_random();

	new_loc=tgt1+tgt2+p[id];

	turk_radius[id] /= sqrt(2.);

	(void) InsertParticle(&new_loc,turk_radius[id],2);
}
void CParticleSamplingSys::InsertParticle(CVector3d *v,float rad,int PntT)
{
	if (size >= MAX_PARTICLES-1) {
		printf("Internal error: no room to insert particle. Please try again later\n");
		return;
	}

	if (particles_poll_size == 0)   // No indices left to be reused
	{
		ci[size] = size;
	}
	else 
	{

		ci[size] = particles_poll[--particles_poll_size];
	}
	pt[ci[size]]=PntT;

	p[ci[size]] = *v;
//	if(IsNearBoundary(v))
//		p[ci[size]].nb=true;

	// Mark entry id as a well and alive particle

	zombie_flag[ci[size]] = NOT_SET_FLAG;

	// Insert in spatial subdivion

	subd->Insert(ci[size],&(p[ci[size]]));

	turk_radius[ci[size]] = rad;

	//// Find neighbors
	vector<int> neighbours;
	subd->FindNeighborhood(ci[size],&neighbours);
	DetermineClosestNeighborsFromNeighbourhood(ci[size],neighbours);
	// Compute normal

	ComputeNormal(ci[size]);

	// Update particle counter

	size++;
}

void CParticleSamplingSys::UpdateParticle(int i)
{
	CVector3d delta_P,gradient,norm_component,norm_gradient;
	float delta_rad,feedback,sq_gradient_length,dst;
	char split_flag,death_flag;
	int this_id;
	bool nb;


	this_id = ci[i];

	if(abs(p[this_id].x()) >= maxx) 
	{  
		p[this_id].x( p[this_id].x() > 0? maxx:-maxx);  
		return;
	}
	if(abs(p[this_id].y()) >= maxy) 
	{  
		p[this_id].y(p[this_id].y() > 0? maxy:-maxy);  
		return;
	}
	if(abs(p[this_id].z()) >= maxz) 
	{  
		p[this_id].z( p[this_id].z() > 0? maxz:-maxz);  
		return;
	}
	if(pt[this_id]==PT_BOUNDARY||pt[this_id]==PT_VIRTUAL)
		return; 

	split_flag = death_flag = NOT_SET_FLAG;

	vector<int> neighbours;
	subd->FindNeighborhood(this_id,&neighbours);
	DetermineClosestNeighborsFromNeighbourhood(this_id,neighbours);


	// Update repulsion radius
	ComputeWHStuff(this_id,&delta_P,&delta_rad,&split_flag,&death_flag);

	turk_radius[this_id] += delta_rad;

	gradient = ImpFunc->GradientAt((p[this_id]));
	sq_gradient_length = gradient.LengthSquared();
	norm_gradient=gradient;
	norm_gradient.Normalize();
	n[this_id] = norm_gradient;

	if (sq_gradient_length >= GRADIENT_EPSILON) {    
		// Rather do nothing than
		// explode as suggested by W/H

		// Remove normal component of delta_P

		norm_component = (norm_gradient.Dot(delta_P))*norm_gradient;
		delta_P -= norm_component;
		p[this_id] += delta_P;
		//	outfile<<"UPDATED PARTICLE["<<p[this_id].x<<","<<p[this_id].y<<","<<p[this_id].z<<"]="<<function->GetValueAt(&(p[this_id]))<<endl;

		// Now compute and add feedback term

		feedback = ImpFunc->GetValueAt(p[this_id]);
		gradient = (WH_DELTA_T*WH_PHI*(-feedback)/(sq_gradient_length))*gradient; 
		p[this_id] += gradient;



		dst = delta_P.Length();

		split_flag = split_flag && 
			(dst < WH_GAMMA*init_turk_radius*turk_radius_factor);
		//	    (dst < WH_GAMMA*init_turk_radius*turk_radius_factor);


		if (split_flag) 
			delta_P += gradient;
		else if (death_flag)
			death_flag = death_flag && (dst < WH_GAMMA*init_turk_radius*turk_radius_factor);

		// Update spatial subdivision
		subd->Update(this_id,p[this_id]);

	}
	else {    // Kill particle if gradient is neglible
		split_flag = NOT_SET_FLAG;
		death_flag = SET_FLAG;
	}

	// A particle life's great moments: reproduction and death

	if (split_flag)
		(void) SplitParticle(this_id);
	else
		if (death_flag)
			(void) DeleteParticle(i);
}

void CParticleSamplingSys::PrecomputeMathTable(void)
{
	float x,dx;
	int i;

	dx = 2.*M_PI/FLOATER_RES;

	for ( i = 0, x = 0.; i < FLOATER_RES; i++, x += dx) {
		cos_table[i] = cos(x);
		sin_table[i] = sin(x);
	}

}

void CParticleSamplingSys::DetermineClosestNeighborsFromNeighbourhood(int id,vector<int> neighbours)
{
	int nbor_id,total_checked=0,cut_id = -1;
	float dst,max_radius,min_radius,norm_radius,cut = -1.;   
	for(int i=0;i<neighbours.size();i++)
	{
		nbor_id=neighbours.at(i);
		max_radius = MAX(turk_radius[id],turk_radius[nbor_id]);
		min_radius = MIN(turk_radius[id],turk_radius[nbor_id]);
		norm_radius = max_radius+2*min_radius;
		dst = (p[id]-p[nbor_id]).LengthSquared();
		dst = dst/(norm_radius*norm_radius);
		if (total_checked < CLOSEST_CACHE_SIZE) {

			// Check the normals to avoid nasty case (HACK!)

			if ((n[id].Dot(n[nbor_id])) > 0.1) {
				closest_cache_lgt[id][total_checked] = sqrt(dst);
				closest_cache[id][total_checked] = nbor_id;
				if (dst > cut) {
					cut = dst;
					cut_id = total_checked;
					//  cut_id always particles to id of largest 
					// of CL_CACHE_SZ smallest values so far  
				}
				total_checked++;
			}
		}
		else
			if ((dst < cut) && ((n[id].Dot(n[nbor_id])) > 0.1)) {	      
				int i;

				closest_cache_lgt[id][cut_id] = dst;
				closest_cache[id][cut_id] = nbor_id;

				// Find new cut and cut_id 

				cut = -1.;
				for ( i = 0; i < total_checked; i++)
					if (closest_cache_lgt[id][i] > cut) {
						cut = closest_cache_lgt[id][i];
						cut_id = i;
					}//for
			}//if

	}

	closest_cache_size[id] = total_checked;



}

void CParticleSamplingSys::ComputeNormal(int id)
{
	n[id] = (ImpFunc->GradientAt((p[id])));
	n[id].Normalize();

}

void CParticleSamplingSys::DeleteParticle(int id)
{
	// Mark entry id as a zombie

	zombie_flag[ci[id]] = SET_FLAG;

	// Delete from spatial subdivion

	subd->Delete(ci[id]);

	particles_poll[particles_poll_size++] = ci[id];
	ci[id] = ci[size-1];

	size--;

}

void CParticleSamplingSys::UpdateParticles(void)
{
	//outfile<<"updating particles"<<endl;
	int i;
	for ( i = 0 ; i < size; i++)
		UpdateParticle(i);


}
void CParticleSamplingSys::ComputeWHStuff(int id,CVector3d *delta_P,float *delta_rad,char *split_flag,char *death_flag)
{
	int i,nbor_id,j;
	float DD,DDt,Di,dst,E_ij,E_ji,E_hat,expected_radius;
	CVector3d delta,vtmp,normal_comp;
	static float time_step_factor = .25;

	delta_P->Set(0.0,0.0,0.0);

	DDt = Di = 0;

	// Loop through neighbor cache and compute forces


	expected_radius = init_turk_radius*turk_radius_factor;



	float sq_radius,sq_nbor_radius,radius,nbor_radius;
	radius = turk_radius[id];
	sq_radius = radius*radius;

	//outfile<<"closest_cache_size[id]="<<closest_cache_size[id]<<endl;

	for ( i = 0; i < closest_cache_size[id]; i++) {

		nbor_id = closest_cache[id][i];

		if ((!zombie_flag[nbor_id]) && (n[id].Dot(n[nbor_id]) > 0.0)) {

			// Don't want previously deleted particles to repel us

			delta= p[id]-p[nbor_id];


			dst=delta.LengthSquared();

			//outfile<<"dst="<<dst<<endl;

			nbor_radius=turk_radius[nbor_id];
			sq_nbor_radius=nbor_radius*nbor_radius;

			if (dst < 16*sq_radius) {

				// Compute delta_P

				E_ij = time_step*time_step_factor*WH_ALPHA*fast_exp(-.5*dst/sq_radius);
				E_ji = time_step*time_step_factor*WH_ALPHA*fast_exp(-.5*dst/sq_nbor_radius);

				vtmp=(E_ij+sq_radius/sq_nbor_radius*E_ji)*delta;

				Di += E_ij;
				DDt += E_ij/(sq_radius*radius);

				//outfile<<"<"<<vtmp[0]<<","<<vtmp[1]<<","<<vtmp[2]<<">"<<endl;

				*delta_P +=vtmp;
			}
		}
	}

	E_hat = .8*WH_ALPHA*time_step*time_step_factor;

	DD = Di - E_hat;
	DDt += WH_BETA*time_step*time_step_factor;

	*delta_rad = (-WH_RHO*DD)/DDt*WH_DELTA_T;
	if (*delta_rad > expected_radius)
		*delta_rad = expected_radius;

	*delta_P *= WH_DELTA_T;
	dst =delta_P->Length();

	// Determine whether or not to split: 
	// (Condition 1: particle is near equilibrium) AND
	// (Condition 2a: repulsion radius is huge. OR Condition 2b: particle is
	// adequately energized and repulsion radius is higher than nominal)

	// Is Particle in equilibrium?

	/*	*split_flag = ((dst < WH_GAMMA*expected_radius*ResPar) &&
	((turk_radius[id] > WH_SIGMA_MAX*expected_radius*ResPar) ||
	((Di > WH_NY*E_hat*ResPar) && (turk_radius[id] > expected_radius*ResPar))));
	*/

	*split_flag = ((dst < WH_GAMMA*expected_radius) &&
		((turk_radius[id] > WH_SIGMA_MAX*expected_radius*1.2) ||
		((Di > WH_NY*E_hat) && (turk_radius[id] > expected_radius*1.2))));
	//if(*split_flag)
	//{
	//	outfile<<"dst="<<dst<<endl;
	//	outfile<<"expected_radius="<<expected_radius<<endl;
	//	outfile<<"turk_radius="<<turk_radius[id]<<endl;
	//	outfile<<"Di="<<Di<<endl;
	//	outfile<<"E_hat="<<E_hat<<endl;
	//}

	// Determine whether or not particle is to die. 3 conds. must be satisfied:
	// 1) Particle velocity is small compared to nominal repulsion radius
	// 2) Repulsion radius is smaller than minimum repulsion radius
	// 3) Biased random test succeds

	if (*split_flag) 
		*death_flag = NOT_SET_FLAG;
	else {
		float random_val;

		random_val = my_random();

		/*		*death_flag = ((dst < .95*ResPar*WH_GAMMA*expected_radius) && 
		(turk_radius[id] < .95*ResPar*WH_DELTA*expected_radius) &&
		(random_val > turk_radius[id]/(WH_DELTA*expected_radius)));
		*/
		*death_flag = ((dst < .95*WH_GAMMA*expected_radius) && 
			(turk_radius[id] < .95*WH_DELTA*expected_radius) &&
			(random_val > turk_radius[id]/(WH_DELTA*expected_radius)));
	}
}








void CParticleSamplingSys::Display(void)
{
	CVector3d tgt1,tgt2,vtmp,tmp;

	int i,j;

	glEnable(GL_DEPTH_TEST);

	for (i = 0; i < size; i++){


		vtmp = n[ci[i]];
		tgt1 = Ortho3(vtmp);
		tgt1.Normalize();
		tgt2 = vtmp.Cross(tgt1);
		tgt2.Normalize();

		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		switch(pt[ci[i]])
		{
		case 0:
			glColor3f(0.0,0.0,1.0);break;
		case 1:
			glColor3f(0.0,1.0,0.0);break;
		case 2:
			glColor3f(1.0,0.0,0.0);break;
		}

		glBegin(GL_POLYGON);
		vtmp*=-1;

		glNormal3dv(vtmp.GetArray());

		for ( j = 0; j < FLOATER_RES; j++) {

            tmp=p[ci[i]] + (tgt1*cos_table[j] + tgt2*sin_table[j])*turk_radius[ci[i]]*.3;
			glVertex3dv(tmp.GetArray());
		}
		glEnd();
	}
   
}

void CParticleSamplingSys::DisplayNI(int index)
{
	CVector3d tgt1,tgt2,vtmp,tmp;


	glEnable(GL_DEPTH_TEST);

	int ncount=0;
	int i,j,k,nbor_id;
	for (i=0;i<size;i++)
	{
		if(pt[ci[i]]!=PT_NORMAL)
			continue;
		if(ncount==index)
			break;
		ncount++;
	}

	vector<int> neighbours;
	subd->FindNeighborhood(ci[i],&neighbours);
	DetermineClosestNeighborsFromNeighbourhood(ci[i],neighbours);

	vtmp = n[ci[i]];
	tgt1 = Ortho3(vtmp);
	tgt1.Normalize();
	tgt2 = vtmp.Cross(tgt1);
	tgt2.Normalize();

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glColor3f(1.0,0.0,0.0);

	glBegin(GL_POLYGON);

	glNormal3dv(vtmp.GetArray());

	for ( j = 0; j < FLOATER_RES; j++) {

		tmp=p[ci[i]] + (tgt1*cos_table[j] + tgt2*sin_table[j])*turk_radius[ci[i]]*.3;
		glVertex3dv(tmp.GetArray());
	}

	glEnd();

	glColor3f(1.0,1.0,0.0);

	for ( k = 0; k < closest_cache_size[ci[i]]; k++) {

		nbor_id = closest_cache[ci[i]][k];
		vtmp = n[nbor_id];
		tgt1 = Ortho3(vtmp);
		tgt1.Normalize();
		tgt2 = vtmp.Cross(tgt1);
		tgt2.Normalize();
		glBegin(GL_POLYGON);
		for ( j = 0; j < FLOATER_RES; j++) {

			tmp=p[nbor_id] + (tgt1*cos_table[j] + tgt2*sin_table[j])*turk_radius[nbor_id]*.3;
			glVertex3dv(tmp.GetArray());
		}
		glEnd();

	}


}

int CParticleSamplingSys::GetParticleNum(void)
{
	return size;
}

void CParticleSamplingSys::GetParticleByIndex(int index,CVector3d& coord, int& PntT)
{
	coord=p[ci[index]];
	PntT=pt[ci[index]];
}

void CParticleSamplingSys::GetParticleByIndex(int index, CVector3d& coord, CVector3d& normal, float& rad, int& PntT)
{
	coord=p[ci[index]];
	normal=n[ci[index]];
	size=turk_radius[ci[index]];
	PntT=pt[ci[index]];
}

void CParticleSamplingSys::InsertParticle(CVector3d& coord, CVector3d& normal, float rad, int PntT)
{
	if (size >= MAX_PARTICLES-1) {
		printf("Internal error: no room to insert particle. Please try again later\n");
		return;
	}

	if (particles_poll_size == 0)   // No indices left to be reused
	{
		ci[size] = size;
	}
	else 
	{

		ci[size] = particles_poll[--particles_poll_size];
	}
	pt[ci[size]]=PntT;

	p[ci[size]] = coord;
	//	if(IsNearBoundary(v))
	//		p[ci[size]].nb=true;

	// Mark entry id as a well and alive particle

	zombie_flag[ci[size]] = NOT_SET_FLAG;

	// Insert in spatial subdivion

	subd->Insert(ci[size],&(p[ci[size]]));

	turk_radius[ci[size]] = rad;

	//// Find neighbors
	vector<int> neighbours;
	subd->FindNeighborhood(ci[size],&neighbours);
	DetermineClosestNeighborsFromNeighbourhood(ci[size],neighbours);
	// Compute normal

	n[ci[size]]=normal;



	size++;

}
