#include <stdio.h>
#include <omp.h>
int main (int argc, char *argv[])
{
    printf("This statement will always print.\n");
	//Prints the below statement if this program is compiled using the -fopenMP command
    #ifdef _OPENMP
    printf("This statement will only print when compiled with \"-fopenmp\"\n");
    #endif
    return 0;
}
