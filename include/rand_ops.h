#ifndef RANDOPS_H
#define RANDOPS_H 1

#include "consts.h"

void rand_colour(const int, unsigned char*);
void centre_colour(const int, const int*, const Image*, unsigned char*);
Image *rand_triangle(const unsigned char *colour, const Image *canvas, const float feature_size, int *vertices=0);
Image *rand_triangle_centre_colour(const Image *canvas, const Image *target, const float feature_size, int *vertices=0);

#endif
