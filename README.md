# System Programming Lab 11 Multiprocessing

### Overview:
The file mandelmovie.c can be compiled to create an executable. This executable takes a command line argument with flag `-n <processes>` where the value after the `-n` is the number of processes you would like to run the program with. Running the program will create 50 images of a mandelbrot fractal in slightly different positions. No `-n` argument will cause the program to default to 1 calculating process. In my implementation the parent does not generate any images itself, but rather manages child processes to do the image generation. <br>

Using the `-t` tag with a number after it will adjust how many threads will be used to generate each image. No `-t` argument defaults to 1 thread. My implementation splits the image up by rows and assigns a range of rows to each thread. In order to make each thread generate a different part of the image I passed in a struct containing useful data including a thread_index int used for telling the thread what chunk of the image to generate.

### Plotting Runtime vs Number of Processes:
![plot](Plot.png)

After running the program at different process counts these were my runtime results. This flattening of the curve makes sense as there are only 16 logical processors in my laptop, so exceeding that number yields minimal benefit.

### Table of Runtime with Processes and Threads (seconds):
![table](threads_and_processes_table.png)

As expected more threads and more processes decreased runtime. As for if threads or processes improved the runtime the most, my data doesn't seem very clear. Both threads and processes provided similar times without more of one or the other clearly being faster. The only time where the runtime was significantly different is 1 process 2 threads at 68 seconds versus the 2 processes 1 thread at 49 seconds. <br>

A sweet spot could be 10 processes 5 threads, as that yielded nearly the fastest time without making a massive number of threads and processes.