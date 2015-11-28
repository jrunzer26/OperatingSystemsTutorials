#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>

double expensive(int num)
{
    sleep(1);
    return exp(num);
}

int main (int argc, char *argv[])
{
    double x = 0; // Used in the summation
    int thread_num, nthreads = 16;
    #ifdef _OPENMP
    omp_set_num_threads(nthreads);
    #endif
    printf("Testing OpenMP, you should see each thread print...\n");
    #pragma omp parallel for reduction(+: x)
    for (int i = 0; i < 100; ++i)
    {
        x += expensive(i);
    }
    printf("SUM of x = %f\n", x);
}
