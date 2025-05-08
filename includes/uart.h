#ifndef _UART_H_
#define _UART_H_

#define _GNU_SOURCE 

#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"


// __ DEFINITIONS ______________________________________________________________
#define BUF_LEN   256

#define UART_t struct uart_t
#define TERMIOS_t struct termios



// __ STRUCTURE DEFINITIONS ____________________________________________________
struct uart_t 
{
    int         fd; 
    size_t      buf_len;
    char*       buf;
    TERMIOS_t   oldtio;
    TERMIOS_t   newtio;
};

// __ FUNCTIONS DECLARATIONS ___________________________________________________
// * Open a serial port and set the UART_t structure according to the serial 
// * interface parameters.
// * @param: serial : pointer to an empty UART_t structure.  
// * @param: port   : the interface to open (ex: /dev/tty*).  
// * @param: buf_len: the size of the char buffer of the UART interface. 
// * @return: 1 in case of errors, 0 otherwise. 
int UART_connect(UART_t* serial, char* port, size_t buf_len);

// * Free the UART char buffer. 
// * @param: serial: the UART_t structure to free. 
void UART_free(UART_t* serial); 

// * Set every parameter of the tty. 
// * @param: serial: The UART_t interface to setup.
// * @param: speed : the baud rate speed we want to configure:
// *                 [4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 
// *                 921600]
// * @return: -1 in case of error, else 0.  
int UART_init(UART_t* serial, int speed);

// * Set the baud rate speed of a serial interface. 
// * @param: serial: the UART_t interface to set speed.
// * @param: speed : the baud rate speed we want to configure:
// *                 [4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 
// *                 921600]
// * return: -1 in case of errors, 0 otherwise. 
int UART_set_speed(UART_t* serial, int speed); 

// * Close the UART connection. 
// * @param: serial: the serial connection to close. 
void UART_close(UART_t* serial); 

// * Retrieve the serial output and save it into the UART_t char buffer. 
// * @param: serial: the serial interface where we will read. 
// * @return: -1 in case of errors, else the number of bytes readed. 
int UART_recv(UART_t* serial); 

// * Print the UART_t buffer to the stdout. 
// * @param: serial: the serial buffer that we will print. 
void UART_print(UART_t* serial, int bytes_readed); 

// * Write to the UART interface. 
// * @param: serial: The UART_t interface to write on. 
// * @param: str   : the string to write. 
// * @return: -1 if there is an error, else the bytes written.  
int UART_write(UART_t* serial, char* str); 

 
#endif