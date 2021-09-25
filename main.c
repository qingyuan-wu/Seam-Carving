#include "seamcarving.h"
#include "c_img.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    //surfing image testing. Image: HJoceanSmall.bmp

    struct rgb_img *im;
    struct rgb_img *cur_im;
    struct rgb_img *grad;
    double *best;
    int *path;

    read_in_img(&im, "C:\\Users\\qingy\\OneDrive\\Documents\\Year_1\\Sem_2\\ESC190-C\\Project2\\p2_github\\Seam-Carving\\HJoceanSmall.bin");
    
    for(int i = 0; i < 52; i++){
        printf("i = %d\n", i);
        
        calc_energy(im,  &grad);
        dynamic_seam(grad, &best);
        recover_path(best, grad->height, grad->width, &path);
        remove_seam(im, &cur_im, path);

        char filename[200];
        sprintf(filename, "img%d.bin", i);
        
        char filePath[] = "C:\\Users\\qingy\\OneDrive\\Documents\\Year_1\\Sem_2\\ESC190-C\\Project2\\p2_github\\Seam-Carving\\carvedImages\\";
        strcat(filePath, filename);
        write_img(cur_im, filePath);

        destroy_image(im);
        destroy_image(grad);
        free(best);
        free(path);
        im = cur_im;
    }
    destroy_image(im);
    return 0;
}