/*
 * appl2.c
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
	pid_t PID = 0;
	PID = fork();

	if (PID == -1) {
		return 0;

	}
	//Child
	if (PID == 0) {
		sleep(1);
		printf("Child Process\n");
		exit(1);
	} //End Child
	//Parent
	else {
		int status;
		if (wait(&status)) {
			printf("Parent Process");
		}
	} //End Parent



}
