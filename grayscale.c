#include "grayscale.h"
#include "ppm_io.h"

Image* grayscale(Image* img_in)
{
    //I don't particularly care about the 2D nature of the image in this case
    for (int i = 0; i < img_in->rows * img_in->cols; i++)
    {
        //Compute the grayscale value using the following formula
        int grayscale_intensity = (0.3 * (int) (img_in->data[i].r)) + (0.59 * (int) (img_in->data[i].g)) + (0.11 * (int) (img_in->data[i].b));
        //Then assign that grayscale value to each color for each pixel
        img_in->data[i].r = (unsigned char) (grayscale_intensity);
        img_in->data[i].g = (unsigned char) (grayscale_intensity);
        img_in->data[i].b = (unsigned char) (grayscale_intensity);
    }
    return img_in;
}