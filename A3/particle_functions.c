#include "particle_functions.h"
#include <math.h>

const float gravConst = 100;
const float eps0 = 0.001;
const float deltaT = 0.00001;



//MASTER BOSS CALCULATE NEW POSITION FUNCTION 
float get_pos_1D(particle_t * target, particle_t * other, char coord){
	float force = get_force_1D(target, other, coord);
	float velocity, position;

	if(coord == 'x'){
		velocity = get_vel_1D(force, target->mass, target->xVel);
		position = (target->xPos) + deltaT*velocity;
		//Update!!
		target->xPos = position;
		target->xVel = velocity;

	}else if(coord == 'y'){
		velocity = get_vel_1D(force, target->mass, target->yVel);
		position = (target->yPos) + deltaT*velocity;
		//Update!!
		target->yPos = position;
		target->yVel = velocity;
	}
	printf("%c velocity is %lg\n", coord, velocity);
	printf("%c position is %lg\n", coord, position);

	return position;
}

//Calculates velocity in 1D
float get_vel_1D(float force, float mass, float currVel){
	float velocity = currVel + (deltaT*force)/mass;
	return velocity;
}

//Calculates the x or y distance between two particles
float get_part_dist_1D(float target1DPos, float other1DPos, float absDist){
	float dist;
	dist = (target1DPos-other1DPos)/absDist;
	return dist;
}

//Calculates the absolute distance between two particles in 2D
float get_abs_dist(float targetXPos, float targetYPos, float otherXPos, float otherYPos){
	float absDist;
	absDist = sqrt((targetXPos-otherXPos)*(targetXPos-otherXPos) + (targetYPos-otherYPos)*(targetYPos-otherYPos));
	return absDist;
}

/*Calculates force of other particles (for now only one.. we have to send in a pointer 
list of all the others or something) on target particle in x OR y */
float get_force_1D(particle_t * target, particle_t * other, char coord){
	float forceSum = 0;
	float partDist, absDist, m2;
	int numPart = 2;
	int i;
	particle_t * currOther;
	float m1 = target->mass;

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

		forceSum += (m2*partDist)/(absDist*absDist);
		
	}

	forceSum = -gravConst*m1*forceSum;
	return forceSum;
}

