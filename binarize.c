#include "binarize.h"
#include "grayscale.h"
#include "ppm_io.h"

Image* binarize(Image* img_in, int threshold)
{
    //convert image to grayscale
    img_in = grayscale(img_in);
    //iterate through each pixel
    for (int i = 0; i < img_in->rows * img_in->cols; i++)
    {
        //ternary operator used to determine if a pixel's intensity is within the specified threshold
        int new_value = ((int) (img_in->data[i].r) >= threshold) ? 255 : 0;
        img_in->data[i].r = (unsigned char) new_value;
        img_in->data[i].g = (unsigned char) new_value;
        img_in->data[i].b = (unsigned char) new_value; 
    }
    return img_in;
}