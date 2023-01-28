CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

project: project.o grayscale.o gradient_seam.o binarize.o transpose.o crop.o
	$(CC) -o project project.o grayscale.o gradient_seam.o ppm_io.o binarize.o transpose.o crop.o

project.o: project.c grayscale.h ppm_io.h gradient_seam.h binarize.h transpose.h
	$(CC) $(CFLAGS) -c project.c grayscale.c gradient_seam.c ppm_io.c binarize.c transpose.c

grayscale.o: grayscale.c grayscale.h ppm_io.h
	$(CC) $(CFLAGS) -c grayscale.c

binarize.o: binarize.c binarize.h grayscale.h ppm_io.h
	$(CC) $(CFLAGS) -c binarize.c grayscale.c ppm_io.c

gradient_seam.o: gradient_seam.c gradient_seam.h transpose.h
	$(CC) $(CFLAGS) -c gradient_seam.c transpose.c

crop.o: crop.c crop.h ppm_io.h
	$(CC) $(CFLAGS) -c crop.c

transpose.o: transpose.c transpose.h
	$(CC) $(CFLAGS) -c transpose.c

clean:
	rm -f *.o project

## Below are commands to link and compile the checkerboard program
# Links together files needed to create the checkerboard executable
checkerboard: checkerboard.o ppm_io.o
	$(CC) -o $@ checkerboard.o ppm_io.o

# Compile the ppm i/o source code
ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

# Removes all object files and the executable named project, so we can start fresh
clean_checkerboard:
	rm -f *.o checkerboard
