/*
============================================================================
Name : 20b.c
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

    fd = open(FIFO_PATH, O_RDONLY);

    read(fd, message, sizeof(message));
    printf("Received: %s", message);

    close(fd);

    return 0;
}