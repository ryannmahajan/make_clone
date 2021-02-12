/*
File: procs.c
Authors: Ryann Mahajan, Tejvir Mann
Others: main.c main_helper.c main_helper.h parse.c parse.h procs.c procs.h graph.h graph.c
About: This module is used to hold helper methods used by main and other modules.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "parse.h"
#include "m_struct.h"
#include "main_helper.h"

//this method runs each command for each target
int run_proc(int cmd_id, Info* info){
	int i = 0;
	int j = 0;

	while(strcmp(info->commands[cmd_id][i][0], "ryann")){
		while(strcmp(info->commands[cmd_id][i][j], "ryann")){ //fix, need to make 
			j++;
		}

		char** copy = info->commands[cmd_id][i]; //set null copy
		copy[j] = NULL;
		
		pid_t pid; 
		int status;

		pid = fork(); 
		if(pid < 0) {     
        	printf("ERROR: forking child process failed\n");
        	return -1;
    	}
		if(pid==0){
        	if (execvp(copy[0], copy) < 0){     
            	printf("ERROR: Invalid command or incorrect inputs/files.\n");
            	return -1;
        	}
		}

		while (wait(&status) != pid){  //this tells us the succ of child, catches child
		}

		if(WIFEXITED(status) == 1){ //checks child and if errors occured in execvp
			if(WEXITSTATUS(status) == 0){} //success
			else{
				return -1; //error: missing file or something
			}
		}
		else{
			return -1; //killed by signal. Problem when executing
		}

		info->commands[cmd_id][i][j] = malloc(6*sizeof(char));
		char* ryann = malloc(6*sizeof(char));
		ryann = "ryann";
		strcpy(info->commands[cmd_id][i][j],ryann); //copy ryann back, not sure if needed tho
		i++;
		j=0;
	}

	return 1;	
}

//gets time last modified
time_t modification_time(char* target_name) {
	struct stat filestat;
	stat(target_name, &filestat);
	time_t time;
	time = (filestat.st_mtime);
	printf("%ld\n", time);

	return time;
}

//gets time last modified if it is a file, else 0
int get_time(char* target_name){
	if (is_file(target_name)) return modification_time(target_name);
	else return 0;
}

// return 1 if the target is out of date, 0 if not
int out_of_date(char* target_name, Info* info){
	time_t time = get_time(target_name);
	char** deps = get_dependencies(target_name, info);
	int length = 0;
	while(strcmp(deps[length], "ryann")!=0) length++;

	if (0==length) return 1; // always out-of-date when no dependencies
	for (int i=0; i < length; i++) {
		if (is_file(deps[i]) && difftime(time,get_time(deps[i])) > 0) return 1;
		else if (is_in_makefile(info, deps[i]) && out_of_date(deps[i], info)) return 1;

	}
	return 0;	
}

/*
//check if the command needs to be run:
	//prolly through a while loop
		//so if the target does not exist (time is 0)
		//pulls the target file name from the struct.
		//if the target fstat time is less than any or its dependencies. Meaning it needs to be updated. 
		//run_proc(order[i]);
		//check if what the parent returned was valid.
*/
int proc_check(char** order, Info* info){
	int length = 0;
	while(strcmp(order[length], "ryann")){
		if (out_of_date(order[length], info)){
			int c = run_proc(find_index_in_makefile(info, order[length]), info);
			if(c == -1){
				return -1;
			}
		}
		length++;
	}
	return 1; 
}
