#include "uart.h"


// * Open a serial port and set the UART_t structure according to the serial 
// * interface parameters.
// * @param: serial : pointer to an empty UART_t structure.  
// * @param: port   : the interface to open (ex: /dev/tty*).  
// * @param: buf_len: the size of the char buffer of the UART interface. 
// * @return: -1 in case of errors, 0 otherwise. 
int UART_connect(UART_t* serial, char* port, size_t buf_len)
{
    char* serial_buf; 
    int retval; 
    int fd; 

    // Open the serial port passed in parameter in non-blocking mode to assure 
    // that the port is opened without delay and don't block the program running. 
    fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK); 
    if (fd < 0)
    {
        puts("\x1b[31m~[ERROR] Interface doesn't exist.\x1b[0m\r"); 
        return -1; 
    }

    // Check if the file descriptor point to a tty interface. 
    retval = isatty(fd); 
    if (!retval)
    {
        puts("\x1b[31m~[ERROR] Interface is not a tty.\x1b[0m\r"); 
        return -1; 
    }

    serial_buf = malloc(buf_len);
    if (!serial_buf)
        return -1; 
    
    // Initialise the UART_t structure. 
    serial->fd = fd; 
    serial->buf_len = buf_len; 
    serial->buf = serial_buf; 

    // Save the current console configuration to restore it at the end of the 
    // execution. 
    if (tcgetattr(fd, &(serial->oldtio)) != 0)
    {
        UART_free(serial); 
        puts("\x1b[31m~[ERROR] While getting default serial I/O configuration.\x1b[0m\r"); 
        return -1; 
    }

    serial->newtio = serial->oldtio; 
    return 0; 
}


// * Free the UART char buffer. 
// * @param: serial: the UART_t structure to free. 
void UART_free(UART_t* serial)
{
    if (!serial)
    return; 
    
    if (serial->buf)
    {
        free(serial->buf); 
        serial->buf = NULL; 
    }
    
    return; 
}


// * Set every parameter of the tty. 
// * @param: serial: The UART_t interface to setup.
// * @param: speed : the baud rate speed we want to configure:
// *                 [4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 
// *                 921600]
// * @return: -1 in case of error, else 0.  
int UART_init(UART_t* serial, int speed)
{
    // Set communication speed. 
    UART_set_speed(serial, speed); 

    // Set 8-bits mode, no parity check, with 1 stop bit. 
    serial->newtio.c_cflag &= ~PARENB;
    serial->newtio.c_cflag &= ~CSTOPB;
    serial->newtio.c_cflag &= ~CSIZE;
    serial->newtio.c_cflag |= CS8;

    serial->newtio.c_cc[VMIN] = 1;
    serial->newtio.c_cc[VTIME] = 0;

    // No flow control, remove \r transformation.
    serial->newtio.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL | INLCR | IGNCR);

    // Disable canonical mode, echo and escape char. 
    serial->newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    serial->newtio.c_oflag &= ~OPOST;

    // Activate the configuration. 
    if (tcsetattr(serial->fd, TCSANOW, &(serial->newtio)) != 0) 
    {
        puts("\x1b[31m~[ERROR] While setting new serial I/O configuration.\x1b[0m\r"); 
        return -1;
    }
    
    return 0;
}


// * Set the baud rate speed of a serial interface. 
// * @param: serial: the UART_t interface to set speed.
// * @param: speed : the baud rate speed we want to configure:
// *                 [4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 
// *                 921600]
// * return: -1 in case of errors, 0 otherwise. 
int UART_set_speed(UART_t* serial, int speed)
{
    int baud_rate; 
    int retval; 

    switch (speed)
    {
        case 4800:
            baud_rate = B4800; 
            break;
        case 9600:
            baud_rate = B9600;  
            break;
        case 19200:
            baud_rate = B19200; 
            break;
        case 38400:
            baud_rate = B38400; 
            break;
        case 57600:
            baud_rate = B57600; 
            break;
        case 115200:
            baud_rate = B115200; 
            break;
        case 230400:
            baud_rate = B230400; 
            break;
        case 460800:
            baud_rate = B460800; 
            break;
        case 921600:
            baud_rate = B921600; 
            break;
         default:
            puts("\x1b[1;33m~[WARNING] Baudrate not recognized, set to default (115200bps).\x1b[0m\r\n"); 
            baud_rate = B115200; 
            break;
    }

    // set baudrate to the selected speed. 
    retval = 0; 
    retval += cfsetispeed(&(serial->newtio), baud_rate);
    retval += cfsetospeed(&(serial->newtio), baud_rate);
    if (retval < 0)
    {
        puts("\x1b[31m~[ERROR] While setting baudrate.\x1b[0m\r"); 
        return -1; 
    }

    return 0; 
}


// * Close the UART connection. 
// * @param: serial: the serial connection to close. 
void UART_close(UART_t* serial)
{
    // Restore the old interface configuration. 
    tcsetattr(serial->fd, TCSANOW, &(serial->oldtio)); 

    // Free ressources. 
    close(serial->fd);
    UART_free(serial); 
}


// * Retrieve the serial output and save it into the UART_t char buffer. 
// * @param: serial: the serial interface where we will read. 
// * @return: -1 in case of errors, else the number of bytes readed. 
int UART_recv(UART_t* serial)
{
    return read(serial->fd, serial->buf, serial->buf_len); 
}


// * Print the UART_t buffer to the stdout. 
// * @param: serial: the serial buffer that we will print. 
void UART_print(UART_t* serial, int bytes_readed)
{
    serial->buf[bytes_readed] = '\0'; 
    printf("%s", serial->buf); 
    fflush(stdout); 
    return; 
}


// * Write to the UART interface. 
// * @param: serial: The UART_t interface to write on. 
// * @param: str   : the string to write. 
// * @return: -1 if there is an error, else the bytes written.  
int UART_write(UART_t* serial, char* str)
{
    int len; 
    int retval; 

    len = str_len(str); 
    retval = write(serial->fd, str, len); 
    tcdrain(serial->fd);
    return retval; 
}
