/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, Albert Fung 100520898, Jason Runzer 100520992, Nicholas Gregorio 100514374
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "utility.h"
#include "myshell.h"
#include <pwd.h>
#include <errno.h>
#include <dirent.h>
#include <termios.h>
#include <unistd.h>

// Put macros or constants here using #define
#define BUFFER_LEN 256

// Put global environment variables here
int userID;
char *userName;
char currentpath[BUFFER_LEN];
char oldPath[BUFFER_LEN];
char buffer[];
char command[BUFFER_LEN] = { 0 };
char arg[BUFFER_LEN] = { 0 };
int totalArguments = 0;
char **allArguments = NULL;
int n_spaces = 0;

// Define functions declared in myshell.h here


//tokenizes the arguments that the user inputs or a line from the file
void tokenize() {
	//Reinitialize arg
	totalArguments = 0;
	// Perform string tokenization to get the command and argument
	char* token = strtok(buffer, " \n");
	while(token){
		//allocate more room for the next command in the buffer
		allArguments = realloc(allArguments,sizeof(char*) * ++totalArguments);

		allArguments[totalArguments-1] = token; //add the argument to the array of arguments
		token = strtok(NULL, " \n"); //get the next token
	}

}

int main(int argc, char *argv[]) {
	// Input buffer and and commands



	//get the username of the current user
	userName = getlogin();

	//if executed in the terminal, the userName pointer will not be null
	if (userName == NULL)
		userName = "user";

	//executed if there is a file name as a command line argument
	if (argc > 1) {

		//open the command file
		FILE *fp;
		FILE *output;

		if ((fp = fopen(argv[1], "r")) == NULL) {
			printf("Error, file not found");
			exit(1);
		}

		//keep executing a command from the file until there are no commands left.
		while (fgets(buffer,BUFFER_LEN,fp) != NULL) {
			//tokenize the commands from the single line
			tokenize();
			execute_command(allArguments, totalArguments);
		}
		//Close files.
		fclose(fp);
		fclose(stdout);
		exit(0);

	}

	//prints the username and path of the user
	normal_display();

	// Parse the commands provided using argc and argv

	// Perform an infinite loop getting command input from users
	while (fgets(buffer, BUFFER_LEN, stdin) != NULL) {
		setenv("SHELL",
				"/Desktop/git_project/OperatingSystem/OperatingSystems/myshell ",
				1); //initalize SHELL environment variable
		tokenize(); //tokenize the input from the user
		execute_command(allArguments, totalArguments);

		//display the username and the path
		normal_display();

	}

	return EXIT_SUCCESS;

}

