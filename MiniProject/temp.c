#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define file paths for user and course data
const char* userFile = "users.txt";
const char* courseFile = "courses.txt";

// User structure
typedef struct {
    char username[50];
    char password[50];
    char role[50];
} User;

// Course structure
typedef struct {
    char courseName[50];
    int availableSeats;
    // Add more course-related information here

} Course;

// Function to display the reqired information on the basis of role
void loadUsers(User users[], int* userCount) {
    FILE* file = fopen(userFile, "r");
    if (file == NULL) {
        *userCount = 0;
        return;
    }

    *userCount = 0;
    while (fscanf(file, "%s %s %s", users[*userCount].username, users[*userCount].password, users[*userCount].role) != EOF) {
        (*userCount)++;
    }
    fclose(file);
}

// Function to save user data to file
void saveUsers(User users[], int userCount) {
    FILE* file = fopen(userFile, "w");
    if (file == NULL) {
        printf("Error saving user data\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s %s\n", users[i].username, users[i].password, users[i].role);
    }
    fclose(file);
}

// Function to load course data from file
void loadCourses(Course courses[], int* courseCount) {
    FILE* file = fopen(courseFile, "r");
    if (file == NULL) {
        *courseCount = 0;
        return;
    }

    *courseCount = 0;
    while (fscanf(file, "%s %d", courses[*courseCount].courseName, &courses[*courseCount].availableSeats) != EOF) {
        (*courseCount)++;
    }
    fclose(file);
}

// Function to save course data to file
void saveCourses(Course courses[], int courseCount) {
    FILE* file = fopen(courseFile, "w");
    if (file == NULL) {
        printf("Error saving course data\n");
        return;
    }

    for (int i = 0; i < courseCount; i++) {
        fprintf(file, "%s %d\n", courses[i].courseName, courses[i].availableSeats);
    }
    fclose(file);
}

// Main function for user authentication and menu
int main() {
    // Load user data from file
    User users[100]; // Adjust the array size as needed
    int userCount = 0;
    loadUsers(users, &userCount);

    // Load course data from file
    Course courses[100]; // Adjust the array size as needed
    int courseCount = 0;
    loadCourses(courses, &courseCount);

    // Implement user authentication, role-based menu, and data management here

    // Save user and course data to file when necessary
    saveUsers(users, userCount);
    saveCourses(courses, courseCount);

    return 0;
}
