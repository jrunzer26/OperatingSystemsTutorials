#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>


int main(int argc, char *argv[])
{

	int nthreads = atoi(argv[1]); //get the number of threads from the command line argument

	//set the number of threads
	#ifdef _OPENMP
	omp_set_num_threads(nthreads);
	#endif

	int n = 100000000;
	double *y = malloc(sizeof(double) * n); //create the array y of size n
	double dx = 1.0/(n+1.0); //calculate dx
	double x = 0; //initialize x

	//set the parallel section for the threads
	#pragma omp parallel for private(x)
	for(int i = 0; i < n; i++){
		//calculate the x
		x = i * dx;
		y[i] = exp(x) * cos(x) * sin(x) * sqrt(5 * x + 6.0); //store it in the y array
	}

	return 0;
}