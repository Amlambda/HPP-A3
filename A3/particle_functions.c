#include "particle_functions.h"
#include <math.h>

const float gravConst = 100;
const float eps0 = 0.001;
//const float deltaT = 0.00001;
const float distMinimum = 0.5;



//MASTER BOSS CALCULATE NEW POSITION FUNCTION 
double get_pos_1D(particle_t * target, particle_t * other, char coord, double delta_t, int N){
	double force = get_force_1D(target, other, coord, N);
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
	// printf("%c velocity is %lg\n", coord, velocity);
	// printf("%c position is %lg\n", coord, position);

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

/*Calculates force of other particles (for now only one.. we have to send in a pointer 
list of all the others or something) on target particle in x OR y */
double get_force_1D(particle_t * target, particle_t * other, char coord, int N){
	double forceSum = 0;
	double partDist, absDist, m2;
	int numPart = 2; //Replace with the number of particles
	int i;
	particle_t * currOther;
	double m1 = target->mass;

	for(i=1; i<=(numPart-1); i++){
		currOther = other;
		m2 = currOther->mass;
		absDist = get_abs_dist(target->xPos, target->yPos, currOther->xPos, currOther->yPos);

		if(coord == 'x'){
			partDist = get_part_dist_1D(target->xPos, currOther->xPos, absDist);
		}else if(coord == 'y'){
			partDist = get_part_dist_1D(target->yPos, currOther->yPos, absDist);
		}else{
			printf("Please enter x or y as coordinate\n");
		}

		//Plummer sphere modification, r<<1
		if((partDist*absDist) < distMinimum){
			forceSum += (m2*partDist*absDist)/(pow((absDist + eps0),3));
		}else{
			forceSum += (m2*partDist)/(pow(absDist,2));
		}
		
		
	}

	forceSum = -gravConst/N*m1*forceSum;
	return forceSum;
}

