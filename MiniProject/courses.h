// The Function below will help in adding the new course to facultycourse.txt
// I have created this function in order to create a link between Faculty
// and the course that he/she will be offering

void readFacultyCourses() {
    int facultyDataFile = open("facultycourse.txt", O_RDONLY);
    if (facultyDataFile == -1) {
        perror("Error opening faculty course data file");
        return;
    }

    FacultyCourse fc;
    ssize_t bytesRead;
    int recordCount = 0; // To keep track of the number of valid records

    // Read and print FacultyCourse data
    while ((bytesRead = read(facultyDataFile, &fc, sizeof(FacultyCourse))) == sizeof(FacultyCourse)) {
        // Check if the faculty_id is valid
        if (fc.faculty_id != 0) {
            recordCount++; // Increment the record count

            // Process the read FacultyCourse structure (print or store it as needed)
            printf("Faculty ID: %d\n", fc.faculty_id);

            // Print the courses taught by this faculty
            for (int i = 0; i < 1; i++) { // Iterate through all possible courses
                if (fc.courses[i].courseId != 0) {
                    printf("Course %d - Name: %s, Department: %s\n", i + 1, fc.courses[i].courseName, fc.courses[i].department);
                }
            }
        }
    }

    // Close the file
    close(facultyDataFile);

    // Print the total number of valid records read
    printf("Total records read: %d\n", recordCount);
}


void addFacultyCourses(Course newCourse, char str[]) {
    // Extract the faculty_id from the entered username
    char* remaining;
    int intValue;

    if (strlen(str) >= 2 && strncmp(str, "PF", 2) == 0) {
        remaining = (char*)malloc(strlen(str) - 1); // Allocate memory for the remaining part
        if (remaining == NULL) {
            perror("Memory allocation failed");
            return;
        }
        strcpy(remaining, str + 2);
        printf("Remaining: %s\n", remaining);

        // Convert "remaining" to an integer using atoi
        intValue = atoi(remaining);
        free(remaining); // Don't forget to free the allocated memory when done.
    } else {
        printf("String does not start with 'PF'\n");
        return;
    }

    // Initialize a FacultyCourse structure
    FacultyCourse newFC;
    newFC.faculty_id = intValue;
    newFC.courses[0] = newCourse; // Add the new course to the first slot

    // Open the faculty course data file for reading and writing, with appending
    int facultyDataFile = open("facultycourse.txt", O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (facultyDataFile == -1) {
        perror("Error opening faculty course data file");
        return;
    }


    // Set the file position at the end of the file and check if it's successful
    if (lseek(facultyDataFile, 0, SEEK_END) == -1) {
        perror("Error setting file position");
        close(facultyDataFile);
        return;
    }

    // Write the new FacultyCourse structure to the file
    ssize_t bytesWritten = write(facultyDataFile, &newFC, sizeof(FacultyCourse));
    if (bytesWritten != sizeof(FacultyCourse)) {
        perror("Error writing data");
    } else {
        printf("Faculty course data added successfully.\n");
    }

    // Close the file
    close(facultyDataFile);
    readFacultyCourses(); // Make sure to read and display the updated data
}


//Function to add a Course with auto-incrementing course_id

void addCourse(Course newCourse, char username[]) {
    int courseDataFile;
    int maxCourseId = 0; // Initialize to 0

    // Open the course data file for reading to find the highest course_id
    courseDataFile = open("courses.txt", O_RDONLY);
    if (courseDataFile == -1) {
        perror("Error opening course data file");
        return;
    }

    char line[256];
    ssize_t bytesRead;

    while ((bytesRead = read(courseDataFile, line, sizeof(line))) > 0) {
        // Tokenize the line based on "$" delimiter
        char* token = strtok(line, "$");
        Course course;

        // Initialize the course structure
        memset(&course, 0, sizeof(Course));

        // Extract and fill the fields of the course structure
        if (token) {
            strncpy(course.courseName, token, sizeof(course.courseName));
            token = strtok(NULL, "$");
        }
        if (token) {
            strncpy(course.department, token, sizeof(course.department));
            token = strtok(NULL, "$");
        }
        if (token) {
            course.totalseats = atoi(token);
            token = strtok(NULL, "$");
        }
        if (token) {
            course.availableSeats = atoi(token);
            token = strtok(NULL, "$");
        }
        if (token) {
            course.credits = atoi(token);
            token = strtok(NULL, "$");
        }
        if (token) {
            course.courseId = atoi(token);
        }

        if (course.courseId > maxCourseId) {
            maxCourseId = course.courseId;
        }
    }

    close(courseDataFile);

    // Create or open the course data file for appending with file locking
    courseDataFile = open("courses.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (courseDataFile == -1) {
        perror("Error opening course data file");
        return;
    }

    // Lock the file for exclusive access
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(courseDataFile, F_SETLKW, &lock) == -1) {
        perror("Error locking file");
        close(courseDataFile);
        return;
    }

    if (maxCourseId == 0) {
        newCourse.courseId = 1;
    } else {
        newCourse.courseId = maxCourseId + 1;
    }

    // Format the course data with "$" delimiter
    char formattedData[256];
    snprintf(formattedData, sizeof(formattedData), "%s$%s$%d$%d$%d$%d\n",
             newCourse.courseName,
             newCourse.department,
             newCourse.totalseats,
             newCourse.availableSeats,
             newCourse.credits,
             newCourse.courseId);

    // Calculate the length of the formatted data
    size_t dataLength = strlen(formattedData);

    // Write the formatted course data to the file
    ssize_t bytesWritten = write(courseDataFile, formattedData, dataLength);
    if (bytesWritten == dataLength) {
        printf("Course added successfully with course_id - : %d\n", newCourse.courseId);
    } else {
        printf("Error adding course.\n");
    }

    // Unlock the file
    lock.l_type = F_UNLCK;
    fcntl(courseDataFile, F_SETLK, &lock);

    close(courseDataFile);

    // Now add the respective details to the facultycourse.txt as well
    addFacultyCourses(newCourse, username);
}

