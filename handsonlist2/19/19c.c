/*
============================================================================
Name : 19c
Author : Shashank Mittra
Description : Create a FIFO file by
a. mknod command
b. mkfifo command
c. use strace command to find out, which command (mknod or mkfifo) is better.
c. mknod system call
d. mkfifo library function
Date: 3rd Oct, 2023.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    mknod("mknod", S_IFIFO | 0666, 0);

    mkfifo("mkfifo", 0666);

    return 0;
}