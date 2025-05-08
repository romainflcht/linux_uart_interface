#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>

// __ FUNCTIONS ________________________________________________________________
// * Count the number of char in a string. 
// * @param: str: the str to count. 
// * @return: the string length. 
int str_len(char* str); 

// * Print the usage message shown if not enough arguments were passed. 
void print_usage();

#endif