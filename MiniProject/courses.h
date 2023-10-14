// The Function below will help in adding the new course to facultycourse.txt
// I have created this function in order to create a link between Faculty
// and the course that he/she will be offering

void readFacultyCourses() {
    int facultyDataFile = open("facultycourse.txt", O_RDONLY);
    if (facultyDataFile == -1) {
        perror("Error opening facultycourses data file");
        return;
    }

    FacultyCourse facultyCourses;
    ssize_t bytesRead;

    while ((bytesRead = read(facultyDataFile, &facultyCourses, sizeof(FacultyCourse)) == sizeof(FacultyCourse))) {
        printf("Faculty ID: %d\n", facultyCourses.faculty_id);

        for (int i = 0; i < 30; i++) {
            if (facultyCourses.courses[i].courseId != 0) {
                printf("Course ID: %d\n", facultyCourses.courses[i].courseId);
                printf("Course Name: %s\n", facultyCourses.courses[i].courseName);
                printf("Department: %s\n", facultyCourses.courses[i].department);
                printf("Total Seats: %d\n", facultyCourses.courses[i].totalseats);
                printf("Credits: %d\n", facultyCourses.courses[i].credits);
                printf("Available Seats: %d\n", facultyCourses.courses[i].availableSeats);
            }
        }
    }

    close(facultyDataFile);
}


void addFacultyCouses(Course newCourse, char str[]){
    // First we will extract the faculty_id from the entered usernmae - 
    char* remaining;
    int intValue;

    if (strlen(str) >= 2 && strncmp(str, "PF", 2) == 0) {
        remaining = (char*)malloc(strlen(str) - 1); // Allocate memory for the remaining part
        if (remaining == NULL) {
            perror("Memory allocation failed");
            return ;
        }
        strcpy(remaining, str + 2);
        printf("Remaining: %s\n", remaining);

        // Convert "remaining" to an integer using atoi
        intValue = atoi(remaining);
        free(remaining); // Don't forget to free the allocated memory when done.
    } else {
        printf("String does not start with 'PF'\n");
    }

    // After we found the value of faculty_id and we the the new Course we will store it 
    // the asssociated file - 

    FacultyCourse existingFC;

    // Update the FacultyCourse structure with the new course
    existingFC.faculty_id = intValue; // Update faculty_id

    // Find an empty slot in the courses array to add the new course
    int emptySlot = -1;
    for (int i = 0; i < 30; i++) {
        if (existingFC.courses[i].courseId == 0) {
            emptySlot = i;
            break;
        }
    }

    if (emptySlot != -1) {
        existingFC.courses[emptySlot] = newCourse; // Add the new course
    } else {
        printf("No empty slot available for the new course.\n");
    }

    // Open the faculty course data file for writing
    int facultyDataFile = open("facultycourse.txt", O_WRONLY | O_TRUNC);
    if (facultyDataFile == -1) {
        perror("Error opening facultycourses data file");
        return;
    }

    // Write the updated FacultyCourse structure back to the file
    int flag = 1;
    ssize_t bytesWritten = write(facultyDataFile, &existingFC, sizeof(FacultyCourse));
    if (bytesWritten != sizeof(FacultyCourse)) {
        flag = 0;
        perror("Error writing updated data");
    }

    // Close the file
    close(facultyDataFile);

    readFacultyCourses();
}

//Function to add a Course with auto-incrementing course_id

void addCourse(Course newCourse, char username[]) {
    // Open the course data file for reading to find the highest course_id
    int maxCourseId = 0; // Initialize to 0

    int courseDataFile = open("courses.txt", O_RDONLY);
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

        // Initialize the student structure
        memset(&course, 0, sizeof(Course));

        // Extract and fill the fields of the student structure
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

    // Create or open the course data file for appending
    courseDataFile = open("courses.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (courseDataFile == -1) {
        perror("Error opening course data file");
        return;
    }

    if (maxCourseId == 0) {
        newCourse.courseId = 1;
    } else {
        newCourse.courseId = maxCourseId + 1;
    }

    // Format the student data with "$" delimiter
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

    // Write the formatted student data to the file
    ssize_t bytesWritten = write(courseDataFile, formattedData, dataLength);
    if (bytesWritten == dataLength) {
        printf("Course added successfully with course_id - : %d\n", newCourse.courseId);
    } else {
        printf("Error adding course.\n");
    }
    close(courseDataFile);

    // Now add the the respective details to the facultycourse.txt as well
    addFacultyCouses(newCourse, username);
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

void sendCourseDetails(int clientSocket, char username[]){
    int courseDataFile = open("facultycourse.txt", O_RDONLY);
    if (courseDataFile == -1) {
        perror("Error opening course data file");
        return;
    }

}