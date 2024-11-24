/*******************************************************************************
Filename: mandelmovie.c
Name: Christopher Myers
Course: CPE 2600
Date: 11/23/2024
gcc -o mandelmovie.out mandelmovie.c mandel.c jpegrw.c -ljpeg -lm -g
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include "jpegrw.h"
#include "mandel.h"


int main( int argc, char *argv[] )
{

    // Get the number of processes from the command argument
    char opt;
    int NUM_PROCESSES = 1;

    while((opt = getopt(argc, argv, "n:")) != -1)
    {
        switch(opt) {
            case 'n':
                NUM_PROCESSES = strtol(optarg, NULL, 10);
                break;
        }
    }

    // Create the number of processes from the command
    int imageIndex = 0;
    int pid = fork();
    //int pids[NUM_PROCESSES];
    //pids[0] = pid;
    for(int i = 1; i < NUM_PROCESSES; i++)
    {
        if(pid != 0) {
            pid = fork();
            //pids[i] = pid;
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

            xcenter = -i*0.015;
            ycenter = i*0.015;
            xscale = 4*pow(0.95, i);
            // Calculate y scale based on x scale (settable) and image sizes in X and Y (settable)
            yscale = xscale / image_width * image_height;

            // Display the configuration of the image.
            //printf("mandel: x=%lf y=%lf xscale=%lf yscale=%1f max=%d outfile=%s\n",xcenter,ycenter,xscale,yscale,max,outfile);

            // Create a raw image of the appropriate size.
            imgRawImage* img = initRawImage(image_width,image_height);

            // Fill it with a black
            setImageCOLOR(img,0);

            // Compute the Mandelbrot image
            compute_image(img,xcenter-xscale/2,xcenter+xscale/2,ycenter-yscale/2,ycenter+yscale/2,max);

            // Save the image in the stated file.
            storeJpegImageFile(img,outfile);

            // free the mallocs
            freeRawImage(img);
        }
    }

    if(pid != 0) {
        for(int i = 0; i < NUM_PROCESSES; i++) {
            wait(NULL);
        }
    }

	return 0;
}