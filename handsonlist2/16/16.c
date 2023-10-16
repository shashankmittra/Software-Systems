/*
============================================================================
Name : 16
Author : Shashank Mittra
Description : Write a program to send and receive data from parent to child vice versa. Use two way
communication.
Date: 3rd Oct, 2023.
============================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// For achieveing 2-way communication we have to use 2 pipes
//  one for sending data from the parent to the child and another for sending data from the child to the parent


int main() {
    int parent_to_child_pipe[2];
    int child_to_parent_pipe[2];
    pid_t pid;

    // Create the pipes
    if (pipe(parent_to_child_pipe) == -1 || pipe(child_to_parent_pipe) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Fork the process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        close(parent_to_child_pipe[1]); // Close write end of the parent-to-child pipe
        close(child_to_parent_pipe[0]); // Close read end of the child-to-parent pipe

        char message_to_child[1024];
        char message_to_parent[1024];

        // Read from parent and display
        ssize_t bytes_read = read(parent_to_child_pipe[0], message_to_child, sizeof(message_to_child));
        if (bytes_read > 0) {
            printf("Child Process Received: %s\n", message_to_child);
        }

        // Send a response to the parent
        strcpy(message_to_parent, "Hello from the child process!");
        write(child_to_parent_pipe[1], message_to_parent, strlen(message_to_parent) + 1);

        // Close the remaining pipe ends
        close(parent_to_child_pipe[0]);
        close(child_to_parent_pipe[1]);
    } else { // Parent process
        close(parent_to_child_pipe[0]); // Close read end of the parent-to-child pipe
        close(child_to_parent_pipe[1]); // Close write end of the child-to-parent pipe

        char message_to_parent[1024];
        char message_to_child[1024];

        // Send a message to the child
        strcpy(message_to_parent, "Hello from the parent process!");
        write(parent_to_child_pipe[1], message_to_parent, strlen(message_to_parent) + 1);

        // Read the child's response
        ssize_t bytes_read = read(child_to_parent_pipe[0], message_to_child, sizeof(message_to_child));
        if (bytes_read > 0) {
            printf("Parent Process Received: %s\n", message_to_child);
        }

        // Close the remaining pipe ends
        close(parent_to_child_pipe[1]);
        close(child_to_parent_pipe[0]);
    }

    return 0;
}
