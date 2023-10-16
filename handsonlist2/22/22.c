/*
============================================================================
Name : 22.c
Author : Shashank Mittra
Description : Write a program to wait for data to be written into FIFO within 10 seconds, use select
system call with FIFO.
Date: 6th Oct, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/stat.h>  // Include the sys/stat.h header
#include <sys/types.h>

int main() {
    int fifo_fd;
    fd_set read_fds;
    struct timeval timeout;

    // Fifo file named myfifo is already created by me 
    if (mkfifo("myfifo", 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    fifo_fd = open("myfifo", O_RDONLY);
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    FD_ZERO(&read_fds);
    FD_SET(fifo_fd, &read_fds);

    // Setting the timeout
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    printf("Waiting for data in FIFO...\n");

    // Wait for data to be written into the FIFO or timeout
    int select_result = select(fifo_fd + 1, &read_fds, NULL, NULL, &timeout);

    if (select_result == -1) {
        perror("select");
        exit(EXIT_FAILURE);
    } else if (select_result == 0) {
        printf("Timeout: No data written into FIFO within 10 seconds.\n");
    } else {
        if (FD_ISSET(fifo_fd, &read_fds)) {
            char buffer[100];
            ssize_t bytes_read = read(fifo_fd, buffer, sizeof(buffer));
            if (bytes_read == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            printf("Data received from FIFO: %.*s\n", (int)bytes_read, buffer);
        }
    }

    close(fifo_fd);
    unlink("myfifo");

    return 0;
}

