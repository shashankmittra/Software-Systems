/*
============================================================================
Name : 14
Author : Shashank Mittra
Description : Write a program to find the type of a file.
a. Input should be taken from command line.
b. program should be able to identify any type of a file.
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fp>\n", argv[0]);
        return 1;
    }

    const char *fp = argv[1];
    struct stat file_info;

    if (stat(fp, &file_info) == -1) {
        perror("Error in stat");
        return 1;
    }

    if (S_ISREG(file_info.st_mode)) {
        printf("%s is a regular file.\n", fp);
    } 
    else if (S_ISDIR(file_info.st_mode)) {
        printf("%s is a directory.\n", fp);
    } 
    else if (S_ISCHR(file_info.st_mode)) {
        printf("%s is a character device.\n", fp);
    } 
    else if (S_ISBLK(file_info.st_mode)) {
        printf("%s is a block device.\n", fp);
    } 
    else if (S_ISFIFO(file_info.st_mode)) {
        printf("%s is a FIFO (named pipe).\n", fp);
    } 
    else if (S_ISLNK(file_info.st_mode)) {
        printf("%s is a symbolic link.\n", fp);
    } 
    else if (S_ISSOCK(file_info.st_mode)) {
        printf("%s is a socket.\n", fp);
    } 
    else {
        printf("%s is of unknown type.\n", fp);
    }

    return 0;
}
