#include "c_img.h"
#include "seamcarving.h"
#include <stdio.h>
#include <math.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MIN3(a,b,c) (MIN(MIN(a,b), MIN(b,c))) // minimum of 3 numbers

void calc_energy(struct rgb_img* im, struct rgb_img** grad) {
    int height = im->height, width = im->width, red, blue, green;
    int rx, gx, bx, ry, gy, by;
    create_img(grad, height, width);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // x gradients
            int x_grad, y_grad;
            if (j == 0 && j == width - 1) {
                // 1 pixel wide
                rx = ry = get_pixel(im, i, j, 0);
                gx = gy = get_pixel(im, i, j, 1);
                bx = by = get_pixel(im, i, j, 2);
            }
            else if (j == 0) {
                rx = get_pixel(im, i, j + 1, 0) - get_pixel(im, i, width - 1, 0);
                gx = get_pixel(im, i, j + 1, 1) - get_pixel(im, i, width - 1, 1);
                bx = get_pixel(im, i, j + 1, 2) - get_pixel(im, i, width - 1, 2);
            }
            else if (j == width - 1) {
                rx = get_pixel(im, i, 0, 0) - get_pixel(im, i, j - 1, 0);
                gx = get_pixel(im, i, 0, 1) - get_pixel(im, i, j - 1, 1);
                bx = get_pixel(im, i, 0, 2) - get_pixel(im, i, j - 1, 2);
            }
            else {
                rx = get_pixel(im, i, j + 1, 0) - get_pixel(im, i, j - 1, 0);
                gx = get_pixel(im, i, j + 1, 1) - get_pixel(im, i, j - 1, 1);
                bx = get_pixel(im, i, j + 1, 2) - get_pixel(im, i, j - 1, 2);
            }
            // y gradients
            if (i == 0 && i == width - 1) {
                // 1 pixel wide
                ry = rx;
                gy = gx;
                by = bx;
            }
            else if (i == 0) {
                ry = get_pixel(im, i + 1, j, 0) - get_pixel(im, height - 1, j, 0);
                gy = get_pixel(im, i + 1, j, 1) - get_pixel(im, height - 1, j, 1);
                by = get_pixel(im, i + 1, j, 2) - get_pixel(im, height - 1, j, 2);
            }
            else if (i == height - 1) {
                ry = get_pixel(im, 0, j, 0) - get_pixel(im, i - 1, j, 0);
                gy = get_pixel(im, 0, j, 1) - get_pixel(im, i - 1, j, 1);
                by = get_pixel(im, 0, j, 2) - get_pixel(im, i - 1, j, 2);
            }
            else {
                ry = get_pixel(im, i + 1, j, 0) - get_pixel(im, i - 1, j, 0);
                gy = get_pixel(im, i + 1, j, 1) - get_pixel(im, i - 1, j, 1);
                by = get_pixel(im, i + 1, j, 2) - get_pixel(im, i - 1, j, 2);
            }
            x_grad = pow(rx, 2) + pow(gx, 2) + pow(bx, 2);
            y_grad = pow(ry, 2) + pow(gy, 2) + pow(by, 2);
            uint8_t energy = (uint8_t)(pow((double)(x_grad + y_grad), 0.5) / 10.0);
            set_pixel(*grad, i, j, (int)energy, (int)energy, (int)energy);
        }
    }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr){
    *best_arr = (double *)malloc(sizeof(double) * (grad->width+1) * (grad->height+1));
    for (int i = 0; i < grad->width; i++){
        (*best_arr)[i] = get_pixel(grad, 0, i, 0);
    }

    for (int y = 1; y < grad->height; y++){
        for (int x = 0; x < grad->width; x++){
            if (x != 0 && x != grad->width - 1){
                double a = (*best_arr)[(y-1)*(grad->width) + x-1];
                double b = (*best_arr)[(y-1)*(grad->width) + x];
                double c = (*best_arr)[(y-1)*(grad->width) + x+1];
                (*best_arr)[y*grad->width + x] = MIN3((a), (b), (c)) + get_pixel(grad, y, x, 0);
            }
            else if (x == 0){
                double b = (*best_arr)[(y-1)*(grad->width) + x];
                double c = (*best_arr)[(y-1)*(grad->width) + x+1];
                (*best_arr)[y*grad->width + x] = MIN((b), (c)) + get_pixel(grad, y, x, 0);
            }
            else if (x == grad->width - 1){
                double a = (*best_arr)[(y-1)*(grad->width) + x-1];
                double b = (*best_arr)[(y-1)*(grad->width) + x];
                (*best_arr)[y*grad->width + x] = MIN((a), (b)) + get_pixel(grad, y, x, 0);
            }
        }
    }
}

void recover_path(double *best, int height, int width, int **path){
    *path = (int *)malloc(sizeof(int) * height);
    double cur_min = best[height * width-1];
    int cur_min_ind = width-1;

    for (int x = 0; x < width; x++){
        if (best[(height-1)*width + x] < cur_min) {
            cur_min = best[(height-1)*width + x];
            cur_min_ind = x;
        }
    }

    (*path)[height-1] = cur_min_ind;
    for (int y = height - 2; y >= 0; y--){
        if ((*path)[y+1] != 0 && (*path)[y+1] != width-1){
            if (MIN3(best[y*width+(*path)[y+1]-1], best[y*width+(*path)[y+1]], best[y*width+(*path)[y+1]+1]) == best[y*width+(*path)[y+1]-1])
                {(*path)[y] = (*path)[y+1]-1;}
            else if (MIN3(best[y*width+(*path)[y+1]-1], best[y*width+(*path)[y+1]], best[y*width+(*path)[y+1]+1]) == best[y*width+(*path)[y+1]])
                {(*path)[y] = (*path)[y+1];}
            else (*path)[y] = (*path)[y+1]+1;
        }
        else if ((*path)[y+1] == 0) {
            if (MIN(best[y*width+(*path)[y+1]], best[y*width+(*path)[y+1]+1]) == best[y*width+(*path)[y+1]])
                {(*path)[y] = (*path)[y+1];}
            else (*path)[y] = (*path)[y+1]+1;
        }
        else if ((*path)[y+1] == width-1) {
            if (MIN(best[y*width+(*path)[y+1]-1], best[y*width+(*path)[y+1]]) == best[y*width+(*path)[y+1]-1])
                {(*path)[y] = (*path)[y+1]-1;}
            else (*path)[y] = (*path)[y+1];
        }
    }
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    *dest = (struct rgb_img *)malloc(sizeof(struct rgb_img));
    (*dest)->height = src->height;
    (*dest)->width = src->width - 1;
    (*dest)->raster = (uint8_t *)malloc(3 * (*dest)->height * (*dest)->width);

    for (int y = 0; y < src->height; y++) {
        for (int x = 0; x < src->width; x++) {
            if (x < path[y]) {
                int r = get_pixel(src, y, x, 0);
                int g = get_pixel(src, y, x, 1);
                int b = get_pixel(src, y, x, 2);
                set_pixel(*dest, y, x, r, g, b);
            }
            else if (x > path[y]) {
                int r = get_pixel(src, y, x, 0);
                int g = get_pixel(src, y, x, 1);
                int b = get_pixel(src, y, x, 2);
                set_pixel(*dest, y, x-1, r, g, b);
            }
        }
    }
}