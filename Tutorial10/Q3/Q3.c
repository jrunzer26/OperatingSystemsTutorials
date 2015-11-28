#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

// Define process 0 as MASTER
#define MASTER 0

#define DATA_SIZE 10000 //array of 10000 values
#define CHUNK_SIZE 100 //each slave will get 100 of the data values
#define SIZE 100 //the size of the array

void master(int n_proc)
{
    int data[DATA_SIZE] = { 0 };    // The data to send
    int chunk[CHUNK_SIZE] = { 0 };  // The chunk to store results

    int total_chunks = DATA_SIZE / CHUNK_SIZE; //sending 100 chunks
    
    int n_sent = 0, n_recv = 0;        // The number of the data chunks sent/recv
    int proc = 0;                      // The process that returned data
    MPI_Status status;                 // MPI status struct

    int A [SIZE][SIZE];
    int B [SIZE][SIZE];
    int C [SIZE][SIZE];

    //print out the A matrix
    
    //build a and b
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            A[i][j] = j + i + 1;
            B[i][j] = j + i + 1;
            //printf("%5d ", A[i][j]);
            //the output would be messy with 100x100
        }
        //puts("");
    }
    //print out just a sample of the matrix
    printf("A column 0 and 1\n");
    for(int i = 0; i < SIZE; i++){
        printf("%4d%4d\n",A[0][i],A[1][i]);
    }

    printf("B column 0 and 1\n");
    for(int i = 0; i < SIZE; i++){
        printf("%4d%4d\n",B[0][i],B[1][i]);
    }
    //print out the B matrix
    /*
    printf("B\n");
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            printf("%5d ", B[i][j]);
        }
        puts("");
    }
    */
    
    //brodcast B to all the slaves
    MPI_Bcast(&B[0][0],SIZE*SIZE,MPI_INT,MASTER,MPI_COMM_WORLD);

    // "Map" the data to slave processes
    // i MUST start from 1, since MASTER is process 0!
    for (int i = 1; i < n_proc; ++i)
    {
        // Send CHUNK_SIZE of data to each process initially
        // Sends data, of type double to process 'i'
        MPI_Send(&A[n_sent][0], CHUNK_SIZE, MPI_INT, i, 
                 n_sent, MPI_COMM_WORLD);
        n_sent++;
    }

    // Receive EACH of the chunks from the slave processes
    for (int i = 0; i < total_chunks; ++i)
    {
        // Receive the computed chunk back from the slave
        MPI_Recv(chunk, CHUNK_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);
        // Get the process that sent the data and send it the next chunk
        proc = status.MPI_SOURCE;
        n_recv = status.MPI_TAG;
       
        // Copy the results from the slave into the C matrix
        for (int i = 0; i < CHUNK_SIZE; ++i)
        {
            C[n_recv][i] = chunk[i];
        }
    
        //send more data to a slave if there is more to calculate
        if (n_sent < total_chunks)
        {
            MPI_Send(&A[n_sent][0], CHUNK_SIZE, MPI_INT, proc, 
                     n_sent, MPI_COMM_WORLD);
            n_sent++;
        } 
    }

    // Send all the slave processes STOP signal, (TAG of CHUNK_SIZE)
    for (int i = 1; i < n_proc; ++i)
    {
        MPI_Send(chunk, CHUNK_SIZE, MPI_INT, i, 
                 CHUNK_SIZE, MPI_COMM_WORLD);
    }

    /* will be messy with 100 x 100
    //print out the C matrix
    printf("C \n");
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            printf("%5d ",C[i][j]);
        }
        puts("");
    }
    */
    //print out a sample of the result
    printf("Two columns of the C Matrix\n");
    for(int i = 0; i < SIZE; i++){
        printf("%10d%10d\n",C[i][0],C[i][1]);
    }

}

void slave(int proc_id)
{
    int chunk[CHUNK_SIZE] = { 0 };  // The chunk to receive for calculations
    int result[CHUNK_SIZE] = { 0 }; // A calculated row of the matrix multiplication
    int n_recv = 0;                    // The number of the data chunk received
    MPI_Status status;                 // MPI status struct
    int B[SIZE][SIZE];

    //recieve the brodcat B from the master
    MPI_Bcast(&B[0][0],SIZE*SIZE,MPI_INT,MASTER,MPI_COMM_WORLD);
    
    // Receive the chunk to calculate from MASTER
    MPI_Recv(chunk, CHUNK_SIZE, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    n_recv = status.MPI_TAG;

    // Calculate all results provided until "STOP" signal recieved (CHUNK_SIZE)
    while (n_recv < CHUNK_SIZE)
    {
        for (int i = 0; i < CHUNK_SIZE; ++i)
        {
            result[i] = 0; // reset the results array
            for(int j = 0; j < CHUNK_SIZE; j++){
                result[i] += chunk[j] * B[j][i];
            }
        }
        // Send the results back to MASTER, include in TAG the chunk that was calculated
        MPI_Send(result, CHUNK_SIZE, MPI_INT, MASTER, n_recv, MPI_COMM_WORLD);
        // Wait for the next chunk of data to be received
        MPI_Recv(chunk, CHUNK_SIZE, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        n_recv = status.MPI_TAG;
    }
}



int main (int argc, char* argv[])
{
    int proc_id;            // Process rank
    int n_proc;             // Number of processes

    // Initialize MPI
    MPI_Init(&argc, &argv);
    
    // Get the current number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

    // Get the current process id
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);



    if (proc_id == MASTER)
    {
        master(n_proc);
    }
    else
    {
        slave(proc_id);
    }

    // Required to terminate all MPI processes
    MPI_Finalize();
}