#include "graphics.h"
#include "particle_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const float particleRadius = 0.025, particleColor = 0;
const int windowWidth = 800;

void keep_within_box(float* xA, float* yA) {
  if(*xA > 1)
    *xA = 0;
  if(*yA > 1)
    *yA = 0;
}

// void push_new_particle( float xPos, float yPos, float xVel, float yVel){

// }


int main (int argc, char *argv[]) {
	float L=1, W=1;    // Dimensions of domain in which particles move

  // Check command line arguments
  if(argc != 6) {   // If not 5 input arguments (argv[0] is the program name)
        printf("Error: Expected number of input arguments is 5\n");
        exit(1);
  }
  
  // Read input arguments
  int N = atoi(argv[1]);                // Number of particles to simulate (atoi = ascii to int)
  char* input_file_name = argv[2];      // Filename of file to read the initial configuration from
  int nsteps = atoi(argv[3]);           // Number of time steps
  float delta_t = atof(argv[4]);        // Timestep
  int graphics = atoi(argv[5]);         // 1 or 0 meaning graphics on/off

	//COPIED CODE TO READ FILE
  //const char* input_file_name = "circles_N_2.gal";

  printf("input_file_name = %s\n", input_file_name);

  /* Open input file and determine its size. */
  FILE* input_file = fopen(input_file_name, "rb");
  if(!input_file) {
    printf("Error: failed to open input file '%s'.\n", input_file_name);
    return -1;
  }
  /* Get filesize using fseek() and ftell(). */
  fseek(input_file, 0L, SEEK_END);
  size_t fileSize = ftell(input_file);
  /* Now use fseek() again to set file position back to beginning of the file. */
  fseek(input_file, 0L, SEEK_SET);
  printf("fileSize = %lu bytes.\n", fileSize);
  /* Allocate buffer into which we can then read the data from input_file. */
  char* buffer = (char*)malloc(fileSize*sizeof(char));
  /* Read contents of input_file into buffer. */
  size_t noOfItemsRead = fread(buffer, sizeof(char), fileSize, input_file);
  if(noOfItemsRead != fileSize) {
    printf("Error: failed to read file contents into buffer.\n");
    return -1;
  }
  /* OK, now we have the file contents in the buffer.
     Since we are finished with the input file, we can close it now. */
  if(fclose(input_file) != 0) {
    printf("Error closing input file.\n");
    return -1;
  }
  // size_t i;
  // for(i = 0; i < fileSize; i++) {
  //   printf("%c ",buffer[i]);
  // }



  //Manuellt skapade testpartiklar
	float xA, yA ,xB ,yB;

	//Läs in alla partiklar från fil som struct particles..  
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


	InitializeGraphics(argv[0],windowWidth,windowWidth);
  SetCAxes(0,1);
	//Hålla koll på partiklarna genom att ha deras pointers i en lista?


	// Att göra för varje partikel varje tidssteg
	// 	Var är den? x, y


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