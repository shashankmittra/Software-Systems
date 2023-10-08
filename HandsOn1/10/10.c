/*
============================================================================
Name : 10
Author : Shashank Mittra
Description : Write a program to open a file with read write mode, write 10 bytes, 
move the file pointer by 10 bytes (use lseek) and write again 10 bytes.
a. check the return value of lseek
b. open the file with od and check the empty spaces in between the data.
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fp = open("file1.txt", O_RDWR);
    if (fp == -1) {
        perror("Error opening file");
        return 1;
    }
    //rewind move the fp to the begning of the file 
    // ftell() functions gives the postion of the file pointer
    char data1[] = "abcdefghij";
    write(fp, data1, 10);   
    off_t of = lseek(fp, 10, SEEK_SET);
    if (of == -1) {
        perror("Error seeking");
        close(fp);
        return 1;
    }

    printf("The value of the offset - ", of);

    char data2[] = "klmnopqrst";
    write(fp, data2, 10);
    close(fp);

    char buff[100];
    snprintf(buff, sizeof(buff), "od -c %s", "file1.txt");
    system(buff);

    return 0;
}