// Ihsan Ahmed
/*****************************************************************************
 * A program to run the image processing operations
 *
 * Summary: This file implements a program for image processing operations.
 *          Different operations take different input arguments. In general,
 *            ./ImageProcessor <input> <output> <operation name> [operation params]
 *          The program will return 0 and write an output file if successful.
 *          Otherwise, the below error codes should be returned:
 *            1: Wrong usage (i.e. mandatory arguments are not provided)
 *            2: Input file I/O error
 *            3: Output file I/O error
 *            4: The Input file cannot be read as a PPM file
 *            5: Unsupported image processing operations
 *            6: Incorrect number of arguments for the specified operation
 *            7: Invalid arguments for the specified operation
 *            8: Other errors 
 *****************************************************************************/
#include "ppm_io.h" // PPM I/O header
#include "gradient_seam.h" // Header with gradient and seam functions.
#include "grayscale.h" //Header file for grayscale function
#include "binarize.h" //Header file for binarize function
#include "crop.h" //Header file for crop function
#include "transpose.h"// Header file for transpose function
#include <string.h> // String library to help with reading command line inputs
#include <stdlib.h> // Required for allocation and freeing of memory

int main(int argc, char **argv) {
  int return_code = 0;
  
  if(argc >= 4) {

    // Opening input file
    FILE *file_in = fopen(argv[1], "r");

    // Checking if successfully opened
    if(file_in == NULL) {
      fprintf(stdout,"Return Value: 2\nInput file I/O error\n");
      return 2;
    }

    // Reading the input file and writing the contentes to an Image object.
    Image *img_in;
    img_in = ReadPPM(file_in);

    // The input file has been successfully read.
    if (img_in == NULL || ferror(file_in)) {
      fprintf(stdout,"Return Value: 2\nInput file I/O error\n");
      return 2;
    }

    fclose(file_in);

    Image *img_out;
    
    // This if-statement will call the proper function depending on the operation.
    if(!strcmp(argv[3], "grayscale")) {
      img_out = grayscale(img_in);
    } else if(!strcmp(argv[3], "binarize")) {
        if(argc == 5)
        {
          int threshold = (int) (atof(argv[4]));
          if (threshold >= 0 && threshold <= 255)
          {
            img_out = binarize(img_in, threshold);
          }
          else
          {
            //The threshold specified was outside the acceptable range
	    free(img_in->data);
	    free(img_in);
            fprintf(stdout,"Return Value: 7\nInvalid arguments for binarize\n");
            return 7;
          }
        }
        else
        {
          //too many or too few arguments for binarize
	  free(img_in->data);
	  free(img_in);
          fprintf(stdout,"Return Value: 6\nIncorrect number of arguments for binarize\n");
          return 6;
        }
    } else if(!strcmp(argv[3], "crop")) {
      //check if we have the correct number of arguments
        if(argc == 8)
        {
	  img_out = malloc(sizeof(Image));
          return_code = crop(img_in, img_out, argv[4], argv[5], argv[6], argv[7]);
	  free(img_in->data);
	  free(img_in);
        }
        else
        {
	  //incorrect number of arguments for crop
	  free(img_in->data);
	  free(img_in);
	  fprintf(stdout,"Return Value: 6\nIncorrect number of arguments for crop\n");
	  return 6;
        }
    } else if(!strcmp(argv[3], "transpose")) {
      img_out = transpose(img_in);
    } else if(!strcmp(argv[3], "gradient")) {
      img_out = gradient(img_in);
      free(img_in->data);
      free(img_in);
    } else if(!strcmp(argv[3], "seam")) {
      //check if we have the correct number of arguments
      if(argc == 6)
      {
	float factor1 = atof(argv[4]);
	float factor2 = atof(argv[5]);
	
	img_out = seamCarve(img_in, factor1, factor2);
      }
      else {
	//incorrect number of arguments for seam
	free(img_in->data);
	free(img_in);
	fprintf(stdout,"Return Value: 6\nIncorrect number of arguments for seam\n");
	return 6;
      }
    } else {
      free(img_in->data);
      free(img_in);
      fprintf(stdout,"Return Value: 5\nUnsupported image processing operations\n");
      return 5;
    }

    // The following code will write the output image to a ppm file.
    FILE *file_out = fopen(argv[2], "w");

    if(file_out == NULL) {
      fprintf(stdout,"Return Value: 3\nOutput file I/O error\n");
      free(img_out->data);
      free(img_out);
      return 3;
    }

    int pixelsWritten = WritePPM(file_out, img_out);

    if(pixelsWritten < (img_out->rows * img_out->cols) || ferror(file_out)) {
      fprintf(stdout,"Return Value: 3\nOutput file I/O error\n");
      free(img_out->data);
      free(img_out);
      return 3;
    }

    fclose(file_out);

    // Freeing the output image object because it was dynamically written.
    free(img_out->data);
    free(img_out);
    
  } else {
    fprintf(stdout,"Return Value: 1\nMandatory arguments are not provided\n");
    return 1;
  }
  
  return return_code;
}
