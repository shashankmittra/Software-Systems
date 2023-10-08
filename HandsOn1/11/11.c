/*
============================================================================
Name : 11
Author : Shashank Mittra
Description : Write a program to open a file, duplicate the file descriptor and append the file with
 both the descriptors and check whether the file is updated properly or not.
a. use dup b. use dup2 c. use fcntl
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    int fp = open("file1.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

    if (fp == -1) {
        perror("Error opening file");
        return 1;
    }

    int dup1 = dup(fp);
    if(dup1 == -1){
        perror("Error while duplicating using dup");
        return 1;
    }

    int dup2_fd = dup2(fp, 10);
    if(dup2_fd == -1){
        perror("Error while duplicating using dup2");
        return 1;
    }

    // The third argument just spefices that if our command runs successfully
    // then it will return an integer greater than the third argument
    int fcntl_fd = fcntl(fp, F_DUPFD, 0);
    if(fcntl_fd < 0){
        perror("Error while duplicating using fnctl");
        return 1;
    }

    const char *data = "Shashank Mittra.\n";

    write(fp, data, strlen(data)); // using orignal

     // Append data using the duplicated file descriptors
    write(dup1, data, strlen(data));
    write(dup2_fd, data, strlen(data));
    write(fcntl_fd, data, strlen(data));

    close(fp);
    close(dup1);
    close(dup2_fd);
    close(fcntl_fd);
    return 0;
}
