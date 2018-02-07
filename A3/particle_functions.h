//Function descriptions

#include <stdio.h>
#include <stdlib.h>

struct particle {		
	double xPos;
	double yPos;
	double mass;
	double xVel;
	double yVel;
	double bright;
};
typedef struct particle particle_t;

double get_pos_1D(particle_t * target, int indexTarget, particle_t * __restrict others, char coord, double delta_t, int N);
double get_vel_1D(double force, double mass, double currVel, double delta_t);
double get_part_dist_1D(double target1DPos, double other1DPos, double absDist);
double get_abs_dist(double targetXPos, double targetYPos, double otherXPos, double otherYPos);
double get_force_1D(particle_t * target, int indexTarget, particle_t * __restrict others, char coord, int N); 
