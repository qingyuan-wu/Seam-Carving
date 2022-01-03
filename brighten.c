#include "c_img.h"
#include <stdio.h>
#include <math.h>

int brighten(float scale, char *imageFile){
    //this function brightens an image by a factor of scale
    struct rgb_img *im;

    read_in_img(&im, imageFile); // convert to .bin
    for(int i = 0; i < (im)->height; i++){
        for(int j = 0; j < (im)->width; j++){
            int red = (int) (scale * get_pixel(im, i, j, 0));
            int green = (int) (scale * get_pixel(im, i, j, 1));
            int blue = (int) (scale * get_pixel(im, i, j, 2));

            // make sure brightness doesn't exceed 255
            if ((int)(red) > 255) {red = 255;}
            if ((int)(green) > 255) {green = 255;}
            if ((int)(blue) > 255) {blue = 255;}

            set_pixel(im, i, j, (int)red, (int)green, (int)blue);            
        }
    }
    write_img(im, "scale1.bin"); // save as .bin
    destroy_image(im);
    return 0;
}

int main() {
    char file[200] = "Meric.bin";
    float scale = 1.2;
    brighten(scale, file);
    printf("Done.\n");

    return 0;
}