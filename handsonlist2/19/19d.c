/*
============================================================================
Name : 19d
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

int main() {
    if (mknod("my_fifo_syscall", __S_IFIFO | 0666, 0) == -1) {
        perror("mknod");
        exit(EXIT_FAILURE);
    }

    printf("FIFO created using mknod system call.\n");
    return 0;
}