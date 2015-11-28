/*
 * Q3.c
 *
 *  Created on: Nov 4, 2015
 *      Author: uoitstudent
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#define SHELL "/bin/sh"

int main(int argc, char *argv[]) {
	int status;
	pid_t pid;
	const char *command ="./process";
	pid = fork();


	if (pid < 0) {
		//Failed
	} else if (pid == 0) {
		//Child Process
		execv(command, argv);
		exit(0);
		_exit(EXIT_FAILURE);


	} else {
		//Parent Process
		sleep(5);
		kill(pid, SIGINT);
		if(wait(&status)){
			printf("Child terminated");
		}
	}
}
