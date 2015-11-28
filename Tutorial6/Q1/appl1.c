/*
 * appl1.c
 *
 *  Created on: Oct 28, 2015
 *      Author: uoitstudent
 */
#define _XOPEN_SOURCE 600
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main( int argc, char *argv[]) {

	//Write
	FILE *child1;
	FILE *child2;

	pid_t PID1 = 0;
	//Create first child
	PID1 = fork();
	//Neither parent nor child
	if (PID1 == -1 ) {
		return 0;
	}
	//First Child
	if (PID1 == 0) {
		//wait();
		//Create character array to store file input
		char input1[256];
		//Delay one second
		sleep(1);
		//Read file and print
		child1 = fopen("child1.txt", "r");
		fscanf(child1, "%s", input1);
		printf("%s", input1);

		return 0;
	}
	//Parent
	else {
		//Create second child
		pid_t PID2 = fork();
		if (PID2 == -1 ) {
				return 0;
			}
			//Second child
			if (PID2 == 0) {
				//Create character array to read file input
				char input2[256];
				//Delay for one second

				sleep(1);
				//Read input and print
				child2=fopen("child2.txt", "r");
				fscanf(child2, "%s", input2);
				printf("%s", input2);
				return 0;
			}
			else { //Parent

				child1 = fopen("child1.txt", "w");
				child2 = fopen("child2.txt", "w");
				//Force files to clear
				fprintf(child1, "");
				fprintf(child2, "");
				//Initialize files
				fprintf(child1, "child1");
				fprintf(child2, "child2");

			}
	}
	return 0;


}
