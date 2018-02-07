CFLAGS= -g -Ofast 
INCLUDES=-I/opt/X11/include
LDFLAGS=-L/opt/X11/lib -lX11 -lm

galsim: galsim.o particle_functions.o graphics.o
	gcc -o galsim galsim.o particle_functions.o graphics.o $(LDFLAGS)

galsim.o: galsim.c particle_functions.h graphics.h
	gcc $(CFLAGS) $(INCLUDES) -c galsim.c

particle_functions.o: particle_functions.c particle_functions.h
	gcc $(CFLAGS) $(INCLUDES) -c particle_functions.c

graphics.o: graphics.c graphics.h
	gcc $(CFLAGS) $(INCLUDES) -c graphics.c

clean:
	rm -f ./galsim *.o