#include <stdio.h>

#include "utils.h"
#include "uart.h"
#include "console.h"


int main(int argc, char** argv)
{
    UART_t  serial;
    CON_t   console; 
    int     running; 
    int     retval; 

    if (argc < 3)
    {
        print_usage(); 
        return 1; 
    }

    // Initialise the user console. 
    retval = CON_init(&console); 
    if (retval < 0)
        return 1; 

    // Initialise the Serial interface and connect to it. 
    retval = UART_connect(&serial, argv[1], BUF_LEN); 
    if (retval < 0)
        return 1; 


    // Set every settings needed for the serial communication. 
    retval = UART_init(&serial, atol(argv[2]));
    if (retval < 0) 
    {
        // Restore everything in case of error. 
        CON_restore(&console); 
        UART_close(&serial); 
        return 1; 
    }


    puts("\x1b[1;32m~[SUCCESS] Connected to the serial interface!\x1b[0m\r"); 
    puts("\x1b[2m~[INFO] To quit the program, press the <ESC> key...\x1b[0m\r"); 
    UART_write(&serial, "\n"); 

    running = 1; 
    while (running) 
    {
        // Read the serial interface (RX) and print it onto stdout.  
        retval = UART_recv(&serial);
        if (retval > 0) 
            UART_print(&serial, retval);

        // Read stdin and write it onto the serial interface (TX). 
        retval = CON_read(&console);
        if (retval > 0)
            UART_write(&serial, console.stdin_buf); 
        
        else if (retval < 0)
            running = 0; 
    }


    puts("\n\n\rThanks for using \x1b[1mstm_uart\x1b[0m made by \x1b[1mromain_flcht\x1b[0m :).\n"); 
    
    // Restore tty settings and free memory space. 
    retval = CON_restore(&console);
    if (retval < 0)
        return 1; 
    
    UART_close(&serial); 
    return 0; 
}
