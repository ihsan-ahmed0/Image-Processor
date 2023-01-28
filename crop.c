#include <stdlib.h>
#include "ppm_io.h"

int crop(Image* img_in, Image* img_out, char topcol[], char toprow[], char botcol[], char botrow[])
{
    int top_col = atoi(topcol);
    int top_row = atoi(toprow);
    int bot_col = atoi(botcol);
    int bot_row = atoi(botrow);
    //check if the corners are on the wrong side of one another, or if either one is outside of the image to be cropped
    if (bot_row < top_row || bot_col < top_col || top_row < 0 || top_col < 0 || bot_row > img_in->rows || bot_col > img_in->cols)
    {
        return 7;
    }
    else
    {
        //set up the output image to be the right size
        img_out->rows = bot_row - top_row;
        img_out->cols = bot_col - top_col;
        img_out->data = malloc(img_out->rows * img_out->cols * sizeof(Pixel));
        //keep track of out output index outside of the loop, because it is not nice to derive from i and j
        int out_index = 0;
        //start i at the first row we want to copy by initializing it to original image width * number of rows to skip
        //stop looping when we get to the bottom row
        //increment i by one row's worth of pixels
        for (int i = (img_in->cols * top_row); i < (img_in->cols * bot_row); i += img_in->cols)
        {
            //start j at the first column we want to copy
            //read until we reach the last column we want to copy
            //increment normally
            for (int j = top_col; j < bot_col; j++)
            {
                //add i and j to get the correct index of the "2D" array
                //copy over the pixel to the output image
                img_out->data[out_index++] = img_in->data[i+j];
            }
        }
        return 0;
    }
}

