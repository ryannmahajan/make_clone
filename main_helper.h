/*
File: main_helper.h
Authors: Ryann Mahajan, Tejvir Mann
Others: main.c main_helper.c main_helper.h parse.c parse.h procs.c procs.h graph.h graph.c
About: This is main_helper .h file to help modulate the code.
*/

#include "m_struct.h"

//to check if there is a makefile
int make_check();

//to get the length of file
int make_length(char* filename);

//to declare the structure
Info* dec_struct(int length, char* filename);

//to free structure
int free_struct(int length, Info* info);

//get line num of a target
int line_num(char* s, Info* info);

//to check if its a file given target
int is_file(char* name);

//to get len of a double array.
int len(char** arr);

//to find the index inside the makefile
int find_index_in_makefile(Info* info, char* name);

//to check if the target is in the makefile
int is_in_makefile(Info* info, char* name);

//to get the dependencies
char** get_dependencies(char* target_name, Info* info);

//to make the graph. should be deleted before turn in.
char** make_graph(Info* info, int len);