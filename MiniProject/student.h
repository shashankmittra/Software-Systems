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
        // Tokenize the line based on "$" delimiter
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

        if (student.stud_id > maxStudId) {
            maxStudId = student.stud_id;
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
    } else {
        newStudent.stud_id = maxStudId + 1;
    }

    // Create the login ID by appending "MT" to the stud_id
    snprintf(newStudent.login_id, sizeof(newStudent.login_id), "MT%d", newStudent.stud_id);

    // Generate a temporary password (you can implement your own logic)
    snprintf(newStudent.password, sizeof(newStudent.password), "1234");

    // Format the student data with "$" delimiter
    char formattedData[256];
    snprintf(formattedData, sizeof(formattedData), "%s$%s$%s$%d$%s$%s$%d\n",
             newStudent.login_id,
             newStudent.password,
             newStudent.name,
             newStudent.age,
             newStudent.email_id,
             newStudent.address,
             newStudent.stud_id);

    // Calculate the length of the formatted data
    size_t dataLength = strlen(formattedData);

    // Write the formatted student data to the file
    ssize_t bytesWritten = write(studentDataFile, formattedData, dataLength);
    if (bytesWritten == dataLength) {
        printf("Student added successfully with stud_id: %d\n", newStudent.stud_id);
    } else {
        printf("Error adding student.\n");
    }
    close(studentDataFile);
}

// Function to send details of a specific student based on stud_id
void sendStudentDetailsByStudId(int clientSocket, int stud_id) {
    // Open the student data file for reading
    int studentDataFile = open("student.txt", O_RDONLY);
    if (studentDataFile == -1) {
        perror("Error opening student data file");
        return;
    }

    char buffer[1024];
    ssize_t bytesRead;
    int studentFound = 0;

    // Inside the loop that reads student data from the file
    while ((bytesRead = readLine(studentDataFile, buffer, sizeof(buffer))) > 0) {
        // Tokenize the data based on "$" delimiter
        char* token = strtok(buffer, "$");
        Student student;

        // Initialize the student structure
        memset(&student, 0, sizeof(Student));

        // Extract and fill the fields of the student structure
        if (token) {
            strcpy(student.login_id, token);
            token = strtok(NULL, "$");
        }
        if (token) {
            strcpy(student.password, token);
            token = strtok(NULL, "$");
        }
        if (token) {
            strcpy(student.name, token);
            token = strtok(NULL, "$");
        }
        if (token) {
            student.age = atoi(token);
            token = strtok(NULL, "$");
        }
        if (token) {
            strcpy(student.email_id, token);
            token = strtok(NULL, "$");
        }
        if (token) {
            strcpy(student.address, token);
            token = strtok(NULL, "$");
        }
        if (token) {
            student.stud_id = atoi(token);
        }

        // Check if the current student's stud_id matches the requested stud_id
        if (student.stud_id == stud_id) {
            // Send the student details to the client
            send(clientSocket, &student, sizeof(Student), 0);
            studentFound = 1; // Set the flag to indicate the student is found
            break;
        }
    }

    // Close the file
    close(studentDataFile);

    if (!studentFound) {
        // Handle the case where the student with the given stud_id was not found
        Student notFoundStudent;
        snprintf(notFoundStudent.login_id, sizeof(notFoundStudent.login_id), "Student not found.");
        send(clientSocket, &notFoundStudent, sizeof(Student), 0);
    }
}

// Fucnction to modify the student details from the given stud_id by the client - 
void modifyStudentDetailsByStudId(int clientSocket, int stud_id) {
    FILE *studentFile = fopen("student.txt", "r+");
    if (studentFile == NULL) {
        perror("Error opening student data file");
        send(clientSocket, "Student not found", sizeof("Student not found"), 0);
        return;
    }

    int studentFound = 0;
    Student tempStudent;
    Student student;

    FILE *tempFile = tmpfile();  // Create a temporary file for storing modified data

    // Read and process each line in the file
    while (fscanf(studentFile, "%49[^$]$%49[^$]$%19[^$]$%d$%29[^$]$%99[^$]$%d\n",
                  student.login_id, student.password, student.name, &student.age,
                  student.email_id, student.address, &student.stud_id) == 7) {
        if (student.stud_id == stud_id) {
            // Store the existing student data
            tempStudent = student;
            studentFound = 1;

            // Receive the modified student details from the client
            ssize_t bytesRead = recv(clientSocket, &student, sizeof(Student), 0);
            if (bytesRead <= 0) {
                printf("Error receiving modified student details.\n");
                break;
            }
        }
        // Write the student data to the temporary file
        fprintf(tempFile, "%s$%s$%s$%d$%s$%s$%d\n",
                student.login_id, student.password, student.name, student.age,
                student.email_id, student.address, student.stud_id);
    }

    // Close the original student data file
    fclose(studentFile);

    // Copy the contents of the temporary file back to the original file
    studentFile = fopen("student.txt", "w");
    if (studentFile == NULL) {
        perror("Error opening student data file");
        send(clientSocket, "Error updating student details", sizeof("Error updating student details"), 0);
        fclose(tempFile);
        return;
    }

    rewind(tempFile);
    int c;
    while ((c = fgetc(tempFile)) != EOF) {
        fputc(c, studentFile);
    }

    fclose(tempFile);
    fclose(studentFile);

    // Send the updated student details or a not found message to the client
    if (studentFound) {
        send(clientSocket, "Student details updated successfully", sizeof("Student details updated successfully"), 0);
    } else {
        send(clientSocket, "Student not found", sizeof("Student not found"), 0);
    }
}