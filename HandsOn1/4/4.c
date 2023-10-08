/*
============================================================================
Name : 4
Author : Shashank Mittra
Description : Write a program to open an existing file with read write mode. Try O_EXCL flag also.
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// int main(int argc, char* argv[]) {
// 	int fd;

// 	fd = open("example.txt", O_CREAT);
// 	if (fd == -1) {
//          perror("Error creating file");
//          return 1;
// 	}
// 	return 0;
// }

int main(){
	int fd;
	fd = open("example.txt", O_CREAT | O_EXCL);
	if (fd == -1) {
        perror("Error creating file");
        return 1;
    }
	return 0;
}