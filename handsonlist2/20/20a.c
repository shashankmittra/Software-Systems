/*
============================================================================
Name : 20a.c
Author : Shashank Mittra
Description :  Write two programs so that both can communicate by FIFO -Use one way communications. 
Date: 6th Oct, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_PATH "./fifo"

int main() {
    int fd;
    char message[100];

    mkfifo(FIFO_PATH, 0666);

    fd = open(FIFO_PATH, O_WRONLY);

    printf("Enter a message: ");
    fgets(message, sizeof(message), stdin);

    write(fd, message, strlen(message) + 1);

    close(fd);

    return 0;
}