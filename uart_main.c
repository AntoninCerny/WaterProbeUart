#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
//GPIO library
#include <wiringPi.h>

void setGpioPinHigh(int pin);


int main() {
    setGpioPinHigh(5);
    int fd;
    FILE *file;

    // Open the Port. We want read/write, no "controlling tty" status, and open it no matter what state DCD is in
    fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("open_port: Unable to open /dev/ttyAMA0 - ");
        return -1;
    }

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

    // Open the file for writing
    file = fopen("received_data.txt", "w");
    if (file == NULL) {
        perror("Unable to open file - ");
        close(fd);
        return -1;
    }

    // Read and write data
    char buf[256];
    int n;
    while (1) {
        n = read(fd, buf, sizeof(buf) - 1);
        if (n < 0) {
            if (errno == EAGAIN) {
                // No data available right now, continue
                usleep(100000); // Sleep for 100ms
                continue;
            } else {
                perror("Read failed - ");
                break;
            }
        } else if (n == 0) {
            // No data, continue
            usleep(100000); // Sleep for 100ms
            continue;
        } else {
            // Data read
            buf[n] = '\0'; // Null-terminate the string
            fprintf(file, "%s", buf);
            fflush(file);
            printf("%i bytes read: %s\n", n, buf);
        }
    }

    // Don't forget to clean up
    fclose(file);
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