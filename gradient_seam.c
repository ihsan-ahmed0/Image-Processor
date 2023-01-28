#include "gradient_seam.h"
#include "transpose.h"
#include <stdlib.h>

Image * gradient(Image *img_in) {

  // create an image where the grayscale of the inputed image will be recorded
  Image *grayscale = malloc(sizeof(Image));
  grayscale->cols = img_in->cols;
  grayscale->rows = img_in->rows;
  grayscale->data = malloc(sizeof(Pixel) * (grayscale->rows) * (grayscale->cols));

  // iterate through each pixel of img_in and record its grayscale values to the grayscale image
  for(int i = 0; i < img_in->rows; i++) {
    for(int j = 0; j < img_in->cols; j++) {
      int gray_intensity = (0.3 * (int) img_in->data[(img_in->cols * i) + j].r)
	+ (0.59 * (int) img_in->data[(img_in->cols * i) + j].g)
	+ (0.11 * (int) img_in->data[(img_in->cols * i) + j].b);
      
      grayscale->data[(img_in->cols * i) + j].r = (unsigned char) gray_intensity;
      grayscale->data[(img_in->cols * i) + j].g = (unsigned char) gray_intensity;
      grayscale->data[(img_in->cols * i) + j].b = (unsigned char) gray_intensity;
    }
  }

  // create the gradient Image where the gradient values of img_in will be stored
  Image *gradient = malloc(sizeof(Image));
  gradient->cols = img_in->cols;
  gradient->rows = img_in->rows;
  gradient->data = malloc(sizeof(Pixel) * (gradient->rows) * (gradient->cols));

  // iterate through each pixel pixel of the grayscale image and calculate the gradient values
  for(int i = 0; i < img_in->rows; i++) {
    for(int j = 0; j < img_in->cols; j++) {
      // If-statement checks if the current pixel is an interior or border pixel.
      if(i > 0 && j > 0 && i < img_in->rows - 1 && j < img_in->cols - 1) {
	// calculates gradient magnitude using horizontal and vertical gradients if the current pixel is not on the border
	int horizontal_gradient = ((int) grayscale->data[(grayscale->cols * i) + j + 1].r
				   - (int) grayscale->data[(grayscale->cols * i) + j - 1].r) / 2;
	int vertical_gradient = ((int) grayscale->data[(grayscale->cols * (i + 1)) + j].r
				 - (int) grayscale->data[(grayscale->cols * (i - 1)) + j].r) / 2;
	
	int gradient_magnitude = abs(horizontal_gradient) + abs(vertical_gradient);
	gradient->data[(gradient->cols * i) + j].r = (unsigned char) gradient_magnitude;
	gradient->data[(gradient->cols * i) + j].g = (unsigned char) gradient_magnitude;
	gradient->data[(gradient->cols * i) + j].b = (unsigned char) gradient_magnitude;
      } else {
	// for border pixels, the gradient magnitude is set to zero
	int gradient_magnitude = 0;
	gradient->data[(gradient->cols * i) + j].r = (unsigned char) gradient_magnitude;
	gradient->data[(gradient->cols * i) + j].g = (unsigned char) gradient_magnitude;
	gradient->data[(gradient->cols * i) + j].b = (unsigned char) gradient_magnitude;
      }
    }
  }

  // free memory from the grayscale image object
  free(grayscale->data);
  free(grayscale);
  return gradient;
}

Image * seamCarve(Image *img_in, float col_scale_factor, float row_scale_factor) {

  // write the original number of rows and columns to variables
  int Rows = img_in->rows;
  int Cols = img_in->cols;
  // record the number of rows and columns we want (to determine number of seams to remove)
  int targetRows = row_scale_factor * img_in->rows;
  int targetCols = col_scale_factor * img_in->cols;

  // set minimum horizontal and vertical dimension to be 2
  if(targetRows < 2) {
    targetRows = 2;
  }
  if (targetCols < 2) {
    targetCols = 2;
  }

  // iterate and remove vertical seams as needed
  for(int i = 0; i < (Cols - targetCols); i++) {
    // find seams
    int **seams;
    seams = seamCreator(img_in);

    // determine seam with the least energy
    int *minimum_seam;
    minimum_seam = lowestSeamFinder(img_in, seams);

    // create an Image object to write the new image to after seam has been removed
    Image *img_carved = malloc(sizeof(Image));

    // remove seam
    seamRemover(img_in, img_carved, minimum_seam);

    // free memory that is no longer needed
    for(int j = 0; j < img_in->cols; j++) {
      free(seams[j]);
    }
    free(seams);
    free(img_in->data);
    free(img_in);
    img_in = img_carved;
  }

  // transpose image
  Image *img_transposed;
  img_transposed = transpose(img_in);

  // repeat steps above, this time to remove horizontal seams
  // image was transposed so that we can run the same operations as before
  for(int i = 0; i < (Rows - targetRows); i++) {
    int **seams;
    seams = seamCreator(img_transposed);

    int *minimum_seam;
    minimum_seam = lowestSeamFinder(img_transposed, seams);

    Image *img_carved = malloc(sizeof(Image));

    seamRemover(img_transposed, img_carved, minimum_seam);

    for(int j = 0; j < img_transposed->cols; j++) {
      free(seams[j]);
    }
    free(seams);
    free(img_transposed->data);
    free(img_transposed);
    img_transposed = img_carved;
  }

  // transpose image again to return to normal and return it
  return transpose(img_transposed);
  
}

