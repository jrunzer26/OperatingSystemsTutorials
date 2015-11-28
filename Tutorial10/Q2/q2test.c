#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

// Define process 0 as MASTER
#define MASTER 0

//#define DATA_SIZE 4
//#define DATA_SIZE 10000000
// #define CHUNK_SIZE 10000000
#define CHUNK_SIZE 1
#define numTimes 1000

void slave(int proc) {

	const int n = 10000000;
	double *data = malloc(sizeof(double) * n);
	double *read = malloc(sizeof(double) * n);

	//double chunk[1] = { 0 };
	int n_recv = 0;
	//double data[1] = { 0 };                // The number of the data chunk received
	MPI_Status status;
	double time = MPI_Wtime();
	for (int i = 0; i < numTimes; i++) {
		MPI_Recv(&read, CHUNK_SIZE, MPI_DOUBLE, MASTER, MPI_ANY_TAG,
				MPI_COMM_WORLD, &status);
		printf("slave receive\n");
		printf("slave send\n");
		MPI_Send(&data, CHUNK_SIZE, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
	}
	double endtime = MPI_Wtime();
	printf("time: %lf\n", endtime - time);

}

void master(int target) {
	const int n = 10000000;
	double *data = malloc(sizeof(double) * n);
	double *read = malloc(sizeof(double) * n);

	for (int i = 0; i < n; i++) {
		double r = rand() % 100;
		data[i] = r;
		//printf("%lf\n", data[i]);
	}

	//double chunk[1] = { 0 };
	int n_recv = 0;
	//double data[1] = { 0 };                  // The number of the data chunk received
	MPI_Status status;

	double time = MPI_Wtime();
	for (int i = 0; i < numTimes; i++) {
		printf("master send\n");
		MPI_Send(&data, CHUNK_SIZE, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
		MPI_Recv(&read, CHUNK_SIZE, MPI_DOUBLE, 1, MPI_ANY_TAG, MPI_COMM_WORLD,
				&status);
		printf("master receive\n");
	}
	double endtime = MPI_Wtime();
	printf("time: %lf\n", endtime - time);

}

int main(int argc, char** argv) {
	int proc_id;            // Process rank
	int n_proc;             // Number of processes

	// Initialize MPI
	MPI_Init(&argc, &argv);

	// Get the current number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

	// Get the current process id
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

	if (n_proc != 2) {
		fprintf(stderr, "Please run with n_proc size 2 %s\n", argv[0]);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	printf("splitting off\n");
	if (proc_id == MASTER) {
		master(1);
	} else {
		slave(proc_id);
	}

	// Required to terminate all MPI processes
	MPI_Finalize();
}
