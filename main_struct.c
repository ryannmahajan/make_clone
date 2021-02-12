#ifndef INFO_H
#define INFO_H

/*
This struct contains all of the targets. 
Each target array is like [[target] [dep1] [dep2]...] 
Each command is like[[command] [specificaion1] [s2] [s3]...]
*/
typedef struct Info {
    char** targets; //array of array of strings for target lines. 
    char** commands; //array of array of strings for comd line.
} Info;
#endif
