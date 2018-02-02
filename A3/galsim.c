#include "graphics.h"
#include "particle_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const float particleRadius = 0.025, particleColor = 0;
const int windowWidth = 800;

// struct particle {
//   float mass;	
//   float xPos;
//   float yPos;
//   float xVel;
//   float yVel;
// };
// typedef struct particle particle_t;

void keep_within_box(float* xA, float* yA) {
  if(*xA > 1)
    *xA = 0;
  if(*yA > 1)
    *yA = 0;
}

void push_new_particle( float xPos, float yPos, float xVel, float yVel){

}


int main (int argc, char *argv[]) {
	float L=1, W=1;
	float xA, yA ,xB ,yB;

	//Läs in alla partiklar från fil som struct particle, 
	particle_t * p1 = malloc(sizeof(particle_t)); 
	particle_t * p2 = malloc(sizeof(particle_t));
	p1->mass = 300;
	p1->xPos = 0.5;
	p1->yPos = 0.5;
	p1->xVel = 0.5;
	p1->yVel = -0.4;
	p2->mass = 300;
	p2->xPos = 0.2;
	p2->yPos = 0.2;
	p2->xVel = -0.2;
	p2->yVel = 0.5;



	// float absDist = get_abs_dist(p1->xPos, p1->yPos, p2->xPos, p2->yPos);
	// printf("X distance: %lg\n", get_part_dist_1D(p1->xPos,p2->xPos,absDist));
	// printf("Y distance: %lg\n", get_part_dist_1D(p1->yPos,p2->yPos,absDist));
	// printf("X force: %lg\n", get_force_1D(p1,p2,'x'));
	// printf("Y force: %lg\n", get_force_1D(p1,p2,'y'));
	// get_pos_1D(p1,p2,'x');
	// get_pos_1D(p1,p2,'y');

	InitializeGraphics(argv[0],windowWidth,windowWidth);
  	SetCAxes(0,1);
	//Hålla koll på partiklarna genom att ha deras pointers i en lista?


	// Att göra för varje partikel varje tidssteg
	// 	Var är den? x, y
	// 	Vilka krafter verkar på den?


  printf("Hit q to quit.\n");
  while(!CheckForQuit()) {
    /* Move A. */
    xA = get_pos_1D(p1,p2,'x');
    yA = get_pos_1D(p1,p2,'y');
    keep_within_box(&xA, &yA);

    xB = get_pos_1D(p2,p1,'x');
    yB = get_pos_1D(p2,p1,'y');
    keep_within_box(&xB, &yB);

    /* Call graphics routines. */
    ClearScreen();
    DrawCircle(xA, yA, L, W, particleRadius, particleColor);
    DrawCircle(xB, yB, L, W, particleRadius, particleColor);
    Refresh();
    /* Sleep a short while to avoid screen flickering. */
    usleep(3000);
  }  	
	FlushDisplay();
  	CloseDisplay();
	free(p1);
	free(p2);
	return 0;
}