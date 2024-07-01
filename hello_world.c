#include <stdio.h>
#include <unistd.h> // for sleep function

int main() {
    while (1) {
        printf("hello world\n");
        fflush(stdout); // Ensure the output is printed immediately
        sleep(1); // Wait for 1 second
    }
    return 0; // This line will never be reached
}