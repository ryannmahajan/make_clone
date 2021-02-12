/*
File: main_helper.c
Authors: Ryann Mahajan, Tejvir Mann
Others: main.c main_helper.c main_helper.h parse.c parse.h procs.c procs.h graph.h graph.c
About: This module is used to hold helper methods used by main and other modules.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "m_struct.h"


//get makefile
int make_check(){
	FILE *f = fopen("makefile", "r");
	if (NULL == f){
		FILE *f = fopen("Makefile", "r");
		if(NULL == f){
			perror("Error: No Makefile found");
			return -1; //if no makefile
		}
		fclose(f);
		return 2; //if "Makefile"
	}
	fclose(f);
	return 1; //if "makefile"
}

//get length of makefile, in order to get a number > the # of targets
int make_length(char* filename){
	int len = 0;
	char c;
	FILE *f;
	f = fopen(filename, "r");
    for (c = getc(f); c != EOF; c = getc(f)){
        if (c == '\n') // Increment count if this character is newline 
            len = len + 1; 
    }
    fclose(f);  //close
    return len;
}


//root is the root of the makefile. length is the total length of the makefile
Info *dec_struct(char* root, int length, char* filename){
	const int size = 4096;
	Info *info = malloc(sizeof(Info));
	info->target_index = -1;//used to be 0
	info->cmd_index = 0;
	
	
	//target set
	info->targets = malloc(length*sizeof(char*));
	for(int i = 0; i < length; i++){
		info->targets[i] = malloc(size*sizeof(char*));
	}

	//command set
	info->commands = malloc(length*sizeof(char*));
	for(int j = 0; j < length; j++){
		info->commands[j] = malloc(length*sizeof(char*));
		for(int k = 0; k < length; k++){
			info->commands[j][k] = malloc(size*sizeof(char*));
		}
	}
	info->lines = length;
	//info->root = root;
	info->filename = filename;

	return info;
}

int free_struct(int length, Info* info){ //need to check free
	//const int size = 4096;
	int i = 0;
	int j = 0;
	int k = 0;

	while((strcmp(info->targets[i][0],"ryann"))){
		while((strcmp(info->commands[i][j][0],"ryann"))){
			while((strcmp(info->commands[i][j][k],"ryann"))){
				free(info->commands[i][j][k]);
				k++;
			}
			free(info->commands[i][j][k]);
			free(info->commands[i][j]);
			j++;
		}
		free(info->commands[i][j][0]);
		free(info->commands[i]);
		i++;
	}
	free(info->commands);
	
 	i = 0; //reset
 	j = 0;

 	//free target array
 	while((strcmp(info->targets[i][0],"ryann"))){
		j = 0;
 		while((strcmp(info->targets[i][j],"ryann"))){
 			free(info->targets[i][j]);
 			j++;
 		}
 		free(info->targets[i][j]);
 		free(info->targets[i]);
 		i++;
 	}
 	free(info->targets[i][0]);
	free(info->targets[i]);
	free(info->targets);

	return 1;
}

int line_num(char* s, Info* info){
	//printf("in line_num\n");
	int line = 1;
	int r;
	char *temp = NULL;
	size_t size = 4096;
	char* ret; 
	FILE *f = fopen(info->filename, "r"); //open file
	if(!f){
		printf("Error: file didn't open\n");
		return -1;
	}

	while(1){
		r = getline(&temp, &size, f);
		if(r == EOF){ //till end 
			break;
		}
		ret = strstr(temp, s); //if not in line, returns null pointer.
		if(NULL != ret){
			return line;
		}
		line++;
	}
    free(temp);
	return -1; //if not in file
}

// tested
// retrun 1 if name is a valid file name
int is_file(char* name){
	//printf("%s\n",name);
        FILE *f = fopen(name, "r");
        if(NULL == f){
        	return 0;
        }
        //return (f!=NULL);
        else{
        	return 1;
        }
}

// tested
int len(char** arr){
	//printf("in len\n");
	int length = 0;
	while(strcmp(arr[length],"ryann")) length++;
	return length;
}

// tested
// returns -1 if not found 
int find_index_in_makefile(Info* info, char* name){
	//printf("in find index in make\n");
    int length = 0;
	while(len(info->targets[length])!=0) length++;

	for (int result =0; result < length; result++){
		if (strcmp(name, info->targets[result][0])==0) return result;
	}

	return -1;
}

// tested
// return 1 if true, 0 if false
int is_in_makefile(Info* info, char* name) {
	//printf("in is in makefile\n");
    return (find_index_in_makefile(info, name)!=-1);
}

//tested
//if target exists, return the possibly empty dep-list. if target doesn't exist, return null
char** get_dependencies(char* target_name, Info* info){
	//printf("in get dependencies\n");
	int index_of_target_in_makefile = find_index_in_makefile(info, target_name);
	printf("a\n");
	if (index_of_target_in_makefile!=-1) {
		printf("b\n");
		return info->targets[index_of_target_in_makefile] + 1;
	} 
	else if (is_file(target_name)){
		char** s= malloc(sizeof(char*));
		s[0] = malloc(5*sizeof(char));
		strcpy(s[0], "ryann");
		printf("c\n");
		return s;
	} 
	else {
		//printf("d\n");
		char error[strlen(target_name)+500];
		sprintf(error, "Error: Could not find %s and resolve dependencies for it.\n", target_name);
		exit(-1);
	}
}

//just for testing, returns order of targets to be processed
//len is length of file
char** make_graph(Info* info, int len){
	//[procs.o, main_helper.o, parse.o, main.o, 537test]
	 char** order= malloc(10*sizeof(char*)); 
	 order[0]= malloc(20*sizeof(char));
	 order[0] = "target";
	 order[1]= malloc(20*sizeof(char));
	 order[1] = "ryann";
	 // order[2]= malloc(20*sizeof(char));
	 // order[2] = "parse.o";
	 // order[3]= malloc(20*sizeof(char));
	 // order[3] = "main.o";
	 // order[4]= malloc(20*sizeof(char));
	 // order[4] = "537test";
	 // order[5]= malloc(20*sizeof(char));
	 // order[5] = "ryann";

	 return order;

}








