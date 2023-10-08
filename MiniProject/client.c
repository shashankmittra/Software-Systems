#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080



int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    char username[100];
    char password[100];
    char userRole[10]; // Change the size as needed

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Error in socket creation");
        exit(1);
    }
    else{
        printf("Socket created successfullt\n");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error in connection");
        exit(1);
    }
    else{
        printf("client to server connection established successfully\n");
    }

    // Giving an introductory message to the client:
    printf("-----------Welcome to the Course Registration Portal-----------\n");

    // Implementing communication with the server
    
    while (1) {

        // Getting user input and send requests to the server
        printf("Login Type ->\n");
        printf("Enter role (1 for Admin, 2 for Faculty, 3 for Student): ");
        fgets(userRole, sizeof(userRole), stdin);
        printf("Writin role\n");
        write(clientSocket, userRole, strlen(userRole));
        printf("Done role\n");
        
        printf("Enter login_id -> ");
        fgets(username, sizeof(username), stdin);
        write(clientSocket, username, strlen(username));
        printf("Enter password -> ");
        fgets(password, sizeof(password), stdin);
        write(clientSocket, password, strlen(password));

        // Receive and process responses from the server using system calls
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = read(clientSocket, buffer, sizeof(buffer));
        if (bytesRead <= 0) {
            break; // Server disconnected or error
        }

        // Process and display the server's response
        printf("Server response: %s\n", buffer);
    }

    close(clientSocket);
    return 0;
}
