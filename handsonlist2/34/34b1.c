/*
============================================================================
Name : 34b1.c
Author : Shashank Mittra
Description : Write a program to create a concurrent server.    a. use fork    b. use pthread_create
Date: 6th Oct, 2023.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void* handle_client(void* arg) {
    int client_socket = *((int*)arg);
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received message: %s\n", buffer);

    const char* response = "Hello from the server!";
    send(client_socket, response, strlen(response), 0);

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating server socket");
        exit(1);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Error setting socket options");
        exit(1);
    }

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    if (listen(server_socket, 5) < 0) {
        perror("Error listening for connections");
        exit(1);
    }

    while (1) {
        int* client_socket = malloc(sizeof(int));
        *client_socket = accept(server_socket, NULL, NULL);
        if (*client_socket < 0) {
            perror("Error accepting client connection");
            exit(1);
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, client_socket) != 0) {
            perror("Error creating thread");
            exit(1);
        }
        pthread_detach(tid);
    }

    close(server_socket);
    return 0;
}