/*
============================================================================
Name : 1
Author : Shashank Mittra
Description : Create the following types of a files using (i) shell command (ii) system call a. soft link (symlink system call)
b. hard link (link system call)
c. FIFO (mkfifo Library Function or mknod system call)
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    const char* sourcePath = "dest";
    const char* softLinkPath = "soft_dest";
    const char* hardLinkPath = "hard_dest";
    const char* fifoPath = "fifo_dest";

    symlink(sourcePath, softLinkPath);
    printf("Soft link created: %s\n", softLinkPath);

    link(sourcePath, hardLinkPath);
    printf("Hard link created: %s\n", hardLinkPath);

    mode_t fifoMode = S_IFIFO | 0666;
    mknod(fifoPath, fifoMode, 0);
    printf("FIFO created: %s\n", fifoPath);

    return 0;
}



