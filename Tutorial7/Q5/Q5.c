#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define SHELL "/bin/sh"

typedef struct {
	char name[256];
	int priority;
	int pid;
	int runtime;
} proc;

int MAX_BUFF_SIZE = 256;

typedef struct node {
	proc process;
	struct node *next;
} node_t;

node_t *queue;
proc tempProc;

//the methods below are taken from Q1 and Q2

void push(proc process) {
	node_t *current = queue; //set the current node to the first one in the queue
	//traverse through the linked list until it finds the next node = to NULL
	while (current->next != NULL) {
		current = current->next;
	}
	//allocate and and the process to the list
	current->next = (node_t *) malloc(sizeof(node_t));
	current->next->process = process;
	current->next->next = NULL; //set the next node to NULL to specify the end of the list
}

proc * delete_name(char *name) {
	node_t *current = queue->next;
	node_t *previous = queue;
	while (current != NULL) {

		tempProc = current->process;
		if (strcmp(tempProc.name, name) == 0) {
			node_t *deletedNode = current;
			previous->next = current->next;
			free(deletedNode);
			return &tempProc;
		}
		previous = current;
		current = current->next;
	}
	printf("process not found\n");
	return NULL;
}

//pops the first node in the list and returns a pointer to the process that was deleted
proc * pop() {
	node_t * next_node = NULL;
	if (queue->next == NULL) {
		printf("in NULL");

		return NULL;
	}

	next_node = queue->next->next; // get the next node after the sentinal
	node_t * currentNode = queue->next;
	tempProc = currentNode->process;

	//strcpy(tempProc.name,currentNode -> process.name);

	free(currentNode);
	queue->next = next_node;

	return &tempProc;
}

//prints the process that is given
void printProc(proc *pro) {
	if (pro != NULL) {
		printf("Name: %s\nPriority: %d\npid: %d\nRuntime: %d\n\n", (pro->name),
				pro->priority, pro->pid, pro->runtime);
	}
}

//prints the contents of the list given
void printList(node_t *list) {
	node_t * current = list;
	//get rid of the sentinel
	current = current->next;
	while (current != NULL) {
		tempProc = current->process;
		printProc(&tempProc); //print the current process
		current = current->next; //advance the node
	}
}

int main(int argc, char *argv[]) {
	//initalize the first node with null
	queue = NULL;
	//allocate memory for the linkedlist
	queue = malloc(sizeof(node_t));
	queue->next = NULL;
	//open the file to get the processes

	proc tempProcess;

	///////////////FILE STRUCTURE////////////////
	//NAME, PRIORITY, PID(0), RUNTIME IN SECONDS
	/////////////////////////////////////////////

	FILE *fp = fopen("processes_q5.txt", "r");

	if (!fp) {
		printf("Error opening file");
		exit(EXIT_FAILURE);
	}

	//the line for each process
	size_t len = 0;
	ssize_t read;
	char *line = NULL;

	//go though the text file and add each process to the list
	while ((read = getline(&line, &len, fp)) != -1) {
		//tokenize the line with , to get each entity of the process
		char * tokens = NULL;
		tokens = strtok(line, ",\n");
		strcpy(tempProcess.name, tokens);
		tokens = strtok(NULL, ", \n");
		tempProcess.priority = atoi(tokens); //atoi casts a char * to an integer
		tokens = strtok(NULL, ", \n");
		tempProcess.pid = 0;
		tempProcess.runtime = atoi(tokens);
		push(tempProcess); //push the process to the list
	}
	//close the file
	fclose(fp);
	node_t * current = queue;
	//get rid of the sentinel node
	current = current->next;
	while (current != NULL) {
		tempProcess = current->process; //advance the node
		if (tempProcess.priority == 0) { //priority zero queue
			//command to execute
			const char *command = tempProcess.name;
			int status;
			pid_t pid;
			//fork the child
			pid = fork();
			if (pid == 0) { //CHILD
				//execute command
				execv(command, argv);
				//remove from process struc
				delete_name(tempProcess.name);
				//execute the process binary
				execv("./process", argv);
				exit(0);
			} else if (pid < 0) { //FORK FAILED
				status = -1;
			} else { //PARENT
				//sleep for the runtime after exec, then kill the proc with SIGINT
				sleep(tempProcess.runtime);
				//send SIGINT to suspend the proc
				kill(pid, SIGINT);
				//wait for child to terminate
				waitpid(pid, &status, 0);
				if (status == 0) { //if child terminates normally
					//set pid returned from exec()
					tempProcess.pid = pid;
					printProc(&tempProcess);
					//iterate to next item in the linkedlist
					current = current->next;
				}
			}
		} else { //rest of the queue
			//command to execute
			const char *command = tempProcess.name;
			int status;
			pid_t pid;
			//fork the child
			pid = fork();
			if (pid == 0) { //CHILD
				//execute command
				execv(command, argv);
				exit(0);
			} else if (pid < 0) { //FORK FAILED
				status = -1;
			} else { //PARENT
				//sleep for the runtime after exec, then kill the proc with SIGINT
				sleep(tempProcess.runtime);
				//send SIGINT to suspend the proc
				kill(pid, SIGINT);
				//wait for child to terminate
				waitpid(pid, &status, 0);
				if (status == 0) { //if child terminates normally
					//set pid returned from exec()
					tempProcess.pid = pid;
					//prints name, priority, pid and runtime of process
					printProc(&tempProcess);
					//iterate to next item in the linkedlist
					current = current->next;
					pop(); //pops the remaining process off the queue
				}
			}
		}
	}
	/*
	printf("Printing out rest of queue: \n");
	printList(queue);
	*/
	return 0;
}
