//question 3

#define _XOPEN_SOURCE 600
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


int moving_sum[5] = {0,0,0,0,0};
sem_t semaphore;


typedef struct {
	int index;
	int number;
} numberInfo;

//the thread funciton to wait and change the moving_sum
void *factorial(void *numInfo){

	
	numberInfo myNumber = *(numberInfo *) numInfo;	//cast the number information
	int facto = calculate_factorial(myNumber.number);	//get the factorial

	//if the index is 0 don't wait to store the factorial
	if(myNumber.index == 0)
		moving_sum[0] = facto; 
	else{	//wait using semaphores for the moving_sum[index -1] > 0
		sem_wait(&semaphore);
		while(moving_sum[myNumber.index - 1] == 0){
			
			sem_post(&semaphore);	//increment the semaphore
			sem_wait(&semaphore);	//decrement the semaphore

		}
		//if the previous moving_sum is not zero, then add that sum and the current factorial to it
		moving_sum[myNumber.index] = moving_sum[myNumber.index -1] + facto;
		sem_post(&semaphore);
	}

	return NULL;
}

//calculates the factorial of the given number
int calculate_factorial(int n){
	if(n <= 1)
		return 1;
	else
		return n * calculate_factorial(n-1);
}
int main(void) {

	pthread_t threads[5];
	int numbers[5];
	numberInfo num[5];
	sem_init(&semaphore,0,1);

		printf("Please enter 5 integer numbers separated by spaces: ");
		for(int i = 0; i < 5; i++){
			scanf("%d",&numbers[i]);

			//create the number information
			num[i].index = i;
			num[i].number = numbers[i];
		}

		//create the threads
		for(int i = 0; i < 5; i++){
			pthread_create(&threads[i],0,factorial, (void *)&num[i]);
		}

		//join all the threads
		for(int i = 0; i < 5; i++){
			pthread_join(threads[i],NULL);
		}
		
		//output the moving sum
		for(int i = 0; i < 5; i++){
			printf("Moving sum index: %d value: %d\n",i,moving_sum[i]);
		}
		
    return 0;
}