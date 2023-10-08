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
    char login_id[50];
    char password[50];
    char name[20];
    int age;
    char email_id[30];
    char address[100];
    int stud_id;
} Student;

// Faculty structure
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


//----------------------------------Below are the various functionalities that in order to manipulae the DataBase-------------------------------------------

//Function to add a student with auto-incrementing stud_id

void addStudent(Student newStudent) {
    // Open the student data file for reading to find the highest stud_id
    int maxStudId = 0; // Initialize to 0

    int studentDataFile = open("student.txt", O_RDONLY);
    if (studentDataFile == -1) {
        perror("Error opening student data file");
        return;
    }

    char line[256];
    ssize_t bytesRead;
    while ((bytesRead = read(studentDataFile, line, sizeof(line))) > 0) {
        // Parse the line to extract the stud_id and check for the maximum
        int stud_id;
        if (sscanf(line, "%*s %*s %*s %*d %*s %*s %d", &stud_id) == 1) {
            if (stud_id > maxStudId) {
                maxStudId = stud_id;
            }
        }
    }
    close(studentDataFile);

    // Create or open the student data file for appending
    studentDataFile = open("student.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (studentDataFile == -1) {
        perror("Error opening student data file");
        return;
    }

    if (maxStudId == 0) {
        newStudent.stud_id = 1;
    }
    else{
        newStudent.stud_id = maxStudId + 1;
    }

    // Create the login ID by appending "MT" to the stud_id
    snprintf(newStudent.login_id, sizeof(newStudent.login_id), "MT%d", newStudent.stud_id);

    // Generate a temporary password (you can implement your own logic)
    snprintf(newStudent.password, sizeof(newStudent.password), "1234");

    // Format the student data with space-separated values
    char formattedData[256];
    snprintf(formattedData, sizeof(formattedData), "%s %s %s %d %s %s %d\n",
             newStudent.login_id,
             newStudent.password,
             newStudent.name,
             newStudent.age,
             newStudent.email_id,
             newStudent.address,
             newStudent.stud_id);

    // Write the formatted student data to the file
    ssize_t bytesWritten = write(studentDataFile, formattedData, strlen(formattedData));
    if (bytesWritten == strlen(formattedData)) {
        printf("Student added successfully with stud_id: %d\n", newStudent.stud_id);
    } else {
        printf("Error adding student.\n");
    }
    close(studentDataFile);
}

// Function to add a New Faculty ->

void addFaculty(Faculty newFaculty) {
    // Open the student data file for reading to find the highest stud_id
    int maxFacultyId = 0; // Initialize to 0

    int facultyDataFile = open("faculty.txt", O_RDONLY);
    if (facultyDataFile == -1) {
        perror("Error opening faculty data file");
        return;
    }

    char line[256];
    ssize_t bytesRead;
    while ((bytesRead = read(facultyDataFile, line, sizeof(line))) > 0) {
        // Parse the line to extract the stud_id and check for the maximum
        int faculty_id;
        if (sscanf(line, "%*s %*s %*s %*s %*s %*s %d", &faculty_id) == 1) {
            if (faculty_id > maxFacultyId) {
                maxFacultyId = faculty_id;
            }
        }
    }
    close(facultyDataFile);

    // Create or open the student data file for appending
    facultyDataFile = open("faculty.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (facultyDataFile == -1) {
        perror("Error opening faculty data file");
        return;
    }

    if (maxFacultyId == 0) {
        newFaculty.faculty_id = 1;
    }
    else{
        newFaculty.faculty_id = maxFacultyId + 1;
    }

    // Create the login ID by appending "MT" to the stud_id
    snprintf(newFaculty.login_id, sizeof(newFaculty.login_id), "PF%d", newFaculty.faculty_id);

    // Generate a temporary password (you can implement your own logic)
    snprintf(newFaculty.password, sizeof(newFaculty.password), "1234");

    // Format the student data with space-separated values
    char formattedData[256];
    snprintf(formattedData, sizeof(formattedData), "%s %s %s %s %s %s %d\n",
             newFaculty.login_id,
             newFaculty.password,
             newFaculty.name,
             newFaculty.designation,
             newFaculty.email_id,
             newFaculty.address,
             newFaculty.faculty_id);

    // Write the formatted student data to the file
    ssize_t bytesWritten = write(facultyDataFile, formattedData, strlen(formattedData));
    if (bytesWritten == strlen(formattedData)) {
        printf("Faculty added successfully with stud_id: %d\n", newFaculty.faculty_id);
    } else {
        printf("Error adding student.\n");
    }
    close(facultyDataFile);
}

