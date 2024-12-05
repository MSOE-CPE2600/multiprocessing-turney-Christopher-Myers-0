/*******************************************************************************
Filename: mandelmovie.c
Name: Christopher Myers
Course: CPE 2600-121
Assignment: Lab 11
Date: 11/23/2024
make or gcc -o mandelmovie.out mandelmovie.c mandel.c jpegrw.c -ljpeg -lm
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <pthread.h>
#include "jpegrw.h"
#include "mandel.h"
#include "compute_image_struct.h"


int main( int argc, char *argv[] )
{

    // Get the arguments from the command
    char opt;
    int NUM_PROCESSES = 1;
    int NUM_THREADS = 1;

    while((opt = getopt(argc, argv, "n:t:")) != -1)
    {
        switch(opt) {
            case 'n':
                NUM_PROCESSES = strtol(optarg, NULL, 10);
                break;
            case 't':
                NUM_THREADS = strtol(optarg, NULL, 10);
        }
    }

    // Create the number of processes from the command
    int imageIndex = 0;
    int pid = fork();
    for(int i = 1; i < NUM_PROCESSES; i++)
    {
        if(pid != 0) {
            pid = fork();
            imageIndex = i;
        }
    }

    if(pid == 0)
    {
        // These are the configuration values
        char outfile[20];
        double xcenter = 0;
        double ycenter = 0;
        double xscale = 4;
        double yscale = 0; // calc later
        int    image_width = 1000;
        int    image_height = 1000;
        int    max = 1000;
        // Calculate first and last image index and round to nearest integer.
        double startImageDouble = imageIndex*50.0/NUM_PROCESSES;
        double endImageDouble = (imageIndex + 1)*50.0/NUM_PROCESSES;
        int    startImage;
        int    endImage;
        if(startImageDouble < 0) {
            startImage = (int)(startImageDouble - 0.5);
        } else {
            startImage = (int)(startImageDouble + 0.5);
        }
        if(endImageDouble < 0) {
            endImage = (int)(endImageDouble - 0.5);
        } else {
            endImage = (int)(endImageDouble + 0.5);
        }
        //printf("PID: %d startImage: %d endImage: %d\n", getpid(), startImage, endImage);
        for(int i = startImage; i < endImage; i++)
        {
            // Create output filename
            sprintf(outfile, "mandel%d.jpg", i);
            // Compute the new image coordinates and scaling
            xcenter = -i*0.015;
            ycenter = i*0.015;
            xscale = 4*pow(0.95, i);
            // Calculate y scale based on x scale (settable) and image sizes in X and Y (settable)
            yscale = xscale / image_width * image_height;

            // Display the configuration of the image.
            printf("mandel: x=%lf y=%lf xscale=%lf yscale=%1f max=%d outfile=%s\n",xcenter,ycenter,xscale,yscale,max,outfile);

            // Create a raw image of the appropriate size.
            imgRawImage* img = initRawImage(image_width,image_height);

            // Fill it with a black
            setImageCOLOR(img,0);

            const int THREAD_MAX = 20;
            pthread_t threads[THREAD_MAX];
            if(NUM_THREADS > THREAD_MAX) {
                printf("Thread count exceeded, limiting to %d threads\n", THREAD_MAX);
            }
            // Compute the Mandelbrot image by splitting image into threads
            for(int thread_index = 0;
                thread_index < NUM_THREADS && thread_index < THREAD_MAX;
                thread_index++)
            {
                compute_image_struct *compute_image_data = malloc(sizeof(compute_image_struct));
                if(compute_image_data == NULL) {
                    perror("malloc failed");
                    exit(1);
                }
                compute_image_data->img = img;
                compute_image_data->xmin = xcenter-xscale/2;
                compute_image_data->xmax = xcenter+xscale/2;
                compute_image_data->ymin = ycenter-yscale/2;
                compute_image_data->ymax = ycenter+yscale/2;
                compute_image_data->max = max;
                compute_image_data->thread_index = thread_index;
                compute_image_data->NUM_THREADS = NUM_THREADS;
                //printf("%d\n", compute_image_data.thread_index);
                if(pthread_create(&threads[thread_index], NULL, compute_image, compute_image_data) != 0) {
                    perror("pthread_create failed");
                    free(compute_image_data);
                    exit(1);
                }
            }
            for(int thread_index = 0;
                thread_index < NUM_THREADS && thread_index < THREAD_MAX;
                thread_index++)
            {
                pthread_join(threads[thread_index], NULL);
            }

            // Save the image in the stated file.
            storeJpegImageFile(img,outfile);

            // free the mallocs
            freeRawImage(img);
        }
    }
    // If parent process, wait for all children to be done
    if(pid != 0) {
        for(int i = 0; i < NUM_PROCESSES; i++) {
            wait(NULL);
        }
    }

	return 0;
}