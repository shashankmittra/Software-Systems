/*
============================================================================
Name : 21b.c
Author : Shashank Mittra
Description :  Write two programs so that both can communicate by FIFO -Use one way communications. 
Date: 6th Oct, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO_FILE "fifo"

int main() {
    int fd;
    char output[1024];

    while (1) {
        fd = open(FIFO_FILE, O_RDWR);

        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        read(fd, output, sizeof(output));

        printf("Writer's Response : %s\n", output);

        printf("Enter a message to send (or type 'exit' to quit): ");
        fgets(output, sizeof(output), stdin);

        if (output == "exit") {
            close(fd);
            exit(EXIT_SUCCESS);
        }

        write(fd, output, strlen(output) + 1);
        close(fd);
    }


    return 0;
}