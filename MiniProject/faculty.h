// Function to add a New Faculty ->

void addFaculty(Faculty newFaculty) {
    // Open the student data file for reading to find the highest stud_id
    int maxFacultId = 0; // Initialize to 0

    int facultyDataFile = open("faculty.txt", O_RDONLY);
    if (facultyDataFile == -1) {
        perror("Error opening faculty data file");
        return;
    }

    char line[256];
    ssize_t bytesRead;

    while ((bytesRead = read(facultyDataFile, line, sizeof(line))) > 0) {
        // Tokenize the line based on "$" delimiter
        char* token = strtok(line, "$");
        Faculty faculty;

        // Initialize the student structure
        memset(&faculty, 0, sizeof(Faculty));

        // Extract and fill the fields of the faculty structure
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

        if (faculty.faculty_id > maxFacultId) {
            maxFacultId = faculty.faculty_id;
        }
    }

    close(facultyDataFile);

    // Create or open the student data file for appending
    facultyDataFile = open("faculty.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (facultyDataFile == -1) {
        perror("Error opening faculty data file");
        return;
    }

    if (maxFacultId == 0) {
        newFaculty.faculty_id = 1;
    } else {
        newFaculty.faculty_id = maxFacultId + 1;
    }

    // Create the login ID by appending "MT" to the stud_id
    snprintf(newFaculty.login_id, sizeof(newFaculty.login_id), "MT%d", newFaculty.faculty_id);

    // Generate a temporary password (you can implement your own logic)
    snprintf(newFaculty.password, sizeof(newFaculty.password), "1234");

    // Format the student data with "$" delimiter
    char formattedData[256];
    snprintf(formattedData, sizeof(formattedData), "%s$%s$%s$%s$%s$%s$%s$%d\n",
             newFaculty.login_id,
             newFaculty.password,
             newFaculty.name,
             newFaculty.department,
             newFaculty.designation,
             newFaculty.email_id,
             newFaculty.address,
             newFaculty.faculty_id);

    // Calculate the length of the formatted data
    size_t dataLength = strlen(formattedData);

    // Write the formatted faculty data to the file
    ssize_t bytesWritten = write(facultyDataFile, formattedData, dataLength);
    if (bytesWritten == dataLength) {
        printf("Student added successfully with stud_id: %d\n", newFaculty.faculty_id);
    } else {
        printf("Error adding student.\n");
    }
    close(facultyDataFile);
}

// Function to send details of a specific student based on stud_id
void sendFacultyDetailsByFacutyId(int clientSocket, int faculty_id) {
    // Open the faculty data file for reading
    int facultyDataFile = open("faculty.txt", O_RDONLY);
    if (facultyDataFile == -1) {
        perror("Error opening faculty data file");
        return;
    }

    char buffer[1024];
    ssize_t bytesRead;
    int facultyFound = 0;

    // Inside the loop that reads faculty data from the file
    while ((bytesRead = readLine(facultyDataFile, buffer, sizeof(buffer))) > 0) {
        // Tokenize the data based on "$" delimiter
        char* token = strtok(buffer, "$");
        Faculty faculty;

        // Initialize the faculty structure
        memset(&faculty, 0, sizeof(Faculty));

        // Extract and fill the fields of the faculty structure
        if (token) {
            strcpy(faculty.login_id, token);
            token = strtok(NULL, "$");
        }
        if (token) {
            strcpy(faculty.password, token);
            token = strtok(NULL, "$");
        }
        if (token) {
            strcpy(faculty.name, token);
            token = strtok(NULL, "$");
        }
        if (token) {
            strcpy(faculty.department, token);
            token = strtok(NULL, "$");
        }
        if (token) {
            strcpy(faculty.designation, token);
            token = strtok(NULL, "$");
        }
        if (token) {
            strcpy(faculty.email_id, token);
            token = strtok(NULL, "$");
        }
        if (token) {
            strcpy(faculty.address, token);
            token = strtok(NULL, "$");
        }
        if (token) {
            faculty.faculty_id = atoi(token);
        }

        // Check if the current faculty's faculty_id matches the requested faculty_id
        if (faculty.faculty_id == faculty_id) {
            // Send the faculty details to the client
            send(clientSocket, &faculty, sizeof(Faculty), 0);
            facultyFound = 1; // Set the flag to indicate the faculty is found
            break;
        }
    }

    // Close the file
    close(facultyDataFile);

    if (!facultyFound) {
        // Handle the case where the faculty with the given faculty_id was not found
        Faculty notFoundFaculty;
        snprintf(notFoundFaculty.login_id, sizeof(notFoundFaculty.login_id), "Faculty not found.");
        send(clientSocket, &notFoundFaculty, sizeof(Faculty), 0);
    }
}