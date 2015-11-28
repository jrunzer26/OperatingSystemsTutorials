
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

//Structs used to represent processes and nodes within the tree. Each node will contain data, as well as
//a right and left subtree, needed for traversals of the tree.
typedef struct proc {
	char parent[256];
	char name[256];
	int priority;
	int memory;
} proc;
proc processes;


typedef struct node {
	proc process;
	struct node* left;
	struct node* right;
} node_t;

node_t *parentNode;
node_t *tree = NULL;
node_t *temp = NULL;

/**
 * Inorder traversals print the left most child, the parent of that, and then the right most child
 * This pattern continues upwards.
 */
void print_inorder(node_t * tree)
{
    if (tree)
    {
        print_inorder(tree->left);
        printf("%s\n",tree->process.name);
        print_inorder(tree->right);
    }
}

/**
 * Because each process has a specified parent, we need to search for that parent, and return the node
 * to determine where to place each child
 */
void search(node_t ** tree, proc searchprocess) {
		if(!(*tree)) {
	        return;
	    }

		if(strcmp(searchprocess.parent, (*tree)->process.name) == 0) {
			parentNode = *tree;
		}
		search(&(*tree)->left, searchprocess);
		search(&(*tree)->right, searchprocess);

}

void add_node(node_t ** tree, proc newprocess) {



	if(!(*tree))
	    {
	        temp = (node_t *)malloc(sizeof(node_t));
	        temp->left = temp->right = NULL;
	        temp->process = newprocess;
	        *tree = temp;

	        return;
	    }
	search(&(*tree), newprocess);

	if (parentNode->left == NULL) {
		add_node(&parentNode->left, newprocess);

	}
	else if (parentNode->right == NULL) {
		add_node(&parentNode->right, newprocess);
	}


}

int main(void){
	parentNode = (node_t *) malloc(sizeof(node_t *));

	FILE *input = fopen("processes_tree.txt", "r");
	if (!input) {
		printf("error");
	}


	//the line for each process
		size_t len = 0;
		ssize_t read;
		char *line = NULL;


		//go though the text file and add each process to the list
		while ((read = getline(&line, &len, input)) != -1) {
			//tokenize the line with , to get each entity of the process
			char * tokens = NULL;
			tokens = strtok(line,", \n");
			strcpy(processes.parent,tokens);
			tokens = strtok(NULL,", \n");
			strcpy(processes.name, tokens);
			tokens = strtok(NULL,", \n");
			processes.priority = atoi(tokens); //atoi casts a char * to an integer
			tokens = strtok(NULL,", \n");
			processes.memory = atoi(tokens);
			add_node(&tree, processes);
		}

		print_inorder(tree);
}
