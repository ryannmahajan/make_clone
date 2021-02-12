/*
File: parse.h
Authors: Ryann Mahajan, Tejvir Mann
Others: main.c main_helper.c main_helper.h parse.c parse.h procs.c procs.h graph.h graph.c
About: This module is used to hold the parse method headers to modulate the code.
*/

#include "m_struct.h"

//main parser method
int parser(char* filename, Info* info);

//parse the target tine
int parsetar(char* target, Info* info, int line_num);

//parse the command time
int parsecom(char* com, Info* info, int line_num);