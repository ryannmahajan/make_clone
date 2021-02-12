/*

File: main.c
Authors: Ryann Mahajan, Tejvir Mann
Others: main.c main_helper.c main_helper.h parse.c parse.h procs.c procs.h graph.h graph.c
About: This method is the main controller and calls many of the other methods in order to run 
correctly manipulate the methods to complete the project.

Main: How it works

1. Parse through the makefile and get all the command and target lines. This happens in parse.c 
These strings are stored in a struct in double arrays targets and commands. The indexes for target and command should correspond. 
So command[1] should be for target[1].

2. In the graph.c file Take these string arrays from the struct. 
Put these lines into a graph.
Check if cyclic. 
We need to find the root node, unless its given. For example, they can give us read_proc.o as the root node. 
If we don't know the root node, ex. not a param given. 

3. Graph runs in a Post order traversal order, traversal done iteritively not recursively. Gets Order. Return order in
a int array to main. 

4. Using Order it calls a method and passes in the order. Now we are in procs.c
Compile the needed files, only files where 'out of date' or when target isn't a file yet. Each time something needs to be compiled, a method is called. Inside that method (which is in the compile mod) fork is called. child process does execvp, and 
we have to wait() for the results and return then with the parent process. 

Compile all that nereds to be compiled. 

Done.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "m_struct.h"
#include "main_helper.h"
#include "parse.h"
#include "procs.h"
#include "graph.h"

int main(int argc, char **argv) {

	const int make_len = 8;
	char* file_name = malloc(make_len*sizeof(char)); //store name of file
	char* root; //to store root name

	//get makefile
	int check = make_check();
	if(check == 1){
		file_name = "makefile";
	}
	else if(check == 2){
		file_name = "Makefile";
	}
	else{
		return -1; //no makefile
	}

	//get length of makefile, in order to get a number > the # of targets
	int len = make_length(file_name);
	if (0 == len){
		perror("Error: makefile empty!");
	}

	//check argc and argv
	if(0 == argc){
		perror("Error: no arguments\n");
		return -1;
	}


	//declare struct
	Info *info = dec_struct(len, file_name);
	if(NULL == info){
		perror("Error: Problem with struct declaration! Null Pointer.");
	}

	//parses whats in the file
	if ( -1 ==parser(file_name, info)){
		return -1;
	}

	//check root, set it in info->root
	if (NULL == argv[1]){
		info->root = malloc((root_len+1)*sizeof(char));
		strcpy(info->root, info->targets[0][0]);

		// int root_len = strlen("ryann");
		// root = malloc((root_len+1)*sizeof(char));
		// root = "ryann";
	}
	else{
		int root_len = strlen(argv[1]);
		info->root = malloc((root_len+1)*sizeof(char));
		//root = argv[1]; //where the root is stored in the cmdline
		strcpy(info->root, argv[1]);
	}


	// if (strcmp(info->root, "ryann")==0) {
	// 	// info-> root = info->target[0][0];
	// 	free(info->root);
	// 	info->root = malloc(strlen(info->targets[0][0])*sizeof(char));
	// 	strcpy(info->root, info->targets[0][0]);

	// 	free(root);
	// 	root = malloc(strlen(info->targets[0][0])*sizeof(char));
	// 	strcpy(root, info->targets[0][0]);
	// }

	printf("parse done\n");
	printf("check val of info\n");
	printf("%s\n", info->targets[0][0]);
	printf("%s\n", info->targets[0][1]);
	printf("%s\n", info->targets[1][0]);
	
	char** order;//make order array
	order = make_graph(info, info->root); //make_graph returns the order
	
	printf("proc start\n");
	int c = proc_check(order, info);
	printf("%d\n", c);
	if( c == -1){
		return -1;
	}
	printf("proc done\n");


   printf("free start\n");
	//free everything at end.
	free_struct(len, info);
	printf("free done\n");

	//free stuff
	printf("done\n");
}
