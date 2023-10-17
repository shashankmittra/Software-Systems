/*
============================================================================
Name : 21a.c
Author : Shashank Mittra
Description :  Write two programs so that both can communicate by FIFO -Use two way communications. 
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
    char input[1024];

    // Creating the FIFO node below
    mkfifo(FIFO_FILE, 0666);

    while (1) {
        fd = open(FIFO_FILE, O_RDWR);

        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        printf("Enter a message to send (or type 'exit' to quit): ");
        fgets(input, sizeof(input), stdin);

        if (input == "exit") {
            close(fd);
            exit(EXIT_SUCCESS);
        }

        write(fd, input, strlen(input) + 1);
        
        read(fd,input,strlen(input));
        printf("Reader's response : %s\n",input);
        close(fd);
    }

    return 0;
}