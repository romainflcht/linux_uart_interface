#include "console.h"


// * Initialize the user console. 
// * @param: console: CONSOLE_t structure that need to be initialized. 
// * @return: -1 in case of errors, 0 otherwise. 
int CON_init(CON_t* console)
{
    int retval; 
    int flags; 

    // Save default stdin configuration. 
    retval = tcgetattr(STDIN_FILENO, &(console->oldtio));
    if (retval < 0)
    {
        puts("\x1b[31m~[ERROR] While getting default stdin configuration.\x1b[0m\r"); 
        return -1; 
    }
    
    // Disable canonical, echo and escape char handling. 
    console->newtio = console->oldtio;
    console->newtio.c_lflag &= ~(ICANON | ECHO | ISIG);
    console->newtio.c_cc[VMIN] = 1;
    console->newtio.c_cc[VTIME] = 0;
    
    // Set newly configured settings. 
    retval = tcsetattr(STDIN_FILENO, TCSANOW, &(console->newtio));
    if (retval < 0)
    {
        puts("\x1b[31m~[ERROR] While setting new stdin configuration.\x1b[0m\r"); 
        return -1; 
    }
    
    // Keep previous flags of stdin and change it as non-blocking mode. 
    flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    
    return 0; 
}


// * Read the user input and check if for special input like ESCAPE. 
// * @param: console: CONSOLE_t struct that contains the stdin buffer. 
// * return: -1 if an end control sequence has been detected, bytes readed 
// *         otherwise. 
int CON_read(CON_t* console)
{
    int retval; 
    char input[3];

    // Read the standard input. 
    retval = read(0, input, 1);
    if (retval > 0)
    {
        // Check if the char readed if an escape char. 
        if (input[0] != 27)
        {
            // If not copy, empty the buffer, copy the char to the buffer and 
            // stop. 
            strncpy(console->stdin_buf, "\0\0\0", 3); 
            (console->stdin_buf)[0] = input[0]; 
            return retval; 
        }

        // Else, check if the escape char is followed by another char. 
        retval = read(0, input + 1, 2); 
        if (retval > 0)
        {
            // Copy the full input into the buffer. 
            strncpy(console->stdin_buf, input, 3); 
            return retval + 1; 
        }

        // The escape char is alone, quitting the program. 
        return -1; 
    }

    return 0; 
}


// * Restore the default configuration. 
// * @param: console: CONSOLE_t structure that contains the default config. 
// * @return: -1 in case of errors, 0 otherwise. 
int CON_restore(CON_t* console)
{
    int retval; 

    // Restore the default saved stdin configuration.  
    retval = tcsetattr(STDIN_FILENO, TCSANOW, &(console->oldtio));
    if (retval < 0)
    {
        return -1; 
        puts("\x1b[31m~[ERROR] While restoring default stdin configuration.\x1b[0m\r"); 
    }

    return 0; 
}