// Function to send details of a specific student based on stud_id
void sendStudentDetailsByStudId(int clientSocket, int stud_id) {
    // Open the student data file for reading
    int studentDataFile = open("student.txt", O_RDONLY);
    if (studentDataFile == -1) {
        perror("Error opening student data file");
        return;
    }
    //printf("Function call\n");
    char buffer[1024];
    ssize_t bytesRead;
    int studentFound = 0;

    // Inside the loop that reads student data from the file
    while ((bytesRead = readLine(studentDataFile, buffer, sizeof(buffer))) > 0) {
        // Tokenize the data based on spaces
        char* token = strtok(buffer, " ");
        
        // Initialize variables to store each field
        char login_id[50];
        char password[50];
        char name[20];
        int age;
        char email_id[30];
        char address[100];
        int currentStudId;

        // Tokenize the line and extract each field
        while (token != NULL) {
            // Assuming the order of fields in each line
            strcpy(login_id, token);
            token = strtok(NULL, " ");
            strcpy(password, token);
            token = strtok(NULL, " ");
            strcpy(name, token);
            token = strtok(NULL, " ");
            age = atoi(token);
            token = strtok(NULL, " ");
            strcpy(email_id, token);
            token = strtok(NULL, " ");
            strcpy(address, token);
            token = strtok(NULL, " ");
            currentStudId = atoi(token); // Last field is stud_id
            break; // Exit the inner loop after processing all fields
        }

        if (currentStudId == stud_id) {
            // Send the details to the client
            printf("Found the ID\n");
            send(clientSocket, buffer, bytesRead, 0);
            studentFound = 1; // Set the flag to indicate the student is found
            break;
        }
    }


    // Send "END" to indicate the end of data transmission
    send(clientSocket, "END", 3, 0);

    // Close the file
    close(studentDataFile);

    if (!studentFound) {
        // Handle the case where the student with the given stud_id was not found
        send(clientSocket, "Student not found.", strlen("Student not found."), 0);
    }
}



//----------------------------------Below are some helper functions which further calls the actual functions to handle DB----------------------------------

// I have done this so as to provide more modularity and reusability of my code

void handleAddStudent(int newSocket){
    char studentData[256];
    recv(newSocket, studentData, sizeof(studentData), 0);

    // Parse the received data into the Student structure
    Student newStudent;
    memset(&newStudent, 0, sizeof(Student)); // Initialize the structure
    sscanf(studentData, "%s %d %s %s",
            newStudent.name,
            &newStudent.age,
            newStudent.email_id,
            newStudent.address);
    // Add the student
    addStudent(newStudent);

    // Notify the client about the operation's success
    send(newSocket, "Student added successfully", sizeof("Student added successfully"), 0);
}

void handleAddFaculty(int newSocket){
    char facultyData[256];
    recv(newSocket, facultyData, sizeof(facultyData), 0);

    // Parse the received data into the Faculty structure
    Faculty newFaculty;
    memset(&newFaculty, 0, sizeof(Faculty)); // Initialize the structure
    sscanf(facultyData, "%s %s %s %s %s",
            newFaculty.name,
            newFaculty.department,
            newFaculty.designation,
            newFaculty.email_id,
            newFaculty.address);
    // Add the faculty
    addFaculty(newFaculty);

    // Notify the client about the operation's success
    send(newSocket, "Faculty added successfully", sizeof("Student added successfully"), 0);
}

// Function to view student details
void handleViewStudDeatils(int clientSocket) {
    
    // Receive the stud_id sent by the client
    char studId[10]; // Assuming stud_id is a string
    ssize_t bytesReceived = recv(clientSocket, studId, sizeof(studId), 0);

    if (bytesReceived <= 0) {
        printf("No stud_id received.\n");
        // Handle no stud_id received or client disconnection if needed
    } else {
        // Remove trailing newline character if present
        studId[strcspn(studId, "\n")] = '\0';
        printf("Received stud_id: %s\n", studId);
        int stud_id = atoi(studId);
        // Now you have the received stud_id, and you can use it as needed.
        // For example, you can call a function to send student details based on stud_id:
        sendStudentDetailsByStudId(clientSocket, stud_id);
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
            printf("Authentication successful. User role: %d\n", role);

            // Notify the client about successful authentication
            send(newSocket, "Authenticated", sizeof("Authenticated"), 0);

            // If the user is an Admin (role == 1), display the menu
            if (role == 1) {

                char choiceStr[10];
                ssize_t bytesReceived = recv(newSocket, choiceStr, sizeof(choiceStr), 0);

                if (bytesReceived <= 0) {
                    printf("Client disconnected or error while receiving choice.\n");
                    // Handle disconnection or error if needed
                } else {
                    // Convert the received choice to an integer
                    int adminChoice = atoi(choiceStr);

                    switch (adminChoice) {
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
                            // Handle Update Student/Faculty details functionality
                            // updateStudentFacultyDetails(clientSocket);
                            break;
                        case 5:
                            // Handle Exit functionality
                            printf("Admin session ended.\n");
                            close(newSocket);
                            return 0;
                        default:
                            printf("Invalid choice received from the client.\n");
                            // Handle invalid choice if needed
                            break;
                    }
                }
            }
        } else {
            printf("Authentication failed. Invalid username or password.\n");
            // Inform the client about authentication failure
            send(newSocket, "Authentication failed. Invalid username or password.", strlen("Authentication failed. Invalid username or password."), 0);
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
