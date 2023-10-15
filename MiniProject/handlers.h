#include "authenticate.h"
#include "faculty.h"
#include "student.h"
#include "courses.h"


//----------------------------------Below are some helper functions which further calls the actual functions to handle DB----------------------------------

// I have done this so as to provide more modularity and reusability of my code

void handleAddStudent(int newSocket) {
    char studentData[256];
    ssize_t bytesRead = recv(newSocket, studentData, sizeof(studentData), 0);

    if (bytesRead <= 0) {
        perror("Error receiving student details from client");
        return;
    }

    // Tokenize the received data based on "$" delimiter
    char* token = strtok(studentData, "$");

    if (token == NULL) {
        perror("Invalid student data received from client");
        return;
    }

    // Parse the received data into the Student structure
    Student newStudent;
    memset(&newStudent, 0, sizeof(Student)); // Initialize the structure

    // Extract and fill the fields of the Student structure
    strncpy(newStudent.name, token, sizeof(newStudent.name));
    token = strtok(NULL, "$");

    if (token != NULL) {
        newStudent.age = atoi(token);
        token = strtok(NULL, "$");
    } else {
        perror("Invalid student data received from client");
        return;
    }

    if (token != NULL) {
        strncpy(newStudent.email_id, token, sizeof(newStudent.email_id));
        token = strtok(NULL, "$");
    } else {
        perror("Invalid student data received from client");
        return;
    }

    if (token != NULL) {
        strncpy(newStudent.address, token, sizeof(newStudent.address));
    } else {
        perror("Invalid student data received from client");
        return;
    }

    // Add the student
    addStudent(newStudent);

    // Notify the client about the operation's success
    send(newSocket, "Student added successfully", sizeof("Student added successfully"), 0);
}

// Function to handle adding of Faculties - 
void handleAddFaculty(int newSocket) {
    char facultyData[256];
    ssize_t bytesRead = recv(newSocket, facultyData, sizeof(facultyData), 0);

    if (bytesRead <= 0) {
        perror("Error receiving faculty details from client");
        return;
    }

    // Tokenize the received data based on "$" delimiter
    char* token = strtok(facultyData, "$");

    if (token == NULL) {
        perror("Invalid faculty data received from client");
        return;
    }

    // Parse the received data into the faculty structure
    Faculty newFaculty;
    memset(&newFaculty, 0, sizeof(Faculty)); // Initialize the structure

    // Extract and fill the fields of the faculty structure
    strncpy(newFaculty.name, token, sizeof(newFaculty.name));
    token = strtok(NULL, "$");

    if (token != NULL) {
        strncpy(newFaculty.department, token, sizeof(newFaculty.department));
        token = strtok(NULL, "$");
    } else {
        perror("Invalid faculty data received from client");
        return;
    }

    if (token != NULL) {
        strncpy(newFaculty.designation, token, sizeof(newFaculty.designation));
        token = strtok(NULL, "$");
    } else {
        perror("Invalid faculty data received from client");
        return;
    }

    if (token != NULL) {
        strncpy(newFaculty.email_id, token, sizeof(newFaculty.email_id));
        token = strtok(NULL, "$");
    } else {
        perror("Invalid faculty data received from client");
        return;
    }

    if (token != NULL) {
        strncpy(newFaculty.address, token, sizeof(newFaculty.address));
    } else {
        perror("Invalid student data received from client");
        return;
    }

    // Add the student
    addFaculty(newFaculty);

    // Notify the client about the operation's success
    send(newSocket, "Student added successfully", sizeof("Student added successfully"), 0);
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
        // Now we have the received stud_id, and you can use it as needed.
        // For example, we can call a function to send student details based on stud_id:
        sendStudentDetailsByStudId(clientSocket, stud_id);
    }
}

// Function to view Faculty details
void handleViewFacultyDeatils(int clientSocket) {
    
    // Receive the faculty_id sent by the client
    char facultyId[10]; // Assuming faculty_id is a string
    ssize_t bytesReceived = recv(clientSocket, facultyId, sizeof(facultyId), 0);

    if (bytesReceived <= 0) {
        printf("No faculty_id received.\n");
        // Handle no faculty_id received or client disconnection if needed
    } else {
        // Remove trailing newline character if present
        facultyId[strcspn(facultyId, "\n")] = '\0';
        printf("Received faculty_id: %s\n", facultyId);
        int faculty_Id = atoi(facultyId);
        // Now we have the received faculty_id, and you can use it as needed.
        // For example, we can call a function to send student details based on faculty_id
        sendFacultyDetailsByFacutyId(clientSocket, faculty_Id);
    }
}

