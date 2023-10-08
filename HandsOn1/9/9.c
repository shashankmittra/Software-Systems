/*
============================================================================
Name : 9
Author : Shashank Mittra
Description : Write a program to print the following information about a given file. a. inode
b. number of hard links
c. uid
d. gid
e. size
f. block size
g. number of blocks
h. time of last access
i. time of last modification j. time of last change

Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    char* filePath;
    printf("Enter the path to the file: ");
    scanf("%s", filePath);
    struct stat fileInfo;
    if (stat(filePath, &fileInfo) == 0) {
        printf("Inode: %llu\n", fileInfo.st_ino);
        printf("Number of hard links: %hu\n", fileInfo.st_nlink);
        printf("UID: %u\n", fileInfo.st_uid);
        printf("GID: %u\n", fileInfo.st_gid);
        printf("Size: %lld bytes\n", fileInfo.st_size);
        printf("Block size: %d bytes\n", fileInfo.st_blksize);
        printf("Number of blocks: %lld\n", fileInfo.st_blocks);
        printf("Time of last access: %s", ctime(&fileInfo.st_atime));
        printf("Time of last modification: %s", ctime(&fileInfo.st_mtime));
        printf("Time of last change: %s", ctime(&fileInfo.st_ctime));
    } else {
        printf("File not found.\n");
    }
    return 0;
}