#ifndef INFO_H
#define INFO_H

/*

File: m_struct.h
Authors: Ryann Mahajan, Tejvir Mann
Others: main.c main_helper.c main_helper.h parse.c parse.h procs.c procs.h graph.h graph.c
About: This module is used create the struct used by other modules

This struct contains all of the targets. 
Each target array is like [[target] [dep1] [dep2]...] 
Each command is like[[command] [specificaion1] [s2] [s3]...]
*/
typedef struct Info {
    int target_index;
    int cmd_index;
    char*** targets; //array of array of strings for target lines. 
    char**** commands; //array of array of strings for comd line.
    int lines; //gets the total number of lines in makefile.
    char* root; //if root not given, then this will be 'ryann'. Else, the root will be here.
    char* filename;
} Info;
#endif