int ** seamCreator(Image *img_in) {
  // find the gradient of the input
  Image *gradient_vals;
  gradient_vals = gradient(img_in);

  // create the 2D array where seams will be recorded
  int **seams = malloc(sizeof(int*) * img_in->cols);
  for(int i = 0; i < img_in->cols; i++) {
    seams[i] = malloc(sizeof(int) * (img_in->rows + 1));
  }

  // iterate though columns and find the seam starting from each column
  for(int i = 0; i < img_in->cols; i++) {
    // start each seam at the first row and current column
    seams[i][0] = i;
    int total_gradience = 0;
    
    for(int j = 1; j < img_in->rows - 1; j++) {
      // find index of the pixel that is directly below
      int current_index = seams[i][j - 1] + img_in->cols;
      int next_index = current_index;

      // checks if pixel to the left has less gradience
      if((current_index - 1 > j * gradient_vals->cols)
	 && ((int) gradient_vals->data[current_index - 1].r < (int) gradient_vals->data[next_index].r)) {
	next_index = current_index - 1;
      }

      // checks if current index is on the left border
      if(current_index % gradient_vals->cols == 0) {
	next_index = current_index + 1;
      }

      // checks if current index is on the right border
      if(current_index % gradient_vals->cols == gradient_vals->cols - 1) {
	next_index = current_index - 1;
      }

      //checks if pixel to the right has less gradience
      if((current_index + 1 < (j + 1) * gradient_vals->cols - 1)
	 && ((int) gradient_vals->data[current_index + 1].r < (int) gradient_vals->data[next_index].r)) {
	next_index = current_index + 1;
      }

      // record the next pixel of the current seam
      seams[i][j] = next_index;
      // increments the total energy of the current seam by the gradience of the next pixel
      total_gradience += (int) gradient_vals->data[next_index].r;
    }

    // add last pixel of the seam (bottom border pixel)
    seams[i][img_in->rows - 1] = seams[i][img_in->rows - 2] + img_in->cols;
    // record total energy of the current seam in the last index of the array
    seams[i][img_in->rows] = total_gradience;
  }

  // free gradient image
  free(gradient_vals->data);
  free(gradient_vals);
  return seams;
}

int * lowestSeamFinder(Image *img_in, int **seams) {
  int lowest_seam = 0;

  // iterate through each seam, check the total energy of each, and find the one with the lowest energy
  for(int i = 0; i < img_in->cols; i++) {
    if(seams[i][img_in->rows] < seams[lowest_seam][img_in->rows]) {
      lowest_seam = i;
    }
  }

  // return lowest energy seam
  return seams[lowest_seam];
}

void seamRemover(Image *img_in, Image *img_out, int *minimum_seam) {
  // initialize the output image we will be writing to
  img_out->rows = img_in->rows;
  img_out->cols = img_in->cols - 1;
  img_out->data = malloc(sizeof(Pixel) * img_out->rows * img_out->cols);
  
  int img_out_index = 0;
  int seam_index = 0;

  // copy over all the pixels of the original image that are not part of the seam
  for(int i = 0; i < (img_in->rows * img_in->cols); i++) {
    if(i == minimum_seam[seam_index]) {
      seam_index++;
    } else {
      img_out->data[img_out_index].r = img_in->data[i].r;
      img_out->data[img_out_index].g = img_in->data[i].g;
      img_out->data[img_out_index].b = img_in->data[i].b;
      img_out_index++;
    }
  }
  
}
