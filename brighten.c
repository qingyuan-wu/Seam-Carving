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
    printf("hi?\n");
    write_img(im, "C:\\Users\\qingy\\OneDrive\\Documents\\Year_1\\Sem_2\\ESC190-C\\Project2\\p2_github\\Seam-Carving\\scale1.bin"); // save as .bin
    destroy_image(im);
    return 0;
}

int main() {
    char file[200] = "C:\\Users\\qingy\\OneDrive\\Documents\\Year_1\\Sem_2\\ESC190-C\\Project2\\p2_github\\Seam-Carving\\Meric.bin";
    float scale = 1.2;
    brighten(scale, file);
    printf("done\n");

    return 0;
}