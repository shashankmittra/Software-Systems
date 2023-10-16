/*
============================================================================
Name : 17a.c
Author : Shashank Mittra
Description :  17. Write a program to execute ls -l | wc.    a. use dup    b. use dup2    c. use fcntl
Date: 5th Oct, 2023.
============================================================================
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main() {
    int fd[2];
    pid_t pid;

    if(pipe(fd) < 0) {
        perror("Pipe");
        exit(EXIT_FAILURE);
    }
    if((pid=fork()) < 0) {
        perror("Fork");
        exit(EXIT_FAILURE);
    }
    if (!pid) {
        close(fd[0]);
        close(1);
        dup(fd[1]);
        char *args[] = {"ls", "-l", NULL};
        execv("/bin/ls", args);
        close(fd[1]);
        exit(0);
    } else {
        close(fd[1]);
        close(0);
        dup(fd[0]);
        char *args[] = {"wc", NULL};
        execv("/bin/wc", args);
        close(fd[0]);
    }
    return 0;
}