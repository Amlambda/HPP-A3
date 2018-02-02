//Function descriptions

#include <stdio.h>
#include <stdlib.h>

struct particle {
  float mass;	
  float xPos;
  float yPos;
  float xVel;
  float yVel;
};
typedef struct particle particle_t;

float get_pos_1D(particle_t * target, particle_t * other, char coord);
float get_vel_1D(float force, float mass, float currVel);
float get_part_dist_1D(float target1DPos, float other1DPos, float absDist);
float get_abs_dist(float targetXPos, float targetYPos, float otherXPos, float otherYPos);
float get_force_1D(particle_t * target, particle_t * other, char coord);
