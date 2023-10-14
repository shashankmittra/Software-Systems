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

    if(role == 2){
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
    else{
        int studentDataFile = open("student.txt", O_RDONLY);
        if (studentDataFile == -1) {
            perror("Error opening student data file");
            return 0;
        }

        Student student;
        int isAuthenticated = 0; // Flag to track if authentication is successful
        char line[256];
        ssize_t bytesRead;

        while ((bytesRead = read(studentDataFile, line, sizeof(line))) > 0) {
            // Null-terminate the strings
            char* token = strtok(line, "$");
            Student student;

            // Initialize the student structure
            memset(&student, 0, sizeof(Student));

            // Extract and fill the fields of the student structure
                if (token) {
                    strncpy(student.login_id, token, sizeof(student.login_id));
                    token = strtok(NULL, "$");
                }
                if (token) {
                    strncpy(student.password, token, sizeof(student.password));
                    token = strtok(NULL, "$");
                }
                if (token) {
                    strncpy(student.name, token, sizeof(student.name));
                    token = strtok(NULL, "$");
                }
                if (token) {
                    student.age = atoi(token);
                    token = strtok(NULL, "$");
                }
                if (token) {
                    strncpy(student.email_id, token, sizeof(student.email_id));
                    token = strtok(NULL, "$");
                }
                if (token) {
                    strncpy(student.address, token, sizeof(student.address));
                    token = strtok(NULL, "$");
                }
                if (token) {
                    student.stud_id = atoi(token);
                }
                if(strcmp(student.login_id, username) == 0 && strcmp(student.password, password) == 0){
                    isAuthenticated = 1;
                    break;
                }
            }
            close(studentDataFile);
            return isAuthenticated;
    }
    return 0;
}