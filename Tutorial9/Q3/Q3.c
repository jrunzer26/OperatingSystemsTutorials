#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main (int argc, char *argv[]) {
	//stores initial and final time
	double t0, t1;

    //100 million 
	int n = 100000000;

	//create the array y of size n
	long *y = malloc(sizeof(long) * n); 

	//stores the norm? value
	long norm = 0;
	
	//assigns a random int generated from 0 to 100 
	//to each value in the array	
	for (int i = 0; i < n; i++) { 
		long r = rand() % 100;
		y[i] = r;
		//printf("%d ", y[i]);
	}
	printf("--------------Single threaded-------------\n");	
	//start the timer
	t0 = omp_get_wtime();
	//print out the start time
	printf("Start wall: %lf\n", t0);
	//serial loop, find the sum of the generated ints and use absolute? function value on it
	for (int i = 0; i < n; ++i) { 
		norm += fabs(y[i]);
	}
	//end the timer
	t1 =  omp_get_wtime();
	//printf("y @ 100m is %d\n", y[99999999]);
	//printf("y @ 0 is %d\n", y[2]);
	//print out the end time and the time taken to sum all the variables
	printf("End wall: %lf\n", t1);
	printf("Elapsed wall clock time: %lf\n", (t1 - t0));
	printf("Norm: %d\n", norm);
	
	printf("--------------Multi threaded-------------\n");
	//stores the norm value for multithreader timing
	long sum = 0;
	//stores the initial and final times
	double t0P, t1P;
	//defines number of threads 
	int thread_num, nthreads = 16;

    //sets number of threads for multithreading
    #ifdef _OPENMP
	omp_set_num_threads(nthreads);
    #endif

	//start the timer
	t0P = omp_get_wtime();

	//stores the current array value accessed 
	

	//print out the start wall timer
	printf("Start wall: %lf\n", t0P);
    //parallel section for multithreading
   
    #pragma omp parallel for reduction(+:sum)
    
    //for loop to iterate through 100million elements
	for (int i = 0; i < n; i++) {
		//store the currently indexed value into temp
		//int temp = fabs(y[i]);
		//critical section
		//#pragma omp critical
		//{
          //#ifdef _OPENMP
			//add temp to the sum
			//sum += temp;
			sum += fabs(y[i]);
		  //#endif
		//}

	}
	//end the timer
	t1P = omp_get_wtime();
	//print out the end time and the time taken to sum all the variables
	printf("End wall: %lf\n", t1P);
	printf("Elapsed wall clock time: %lf\n", (t1P - t0P));
	printf("Sum: %d\n", sum);
}

