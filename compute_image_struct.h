#ifndef COMPUTE_IMAGE_STRUCT
#define COMPUTE_IMAGE_STRUCT

typedef struct
{
    imgRawImage* img;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    int max;
    const int thread_index;
    const int NUM_THREADS;
} compute_image_struct;

#endif