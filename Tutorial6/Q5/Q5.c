/*
 * Q5 for SOFE 3950U / CSCI 3020U: Operating Systems Tutorial
 *
 * Copyright (C) 2015, Albert Fung 100520898, Jason Runzer 100520992, Nicholas Gregorio 100514374
 * All rights reserved.
 *
 */
#define _XOPEN_SOURCE 700 //required for barriers
#define NUMBERS 5

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

int total_sum, status;
sem_t sem;

void *factorial(int number) {
	int factorialProd = 1;
	for (int i = number; i > 0; --i) {
		factorialProd *= i;
	}
	printf("number: %d ", number); //test output, remove
	printf("factorial: %d\n", factorialProd); //test output, remove
	sem_wait(&sem); //CS start
	total_sum += factorialProd;
	sem_post(&sem); //CS end
}

int main(void) {
	int input[NUMBERS];

	FILE *fp = fopen("numbers.txt", "w");
	for (int i = 0; i < NUMBERS; i++) {
		printf("Enter five numbers: ");
		scanf("%d", &input[i]);
		fprintf(fp, "%d\n", input[i]);
	}
	fclose(fp);

	pid_t PID = 0;
	// Create the child.
	PID = fork();

	// Not the child or parent
	if (PID == -1) {
		return 0;
	}

	sem_init(&sem, 0, 1);
	//Is it a child process or the parent?
	//Child process:
	if (PID == 0) { //fork succeeded
		int readIn[NUMBERS];
		FILE *fp = fopen("numbers.txt", "r");
		for (int i = 0; i < NUMBERS; i++) {
			fscanf(fp, "%d\n", &readIn[i]);
		}
		fclose(fp);

		//create 5 threads
		pthread_t pth[NUMBERS];
		for (int i = 0; i < NUMBERS; i++) {
			pthread_create(&pth[i], 0, factorial, (int *) readIn[i]);
		}

		//join 5 threads
		for (int i = 0; i < NUMBERS; i++) {
			pthread_join(pth[i], 0);
		}

		sem_destroy(&sem);

		//write total sum variable to the file sum.txt
		FILE *fp2 = fopen("sum.txt", "w");
		fprintf(fp2, "%d", total_sum);
		fclose(fp2);
		//Finish on success
		return 0;
	} else {
		//Parent process:
		//wait for children to terminate
		waitpid(PID, &status, 0);
		//if child terminated normally
		if (status == 0) {
			int readtotal_sum;
			// read contents of sum.txt
			FILE *fp = fopen("sum.txt", "r");
			fscanf(fp, "%d", &readtotal_sum);
			printf("Total Sum: %d", readtotal_sum);
		}
	}
	return 0;
}
