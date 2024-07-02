#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
//#include <termios.h>
//GPIO library
#include <wiringPi.h>
#include <wiringSerial.h>

void setGpioPinHigh(int pin);


int main() {
    setGpioPinHigh(24);
    int fd;
    

    // Open the Port. We want read/write, no "controlling tty" status, and open it no matter what state DCD is in
    //fd = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    fd = serialOpen("/dev/serial0", 9600);
    if (fd == -1) {
        perror("open_port: Unable to open /dev/ttyAMA0 - ");
        return -1;
    }
    /*
    // Configure the port
    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_cflag |= (CLOCAL | CREAD); // Enable the receiver and set local mode
    options.c_cflag &= ~PARENB;          // No parity
    options.c_cflag &= ~CSTOPB;          // 1 stop bit
    options.c_cflag &= ~CSIZE;           // Mask the character size bits
    options.c_cflag |= CS8;              // 8 data bits
    tcsetattr(fd, TCSANOW, &options);
    */

    //read data in endless loop
    int bits;
    int n;
    while (1) {
        bits = serialDataAvail(fd);
        if(bits == 0){

        }
        if(bits == -1){
            printf("error reading serial");
            serialFlush(fd);
        }
        else{
            for(size_t i=0;i<bits;i++){
                n = serialGetchar(fd);
                printf("read: %d\n", n);
                fflush(stdout);
            }
        }
    }
    close(fd);
    return 0;
}

void setGpioPinHigh(int pin) {
    // Set up the wiringPi library
    if (wiringPiSetup() == -1) {
        printf("wiringPi setup failed\n");
        return;
    }

    // Set the GPIO pin to output mode
    pinMode(pin, OUTPUT);

    // Set the GPIO pin high
    digitalWrite(pin, HIGH);

    printf("GPIO pin %d set to high\n", pin);
}