// Fucnction to modify the student details from the given stud_id by the client - 
void updateCourseDetails(int clientSocket, int courseId) {
    FILE *courseFile = fopen("courses.txt", "r+");
    if (courseFile == NULL) {
        perror("Error opening course data file");
        send(clientSocket, "Course not found", sizeof("Course not found"), 0);
        return;
    }

    int courseFound = 0;
    Course tempCourse;
    Course course;

    FILE *tempFile = tmpfile();  // Create a temporary file for storing modified data

    // Read and process each line in the file
    while (fscanf(courseFile, "%49[^$]$%19[^$]$%d$%d$%d$%d\n",
                  course.courseName, course.department, &course.totalseats,
                  &course.credits, &course.availableSeats, &course.courseId) == 6) {
        if (course.courseId == courseId) {
            // Store the existing course data
            tempCourse = course;
            courseFound = 1;

            // Receive the modified course details from the client
            ssize_t bytesRead = recv(clientSocket, &course, sizeof(Course), 0);
            if (bytesRead <= 0) {
                printf("Error receiving modified course details.\n");
                break;
            }
        }
        // Write the course data to the temporary file
        fprintf(tempFile, "%s$%s$%d$%d$%d$%d\n",
                course.courseName, course.department, course.totalseats,
                course.credits, course.availableSeats, course.courseId);
    }

    // Close the original course data file
    fclose(courseFile);

    // Copy the contents of the temporary file back to the original file
    courseFile = fopen("courses.txt", "w");
    if (courseFile == NULL) {
        perror("Error opening course data file");
        send(clientSocket, "Error updating course details", sizeof("Error updating course details"), 0);
        fclose(tempFile);
        return;
    }

    rewind(tempFile);
    int c;
    while ((c = fgetc(tempFile)) != EOF) {
        fputc(c, courseFile);
    }

    fclose(tempFile);
    fclose(courseFile);

    // Send the updated course details or a not found message to the client
    if (courseFound) {
        send(clientSocket, "Course details updated successfully", sizeof("Course details updated successfully"), 0);
    } else {
        send(clientSocket, "Course not found", sizeof("Course not found"), 0);
    }
}

void sendCourseDetails(int clientSocket, char str[]) {
    int courseDataFile = open("facultycourse.txt", O_RDONLY);
    if (courseDataFile == -1) {
        perror("Error opening facultycourse data file");
        return;
    }

    char* remaining;
    int intValue;

    if (strlen(str) >= 2 && strncmp(str, "PF", 2) == 0) {
        remaining = (char*)malloc(strlen(str) - 1); // Allocate memory for the remaining part
        if (remaining == NULL) {
            perror("Memory allocation failed");
            close(courseDataFile);
            return;
        }
        strcpy(remaining, str + 2);
        printf("Remaining: %s\n", remaining);

        // Convert "remaining" to an integer using atoi
        intValue = atoi(remaining);
        free(remaining); // Don't forget to free the allocated memory when done.
    } else {
        printf("String does not start with 'PF'\n");
        close(courseDataFile);
        return;
    }

    FacultyCourse fc;
    ssize_t bytesRead;

    // Read and print FacultyCourse data
    while ((bytesRead = read(courseDataFile, &fc, sizeof(FacultyCourse)) > 0)) {
        if (bytesRead != sizeof(FacultyCourse)) {
            perror("Error reading faculty course data");
            break;
        }

        if (fc.faculty_id == intValue) {
            // Send the course details for this faculty to the client
            for (int i = 0; i < 30; i++) { // Iterate through all possible courses
                if (fc.courses[i].courseId != 0) {
                    // Prepare a formatted message with course details
                    char courseDetails[256];
                    snprintf(courseDetails, sizeof(courseDetails), "Course ID: %d, Name: %s, Department: %s\n",
                             fc.courses[i].courseId, fc.courses[i].courseName, fc.courses[i].department);
                    
                    // Send the course details to the client
                    send(clientSocket, courseDetails, strlen(courseDetails), 0);
                }
            }
            break; // No need to continue searching after finding the faculty
        }
    }

    // Close the file
    close(courseDataFile);
}
