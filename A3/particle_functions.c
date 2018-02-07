#include "particle_functions.h"
#include <math.h>

const double gravConst = 100;
const double eps0 = 0.001;		
const double distMinimum = 0.02;	// Minimum distance before Plummer sphere modification sets in
								// Is set to twice the diameter of a particle

//MASTER BOSS CALCULATE NEW POSITION FUNCTION 
double get_pos_1D(particle_t * target, int indexTarget, particle_t * __restrict others, char coord, double delta_t, int N){
	double force = get_force_1D(target, indexTarget, others, coord, N);
	double velocity, position;

	if(coord == 'x'){
		velocity = get_vel_1D(force, target->mass, target->xVel, delta_t);
		position = (target->xPos) + delta_t*velocity;
		//Update!!
		target->xPos = position;
		target->xVel = velocity;

	}else if(coord == 'y'){
		velocity = get_vel_1D(force, target->mass, target->yVel, delta_t);
		position = (target->yPos) + delta_t*velocity;
		//Update!!
		target->yPos = position;
		target->yVel = velocity;
	}

	return position;
}

//Calculates velocity in 1D
double get_vel_1D(double force, double mass, double currVel, double delta_t){
	double velocity = currVel + (delta_t*force)/mass;
	return velocity;
}

//Calculates the x or y distance between two particles
double get_part_dist_1D(double target1DPos, double other1DPos, double absDist){
	double dist;
	dist = (target1DPos-other1DPos)/absDist;
	return dist;
}

//Calculates the absolute distance between two particles in 2D
double get_abs_dist(double targetXPos, double targetYPos, double otherXPos, double otherYPos){
	double absDist;
	absDist = sqrt((targetXPos-otherXPos)*(targetXPos-otherXPos) + (targetYPos-otherYPos)*(targetYPos-otherYPos));
	return absDist;
}

// Calculates force on terget particle from other particles 
double get_force_1D(particle_t * target, int indexTarget, particle_t * __restrict others, char coord, int N){
	double forceSum = 0;
	double partDist, absDist, m2;
	int i;
	particle_t * currOther;
	double m1 = target->mass;

	for(i=0; i < N; i++){				// Loop over all particles
		if( i != indexTarget ) {
			currOther = others;			// Get the other particle
			m2 = currOther->mass;
			absDist = get_abs_dist(target->xPos, target->yPos, currOther->xPos, currOther->yPos);

			if(coord == 'x'){
				partDist = get_part_dist_1D(target->xPos, currOther->xPos, absDist);
			}else if(coord == 'y'){
				partDist = get_part_dist_1D(target->yPos, currOther->yPos, absDist);
			}else{
				printf("Please enter x or y as coordinate\n");
			}

			// Plummer sphere modification, r<<1 
			if(absDist < distMinimum){
				forceSum += (m2*partDist*absDist)/(pow((absDist + eps0),3)); 
			}else{
				forceSum += (m2*partDist)/(pow(absDist,2));		
			}
		}
		others++;		// Increase pointer to array of particles so that it points at next element
	}

	forceSum = -(gravConst/N)*m1*forceSum;
	return forceSum;
}

