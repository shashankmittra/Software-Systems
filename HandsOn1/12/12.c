/*
============================================================================
Name : 12
Author : Shashank Mittra
Description : Write a program to find out the opening mode of a file. Use fcntl.

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
    int fp = open("file1.txt", O_WRONLY);
    if(fp == -1){
        perror("Error while reading the file");
        return 1;
    }

    // get file status  - 

    int f = fcntl(fp, F_GETFL);
    printf("The value of f - %d", f);
    if (f == -1)
    {
        perror("Error while getting the status of the file");
        return 1;
    }
    else {
        if (f & O_RDONLY)
            printf("File opened in read-only mode\n");
        else if (f & O_RDWR)
            printf("File opened in read-write mode\n");
        else if (f & O_WRONLY)
            printf("File opened in write-only mode\n");
    }
    close(fp);
    return 0;
}
