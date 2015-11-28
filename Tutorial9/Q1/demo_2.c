#include <stdio.h>
#include <omp.h>
int main (int argc, char *argv[])
{
    int num = 0; // Watch out for race conditions!
    int thread_num, nthreads = 16;
	//Creates 16 threads when compiled with the -fopenmp tag
    #ifdef _OPENMP
    omp_set_num_threads(nthreads);
    #endif
    printf("Testing OpenMP, you should see each thread print...\n");

	//The following lines of code will be executed on each of the 16 threads in parallel
    #pragma omp parallel
    {
	//The following code is a critical section; it will be executed on one thread at a time.
        #pragma omp critical
        {
            #ifdef _OPENMP
            num += 1; // THREAD SAFE INCREMENT
            thread_num = omp_get_thread_num();
            printf("This thread = %d, num = %d\n", thread_num, num);
            #endif
        }
    }
    return 0;
}
