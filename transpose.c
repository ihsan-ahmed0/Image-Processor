#include "transpose.h"
#include <stdlib.h>


Image * transpose(Image *img_in)
{
  Image *transpose = malloc(sizeof(Image));
  
  transpose->rows = img_in->cols;
  transpose->cols = img_in->rows;
  transpose->data = malloc(sizeof(Pixel) * (transpose->rows) * (transpose->cols));
  
  
  for(int i = 0; i < transpose->rows; i++) {
    for(int j = 0; j < transpose->cols; j++) {

      transpose->data[(i * transpose->cols) + j].r = img_in->data[(j * img_in->cols) + i].r;
      transpose->data[(i * transpose->cols) + j].g = img_in->data[(j * img_in->cols) + i].g;
      transpose->data[(i * transpose->cols) + j].b = img_in->data[(j * img_in->cols) + i].b;
      
    }
  }

  free(img_in->data);
  free(img_in);
  return transpose;
}
