#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MEMORY 1024

typedef struct{
	char name[256];
	int priority;
	int pid;
	int address;
	int memory;
	int runtime;
	bool suspended;
} proc;

int MAX_BUFF_SIZE = 256;

typedef struct node{
	proc process;
	struct node *next;
} node_t;


proc tempProc;
int avail_mem [MEMORY];




//adds a process to the end of the list
void push (proc process, node_t  * queue){
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
proc * pop(node_t * queue){
	node_t * next_node = NULL;
	if(queue -> next == NULL){
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
		printf("Name: %s\nPriority: %d\npid: %d\nMemory: %d\nRuntime: %d\n\n",
				(pro -> name),pro ->priority,pro ->pid,pro->memory,pro->runtime);
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
//allocates memory for the current process. returns true if successful, false otherwise
bool allocateMemory(int *memory,int size){
	printf("Trying to allocate memory for : %s\n",tempProc.name);
	sleep(1); // used to delay the output so the prints for each process when running looks normal
	int allocated = 0;//the number of allocated spots possible
	int start = 0;//the starting position of a valid address for a proc
	/*
	for(int i = 0; i < size; i++){
		printf("%d",memory[i]);
	}
	*/
	
	//go through the size of the array and check each number for unallocated spaces
	for(int i = 0; i < size; i++){
		if(memory[i] == 1){
			allocated = 0;
			start = i + 1; //reset the start address
			
		}
		else{
			allocated++;
			
		}
		//check if the total space is free for the proc
		if(allocated == tempProc.memory){
			tempProc.address = start; //set the address of the proc
			
			//allocate the memory for the proc
			for(int k = 0; k < tempProc.memory; k++){
				memory[k+tempProc.address] = 1;
				
			}
			return true;
		}


	}
	return false;
}

//deallocated the memory that is specified for the current process
void deallocateMem(int *memory){
	printf("deallocating memory for : %s\n",tempProc.name);
	for(int i = 0; i < tempProc.memory;i++){
		memory[i+tempProc.address] = 0;
	}
	
	
	
}
int main(int argc, char *argv[]){
	
	//make the two queues
	node_t * priority = NULL;
	node_t * secondary = NULL;
	

	//allocate memory for the queue
	priority = malloc(sizeof(node_t));
	priority->next = NULL;
	secondary = malloc(sizeof(node_t));
	secondary->next = NULL;


	
	for(int i = 0; i < MEMORY; i++){
		avail_mem[i] = 0;
	}

		//open the file to get the processes
	FILE *fp = fopen("processes_q5.txt","r");

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
		tempProc.memory = atoi(tokens);
		tokens = strtok(NULL,",\n");
		tempProc.runtime = atoi(tokens);
		tempProc.pid = 0;
		tempProc.address = 0;

		if(tempProc.priority == 0)
			push(tempProc,priority); //push the process to the list
		else
			push(tempProc,secondary);//push to the seconday queue since it is not real time
	}
	fclose(fp);

		
	printf("POP PRIORITY 0\n");
	while(priority -> next != NULL){
		tempProc = *pop(priority);
		
		const char *command = tempProc.name;
		int status;
		pid_t pid;
		//fork the child
		pid = fork();
		if (pid == 0) { //CHILD
			//execute command
			
			tempProc.pid= getpid();
			tempProc.runtime--;
			allocateMemory(avail_mem,MEMORY); // allocate the memory for the process
			printProc(&tempProc);
			execv("./process", argv);		
			exit(0);
		} else if (pid < 0) { //FORK FAILED
			status = -1;
		} else { //PARENT
			//sleep for the runtime after exec, then kill the proc with SIGINT
			sleep(tempProc.runtime);

			//send SIGINT to suspend the proc
			kill(pid, SIGTSTP);
			kill(pid,SIGINT);
			//wait for child to terminate
			waitpid(pid, &status, 0);
			deallocateMem(avail_mem); //deallocate the memory since it is done
		}										
	}
	
	printf("POP SECONDARY\n");
	while(secondary -> next != NULL){
		
		
		const char *command = tempProc.name;
		int status;
		pid_t pid;
	
		bool mem = true;
		
		do{
			mem = true;
			tempProc = *pop(secondary);//get the next process
			if(!tempProc.suspended){ //check if it was suspended or not
				mem = allocateMemory(avail_mem,MEMORY);//allocate memory if it was not run before
				if (mem == false){
					printf("NOT ENOUGH MEMORY FOR PROCESS: %s\n",tempProc.name);
					//push the process to the end of the list
					push(tempProc,secondary);
				}
			}
		}while(!mem && !tempProc.suspended);
		
		
		if(tempProc.suspended == false){//check if it was suspended or not
			pid = fork();//fork the child
			if (pid == 0) { //CHILD
				//execute command
				
				execv("./process", argv);		
				exit(0);
			} else if (pid < 0) { //FORK FAILED
				status = -1;
			} else { //PARENT
				
				
				tempProc.pid = pid; //set the pid to the process
				printProc(&tempProc);
				tempProc.runtime--;//decrease the runtime of the process
				tempProc.suspended = true;
				sleep(1);
				kill(pid, SIGTSTP); //suspend the process
				push(tempProc,secondary);
				
			}
		}else{
			
			printProc(&tempProc);
			kill(tempProc.pid,SIGCONT);//signal the process to continue
			sleep(1);
			kill(tempProc.pid,SIGTSTP);//tell the process to suspend
			
			if(tempProc.runtime <= 1){ //kill the process if it finished
				kill(tempProc.pid,SIGINT);
				waitpid(tempProc.pid, &status, 0);
				deallocateMem(&avail_mem);//deallocate the used memory
			}
			else{
				tempProc.runtime--;
				push(tempProc,secondary);//if the process is not finished, add it to the end of the queue.
			}
			
		}								
	}
	free(secondary);
	free(priority);
	free(line);
	return 0;
}