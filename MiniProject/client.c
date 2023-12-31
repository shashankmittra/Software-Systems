#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <termios.h>

#define MAX_PASSWORD_LENGTH 100
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

typedef struct {
    char courseName[50];
    char department[20];
    int totalseats;
    int credits;
    int availableSeats;
    int courseId;
} Course;

// Link structure
typedef struct {
    int stud_id;
    int faculty_id;
    Course courses[30];
} Link;

typedef struct {
    int stud_id;
    Course courses[30];
} StudentCourse;

typedef struct {
    int faculty_id;
    Course courses[30];
} FacultyCourse;

void addStudent(int clientSocket) {
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

    // Remove the trailing newline characters
    strtok(newStudent.name, "\n");
    strtok(newStudent.email_id, "\n");
    strtok(newStudent.address, "\n");

    // Format the student data with "$" delimiter
    char formattedData[256];
    snprintf(formattedData, sizeof(formattedData), "%s$%d$%s$%s",
            newStudent.name,
            newStudent.age,
            newStudent.email_id,
            newStudent.address);

    // Send the formatted data to the server
    send(clientSocket, formattedData, strlen(formattedData), 0);

    // Receive and display the server's response
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    printf("Server response: %s\n", buffer);
}


void addFaculty(int clientSocket) {
    Faculty newfaculty;
    memset(&newfaculty, 0, sizeof(Faculty)); // Initialize the structure
    printf("Enter faculty name: ");
    fgets(newfaculty.name, sizeof(newfaculty.name), stdin);
    printf("Enter faculty department: ");
    fgets(newfaculty.department, sizeof(newfaculty.department), stdin);
    printf("Enter faculty designation: ");
    fgets(newfaculty.designation, sizeof(newfaculty.designation), stdin);
    printf("Enter faculty email_id: ");
    fgets(newfaculty.email_id, sizeof(newfaculty.email_id), stdin);
    printf("Enter faculty address: ");
    fgets(newfaculty.address, sizeof(newfaculty.address), stdin);

    // Remove the trailing newline characters
    strtok(newfaculty.name, "\n");
    strtok(newfaculty.department, "\n");
    strtok(newfaculty.designation, "\n");
    strtok(newfaculty.email_id, "\n");
    strtok(newfaculty.address, "\n");

    // Format the student data with "$" delimiter
    char formattedData[256];
    snprintf(formattedData, sizeof(formattedData), "%s$%s$%s$%s$%s",
            newfaculty.name,
            newfaculty.department,
            newfaculty.designation,
            newfaculty.email_id,
            newfaculty.address);

    // Send the formatted data to the server
    send(clientSocket, formattedData, strlen(formattedData), 0);

    // Receive and display the server's response
    char buffer[256];
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

// Function to view details of a specific faculty based on faculty_id
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

// Function to modify details of a specific student based on stud_id
void modifyStudentDetails(int serverSocket) {
    char studId[10]; // Assuming stud_id is a string
    Student student;

    printf("Enter the stud_id of the student you want to modify the details of: ");
    scanf("%s", studId); // Read stud_id as a string

    // Send the entered stud_id to the server
    send(serverSocket, studId, strlen(studId), 0);

    // Receive a message from the server
    char response[256];
    ssize_t bytesRead = recv(serverSocket, response, sizeof(response), 0);

    if (bytesRead <= 0) {
        printf("Error receiving server response.\n");
        return;
    }

    // Check if the student was found or not
    if (strncmp(response, "Student not found", sizeof(response)) == 0) {
        printf("No student found with the provided stud_id.\n");
    } else if (strncmp(response, "Error", sizeof(response)) == 0) {
        printf("Error updating student details on the server.\n");
    } else {
        // Now, you can prompt the user for the modified student details
        // and send them to the server for updating.

        printf("Enter new name: ");
        scanf("%s", student.name);

        printf("Enter new age: ");
        scanf("%d", &student.age);

        printf("Enter new email_id: ");
        scanf("%s", student.email_id);

        printf("Enter new address: ");
        scanf("%s", student.address);

        // Send the modified student structure to the server for updating
        send(serverSocket, &student, sizeof(Student), 0);

        // Receive and display the server's response
        memset(response, 0, sizeof(response));
        bytesRead = recv(serverSocket, response, sizeof(response), 0);

        if (bytesRead <= 0) {
            printf("Error receiving server response.\n");
            return;
        }

        printf("Server response: %s\n", response);
    }
}


// Function to modify details of a specific student based on stud_id
void modifyFacultyDetails(int serverSocket) {

}

//----------------------------------Below are the functinalitites related to Faculty-------------------------------//

void viewOfferingCourses(int serverSocket, char username[]) {
    // Send the faculty_id (username) to the server
    send(serverSocket, username, strlen(username), 0);

    // Receive and process course details from the server
    FacultyCourse facultyCourse;
    ssize_t bytesRead = recv(serverSocket, &facultyCourse, sizeof(FacultyCourse), 0);

    if (bytesRead <= 0) {
        printf("Error receiving course details from the server.\n");
        return;
    }

    printf("Received course details for faculty ID: %d\n", facultyCourse.faculty_id);

    // Print or process course details here
    printf("Courses offered by faculty ID %d:\n", facultyCourse.faculty_id);
    for (int i = 0; i < 1; i++) { // Iterate through all possible courses
        if (facultyCourse.courses[i].courseId != 0) {
            printf("Course %d - Name: %s, Department: %s\n", i + 1, facultyCourse.courses[i].courseName, facultyCourse.courses[i].department);
        }
    }
}


void addNewCourse(int serverSocket){
    Course courses;
    memset(&courses, 0, sizeof(Course)); // Initialize the structure
    printf("Enter Course name: ");
    fgets(courses.courseName, sizeof(courses.courseName), stdin);
    printf("Enter course department: ");
    fgets(courses.department, sizeof(courses.department), stdin);
    printf("Enter tatal Seats: ");
    scanf("%d", &courses.totalseats);
    getchar(); // Consume newline character
    printf("Enter available seats: ");
    scanf("%d", &courses.availableSeats);
    getchar(); // Consume newline character
    printf("Enter Course credits: ");
    scanf("%d", &courses.credits);
    getchar(); // Consume newline character

    // Remove the trailing newline characters
    strtok(courses.courseName, "\n");
    strtok(courses.department, "\n");;

    // Format the student data with "$" delimiter
    char formattedData[256];
    snprintf(formattedData, sizeof(formattedData), "%s$%s$%d$%d$%d",
            courses.courseName,
            courses.department,
            courses.totalseats,
            courses.availableSeats,
            courses.credits);

    // Send the formatted data to the server
    send(serverSocket, formattedData, strlen(formattedData), 0);

    // Receive and display the server's response
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    recv(serverSocket, buffer, sizeof(buffer), 0);
    printf("Server response: %s\n", buffer);
}

void removeCourse(int serverSocket){
    int courseId;
    
    // Prompt the user to enter the course ID to delete
    printf("Enter the course ID to delete: ");
    if (scanf("%d", &courseId) != 1) {
        printf("Invalid input. Please enter a valid course ID.\n");
        return;
    }
    
    // Send the course ID to the server for removal
    send(serverSocket, &courseId, sizeof(int), 0);

    char responseMessage[256];
    ssize_t bytesRead = recv(serverSocket, responseMessage, sizeof(responseMessage), 0);

    if (bytesRead <= 0) {
        printf("Error receiving server response.\n");
    } else {
        printf("Server response: %s\n", responseMessage);
    }

    // Close the server socket and perform any other necessary cleanup
    close(serverSocket);
}

void updateCourseDetails(int serverSocket) {
    char courseId[10]; // Assuming courseId is a string
    Course course;

    printf("Enter the courseId of the course you want to modify the details of: ");
    scanf("%s", courseId); // Read courseId as a string

    // Send the entered courseId to the server
    send(serverSocket, courseId, strlen(courseId), 0);

    // Receive a message from the server
    char response[256];
    ssize_t bytesRead = recv(serverSocket, response, sizeof(response), 0);

    if (bytesRead <= 0) {
        printf("Error receiving server response.\n");
        return;
    }

    // Check if the course was found or not
    if (strncmp(response, "Course not found", sizeof(response)) == 0) {
        printf("No course found with the provided courseId.\n");
    } else if (strncmp(response, "Error", sizeof(response)) == 0) {
        printf("Error updating course details on the server.\n");
    } else {
        // Now, you can prompt the user for the modified course details
        // and send them to the server for updating.

        printf("Enter new course name: ");
        scanf("%s", course.courseName);

        printf("Enter new department: ");
        scanf("%s", course.department);

        printf("Enter new total seats: ");
        scanf("%d", &course.totalseats);

        printf("Enter new credits: ");
        scanf("%d", &course.credits);

        printf("Enter new available seats: ");
        scanf("%d", &course.availableSeats);

        // Send the modified course structure to the server for updating
        send(serverSocket, &course, sizeof(Course), 0);

        // Receive and display the server's response
        memset(response, 0, sizeof(response));
        bytesRead = recv(serverSocket, response, sizeof(response), 0);

        if (bytesRead <= 0) {
            printf("Error receiving server response.\n");
            return;
        }

        printf("Server response: %s\n", response);
    }
}

void changeFacultyPassword(int serverSocket, char username[]) {
    char newPassword[50];

    // Send the username to the server
    printf("Username: %s\n", username);
    send(serverSocket, username, strlen(username), 0);

    // Prompt the user for the new password
    printf("Enter the new password: ");
    fgets(newPassword, sizeof(newPassword), stdin);

    // Remove the newline character from the input
    newPassword[strcspn(newPassword, "\n")] = '\0';

    // Send the new password to the server
    send(serverSocket, newPassword, strlen(newPassword), 0);
    
    char terminationSignal[5] = "Done";
    send(serverSocket, terminationSignal, strlen(terminationSignal), 0);

    // Handle the server response
    char response[256];
    ssize_t bytesRead = recv(serverSocket, response, sizeof(response), 0);

    if (bytesRead <= 0) {
        printf("Error receiving server response.\n");
    } else {
        printf("Server response: %s\n", response);
    }
}



//----------------------------------Below are the functinalitites related to Student-------------------------------//

void viewAllCourses(int serverSocket){
    const char* request = "ViewAllCourses";
    send(serverSocket, request, strlen(request), 0);

    // Receive and print course data from the server
    while (1) {
        Course course;
        ssize_t bytesRead = recv(serverSocket, &course, sizeof(Course), 0);
        if (bytesRead <= 0) {
            break; // End of course data
        }
        if (memcmp(&course, &(Course){0}, sizeof(Course)) == 0) {
            break; // End marker received
        }
        printf("Course: %s\n", course.courseName);
        // You can access other course fields as needed
    }
}

void enrollNewCourse(int serverSocket, char username[]){
    int courseId;
    
    // Prompt the user to enter the course ID to enroll into
    printf("Enter the course ID that you want to enroll into : ");
    if (scanf("%d", &courseId) != 1) {
        printf("Invalid input. Please enter a valid course ID.\n");
        return;
    }
    
    // Send the course ID to the server for removal
    send(serverSocket, &courseId, sizeof(int), 0);

    printf("Username: %s\n", username);
    send(serverSocket, username, strlen(username), 0);

    char responseMessage[256];
    ssize_t bytesRead = recv(serverSocket, responseMessage, sizeof(responseMessage), 0);

    if (bytesRead <= 0) {
        printf("Error receiving server response.\n");
    } else {
        printf("Server response: %s\n", responseMessage);
    }

    // Close the server socket and perform any other necessary cleanup
    close(serverSocket);
}

void dropCourse(int serverSocket, char username[]){
    int courseId;
    
    // Prompt the user to enter the course ID to drop from
    printf("Enter the course ID that you want to drop from : ");
    if (scanf("%d", &courseId) != 1) {
        printf("Invalid input. Please enter a valid course ID.\n");
        return;
    }
    
    // Send the course ID to the server for removal
    send(serverSocket, &courseId, sizeof(int), 0);

    printf("Username: %s\n", username);
    send(serverSocket, username, strlen(username), 0);

    char responseMessage[256];
    ssize_t bytesRead = recv(serverSocket, responseMessage, sizeof(responseMessage), 0);

    if (bytesRead <= 0) {
        printf("Error receiving server response.\n");
    } else {
        printf("Server response: %s\n", responseMessage);
    }

    // Close the server socket and perform any other necessary cleanup
    close(serverSocket);
}

void viewEnrolledCourseDetails(int serverSocket, char username[]){
    send(serverSocket, username, strlen(username), 0);

    // Receive and process course details from the server
    StudentCourse studentCourse;
    ssize_t bytesRead = recv(serverSocket, &studentCourse, sizeof(StudentCourse), 0);

    if (bytesRead <= 0) {
        printf("Error receiving course details from the server.\n");
        return;
    }

    printf("Received course details for Student ID: %d\n", studentCourse.stud_id);

    // Print or process course details here
    printf("Courses enrolled by stud_id - %d:\n", studentCourse.stud_id);
    for (int i = 0; i < 1; i++) { // Iterate through all possible courses
        if (studentCourse.courses[i].courseId != 0) {
            printf("Course %d - Name: %s, Department: %s\n", i + 1, studentCourse.courses[i].courseName, studentCourse.courses[i].department);
            printf("Total Seats: %d\n", studentCourse.courses[i].totalseats);
            printf("Available Seats: %d\n", studentCourse.courses[i].availableSeats);
            printf("Credits: %d\n", studentCourse.courses[i].credits);
        }
    }
}

void changeStudentPassword(int serverSocket, char username[]) {
    char newPassword[50];

    // Send the username to the server
    printf("Username: %s\n", username);
    send(serverSocket, username, strlen(username), 0);

    // Prompt the user for the new password
    printf("Enter the new password: ");
    fgets(newPassword, sizeof(newPassword), stdin);

    // Remove the newline character from the input
    newPassword[strcspn(newPassword, "\n")] = '\0';

    // Send the new password to the server
    send(serverSocket, newPassword, strlen(newPassword), 0);
    
    char terminationSignal[5] = "Done";
    send(serverSocket, terminationSignal, strlen(terminationSignal), 0);

    // Handle the server response
    char response[256];
    ssize_t bytesRead = recv(serverSocket, response, sizeof(response), 0);

    if (bytesRead <= 0) {
        printf("Error receiving server response.\n");
    } else {
        printf("Server response: %s\n", response);
    }
}

char *readPassword() {
    struct termios oldt, newt;
    char *password = (char *)malloc(MAX_PASSWORD_LENGTH);
    if (password == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    // Disable echoing of characters
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Read the password
    if (fgets(password, MAX_PASSWORD_LENGTH, stdin) == NULL) {
        perror("Error reading password");
        exit(1);
    }

    // Remove the newline character
    char *newline = strchr(password, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }

    // Restore the terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return password;
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char username[100];
    //char password[100];
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
        printf("Enter role 1) for Admin, 2) for Faculty, 3) for Student): 4) Exit the system\n");
        fgets(userRole, sizeof(userRole), stdin);

        if(atoi(userRole) == 4){
            printf("Thank You for using the Course Regsitration Portal\n");
            break;
        }

        // Sending the userRole to the server ->
        write(clientSocket, userRole, strlen(userRole));
        printf("Enter login_id -> ");
        fgets(username, sizeof(username), stdin);

        // Sending the username to the server ->
        write(clientSocket, username, strlen(username));
        printf("Enter password -> ");
        //fgets(password, sizeof(password), stdin);
        char *password = readPassword();
        // Sending the password to the server ->
        write(clientSocket, password, strlen(password));
        // Recivig the authentication information based on the server
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);

        if(strcmp(buffer, "Authenticated") == 0){
            if (atoi(userRole) == 1) {
                // Collect student data from the client
                while(1){
                    
                    char adminMenu[] = "\n----------------------------- Welcome to Faculty Menu ------------------------------\n"
                                            "                             1. Add Student\n"
                                            "                             2. View Student Details\n"
                                            "                             3. Add Faculty\n"
                                            "                             4. View Faculty Details\n"
                                            "                             5. Modify Student Details\n"
                                            "                             6. Modify Faculty Details\n"
                                            "                             7. Logout and Exit\n"
                                            "Enter your choice: ";

                    // memset(buffer, 0, sizeof(buffer));
                    // recv(clientSocket, buffer, sizeof(buffer), 0);
                    // printf("%s\n", buffer);  // Display the received menu

                    printf("Sending menu: %s\n", adminMenu); 

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
                            modifyStudentDetails(clientSocket);
                            break;

                        case 6:
                            modifyFacultyDetails(clientSocket);
                            break;
                            
                        case 7:
                            // Exit
                            printf("Admin session ended.\n");
                            close(clientSocket);
                            break;

                        default:
                            printf("Invalid choice. Please try again.\n");
                            close(clientSocket);
                            break;
                    }

                    if(adminChoice == 7){
                            break;
                    }
                }
            }
            
            // When user role == 2 Then the user is a Faculty - 
            else if(atoi(userRole) == 2){
                while(1){
                    
                    char facultyMenu[] = "\n----------------------------- Welcome to Faculty Menu ------------------------------\n"
                                            "                             1. View Offering Course\n"
                                            "                             2. Add new Course\n"
                                            "                             3. Remove courses from the Catalog\n"
                                            "                             4. Update Course Details\n"
                                            "                             5. Change Password\n"
                                            "                             6. Logout and Exit\n"
                                            "Enter your choice: ";
                    printf("Sending menu: %s\n", facultyMenu); 

                    // memset(buffer, 0, sizeof(buffer));
                    // recv(clientSocket, buffer, sizeof(buffer), 0);
                    // printf("%s\n", buffer);  // Display the received menu

                    int facultyChoice;
                    scanf("%d", &facultyChoice);
                    char choiceStr[10];
                    snprintf(choiceStr, sizeof(choiceStr), "%d", facultyChoice);
                    send(clientSocket, choiceStr, strlen(choiceStr), 0);
                    getchar();

                    switch (facultyChoice) {
                        case 1:
                            // Add Student functionality
                            viewOfferingCourses(clientSocket, username);
                            break;
                            
                        case 2:
                            addNewCourse(clientSocket);
                            break;

                        case 3:
                            removeCourse(clientSocket);
                            break;

                        case 4:
                            updateCourseDetails(clientSocket);
                            break;
                        
                        case 5:
                            changeFacultyPassword(clientSocket, username);
                            break;
                            
                        case 6:
                            // Exit
                            printf("Student session ended.\n");
                            close(clientSocket);
                            break;

                        default:
                            printf("Invalid choice. Please try again.\n");
                            close(clientSocket);
                            break;
                    }

                    if(facultyChoice == 6){
                            break;
                    }
                }
            }


            // When user role == 3 Then the user is a student - 
            else if(atoi(userRole) == 3){
                while(1){
                    char studentMenu[] = "\n----------------------------- Welcome to Student Menu ------------------------------\n"
                                        "                             1. View All Courses\n"
                                        "                             2. Enroll in a New Course\n"
                                        "                             3. Drop a Course\n"
                                        "                             4. View Enrolled Course Details\n"
                                        "                             5. Change Password\n"
                                        "                             6. Logout and Exit\n"
                                        "Enter your choice: ";
                printf("Sending menu: %s\n", studentMenu);  



                    int studChoice;
                    scanf("%d", &studChoice);
                    char choiceStr[10];
                    snprintf(choiceStr, sizeof(choiceStr), "%d", studChoice);
                    send(clientSocket, choiceStr, strlen(choiceStr), 0);
                    getchar();

                    switch (studChoice) {
                        case 1:
                            // Add Student functionality
                            viewAllCourses(clientSocket);
                            break;
                            
                        case 2:
                            enrollNewCourse(clientSocket, username);
                            break;

                        case 3:
                            dropCourse(clientSocket, username);
                            break;

                        case 4:
                            viewEnrolledCourseDetails(clientSocket, username);
                            break;
                        
                        case 5:
                            changeStudentPassword(clientSocket, username);
                            break;
                            
                        case 6:
                            // Exit
                            printf("Student session ended.\n");
                            close(clientSocket);
                            break;

                        default:
                            printf("Invalid choice. Please try again.\n");
                            close(clientSocket);
                            break;
                    }

                    if(studChoice == 6){
                            break;
                    }
                }
            }
        }
        else {
            printf("Authentication failed. Invalid username or password.\n");
        }
    }
    close(clientSocket);
    return 0;
}
