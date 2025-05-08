#include "utils.h"


// * Count the number of char in a string. 
// * @param: str: the str to count. 
// * @return: the string length. 
int str_len(char* str)
{
    int count; 

    count = 0; 
    while (str[count])
    {
        count++; 
    }

    return count; 
}


// * Print the usage message shown if not enough arguments were passed. 
void print_usage()
{
    puts("\x1b[1mUsage: uart <tty> <speed>\x1b[0m\r"); 
    puts("ex: uart /dev/ttyS0 9600\r"); 
}