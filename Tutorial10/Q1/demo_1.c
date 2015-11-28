/*
 * Tutorial 10 MPI Demo for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Author: Jonathan Gillett
 *
 */

 //compile command: mpicc -Wall -Wextra -std=c99 <program name>.c -o <program name> -lm
 //run command: mpiexec -np <num processes> ./<program>

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

// Define process 0 as MASTER
#define MASTER 0

int main (int argc, char* argv[])
{
    int proc_id;            // Process rank
    int n_proc;             // Number of processes

    // Initialize MPI
    MPI_Init(&argc, &argv);
    
    // Get the current process id
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    
    // Get the current number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

    // Display a "Hello world" message from each child/parent
    if (proc_id == MASTER)
    {
        printf( "Hello world from MATER, process %d of %d\n", proc_id, n_proc);
    }
    else
    {
        printf( "Hello world from SLAVE, process %d of %d\n", proc_id, n_proc);
    }

    // Required to terminate all MPI processes
    MPI_Finalize();
}