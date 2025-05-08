#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>

// __ DEFINITIONS ______________________________________________________________
#define CON_t struct console_t
#define TERMIOS_t struct termios


// __ STRUCTURE DEFINITIONS ____________________________________________________
struct console_t
{
    char        stdin_buf[3]; 
    TERMIOS_t   oldtio; 
    TERMIOS_t   newtio; 
}; 


// __ FUNCTIONS DECLARATIONS ___________________________________________________
// * Initialize the user console. 
// * @param: console: CONSOLE_t structure that need to be initialized. 
// * @return: -1 in case of errors, 0 otherwise. 
int CON_init(CON_t* console); 

// * Read the user input and check if for special input like ESCAPE. 
// * @param: console: CONSOLE_t struct that contains the stdin buffer. 
// * return: -1 if an end control sequence has been detected, bytes readed 
// *         otherwise. 
int CON_read(CON_t* console);

// * Restore the default configuration. 
// * @param: console: CONSOLE_t structure that contains the default config. 
// * @return: -1 in case of errors, 0 otherwise. 
int CON_restore(CON_t* console);

#endif 