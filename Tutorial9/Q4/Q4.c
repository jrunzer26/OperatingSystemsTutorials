#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main (int argc, char *argv[] ) {
	//initialize the array variables
	int size = 10;
	int a[size][size];
	int b[size][size];
	int multi[size][size];

	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			a[i][j] = i + j+ 1;
			b[i][j] = i  + j+ 1;
		}
	}
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			printf("%d ",a[i][j]);
		}
		puts("");
	}
	puts("");
	
	//define the number of threads
	int nthreads = atoi(argv[1]);
	#ifdef _OPENMP
    omp_set_num_threads(nthreads);
    #endif

    int sum; //the sum of the multiplication for the rows

    for(int i = 0; i < size; i++){
    	#pragma omp parallel for private (sum)
    	for(int j = 0; j < size; j++){
    		sum = 0;//initialize the sum to 0 so it resets for each multiplication index
    		for(int k = 0; k < size; k++){
    			sum += a[i][k] * b[k][j]; //calculate the sum for all the rows for a and b
    			
    		}
    		multi[i][j] = sum; //store the sum into the multiplication array
    	}
    }
	
	printf("\none column of the matrix\n");
	for(int i = 0; i < size; i++){
		printf("%d\n",multi[i][0]);
	}
	printf("second column of the matrix\n");
	for(int i = 0; i < size; i++){
		printf("%d\n",multi[i][2]);
	}
	

	/* //used to print out the whole array (will be messy with a 100x100)
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			printf("%d ",multi[i][j]);
		}
		puts("");
	}
	*/

	return 0;
}