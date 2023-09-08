// ques -5 : Write a program to create five new files with infinite loop. 
// Execute the program in the background and check the file descriptor table at /proc/pid/fd.

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {

    int fd1, fd2, fd3, fd4, fd5;

	fd1 = open("example1.txt", O_CREAT);
	if (fd1 == -1) {
         perror("Error creating file");
         return 1;
	}
    fd2 = open("example2.txt", O_CREAT);
	if (fd2 == -1) {
         perror("Error creating file");
         return 1;
	}
    fd3 = open("example3.txt", O_CREAT);
	if (fd3 == -1) {
         perror("Error creating file");
         return 1;
	}
    fd4 = open("example4.txt", O_CREAT);
	if (fd4 == -1) {
         perror("Error creating file");
         return 1;
	}
    fd5 = open("example5.txt", O_CREAT);
	if (fd5 == -1) {
         perror("Error creating file");
         return 1;
	}

    while(1)
    {
        
    }
    // We run the code in backgroound.
    printf("Files created successfully.\n");

    return 0; 
}