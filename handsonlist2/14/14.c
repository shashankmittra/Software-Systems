/*
============================================================================
Name : 14
Author : Shashank Mittra
Description : Write a simple program to create a pipe, write to the pipe, read from pipe and display on
the monitor.
Date: 3rd Oct, 2023.
============================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int pipe_fd[2];
    pid_t pid;

    //Here we create the pipe ->
    if(pipe(pipe_fd) == -1){
        perror("Error while creating the pipe");
        return 1;
    }

    // Here we use the parent process to write and the child will read the message

    pid = fork();
    if(pid < 0){
        perror("Child is not created properly");
        return 1;
    }

    // Child - 
    if(pid == 0){
        // Close the write end first -
        close(pipe_fd[1]);
        char buffer[1024];
        ssize_t br = read(pipe_fd[0], buffer, sizeof(buffer));
        if(br > 0)
            printf("Child has successfully read");
        close(pipe_fd[0]);
        }
        

        // Parent -
        else{
        // Close the read end -
            close(pipe_fd[0]);
            char msg[] = "This is Shashank";
            ssize_t br = write(pipe_fd[1], msg, sizeof(msg));
            close(pipe_fd[1]);
        }
    return 0;
}
