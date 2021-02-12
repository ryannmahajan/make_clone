/*
File: parse.c
Authors: Ryann Mahajan, Tejvir Mann
Others: main.c main_helper.c main_helper.h parse.c parse.h procs.c procs.h graph.h graph.c
About: This module is responsible for parsing the makefile line by line. char by char.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <dirent.h>
#include "m_struct.h"

/*
This method parses each target line of the makefile
*/
int parsetar(char* target, Info* info, int line_num){
	printf("%s\n", target);
	const int size = 4096;
	const int ry_size = 5;
	int total_colons = 0;
	int reading_letter = 0;
	int i = 0;
	int len;
	int string_index = 0;
	int letter_index = 0;
	char* ryann= "ryann";
	char* tar_temp;
	tar_temp = malloc(size*sizeof(char));
	
	if(target[0] == ' '){ //if first character of line is not part of target, error. 
		printf("%d: <Error: line incorrect syntax>: %s\n", line_num, target);
		return -1;
	}

	//index maintenance.
	info->target_index++; //increment target index.
	if(info->target_index >= 1){ //used to be 1
		info->commands[(info->target_index-1)][info->cmd_index][0] = malloc(ry_size*sizeof(char));  //info->target_index)-1
		strcpy(info->commands[(info->target_index-1)][info->cmd_index][0], ryann); //marker
		//printf("command ry:[%d][%d][%d]: %s\n", info->target_index-1, info->cmd_index, 0, info->commands[(info->target_index)-1][info->cmd_index][0]);
	}
	info->cmd_index = 0; //reset command index for new target

	//parse through string
	while(target[i] != '\n'){
		if(target[i] == '\t' || target[i] == ' '){
			if(reading_letter == 1){ //then just got done reading word
				tar_temp[letter_index] = '\0'; //add end to string
				len = strlen(tar_temp);
				info->targets[info->target_index][string_index] = malloc(len*sizeof(char));
				strcpy(info->targets[info->target_index][string_index], tar_temp);
				memset(tar_temp,0,strlen(tar_temp)); //clear temp string
				reading_letter = 0;
				string_index++;
				letter_index = 0;
			}
		}
		else{
			if(target[i] == ':'){ //check colon
				total_colons++;
				if(string_index != 0){ //then : not directly after target string.
					printf("%d: <Error: no colon after target, or 2nd colon in line>: %s\n", line_num, target);
					return -1;
				}
			}
			else if(target[i] == '\0'){ //check this
				if(i != strlen(target)){
					printf("%d: <Error: contains zero byte>: %s\n", line_num, target);
					return -1;
				}
			}
			else if(target[i] == '@'){ //check this
				if(i != strlen(target)){
					printf("%d: <Error: contains zero byte>: %s\n", line_num, target);
					return -1;
				}
			}
			else{
				reading_letter = 1;
				tar_temp[letter_index] = target[i];
				letter_index++;
			}
		}
		i++;
	}	

	printf("reading letter: %d\n", reading_letter);
	if(reading_letter == 1){ //then just got done reading word
		tar_temp[letter_index] = '\0'; //add end to string
		len = strlen(tar_temp);
		info->targets[info->target_index][string_index] = malloc(len*sizeof(char));
		strcpy(info->targets[info->target_index][string_index], tar_temp);
		memset(tar_temp,0,strlen(tar_temp)); //clear temp string
		reading_letter = 0;
		string_index++;
		letter_index = 0;
	}

	if(total_colons > 1){ //checks correct number of colons
		printf("%d: <Error: multiple colons>: %s\n", line_num, target);
		return -1;
	}
	if(total_colons == 0){
		printf("%d: <Error: No colon, so no target>: %s\n", line_num, target);
		return -1;
	}

	info->targets[info->target_index][string_index] = malloc(ry_size*sizeof(char)); //set end string
	strcpy(info->targets[info->target_index][string_index], ryann); //marker
	free(tar_temp);
	return 1;
}

