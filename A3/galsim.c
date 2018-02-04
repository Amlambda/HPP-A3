#include "graphics.h"
#include "particle_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

const float particleRadius = 0.02, particleColor = 0;
const int windowWidth = 800;

void keep_within_box(double* x, double* y) {
  if(*x > 1)
    *x = 0;
  if(*y > 1)
    *y = 0; 
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
  printf("-------- Input Arguments -----------\n");
  const int N = atoi(argv[1]);                // Number of particles to simulate (atoi = ascii to int)
  printf("N: \t\t\t%d\n", N);
  const char* input_file_name = argv[2];      // Filename of file to read the initial configuration from
  printf("input_file_name: \t%s\n", input_file_name);
  const int nsteps = atoi(argv[3]);           // Number of time steps
  printf("nsteps: \t\t%d\n", nsteps);
  const float delta_t = atof(argv[4]);        // Timestep
  printf("delta_t: \t\t%.3f\n", delta_t);
  const int graphics = atoi(argv[5]);         // 1 or 0 meaning graphics on/off
  printf("graphics: \t\t%d\n", graphics);
    printf("------------------------------------\n\n");

	//COPIED CODE TO READ FILE
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
  // END OF COPIED CODE

  /* Read initial configuration from buffer */
  struct particle particles[N];                 // Array of particle structs staicly allocated on stack
  char* ptr = &buffer[0];                       // Pointer to use when extracting doubles from buffer
  int offset = sizeof(double);
  int index = 0;
  for (int i = 0; i < fileSize; i+=6*offset) {      // Increase by six*sizeof(double) (six attributes per particle)
    ptr = &buffer[i];
    memcpy(&particles[index].xPos, ptr, sizeof(double));

    ptr = &buffer[i+offset];
    memcpy(&particles[index].yPos, ptr, sizeof(double));

    ptr = &buffer[i+2*offset];
    memcpy(&particles[index].mass, ptr, sizeof(double));

    ptr = &buffer[i+3*offset];
    memcpy(&particles[index].xVel, ptr, sizeof(double));

    ptr = &buffer[i+4*offset];
    memcpy(&particles[index].yVel, ptr, sizeof(double));

    ptr = &buffer[i+5*offset];
    memcpy(&particles[index].bright, ptr, sizeof(double));

    index++;
   }

  /* If graphics are to be used, prepare graphics window */
  if (graphics == 1) {
    InitializeGraphics(argv[0],windowWidth,windowWidth);
    SetCAxes(0,1);
  }

  /* Initialize variables */
  particle_t * p1, * p2;
  int i, j; 
  double x, y;

  /* Start simulation */

  for (double time_step = 0; time_step < nsteps; time_step++) {   // Loop over all timesteps
    
    /* Update position of particle i with respect to all other particles */
    for (i = 0; i < N; i++) {
      p1 = &particles[i];
      for (j = 0; j < N; j++) {
        if( i != j ) {
          p2 = &particles[j];
          x = get_pos_1D(p1,p2,'x', delta_t, N); 
          y = get_pos_1D(p1,p2,'y', delta_t, N);
          keep_within_box(&x, &y);
        }
      }
    }

    if (graphics == 1) {
      /* Call graphics routines. */
      ClearScreen();
      for (i = 0; i < N; i++) {
        DrawCircle(particles[i].xPos, particles[i].yPos, L, W, particleRadius, particleColor);
      }
      Refresh();
      /* Sleep a short while to avoid screen flickering. */ 
      usleep(50000);
    }
  }  	

  if (graphics == 1) {
  	FlushDisplay();
    CloseDisplay();
  }

	return 0;
}