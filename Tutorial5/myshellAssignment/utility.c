/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, Albert Fung 100520898, Jason Runzer 100520992, Nicholas Gregorio 100514374
 * All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "utility.h"
#include "myshell.h"
#include <pwd.h>
#include <errno.h>
#include <dirent.h>
#include <termios.h>
#include <unistd.h>

// Define your utility functions here, these will most likely be functions that you call
// in your myshell.c source file

//produces the the current directory on a new line like a normal shell
void normal_display(void) {
	printf("%s:", userName);
	getcwd(currentpath, sizeof(currentpath));
	printf("%s$ ", currentpath);
}

//executed the given command with the given arguments
void execute_command(char** allArguments, int totalArguments) {
	// Check the command and execute the operations for each command
	// cd command -- change the current directory
	if (strcmp(allArguments[0], "cd") == 0) {
		//Get current working directory
		getcwd(currentpath, sizeof(currentpath));
		//If no argument is specified, print the current path
		if (strcmp(allArguments[1], "") == 0) {
			printf("%s\n", currentpath);
			//If argument specified, change directory
		} else {
			setenv("PWD", allArguments[1], 2); //Set environment variable to the argument
			//If the directory exists, change.
			if (chdir(getenv("PWD")) == 0) {
				printf("\nThe new directory is: %s\n", getenv("PWD"));

				//If the directory does not exist, don't change and display error
			} else {

				printf("Directory not changed. [%s]\n", strerror(errno));
			}
		}
	}					//END CD
	else if (strcmp(allArguments[0], "dir") == 0) {
		// Declare directory states
		DIR *dir;
		struct dirent *ent;
		int dirfound = 1; //Variable used in place of boolean

		//Store the current path into an array (used to prevent changing directory)

		getcwd(oldPath, sizeof(oldPath));

		//If there is an argument, change to the new directory
		if (strcmp(allArguments[1], "") != 0) {
			setenv("PWD", allArguments[1], 2); //Set the environment variable PWD to the argument
			if (chdir(getenv("PWD")) != 0) { //Change the directory to the new one, check if directory exists
				printf("Directory not found. [%s]", strerror(errno)); //If directory does not exist, show error
				dirfound = 0;
			}
		}

		if (dirfound == 1) { //If directory exists, continue
			getcwd(currentpath, sizeof(currentpath)); //Get the current working directory
			dir = opendir(currentpath); //Open the current working directory
			while ((ent = readdir(dir)) != NULL) { //Print the contents of the directory
				printf("%s\n", ent->d_name);
			}
			setenv("PWD", oldPath, 3); //Set the project working directory back to the old path
			chdir(getenv("PWD")); //Change directory
			closedir(dir); //Close directory variable.
		}
	}
	//prints out the message on the next line
	else if (strcmp(allArguments[0], "echo") == 0) {
		for (int i = 1; i < totalArguments; i++) {
			printf("%s ", allArguments[i]);
		}
		puts("");
	}

	//clears the console if executed in the terminal
	else if (strcmp(allArguments[0], "clr") == 0) {
		//clears the screen
		printf("\%c[2J", 033);
		//puts the cursor at the top left
		printf("\%c[0;0f", 033);

	} else if (strcmp(allArguments[0], "pause") == 0) {

		//taken from http://www.cyberciti.biz/tips/linux-unix-pause-command.html to replace the getch() method that is depreciated
		//disables echo from the keystroke presses
		char a[BUFFER_LEN];
		struct termios term, term_orig;
		tcgetattr(STDIN_FILENO, &term);
		term_orig = term;
		term.c_lflag &= ~ECHO;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);

		//clear the buffer from all the garbage the user entered until enter is pressed
		fflush(stdin);
		fgets(buffer, BUFFER_LEN, stdin);

		//re-enables echo from the keystroke presses
		tcsetattr(STDIN_FILENO, TCSANOW, &term_orig);

	} else if (strcmp(allArguments[0], "help") == 0) {
		//make a buffer
		char *buffer = malloc(sizeof(char) * 4096);
		//
		int ret;
		//stores the file to be read
		FILE *fp;
		//if file does not exist, print error message and quit the program
		if ((fp = fopen("readme", "r")) == NULL) {
			printf("Error, file not found");
			exit(1);
		}
		//while end of file is not true
		while (!feof(fp)) {
			//continues, delimits by new lines
			ret = fscanf(fp, "%4095[^\n]\n", buffer);
			//if it is not the end of the file, store the line inside the buffer
			if (ret != EOF) {
				fprintf(stdout, "%s\n", buffer);
			}
			//print a new line
			printf("\n");
		}
		//free the buffer
		free(buffer);
		//close the file
		fclose(fp);
	} else if (strcmp(allArguments[0], "environ") == 0) {
		//use the system call for printing the environment variables
		system("printenv");
	}
	// quit command -- exit the shell
	else if (strcmp(allArguments[0], "quit") == 0) {
		exit(0);
	}

	// program detected
	else {

		int i = 1;

		char programName[] = "./";
		strcat(programName, allArguments[0]);

		//store the locations of the standard inputs and outputs so we can restore back after redirecting.

		int standardOutput = 0;
		int redirectedOut = 0;
		int standardInput = dup(fileno(stdin)); //save the standardinput for the console to be used to restore after redirection

		int inputArgumentTotal = 0;
		int doneRedirection = 0;

		//check for input and output redirection
		while (i < totalArguments) {

			//check if there was input redirection
			if (strcmp(allArguments[i], "<") == 0) {
				//open the new stdin in read mode
				freopen(allArguments[i + 1], "r", stdin);
				//sets redirection to true, so the input arguments are not counted
				doneRedirection = 1;

			}
			//check if there was output redirection
			else if (strcmp(allArguments[i], ">") == 0) {
				//get the standard output inforamtion
				standardOutput = dup(fileno(stdout));

				//redirect the output
				freopen(allArguments[i + 1], "w", stdout);
				//printf("Truncate\n");
				doneRedirection = 1;
				redirectedOut = 1;
			} else if (strcmp(allArguments[i], ">>") == 0) {
				//same process as the > just with append value when redirection output
				standardOutput = dup(fileno(stdout));

				freopen(allArguments[i + 1], "a", stdout);
				//printf("Append\n");
				doneRedirection = 1;
				redirectedOut = 1;

			}
			i++;
			//counting the arguments to send into the system call
			if (doneRedirection == 0)
				inputArgumentTotal++;
		}

		//attach the arguments to the program name system call
		for (int j = 1; j < inputArgumentTotal + 1; j++) {

			strcat(programName, " ");
			strcat(programName, allArguments[j]);
		}

		//start the program
		system(programName);

		//restoring the standard in and out
		fflush(stdin);
		dup2(standardInput, fileno(stdin)); //restore the input to the console
		close(standardInput);
		clearerr(stdin);

		//restore output if the output was redirected
		if (redirectedOut) {
			fflush(stdout);
			dup2(standardOutput, fileno(stdout)); //restore the output to the console
			close(standardOutput);
			clearerr(stdout);
			freopen(NULL, "w", stdout); //reopen stdout in write mode

		}

	}
}
