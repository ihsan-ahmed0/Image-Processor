#ifndef GRAD_SEAM
#define GRAD_SEAM

#include "ppm_io.h"

// function that will find the gradient of an image
Image * gradient(Image *img_in);

// rescales an image by removing a number of seams
Image * seamCarve(Image *img_in, float col_scale_factor, float row_scale_factor);

// creates seams and records them in a 2D integer array
int ** seamCreator(Image *img_in);

// takes in a 2D array of seams and determines which seam has the least magnitude
int * lowestSeamFinder(Image *img_in, int **seams);

// removes one seam (the inputted minimum_seam) from img_in and writes the result to img_out
void seamRemover(Image *img_in, Image *img_out, int *minimum_seam);

#endif
