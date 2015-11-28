/*
 * Q4.c
 *
 *  Created on: Nov 4, 2015
 *      Author: god
 */

//includes
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
	//command to execute
	const char *command = "./process";
	int status;
	pid_t pid;

	//fork the child
	pid = fork();
	if (pid == 0) { //CHILD
		//execute command
		execv(command, argv);
		exit(0);
		_exit(EXIT_FAILURE);
	} else if (pid < 0) {
		//FORK FAILED
		status = -1;
	} else { //PARENT
		//sleep for 5 seconds
		sleep(5);
		//send SIGTSTP to suspend the proc
		kill(pid, SIGTSTP);
		//sleep for 10 seconds
		sleep(10);
		//send SIGCONT to resume the proc
		kill(pid, SIGCONT);
		//wait for child to terminate
		waitpid(pid, &status, 0);
		//if child terminates normally
		if (status == 0) {
			exit(0);
		}
		return status;
	}
}
