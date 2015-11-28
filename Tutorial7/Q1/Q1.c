#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

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

int main(void) {
	//initalize the first node with null
	queue = NULL;
	//the temp process when pushing to the list
	//proc tempProcess;

	//allocate memory for the 
	queue = malloc(sizeof(node_t));
	queue->next = NULL;

	//open the file to get the processes
	FILE *fp = fopen("processes.txt", "r");

	if (!fp) {
		printf("Error opening file");
		exit(EXIT_FAILURE);
	}

	//the line for each process
	size_t len = 0;
	ssize_t read;
	char *line = NULL;

	//while ((read = getline(&line, &len, fp)) != -1) {

	//go though the text file and add each process to the list
	while ((read = getline(&line, &len, fp)) != -1) {
		//tokenize the line with , to get each entity of the process
		proc tempProcess;
		char * tokens = NULL;
		tokens = strtok(line, ",");
		strcpy(tempProcess.name, tokens);
		tokens = strtok(NULL, ",");
		tempProcess.priority = atoi(tokens); //atoi casts a char * to an integer
		tokens = strtok(NULL, ",");
		tempProcess.pid = atoi(tokens);
		tokens = strtok(NULL, ",");
		tempProcess.runtime = atoi(tokens);
		push(tempProcess); //push the process to the list
		
	}
	fclose(fp);

	//traverse through and display each item in the linked list.
	node_t * current = queue;
	//get rid of the sentinel node
	current = current->next;

	//traverse through the whole linked list
	while (current != NULL) {
		proc tempProcess = current->process;
		printf("Name: %s\nPriority: %d\npid: %d\nRuntime: %d\n\n",
				tempProcess.name, tempProcess.priority, tempProcess.pid,
				tempProcess.runtime);
		current = current->next; //advance the node
	}
	free(line);
	return 0;
	
}
