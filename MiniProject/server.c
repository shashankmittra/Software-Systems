#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "handlers.h"

// Setting the port to be 8080
// Can check the available ports using - lsof -i -n -P | grep TCP command 
#define PORT 8080
#define MAX_CLIENTS 5

int main() {
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr, newAddr;
    socklen_t addrSize = sizeof(newAddr);
    char buffer[1024];
    char username[50];
    char password[50];
    char userRole[10]; // Change the size as needed

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error in socket creation");
        exit(1);
    }
    printf("Server socket is created.\n");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error in binding");
        exit(1);
    }
    printf("Binding success.\n");

    // Listen for client connections
    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        perror("Error in listening");
        exit(1);
    }
    printf("Listening...\n");

    // Accept multiple client connections and handle them sequentially
    while (1) {
        newSocket = accept(serverSocket, (struct sockaddr*)&newAddr, &addrSize);
        if (newSocket < 0) {
            perror("Error in accepting connection");
            exit(1);
        }

        if(fork() == 0){
        // Code for Authentication :->
            close(serverSocket);

            memset(username, 0, sizeof(username));
            memset(password, 0, sizeof(password));
            memset(userRole, 0, sizeof(userRole));

            // Receive the username, password, and userRole from the client
            recv(newSocket, userRole, sizeof(userRole), 0);
            // printf("UserRole received\n");
            recv(newSocket, username, sizeof(username), 0);
            // printf("Username received\n");
            recv(newSocket, password, sizeof(password), 0);
            // printf("Password received\n");

            // Remove trailing newline characters
            username[strcspn(username, "\n")] = '\0';
            password[strcspn(password, "\n")] = '\0';
            userRole[strcspn(userRole, "\n")] = '\0';

            // converting role to integer value ->
            int role = atoi(userRole);
            printf("completed conversion\n");

            // Authenticate the user and get their role
            if (authenticate(username, password, role))
            {
                printf("Authentication successful. User role: %d\n", role);

                // Notify the client about successful authentication
                send(newSocket, "Authenticated", sizeof("Authenticated"), 0);

                // If the user is an Admin (role == 1), display the menu
                while(1){
                    if (role == 1)
                    {

                        char adminMenu[] = "\n----------------------------- Welcome to Faculty Menu ------------------------------\n"
                                            "                             1. Add Student\n"
                                            "                             2. View Student Details\n"
                                            "                             3. Add Faculty\n"
                                            "                             4. View Faculty Details\n"
                                            "                             5. Modify Student Details\n"
                                            "                             6. Modify Faculty Details\n"
                                            "                             7. Logout and Exit\n"
                                            "Enter your choice: ";
                        
                    
                        send(newSocket, adminMenu, sizeof(adminMenu), 0);

                        char choiceStr[10];
                        ssize_t bytesReceived = recv(newSocket, choiceStr, sizeof(choiceStr), 0);

                        if (bytesReceived <= 0)
                        {
                            printf("Client disconnected or error while receiving choice.\n");
                            // Handle disconnection or error if needed
                        }
                        else
                        {
                            // Convert the received choice to an integer
                            int adminChoice = atoi(choiceStr);

                            switch (adminChoice)
                            {
                            case 1:
                                handleAddStudent(newSocket);
                                break;
                            case 2:
                                handleViewStudDeatils(newSocket);
                                break;
                            case 3:
                                handleAddFaculty(newSocket);
                                break;
                            case 4:
                                handleViewFacultyDeatils(newSocket);
                                break;
                            case 5:
                                handleModifyStudentDetails(newSocket);
                                break;
                            case 6:
                                handleModifyFacultyDeatils(newSocket);
                                break;
                            case 7:
                                // Handle Exit functionality
                                printf("Admin session ended.\n");
                                close(newSocket);
                                return 0;
                            default:
                                printf("Invalid choice received from the client.\n");
                                // Handle invalid choice if needed
                                break;
                        }
                        if(adminChoice == 7)
                            break;
                    }
                }
                
                // Below is the code to handle faculty menu options
                if (role == 2) {

                    char facultyMenu[] = "\n----------------------------- Welcome to Faculty Menu ------------------------------\n"
                                            "                             1. View Offering Course\n"
                                            "                             2. Add new Course\n"
                                            "                             3. Remove courses from the Catalog\n"
                                            "                             4. Update Course Details\n"
                                            "                             5. Change Password\n"
                                            "                             6. Logout and Exit\n"
                                            "Enter your choice: ";

                    send(newSocket, facultyMenu, sizeof(facultyMenu), 0);

                    char choiceStr[10];
                    ssize_t bytesReceived = recv(newSocket, choiceStr, sizeof(choiceStr), 0);

                    if (bytesReceived <= 0) {
                        printf("Client disconnected or error while receiving choice.\n");
                        // Handle disconnection or error if needed
                    } else {
                        // Convert the received choice to an integer
                        int facultyChoice = atoi(choiceStr);

                        switch (facultyChoice) {
                            case 1:
                                handleViewOfferingCourses(newSocket);
                                break;
                            case 2:
                                handleAddCourse(newSocket, username);
                                break;
                            case 3:
                                handleRemoveCourse(newSocket);
                                break;
                            case 4:
                                handleUpdateCourseDetails(newSocket);
                                break;
                            case 5:
                                handleChangeFacultyPass(newSocket);
                                break;
                            case 6:
                                printf("Faculty session ended.\n");
                                close(newSocket);
                                return 0;
                            default:
                                printf("Invalid choice received from the client.\n");
                                // Handle invalid choice if needed
                                break;
                        }
                        if(facultyChoice == 6)
                                break;
                    }
                }

                // Below is the code to handle the students -> 
                if (role == 3) {

                    char choiceStr[10];
                    ssize_t bytesReceived = recv(newSocket, choiceStr, sizeof(choiceStr), 0);

                    if (bytesReceived <= 0) {
                        printf("Client disconnected or error while receiving choice.\n");
                        // Handle disconnection or error if needed
                    } else {
                        // Convert the received choice to an integer
                        int studentChoice = atoi(choiceStr);

                        switch (studentChoice) {
                            case 1:
                                sendCourseData(newSocket);
                                break;
                            case 2:
                                handleEnrollNewCourse(newSocket);
                                break;
                            case 3:
                                handleDropCourse(newSocket);
                                break;
                            case 4:
                                handleViewEnrolledCourseDetails(newSocket);
                                break;
                            case 5:
                                handleChangeStudentPass(newSocket);
                                break;
                            case 6:
                                printf("Student session ended.\n");
                                close(newSocket);
                                return 0;
                            default:
                                printf("Invalid choice received from the client.\n");
                                // Handle invalid choice if needed
                                break;
                        }
                        if(studentChoice == 6)
                                break;
                    }
                }   
                }
        } else {
            printf("Authentication failed. Invalid username or password.\n");
            // Inform the client about authentication failure
            send(newSocket, "Authentication failed. Invalid username or password.", strlen("Authentication failed. Invalid username or password."), 0);
        }
        exit(0);
        }
        else{
            close(newSocket);
        }
    }

    close(serverSocket);
    return 0;
}
