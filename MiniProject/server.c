#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

// Setting the port to be 8080
// Can check the available ports using - lsof -i -n -P | grep TCP command 
#define PORT 8080
#define MAX_CLIENTS 5

// Define file paths for user and course data
const char* userFile = "users.txt";
const char* courseFile = "courses.txt";

// Below I am definig all the structures for all the required roles and admins.

// student structure
typedef struct {
    char name[20];
    int age;
    int stud_id;
    char email_id[30];
    char address[100];
    char login_id[50];
    char password[50];
} Student;

// Faculty structure
typedef struct {
    char name[20];
    char department[20];
    char designation[20];
    char email_id[30];
    char address[100];
    char login_id[50];
    char password[50];
} Faculty;

// Admin Structure
// Admin in itself does not contain much information except for id and password

typedef struct {
    char login_id[50];
    char password[50];
} Admin;

// Course structure
typedef struct {
    char courseName[50];
    char department[20];
    int totalseats;
    int credits;
    int availableSeats;
} Course;

// function to implement correct readline in the server
ssize_t readLine(int fd, char* buffer, size_t maxLength) {
    ssize_t bytesRead = 0;
    ssize_t totalBytesRead = 0;
    char ch;

    while (totalBytesRead < maxLength - 1) {
        ssize_t bytesRead = read(fd, &ch, 1);

        if (bytesRead < 0) {
            return bytesRead; // Error
        } else if (bytesRead == 0 || ch == '\n') {
            break; // End of file or end of line
        } else {
            buffer[totalBytesRead] = ch;
            totalBytesRead++;
        }
    }

    buffer[totalBytesRead] = '\0'; // Null-terminate the string
    return totalBytesRead;
}

// Function to authenticate a user based on username and password

// Role : 1 -> Admin
// Role : 2 -> Faculty
// Role : 1 -> Student

int authenticate(const char* username, const char* password, int role) {

    // Authentication for admin : - >
    if (role == 1)
    {
        int adminInfoFile = open("admin.txt", O_RDONLY);

        if (adminInfoFile == -1) {
            perror("Error opening user info file");
            return 0;
        }

        Admin admin;
        memset(&admin, 0, sizeof(Admin));
        int isAuthenticated = 0; // Flag to track if authentication is successful

        char line[100]; 

        // Read admin information from the file and compare with provided credentials
        while (readLine(adminInfoFile, line, sizeof(line))) {
        char fileUsername[50];
        char filePassword[50];
        sscanf(line, "%s %s", fileUsername, filePassword);

        if (role == 1) { // For Admin
            if (strcmp(fileUsername, username) == 0 && strcmp(filePassword, password) == 0) {
                // Authentication successful
                isAuthenticated = 1;
                break;
            }
        }
        // Handle other roles (Faculty and Student) similarly
    }

    close(adminInfoFile);

    return isAuthenticated;
    }

    if(role == 2){
        int facultyInfoFile = open("faculty.txt", O_RDONLY);
        
        if (facultyInfoFile == -1) {
            perror("Error opening user info file");
            return 0;
        }

        Faculty faculty;
        int isAuthenticated = 0; // Flag to track if authentication is successful

        // Read faculty information from the file and compare with provided credentials
        ssize_t bytesRead;
        while ((bytesRead = read(facultyInfoFile, &faculty, sizeof(Faculty))) == sizeof(Faculty)) {
            if (strcmp(faculty.login_id, username) == 0 && strcmp(faculty.password, password) == 0) {
                // Authentication successful
                isAuthenticated = 1;
                break;
            }
        }

        close(facultyInfoFile);

        return isAuthenticated;
    }
    else{
        int studentInfoFile = open("student.txt", O_RDONLY);
        
        if (studentInfoFile == -1) {
            perror("Error opening user info file");
            return 0;
        }

        Student student;
        int isAuthenticated = 0; // Flag to track if authentication is successful

        // Read faculty information from the file and compare with provided credentials
        ssize_t bytesRead;
        while ((bytesRead = read(studentInfoFile, &student, sizeof(Student))) == sizeof(Student)) {
            if (strcmp(student.login_id, username) == 0 && strcmp(student.password, password) == 0) {
                // Authentication successful
                isAuthenticated = 1;
                break;
            }
        }

        close(studentInfoFile);

        return isAuthenticated;
    }
}

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
            continue;
        }

        //printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        // Code for Authentication :-> 

        memset(username, 0, sizeof(username));
        memset(password, 0, sizeof(password));
        memset(userRole, 0, sizeof(userRole));

        // Receive the username, password, and userRole from the client
        recv(newSocket, userRole, sizeof(userRole), 0);
        //printf("UserRole received\n");
        recv(newSocket, username, sizeof(username), 0);
        //printf("Username received\n");
        recv(newSocket, password, sizeof(password), 0);
        //printf("Password received\n");

        // Remove trailing newline characters
        username[strcspn(username, "\n")] = '\0';
        password[strcspn(password, "\n")] = '\0';
        userRole[strcspn(userRole, "\n")] = '\0';

        // converting role to integer value ->
        int role = atoi(userRole);
        printf("completed conversion\n");

        // Authenticate the user and get their role
        if (authenticate(username, password, role)) {
            printf("Authentication successful\n");
            // Depending on the user's role, you can perform role-specific actions here
        } else {
            printf("Authentication failed. Invalid username or password.\n");
        }

        // Implement communication with the client using system calls
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = read(newSocket, buffer, sizeof(buffer));
            if (bytesRead <= 0) {
                break; // Client disconnected or error
            }

            // Process the client's request and send responses using system calls
            // Implement your logic here

            write(newSocket, "Response from server", strlen("Response from server"));
        }

        close(newSocket);
    }

    close(serverSocket);
    return 0;
}
