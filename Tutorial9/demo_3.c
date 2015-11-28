#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>

double expensive(int num)
{
    sleep(1);
    return exp(num)*sin(num);
}

int main (int argc, char *argv[])
{
    double x = 0; // Watch out for race conditions!
    int thread_num, nthreads = 16;
    #ifdef _OPENMP
    omp_set_num_threads(nthreads);
    #endif
    printf("Testing OpenMP, you should see each thread print...\n");
    #pragma omp parallel for private(x)
    for (int i = 0; i < 100; ++i)
    {
        x = expensive(i);
        #pragma omp critical
        {
          #ifdef _OPENMP
          thread_num = omp_get_thread_num();
          printf("This thread = %d, num = %d, expensive = %f\n", 
                 thread_num, i, x);
          #endif
        }
    }
}
