#include "graphics.h"
#include "particle_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

const double particleRadius = 0.005, particleColor = 0;
const int windowWidth = 800;


int main (int argc, char *argv[]) {
	double L=1, W=1;    // Dimensions of domain in which particles move

  // Check command line arguments
  if(argc != 6) {   // End program if not 5 input arguments (argv[0] is the program name)
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
  const double delta_t = atof(argv[4]);        // Timestep
  printf("delta_t: \t\t%.5f\n", delta_t);
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
  printf("fileSize = %lu bytes.\n\n", fileSize);
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
  struct particle particles[N];                 // Array of particle structs staticly allocated on stack
  char* ptr = &buffer[0];                       // Pointer to use when extracting doubles from buffer
  const int offset = sizeof(double);            // Constant used as offset into buffer when reading and writing doubles
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
  particle_t * target, * other; 

  /* Start simulation */
  for (int time_step = 0; time_step < nsteps; time_step++) {   // Loop over all timesteps

    /* Update position of particle i with respect to all other particles */
    for (int i = 0; i < N; i++) {
      target = &particles[i];
      // This get method could be modified to a set since values are not used
      get_pos_1D(target,i,particles,'x', delta_t, N);     
      get_pos_1D(target,i,particles,'y', delta_t, N);
    }

    if (graphics == 1) {
      /* Call graphics routines. */
      ClearScreen();
      for (int i = 0; i < N; i++) {
        DrawCircle(particles[i].xPos, particles[i].yPos, L, W, particleRadius, particleColor);
      }
      Refresh();
      /* Sleep a short while to avoid screen flickering. (SHOULD ONLY BE USED FOR SMALL N)*/ 
      if(N<500){
        usleep(5000);
      }
    }
  }  	

  if (graphics == 1) {
  	FlushDisplay();
    CloseDisplay();
  }

  /* Overwrite buffer with simulation results */
  index = 0;
  for (int i = 0; i < fileSize; i+=6*offset) {      // Increase by six*sizeof(double) (six attributes per particle)
    ptr = &buffer[i];
    memcpy(ptr, &particles[index].xPos, sizeof(double));

    ptr = &buffer[i+offset];
    memcpy(ptr, &particles[index].yPos, sizeof(double));
    
    ptr = &buffer[i+2*offset];
    memcpy(ptr, &particles[index].mass, sizeof(double));

    ptr = &buffer[i+3*offset];
    memcpy(ptr, &particles[index].xVel, sizeof(double));

    ptr = &buffer[i+4*offset];
    memcpy(ptr, &particles[index].yVel, sizeof(double));
    
    ptr = &buffer[i+5*offset];
    memcpy(ptr, &particles[index].bright, sizeof(double)); 

    index++;
  }

  /* Create output file write results to it */
  int no_of_chars_to_copy = strlen(input_file_name) - 4;   // Copy input file name except for ".gal"
  char* copy_file_name = (char *)malloc(sizeof(char)*no_of_chars_to_copy);      
  strncpy(copy_file_name, input_file_name, no_of_chars_to_copy);

  char* output_file_name;  // Build output file name based on input file name and number of steps 
  //asprintf(&output_file_name, "%s_after%dsteps_result.gal", copy_file_name, nsteps);
  output_file_name = "result.gal";

  // Open output file for writing
  FILE* output_file = fopen(output_file_name, "wb");
  if(!output_file) {
    printf("Error: failed to open output file '%s' for writing.\n", output_file_name);
    return -1;
  }
  
  /* Write contents of buffer into output_file. */
  noOfItemsRead = fwrite(buffer, sizeof(char), fileSize, output_file);
  if(noOfItemsRead != fileSize) {
    printf("Error: failed to write buffer contents into result file.\n");
    return -1;
  }

  if(fclose(output_file) != 0) {
    printf("Error closing output file.\n");
    return -1;
  }

  printf("Result saved in file '%s'\n", output_file_name);

  free(buffer);
  free(copy_file_name);
	return 0;
}
