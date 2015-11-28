#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


typedef struct{
	char name[256];
	int priority;
	int pid;
	int runtime;
} proc;

int MAX_BUFF_SIZE = 256;

typedef struct node{
	proc process;
	struct node *next;
} node_t;


proc tempProc;

node_t *queue; 

//adds a process to the end of the list
void push (proc process){
	node_t *current = queue; //set the current node to the first one in the queue
	
	//traverse through the linked list until it finds the next node = to NULL
	while(current -> next != NULL){
		current = current -> next;
	}
	//allocate and and the process to the list
	current -> next =  (node_t *) malloc(sizeof(node_t));
	current -> next-> process = process;
	current-> next-> next = NULL;  //set the next node to NULL to specify the end of the list
}

//pops the first node in the list and returns a pointer to the process that was deleted
proc * pop(){
	node_t * next_node = NULL;
	if(queue -> next == NULL){
		printf("in NULL");

		return NULL;
	}
	
	next_node = queue -> next -> next; // get the next node after the sentinal
	node_t * currentNode = queue -> next;
 	tempProc = currentNode-> process;
 	
 	//strcpy(tempProc.name,currentNode -> process.name);
	
 	free(currentNode);
	queue -> next = next_node;

	return &tempProc;
	


}


//prints the process that is given
void printProc(proc *pro){

	if(pro != NULL){
		printf("Name: %s\nPriority: %d\npid: %d\nRuntime: %d\n\n",
				(pro -> name),pro ->priority,pro ->pid,pro ->runtime);
	}
}

/* Prints the list processes that is given.*/
void printList(node_t *list){
	node_t * current = list;
	//get rid of the sentinal
	current = current -> next;

	while(current != NULL){
		
		tempProc = current ->process;
		printProc(&tempProc); //print the current process
		current = current -> next; //advance the node
	}

}


//deletes the node with the name specified, If not there, returns null
proc * delete_name(char *name){
	node_t *current = queue -> next;
	node_t *previous = queue;
	while(current != NULL){
	
		//check the current process' name
		tempProc = current -> process;
		if(strcmp(tempProc.name,name) == 0){
			node_t *deletedNode = current;
			previous -> next = current -> next;
			free(deletedNode); //free up the memory of the node
			return &tempProc;
		}
		//get the next node in the list
		previous = current;
		current = current -> next;
	}
	printf("process not found\n");
	return NULL;
}

//deletes the node with the pid specified, If not there, returns null
proc * delete_pid(int pid){
	node_t *current = queue -> next;
	node_t *previous = queue;
	while(current != NULL){
		//check the current process' pid
		tempProc = current -> process;
		if(tempProc.pid == pid){
			node_t *deletedNode = current;
			previous -> next = current -> next;
			free(deletedNode); //free up the deleted nodes memory
			return &tempProc; //return the process that was deleted
		}
		//get the next item in the list
		previous = current;
		current = current -> next;
	}
	printf("process not found\n");
	return NULL;
}

int main(void)
{
	//initalize the first node with null
	queue = NULL;
	

	//allocate memory for the 
	queue = malloc(sizeof(node_t));
	queue->next = NULL;


	//open the file to get the processes
	FILE *fp = fopen("processes.txt","r");

	if(!fp){
		printf("Error opening file");
	}

	//the line for each process

	size_t len = 0;
	ssize_t read;
	char *line = NULL;

	//go though the text file and add each process to the list
	while ((read = getline(&line, &len, fp)) != -1) {
		//tokenize the line with , to get each entity of the process
		char * tokens = NULL;
		tokens = strtok(line,",\n");
		strcpy(tempProc.name,tokens);
		tokens = strtok(NULL,",\n");
		tempProc.priority = atoi(tokens); //atoi casts a char * to an integer
		tokens = strtok(NULL,",\n");
		tempProc.pid = atoi(tokens);
		tokens = strtok(NULL,",\n");
		tempProc.runtime = atoi(tokens);
		push(tempProc); //push the process to the list
	}
	fclose(fp);
	

	
	//traverse through and display each item in the linked list.
	printList(queue);

	printf("\n\n\n");

	//delete the processes
	delete_name("emacs");
	delete_pid(12235);

	printf("POP\n");
	while(queue -> next != NULL){
		printProc(pop());														
	}


	free(line);
	return 0;

}