#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

// Define process 0 as MASTER
#define MASTER 0
#define DATA_SIZE 50000
#define CHUNK_SIZE 10

//Calculate the number of primes to be expected
int num_of_primes(int start, int end) {
	int result = 0;
	if (start <= 1) {
		result = ((end)/(log(end))) + (end/10); //end/10 is an error safestate based on the approximation
	} else {
		result = (((end)/(log(end))) - ((start)/(log(start)))) + (end/10);
	}
	return result;
}



void master(int n_proc) {
	int chunk[DATA_SIZE] = {0};
	int slave_data[2] = {0};
	int n_sent = 0;
	MPI_Status status;
	int slave_source = 0;

	int* chunk_size = (int *) malloc(sizeof(int) * n_proc);
	int* results[n_proc];
	int slave_chunk_size[n_proc];	
	

	//Populate the slave arrays to be sent.
	for (int i = 1; i<n_proc; ++i) {
		//Calculate integers to send to the slave
		int range = DATA_SIZE / (n_proc-1);
		//First entry is the starting index.
		slave_data[0] = ((n_sent)*range)+1;
		slave_data[1] = ((n_sent+ 1) *range);
		
		//Determine an estimate for size of chunk.
		chunk_size[i] = num_of_primes(slave_data[0], slave_data[1]);

		//Send the current slave data.
		MPI_Send(&slave_data[0], 2, MPI_INT, i, n_sent, MPI_COMM_WORLD);
		n_sent++;
	}



	//Recover all slave processes
	for(int i = 1; i<n_proc; ++i){
		MPI_Recv(chunk, DATA_SIZE, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		slave_source = status.MPI_SOURCE;

		//Get the amount of values retrieved by the slace
		MPI_Get_count(&status, MPI_INT, &slave_chunk_size[i]);

		//Allocate memory for that specific pointer
		results[slave_source] = (int *) malloc (sizeof(int) * slave_chunk_size[i]);

		//Iterate through the retrieved chunk
		for (int j = 0; j<slave_chunk_size[i]; ++j) {
			//Remove any extra 0s appended to the end.
			if (chunk[j] != 0) {
				//Do not re-print any values that were not over-written.
					//Store results
					results[slave_source][j] = chunk[j];
			
				
			}
		}

	}


	//Send all slave processes a STOP signal
	for (int i = 1; i < n_proc; ++i ){
		MPI_Send(chunk, 2, MPI_INT, i, chunk_size[i], MPI_COMM_WORLD);

	}

	//Print out the results array.


	printf("Results array: \n");
	for(int i = 1; i < n_proc; ++i) {
		for(int j = 0; j < slave_chunk_size[i]; j++) {
			printf("%d,", results[i][j]);

		}
	}


	//Free allocated memory
	free(chunk_size);
	for (int i=1; i<n_proc; i++) {
		free(results[i]);
	}

}

void slave(){
	int chunk[2];
	MPI_Status status;
	int isPrime = 1;
	int n_recv;
	int result_location = 0;
	


	//Return the chunk, so that an array size can be determined
	MPI_Recv(chunk, 2, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	n_recv = status.MPI_TAG;




	//Using the values given to us by the master, compute the amount of primes we can expect to see.
	int primes = num_of_primes(chunk[0], chunk[1]);


	//Create an array that holds the primes found
	int* slave_data = (int *) malloc(sizeof(int)*primes);
	//slave_data[primes]; IF NEEDED

	while (n_recv < primes) {
		//Find the primes between the given start and end


		//Loop for determining the current value to be checking
		for (int i = chunk[0]; i <= chunk[1]; i++) {

			//Loop for determining the values to test against.
			for (int j = 2; j<i; j++ ){
				isPrime = 1;
				slave_data[result_location] = 0;


				if (i % j == 0) {
					//NOT prime
					isPrime = 0;
					break;
				}

				
			}

			
			//If the value is prime, store it, and increment the current location.
			if (isPrime == 1) {

				slave_data[result_location] = i;


				result_location++;

			}

		}
		//Send the results array back to the master
		MPI_Send(&slave_data[0], (result_location), MPI_INT, MASTER, n_recv, MPI_COMM_WORLD);


		//Wait for the next signal
		MPI_Recv(chunk, 2, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		n_recv = status.MPI_TAG;


	}




	free(slave_data);
}


int main(int argc, char* argv[]){
 	
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