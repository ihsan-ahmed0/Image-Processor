#ifndef CROP
#define CROP

#include <stdlib.h>
#include "ppm_io.h"

int crop(Image* img_in, Image* img_out, char toprow[], char top_col[], char bot_row[], char bot_col[]);

#endif