/*
 * Q4 for SOFE 3950U / CSCI 3020U: Operating Systems Tutorial
 *
 * Copyright (C) 2015, Albert Fung 100520898, Jason Runzer 100520992, Nicholas Gregorio 100514374
 * All rights reserved.
 *
 */
#define _XOPEN_SOURCE 700 //required for barriers
#define NUMBERS 10

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int BUFFER[5] = { 0, 0, 0, 0, 0 };

// remove assigned values later because i am not typing that shit in everytime i run to test
int input[NUMBERS] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

sem_t sem;

void *producer(int usersInput[]) {
	int elements = 0;
	int counter = 0;
	while (elements < NUMBERS) {
		sleep(1); //random delay before proceeding
		if (counter < 6) {
			sem_wait(&sem); //CS start
			if (BUFFER[counter] == 0) {
				printf("Produced %d%s", usersInput[elements], "\n");
				BUFFER[counter] = usersInput[elements];
				elements++;
			} else {
				counter++;
			}
			sem_post(&sem); //CS end
		} else {
			printf("BUFFER IS FULL!\n");
			counter = 0;
		}
	}
}

void *consumer() {
	int elements = 0;
	int counter = 0;
	while (elements < NUMBERS) {
		sleep(1); //random delay before proceeding
		if (counter < 6) {
			sem_wait(&sem); //CS start
			if (BUFFER[counter] != 0) {
				printf("Consumed %d%s", BUFFER[counter], "\n");
				elements++;
				BUFFER[counter] = 0;
			} else {
				printf("BUFFER IS EMPTY!\n");
				counter++;
			}
			sem_post(&sem); //CS end
		} else {
			counter = 0;
		}
	}
}

int main(void) {
	//uncomment this to ask user for input cause rn GUESS WHAT? IDGAF!
	/*
	 for (int i = 0; i < NUMBERS; i++) {
	 printf("Give me the fucking 10 numbers: ");
	 scanf("%d", &input[i]);
	 }
	 */
	pthread_t pth;
	pthread_t pth2;

//semaphore init
	sem_init(&sem, 0, 1);

// Create the threads
	pthread_create(&pth, 0, producer, (int *) input);
	pthread_create(&pth2, 0, consumer, (void *) NULL);

// wait for our thread to finish before continuing
	pthread_join(pth, 0);
	pthread_join(pth2, 0);

// Clean up
	sem_destroy(&sem);

//print out the contents of the buffer
	for (int i = 0; i < 5; i++) {
		printf("%d ", BUFFER[i]);
	}
	return 0;
}