/*
This method parses each command line of the makefile
*/
int parsecom(char* com, Info* info, int line_num){
	const int size = 4096;
	const int ry_size = 5;
	int i = 0;
	int reading_letter = 0;
	int len;
	int string_index = 0;
	int letter_index = 0;
	char* ryann= "ryann";
	char* com_temp;
	com_temp = malloc(size*sizeof(char));

	if(info->target_index == -1){
		printf("%d: <Error: No target before command>: %s\n", line_num, com);
		return -1;
	}

	while(com[i] != '\n'){
		if(com[i] == '\t' || com[i] == ' '){
			if(reading_letter == 1){ //then just got done reading word
				//add a '\0' at end of string vi letter index
				com_temp[letter_index] = '\0';
		
				len = strlen(com_temp);
				info->commands[info->target_index][info->cmd_index][string_index] = malloc(len*sizeof(char));

				strcpy(info->commands[info->target_index][info->cmd_index][string_index], com_temp);
				memset(com_temp,0,strlen(com_temp)); //clear temp string
				reading_letter = 0;
				string_index++;
				letter_index = 0;
			}
		}
		else{
			if(com[i] == '\0'){ //check null byte
				if(i != strlen(com)){
					printf("%d: <Error: contains zero byte>: %s\n", line_num, com);
					return -1;
				}
			}
			if(com[i] == '@'){ //check null byte
				if(i != strlen(com)){
					printf("%d: <Error: contains zero byte>: %s\n", line_num, com);
					return -1;
				}
			}
			reading_letter = 1;
			com_temp[letter_index] = com[i];
			letter_index++;
		}
		i++;
	}

	if(reading_letter == 1){ //then just got done reading word
		com_temp[letter_index] = '\0'; //add end to string
		len = strlen(com_temp);
		info->commands[info->target_index][info->cmd_index][string_index] = malloc(len*sizeof(char));
		strcpy(info->commands[info->target_index][info->cmd_index][string_index], com_temp);

		//printf("command:[%d][%d][%d]: %s\n", info->target_index, info->cmd_index, string_index, info->commands[info->target_index][info->cmd_index][string_index]);

		memset(com_temp,0,strlen(com_temp)); //clear temp string
		reading_letter = 0;
		string_index++;
		letter_index = 0;
	}
	info->commands[info->target_index][info->cmd_index][string_index] = malloc(ry_size*sizeof(char)); //set end string
	strcpy(info->commands[info->target_index][info->cmd_index][string_index], ryann);
	
	info->cmd_index++; //increment command number
	free(com_temp);
	return 1;

}

/*
*/
int parser(char* filename, Info* info){
	const int ry_size = 5; //size if ryanns name
	int line_num = 1; //this keeps track of line number
	int val = 0; //checking parsetar parsecom
	char *temp = NULL;
	int r;
	size_t size = 4096;
	int string_len;
    int len = 4095; 
	char* ryann= "ryann";

	FILE *f = fopen(filename, "r"); //open file
	if(!f){
		printf("Error: file didn't open\n");
		return -1;
	}

	while(1){
		r = getline(&temp, &size, f);
		if(r == EOF){ //till end 
			break;
		}
		string_len = strlen(temp);
		if(len < string_len){ //if line too long
			printf("%d: <Error: line too long>: %s\n", line_num, temp);
			return -1;
		}
		if(temp[0] == '#'){ //if its a comment line
			line_num++;
			continue;
		}
		if(temp[0] == '\n'){ //if empty line
			line_num++;
			continue;
		}
		if(string_len ==(strspn(temp, " \t")+1)){ //if string is only spaces or tabs
			line_num++;
			continue;
		}
		if(temp[0] == '\t'){ //tab
			val = parsecom(temp, info, line_num);
			if(-1 == val){
				return -1;
			}
		}
		else{
			val = parsetar(temp, info, line_num);
			if(-1 == val){
				return -1;
			}
		}

		line_num++;
	}
	
	//after last target add ryann marker
	info->target_index++; //increment for ryann
	info->targets[info->target_index][0] = malloc(ry_size*sizeof(char));
	strcpy(info->targets[info->target_index][0],ryann);

	//after last set of commands add ryann marker to it
	//for last target: 
	info->commands[(info->target_index)-1][info->cmd_index][0] = malloc(ry_size*sizeof(char));
	strcpy(info->commands[(info->target_index)-1][info->cmd_index][0], ryann);
	free(temp);

	return 1;
}