// Handler function to Modify the student details based on the student id - 
void handleModifyStudentDetails(int clientSocket) {
    char studId[10]; // Assuming stud_id is a string
    ssize_t bytesReceived = recv(clientSocket, studId, sizeof(studId), 0);

    if (bytesReceived <= 0) {
        printf("No stud_id received.\n");
        // Handle no stud_id received or client disconnection if needed
        return;
    } else {
        // Remove trailing newline character if present
        studId[strcspn(studId, "\n")] = '\0';
        int stud_id = atoi(studId); // Convert the string to an integer
        printf("Received stud_id: %d\n", stud_id);
        // Now we have the received stud_id as an integer, and you can use it as needed.
        // For example, we can call a function to modify student details based on stud_id:
        send(clientSocket, "Received stud_id", sizeof("Received stud_id"), 0);
        modifyStudentDetailsByStudId(clientSocket, stud_id);
    }
}

// Handler function to Modify the faculty details based on the student id - 
void handleModifyFacultyDeatils(int clientSocket){

}

// Handler function to Add New Course - 
void handleAddCourse(int newSocket, char username[]){
    char courseData[256];
    ssize_t bytesRead = recv(newSocket, courseData, sizeof(courseData), 0);

    if (bytesRead <= 0) {
        perror("Error receiving course details from client");
        return;
    }

    // Tokenize the received data based on "$" delimiter
    char* token = strtok(courseData, "$");

    if (token == NULL) {
        perror("Invalid course data received from client");
        return;
    }

    // Parse the received data into the Course structure
    Course newCourse;
    memset(&newCourse, 0, sizeof(Course)); // Initialize the structure

    // Extract and fill the fields of the Student structure
    strncpy(newCourse.courseName, token, sizeof(newCourse.courseName));
    token = strtok(NULL, "$");

    if (token != NULL) {
        strncpy(newCourse.department, token, sizeof(newCourse.department));
    } else {
        perror("Invalid course data received from client");
        return;
    }

    if (token != NULL) {
        newCourse.totalseats = atoi(token);
        token = strtok(NULL, "$");
    } else {
        perror("Invalid course data received from client");
        return;
    }

    if (token != NULL) {
        newCourse.availableSeats = atoi(token);
        token = strtok(NULL, "$");
    } else {
        perror("Invalid course data received from client");
        return;
    }

    if (token != NULL) {
        newCourse.credits = atoi(token);
        token = strtok(NULL, "$");
    } else {
        perror("Invalid course data received from client");
        return;
    }

    // Add the student
    addCourse(newCourse, username);

    // Notify the client about the operation's success
    send(newSocket, "Course added successfully", sizeof("Course added successfully"), 0);
}

void handleViewOfferingCourses(int clientSocket) {
    char callrecv[10];
    printf("HEllo\n");
    ssize_t bytesReceived = recv(clientSocket, callrecv, sizeof(callrecv), 0);
    printf("YES!\n");
    if (bytesReceived <= 0)
    {
        printf("Some issue while receiving the signal from the client.\n");
        // Handle no signal received or client disconnection if needed
        return;
    }
    else if (strncmp(callrecv, "Call", 4) == 0)
    {
        // Now we have the received signal to view offering courses

        char facultyId[256];
        bytesReceived = recv(clientSocket, facultyId, sizeof(facultyId), 0);

        if (bytesReceived <= 0) {
            printf("Error receiving faculty_id from the client.\n");
            // Handle no faculty_id received or client disconnection if needed
            return;
        } else {
            // Now you can call the sendCourseDetails function based on the received faculty_id.
            sendCourseDetails(clientSocket, facultyId);
        }
    }
}


void handleRemoveCourse(int newSocket){
}

void handleUpdateCourseDetails(int clientSocket){
    char courseId[10]; // Assuming stud_id is a string
    ssize_t bytesReceived = recv(clientSocket, courseId, sizeof(courseId), 0);

    if (bytesReceived <= 0) {
        printf("No courseId received.\n");
        // Handle no courseId received or client disconnection if needed
        return;
    } else {
        // Remove trailing newline character if present
        courseId[strcspn(courseId, "\n")] = '\0';
        int course_id = atoi(courseId); // Convert the string to an integer
        printf("Received courseId: %d\n", course_id);
        // Now we have the received courseId as an integer, and you can use it as needed.
        // For example, we can call a function to modify student details based on courseId:
        send(clientSocket, "Received courseId", sizeof("Received courseId"), 0);
        updateCourseDetails(clientSocket, course_id);
    }
}

void handleChangeFacultyPass(int newSocket){
}