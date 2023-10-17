#include "declarations.h"
#include "readline.h"

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

    else if(role == 2){
        int facultyDataFile = open("faculty.txt", O_RDONLY);
        if (facultyDataFile == -1) {
            perror("Error opening faculty data file");
            return 0;
        }

        Faculty faculty;
        int isAuthenticated = 0; // Flag to track if authentication is successful
        char line[256];
        ssize_t bytesRead;

        while ((bytesRead = read(facultyDataFile, line, sizeof(line))) > 0) {
            // Null-terminate the strings
            char* token = strtok(line, "$");
            Faculty faculty;

            // Initialize the student structure
            memset(&faculty, 0, sizeof(Faculty));

            // Extract and fill the fields of the student structure
                if (token) {
                    strncpy(faculty.login_id, token, sizeof(faculty.login_id));
                    token = strtok(NULL, "$");
                }
                if (token) {
                    strncpy(faculty.password, token, sizeof(faculty.password));
                    token = strtok(NULL, "$");
                }
                if (token) {
                    strncpy(faculty.name, token, sizeof(faculty.name));
                    token = strtok(NULL, "$");
                }
                if (token) {
                    strncpy(faculty.department, token, sizeof(faculty.department));
                    token = strtok(NULL, "$");
                }
                if (token) {
                    strncpy(faculty.designation, token, sizeof(faculty.designation));
                    token = strtok(NULL, "$");
                }
                if (token) {
                    strncpy(faculty.email_id, token, sizeof(faculty.email_id));
                    token = strtok(NULL, "$");
                }
                if (token) {
                    strncpy(faculty.address, token, sizeof(faculty.address));
                    token = strtok(NULL, "$");
                }
                if (token) {
                    faculty.faculty_id = atoi(token);
                }
                if(strcmp(faculty.login_id, username) == 0 && strcmp(faculty.password, password) == 0){
                    isAuthenticated = 1;
                    break;
                }
            }
            close(facultyDataFile);
            return isAuthenticated;
    }
    else if(role == 3){
        int studentFile = open("student.txt", O_RDONLY);
        if (studentFile == -1) {
            perror("Error opening student data file");
            return 0;
        }

        char line[256];
        char buffer[256];
        int bytesRead;
        int lineIndex = 0;
        int isAuthenticated = 0;

        while ((bytesRead = read(studentFile, buffer, sizeof(buffer))) > 0) {
            for (int i = 0; i < bytesRead; i++) {
                if (lineIndex < sizeof(line) - 1) {
                    line[lineIndex++] = buffer[i];

                    if (buffer[i] == '\n') {
                        line[lineIndex] = '\0'; // Null-terminate the line
                        lineIndex = 0; // Reset the line index

                        Student student;
                        if (sscanf(line, "%49[^$]$%49[^$]$%19[^$]$%d$%49[^$]$%99[^$]$%d",
                                student.login_id, student.password, student.name,
                                &student.age, student.email_id, student.address, &student.stud_id) == 7) {
                            // Check if the username and password match
                            if (strcmp(student.login_id, username) == 0 && strcmp(student.password, password) == 0) {
                                isAuthenticated = 1;
                                break;
                            }
                        }
                    }
                }
            }
        }

        close(studentFile);
        return isAuthenticated;
    }
    return 0;
}