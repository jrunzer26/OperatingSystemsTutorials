#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

double xcalc(int i, double j) {
	
	return i* j;
}
double ycalc(double num) {
	//Sin and cos are using radians
	return exp(num) * sin(num) *cos(num) * sqrt((5.0 * num) + 6.0);
}

int main (int argc, char *argv[] ) {
	
	//initialize n with 100000000, and dx.
	int n = 100000000;
	double dx = (1.0/(n+1.0));
	//Grab amount of threads from the user after the ./q5 execution. Optimal amount of threads is 1.
	int nthreads = atoi(argv[1]);
	int thread_num;
	double x=0.0, y=0.0;
	double start;

	//Create threads, only if openmp is specified in compilation
	#ifdef _OPENMP
    omp_set_num_threads(nthreads);
    #endif
	//Initialize the file. File needs to be cleared before writing so that the concurrent entries are not appended.
	FILE *output = fopen("calculations.txt", "w");	//Clear the file
	output = freopen("calculations.txt", "a", stdout);

	//Specify the operation to be run on the seperate threads
	#pragma omp parallel for private(x) private(y)
	for (int i = 0; i <= n; ++i){

		x = xcalc(i, dx);
		y = ycalc(x);
		//Every 1 million instructions, perform the following critical section
		if ((i % 1000000) == 0) {
			#pragma omp critical 
			{
				#ifdef _OPENMP
				thread_num = omp_get_thread_num();
				fprintf(output, "threads: %d\n", thread_num);
				fprintf(output, "i: %d\nx: %lf\ny: %lf\n\n", i, x, y);
				#endif
			}
		}
		
	}
	#ifdef _OPENMP
	start = omp_get_wtime();
	#endif
	printf("time: %lf", start);
	fclose(output);
	return 0;
	
}

