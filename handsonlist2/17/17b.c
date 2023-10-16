/*
============================================================================
Name : 17.a
Author : Shashank Mittra
Description : Write a program to send and receive data from parent to child vice versa. Use two way
communication.
Date: 3rd Oct, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char const *argv[])
{
    int pipe_fd[2];
    pid_t pid;

    if(pipe(pipe_fd) == -1){
        perror("Error while creating the pipe");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    // Child process
    if (pid == 0) { 

        // Closing the read end of the pipe
        close(pipe_fd[0]);
        
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        execlp("ls", "ls", "-l", NULL);
        perror("exec failed");
        return 1;
    }

    // parent process
    else { 

        // Closing the write end of the pipe:
        close(pipe_fd[1]);
        
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);

        execlp("wc", "wc", NULL);
        perror("exec failed");
        return 1;
    }

    return 0;
}
