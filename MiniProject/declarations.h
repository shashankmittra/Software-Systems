#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <ctype.h>

// Setting the port to be 8080
// Can check the available ports using - lsof -i -n -P | grep TCP command 
#define PORT 8080
#define MAX_CLIENTS 5
#define MAX_COURSES_PER_STUDENT 30
#define MAX_LINE_LENGTH 256

// Define file paths for user and course data
const char* userFile = "users.txt";
const char* courseFile = "courses.txt";

// Below I am definig all the structures for all the required roles and admins.

// student structure
typedef struct {
    char login_id[50];
    char password[50];
    char name[20];
    int age; 
    char email_id[30];
    char address[100];
    int stud_id;
} Student;

// Faculty structure
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

// Admin Structure
// Admin in itself does not contain much information except for id and password

typedef struct {
    char login_id[50];
    char password[50];
} Admin;

// Course structure
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
    Course courses[30];
} StudentCourse;

typedef struct {
    int faculty_id;
    Course courses[30];
} FacultyCourse;
