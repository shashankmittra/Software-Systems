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

        // Convert "remaining" to an integer using atoi
        intValue = atoi(remaining);
        free(remaining); // Don't forget to free the allocated memory when done.
    } else {
        printf("String does not start with 'PF'\n");
        return;
    }

    // Check if the student already has too many courses
    if (countCoursesForStudent(intValue) >= MAX_COURSES_PER_STUDENT) {
        printf("Student already has the maximum number of courses.\n");
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

    // Implement record-level locking using fcntl
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK; // Set a write lock
    lock.l_whence = SEEK_END;
    lock.l_start = 0;
    lock.l_len = sizeof(FacultyCourse);

    if (fcntl(facultyDataFile, F_SETLKW, &lock) == -1) {
        perror("Error setting file lock");
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

    // Release the lock
    lock.l_type = F_UNLCK;
    if (fcntl(facultyDataFile, F_SETLK, &lock) == -1) {
        perror("Error releasing file lock");
    }

    // Close the file
    close(facultyDataFile);
    readFacultyCourses(); // Make sure to read and display the updated data
}

int countCoursesForStudent(int studentId) {
    // Open the faculty course data file for reading
    int facultyDataFile = open("facultycourse.txt", O_RDONLY);
    if (facultyDataFile == -1) {
        perror("Error opening faculty course data file");
        return -1;
    }

    int courseCount = 0;

    // Initialize a FacultyCourse structure
    FacultyCourse facultyCourse;

    // Read and count courses for the student
    while (read(facultyDataFile, &facultyCourse, sizeof(FacultyCourse)) == sizeof(FacultyCourse)) {
        if (facultyCourse.faculty_id == studentId) {
            // Count the courses associated with this student
            for (int i = 0; i < MAX_COURSES_PER_STUDENT; i++) {
                if (facultyCourse.courses[i].courseId != -1) {
                    courseCount++;
                } else {
                    break;
                }
            }
        }
    }

    // Close the file
    close(facultyDataFile);

    return courseCount;
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

void sendCourseDetails(int serverSocket, char str[]) {
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
        if (fc.faculty_id != 0 && fc.faculty_id == intValue) {
            recordCount++; // Increment the record count

            // Send the faculty course details to the client
            send(serverSocket, &fc, sizeof(FacultyCourse), 0);
        }
    }

    // Close the file
    close(facultyDataFile);
}

void removeCourse(int courseId, int clientSocket) {
    // Create a temporary file descriptor
    int tempFile = open("tempcourses.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (tempFile == -1) {
        perror("Error creating temporary file");
        return;
    }

    int coursesFile = open("courses.txt", O_RDONLY);
    if (coursesFile == -1) {
        perror("Error opening courses data file");
        close(tempFile);
        return;
    }

    Course course;

    while (read(coursesFile, &course, sizeof(Course)) == sizeof(Course)) {
        if (course.courseId != courseId) {
            // Write the course to the temporary file (excluding the one to be deleted)
            if (write(tempFile, &course, sizeof(Course)) != sizeof(Course)) {
                perror("Error writing to temporary file");
                close(coursesFile);
                close(tempFile);
                return;
            }
        }
    }

    close(coursesFile);
    close(tempFile);

    // Remove the original courses.txt file
    if (remove("courses.txt") != 0) {
        perror("Error removing original courses file");
        return;
    }

    // Rename the temporary file to courses.txt
    if (rename("tempcourses.txt", "courses.txt") != 0) {
        perror("Error updating courses file");
        return;
    }

    // Send a response message to the client
    const char* responseMessage = "Course removed successfully.";
    send(clientSocket, responseMessage, strlen(responseMessage), 0);
}

void sendCourseData(int clientSocket) {
    FILE* coursesFile = fopen("courses.txt", "r");
    if (coursesFile == NULL) {
        perror("Error opening courses data file");
        return;
    }

    Course course;

    // Read and send course data to the client
    while (fscanf(coursesFile, "%[^$]$%[^$]$%d$%d$%d$%d\n",
                  course.courseName, course.department, &course.totalseats,
                  &course.credits, &course.availableSeats, &course.courseId) != EOF) {
        send(clientSocket, &course, sizeof(Course), 0);
    }

    // Close the file
    fclose(coursesFile);

    // Send an end-of-course marker
    Course endMarker;
    memset(&endMarker, 0, sizeof(Course));
    send(clientSocket, &endMarker, sizeof(Course), 0);
}



void addStudentCourses(Course newCourse, int clientSocket, char str[]){
    char* remaining;
    int intValue;

    if (strlen(str) >= 2 && strncmp(str, "MT", 2) == 0) {
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
        printf("String does not start with 'MT'\n");
        return;
    }

    // Initialize a FacultyCourse structure
    StudentCourse newFC;
    newFC.stud_id = intValue;
    newFC.courses[0] = newCourse; // Add the new course to the first slot

    // Open the faculty course data file for reading and writing, with appending
    int facultyDataFile = open("studentcourse.txt", O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (facultyDataFile == -1) {
        perror("Error opening studentcourse course data file");
        return;
    }

    // Set the file position at the end of the file and check if it's successful
    if (lseek(facultyDataFile, 0, SEEK_END) == -1) {
        perror("Error setting file position");
        close(facultyDataFile);
        return;
    }

    // Write the new StudentCourse structure to the file
    ssize_t bytesWritten = write(facultyDataFile, &newFC, sizeof(StudentCourse));
    if (bytesWritten != sizeof(StudentCourse)) {
        perror("Error writing data");
    } else {
        printf("Faculty course data added successfully.\n");
    }

    // Close the file
    close(facultyDataFile);
}

void enrollCourse(int courseId, int clientSocket, char username[]) {
    int coursesFile = open("courses.txt", O_RDWR);
    if (coursesFile == -1) {
        perror("Error opening courses data file");
        return;
    }

    Course tempCourse; // Temporary variable to store the course structure

    // Read and write each line of the file
    char line[256];
    ssize_t bytesRead;
    int found = 0;

    while ((bytesRead = read(coursesFile, line, sizeof(line))) > 0) {
        line[bytesRead] = '\0'; // Null-terminate the line

        // Parse the course structure from the line
        Course course;
        sscanf(line, "%49[^$]$%19[^$]$%d$%d$%d$%d",
               course.courseName, course.department,
               &course.totalseats, &course.credits,
               &course.availableSeats, &course.courseId);

        // Check if the courseId matches the one you're looking for
        if (course.courseId == courseId) {
            // Store the entire course structure in the tempCourse variable
            tempCourse = course;
            found = 1;
            break; // Exit the loop, as we found the course
        }
    }

    // Close and reopen the courses.txt file for writing
    close(coursesFile);
    coursesFile = open("courses.txt", O_WRONLY | O_TRUNC);
    if (coursesFile == -1) {
        perror("Error reopening courses data file");
        return;
    }

    if (found) {
        if (tempCourse.availableSeats > 0) {
            // Reduce the available seats by 1
            tempCourse.availableSeats--;

            // Write the updated course structure back to the file
            dprintf(coursesFile, "%s$%s$%d$%d$%d$%d\n",
                    tempCourse.courseName, tempCourse.department,
                    tempCourse.totalseats, tempCourse.credits,
                    tempCourse.availableSeats, tempCourse.courseId);

            // Close the courses.txt file
            close(coursesFile);

            // Enroll the student in the course (implement this function)
            addStudentCourses(tempCourse, clientSocket, username);

            // Send a response to the client
            const char* responseMessage = "Enrolled in the course successfully.";
            send(clientSocket, responseMessage, strlen(responseMessage), 0);
        } else {
            // Send a response to the client that the course is full
            const char* responseMessage = "Course is full. Cannot enroll.";
            send(clientSocket, responseMessage, strlen(responseMessage), 0);
        }
    } else {
        // Send a response to the client that the course was not found
        const char* responseMessage = "Course with courseId not found.";
        send(clientSocket, responseMessage, strlen(responseMessage), 0);
    }
}


void sendStudentCourseDetails(int serverSocket, char str[]) {
    char* remaining;
    int intValue;

    if (strlen(str) >= 2 && strncmp(str, "MT", 2) == 0) {
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
    
    int facultyDataFile = open("studentcourse.txt", O_RDONLY);
    if (facultyDataFile == -1) {
        perror("Error opening student course data file");
        return;
    }

    StudentCourse fc;
    ssize_t bytesRead;
    int recordCount = 0; // To keep track of the number of valid records

    // Read and print FacultyCourse data
    while ((bytesRead = read(facultyDataFile, &fc, sizeof(StudentCourse))) == sizeof(StudentCourse)) {
        // Check if the faculty_id is valid
        if (fc.stud_id != 0 && fc.stud_id == intValue) {
            recordCount++; // Increment the record count

            // Send the student course details to the client
            send(serverSocket, &fc, sizeof(FacultyCourse), 0);
        }
    }

    // Close the file
    close(facultyDataFile);
}

void dropCourse(int courseId, int clientSocket, char str[]) {
    char* remaining;
    int intValue;

    if (strlen(str) >= 2 && strncmp(str, "MT", 2) == 0) {
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
        printf("String does not start with 'MT'\n");
        return;
    }

    int stud_id = intValue;

    FILE* studentFile = fopen("studentcourse.txt", "r+");
    if (studentFile == NULL) {
        perror("Error opening student data file");
        return;
    }

    StudentCourse studentCourse;
    int found = 0;

    while (fread(&studentCourse, sizeof(StudentCourse), 1, studentFile) == 1) {
        if (studentCourse.stud_id == stud_id) {
            // Locate the StudentCourse entry for the given stud_id
            for (int i = 0; i < 1; i++) {
                // Search for the course with the specified courseId within the courses array
                if (studentCourse.courses[i].courseId == courseId) {
                    // Course found, remove it and update available seats
                    studentCourse.courses[i].courseId = 0;
                    studentCourse.courses[i].availableSeats++;

                    // Ensure available seats do not exceed total seats
                    if (studentCourse.courses[i].availableSeats > studentCourse.courses[i].totalseats) {
                        studentCourse.courses[i].availableSeats = studentCourse.courses[i].totalseats;
                    }

                    found = 1;
                    break;
                }
            }

            // Update the StudentCourse structure in the file
            fseek(studentFile, -sizeof(StudentCourse), SEEK_CUR);
            fwrite(&studentCourse, sizeof(StudentCourse), 1, studentFile);

            break;
        }
    }

    fclose(studentFile);

    if (found) {
        // Send a response message to the client
        const char* responseMessage = "Course dropped successfully.";
        send(clientSocket, responseMessage, strlen(responseMessage), 0);
    } else {
        // Send an error message to the client
        const char* errorMessage = "Course not found or enrollment failed.";
        send(clientSocket, errorMessage, strlen(errorMessage), 0);
    }
}
