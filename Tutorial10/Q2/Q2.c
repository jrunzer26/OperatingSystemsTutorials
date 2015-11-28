#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

// Define process 0 as MASTER
//#define MASTER 0

int main (int argc, char** argv) {

	const int n = 10;
	double *data = malloc(sizeof(double) * n); 
    double *read = malloc(sizeof(double) * n);

    int proc_id;            // Process rank
    int n_proc;             // Number of processes

	//assigns a random int generated from 0 to 100 
	//to each value in the array	
    for (int i = 0; i < n; i++) { 
    	double r = rand() % 100;
    	data[i] = r;
		
        printf("%f ", data[i]);
    }

    // Initialize MPI
    MPI_Init(&argc, &argv);
    
    // Get the current process id
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    
    // Get the current number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    //Force 2 processes. This is because we want to force a 1:1 send to receive ratio (each send has a receive).
    if (n_proc != 2) {
        fprintf(stderr, "Please run with n_proc size 2 %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1); 
    }
  

    int target = (proc_id + 1) %2;


    //double test = y[0];
    int inc = 0; //The current pingpong count
    //int recv = 0;
    	//n_proc--;
    //master
    while (inc < n) {
    	if (proc_id == inc % 2) {
            printf("Inside Master\n");

            inc++;

            //printf("Inside Master\n");

            MPI_Send(&data[0], n, MPI_INT, target, 0, MPI_COMM_WORLD);

            printf("SENDING. Process: %d. Current Count: %d. Target: %d.\n", proc_id, inc, target);
    		


    		/*
    		printf("Inside Master, ");
    		double time = MPI_Wtime();
    		//data, data size, type, proc id? idk the fuck, number sent, MPICOMMWORLD
    		MPI_Send(&recv, 1, MPI_INT, whatthefuck, 0, MPI_COMM_WORLD);
    		//sleep(1);
    		double endtime = MPI_Wtime();

    		printf("sent the fucking value %d, process %d of %d, whatthefuck is: %d\n", recv, proc_id, n_proc, whatthefuck);
    		printf("time: %f\n", endtime - time);
    		//receive from slaves
    		//MPI_Recv(datas, size, type, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    		//MPI_Recv(&recv, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    		

    		//proc = status.MPU_SOURCE;
    		//n_recv = status.MPI_TAG;
    		//number sent++;
            printf("Current inc value: %d\n", inc);
            */

    //slave
    	} else {
            printf("Inside slave\n");
    		//printf("Inside Slave\n ");
    	//MPI_Recv(recv, 1, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    		//MPI_Recv(&recv, 1, MPI_INT, whatthefuck, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    		MPI_Recv(read, n, MPI_INT, target, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    	//nrecv = status.MPI_TAG;

    		printf("RECEIVING. Process: %d. Current Count: %d. From: %d.\n", proc_id, inc, target);
    		//send back to master
    		//MPI_Send(&recv, 1, MPI_DOUBLE, whatthefuck, 0, MPI_COMM_WORLD);
    	}
    	//whatthefuck = 0;
    	
    	//inc++;
    }


    // Required to terminate all MPI processes
    printf("Finalized");
    MPI_Finalize();
    return 0;
}