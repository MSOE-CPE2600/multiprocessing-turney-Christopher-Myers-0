#ifndef MANDEL_H
#define MANDEL_H

#include "jpegrw.h"

int iteration_to_color( int i, int max );

int iterations_at_point( double x, double y, int max );

void compute_image( imgRawImage *img, double xmin, double xmax, double ymin, double ymax, int max );

void show_help();

#endif