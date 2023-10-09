#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
char buffer[1024];

//Student structure
typedef struct {
    char login_id[50];
    char password[50];
    char name[20];
    int age;
    char email_id[30];
    char address[100];
    int stud_id;
} Student;

typedef struct {
    char login_id[50];
    char password[50];
    char name[20];
    char department[20];
    char designation[20];
    char email_id[30];
    char address[100];
    int faculty_id;
} Faculty;


void addStudent(int clientSocket){
    Student newStudent;
    memset(&newStudent, 0, sizeof(Student)); // Initialize the structure
    printf("Enter student name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    printf("Enter student age: ");
    scanf("%d", &newStudent.age);
    getchar(); // Consume newline character
    printf("Enter student email_id: ");
    fgets(newStudent.email_id, sizeof(newStudent.email_id), stdin);
    printf("Enter student address: ");
    fgets(newStudent.address, sizeof(newStudent.address), stdin);

    // Format the student data as space-separated values
    char formattedData[256];
    snprintf(formattedData, sizeof(formattedData), "%s %d %s %s\n",
            newStudent.name,
            newStudent.age,
            newStudent.email_id,
            newStudent.address);

    // Send the formatted data to the server
    send(clientSocket, formattedData, sizeof(formattedData), 0);


    // Receive and display the server's response
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    printf("Server response: %s\n", buffer);
}

void addFaculty(int clientSocket){
    Faculty newFaculty;
    memset(&newFaculty, 0, sizeof(Faculty)); // Initialize the structure
    printf("Enter Faculty name: ");
    fgets(newFaculty.name, sizeof(newFaculty.name), stdin);
    printf("Enter Faculty Department: ");
    fgets(newFaculty.department, sizeof(newFaculty.department), stdin);
    printf("Enter Faculty Designation: ");
    fgets(newFaculty.designation, sizeof(newFaculty.designation), stdin);
    printf("Enter faculty email_id: ");
    fgets(newFaculty.email_id, sizeof(newFaculty.email_id), stdin);
    printf("Enter faculty address: ");
    fgets(newFaculty.address, sizeof(newFaculty.address), stdin);

    // Format the student data as space-separated values
    char formattedData[256];
    snprintf(formattedData, sizeof(formattedData), "%s %s %s %s %s\n",
            newFaculty.name,
            newFaculty.department,
            newFaculty.designation,
            newFaculty.email_id,
            newFaculty.address);

    // Send the formatted data to the server
    send(clientSocket, formattedData, sizeof(formattedData), 0);


    // Receive and display the server's response
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    printf("Server response: %s\n", buffer);
}

// Function to view details of a specific student based on stud_id
void viewStudentDetails(int serverSocket) {
    char studId[10]; // Assuming stud_id is a string
    Student student;
    ssize_t bytesRead;

    printf("Enter the stud_id of the student you want to view: ");
    fgets(studId, sizeof(studId), stdin);
    strtok(studId, "\n"); // Remove newline character

    // Send the entered stud_id to the server
    send(serverSocket, studId, strlen(studId), 0);

    // Receive and display student details
    bytesRead = recv(serverSocket, &student, sizeof(Student), 0);

    if (bytesRead <= 0) {
        printf("Error receiving student details.\n");
        return;
    }

    // Check if the received data indicates that the student was not found
    if (strncmp(student.login_id, "Student not found.", sizeof(student.login_id)) == 0) {
        printf("No student details found for the provided stud_id.\n");
    } else {
        // Print the received student details
        printf("Login ID: %s\n", student.login_id);
        printf("Name: %s\n", student.name);
        printf("Age: %d\n", student.age);
        printf("Email ID: %s\n", student.email_id);
        printf("Address: %s\n", student.address);
        printf("Stud ID: %d\n", student.stud_id);
    }
}

// Function to view details of a specific faculty based on stud_id
void viewFacultyDetails (int serverSocket) {
    char facultyId[10]; // Assuming faculty_id is a string
    Faculty faculty;
    ssize_t bytesRead;

    printf("Enter the faculty_id of the Faculty you want to view: ");
    fgets(facultyId, sizeof(facultyId), stdin);
    strtok(facultyId, "\n"); // Remove newline character

    // Send the entered stud_id to the server
    send(serverSocket, facultyId, strlen(facultyId), 0);

    // Receive and display student details
    bytesRead = recv(serverSocket, &faculty, sizeof(Faculty), 0);

    if (bytesRead <= 0) {
        printf("Error receiving faculty details.\n");
        return;
    }

    // Check if the received data indicates that the faculty was not found
    if (strncmp(faculty.login_id, "Faculty not found.", sizeof(faculty.login_id)) == 0) {
        printf("No faculty details found for the provided stud_id.\n");
    } else {
        // Print the received faculty details
        printf("Login ID: %s\n", faculty.login_id);
        printf("Name: %s\n", faculty.name);
        printf("Department: %s\n", faculty.department);
        printf("Designation: %s\n", faculty.designation);
        printf("Email ID: %s\n", faculty.email_id);
        printf("Address : %s\n", faculty.address);
        printf("Faculty ID : %d\n", faculty.faculty_id);
    }
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
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

        // Sending the userRole to the server ->
        write(clientSocket, userRole, strlen(userRole));
        printf("Enter login_id -> ");
        fgets(username, sizeof(username), stdin);

        // Sending the username to the server ->
        write(clientSocket, username, strlen(username));
        printf("Enter password -> ");
        fgets(password, sizeof(password), stdin);

        // Sending the password to the server ->
        write(clientSocket, password, strlen(password));

        // Recivig the authentication information based on the server
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);

        if(strcmp(buffer, "Authenticated") == 0){
            if (atoi(userRole) == 1) {
                // Collect student data from the client
                while(1){
                    printf("------------Welcome to Admin Menu-----------\n");
                    printf("1.) Add Student\n");
                    printf("2.) View Student Details\n");
                    printf("3.) Add Faculty\n");
                    printf("4.) View Faculty Details\n");
                    printf("5.) Modify Student Details\n");
                    printf("6.) Modify Faculty Details\n");
                    printf("7.) Logout and Exit\n");

                    int adminChoice;
                    scanf("%d", &adminChoice);
                    char choiceStr[10];
                    snprintf(choiceStr, sizeof(choiceStr), "%d", adminChoice);
                    send(clientSocket, choiceStr, strlen(choiceStr), 0);
                    getchar();

                    switch (adminChoice) {
                        case 1:
                            // Add Student functionality
                            addStudent(clientSocket);
                            break;
                            
                        case 2:
                            viewStudentDetails(clientSocket);
                            break;

                        case 3:
                            addFaculty(clientSocket);
                            break;

                        case 4:
                            viewFacultyDetails(clientSocket);
                            break;
                            
                        case 5:
                            // Exit
                            printf("Admin session ended.\n");
                            close(clientSocket);
                            break;

                        default:
                            printf("Invalid choice. Please try again.\n");
                            break;
                    }
                }

                addStudent(clientSocket);
            }
        }
        else {
            printf("Authentication failed. Invalid username or password.\n");
        }

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
