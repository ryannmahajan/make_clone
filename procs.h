/*
File: procs.h
Authors: Ryann Mahajan, Tejvir Mann
Others: main.c main_helper.c main_helper.h parse.c parse.h procs.c procs.h graph.h graph.c
About: This module is used to hold procs method headers to help modulate the code.
*/

#include "m_struct.h"

//to run a process and execute 
int run_proc(int cmd_id, Info* info);

//to run processes
int proc_check(char** order, Info* info);

//to get the time
int modification_time(char* target_name);

//to get the time
time_t get_time(char* target_name);

//to check if the target is out of date
int out_of_date(char* target_name, Info* info);

