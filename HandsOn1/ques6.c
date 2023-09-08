// Ques - 6 : Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    char buffer[1024];
    while(1){
        ssize_t r = read(STDIN_FILENO, buffer, sizeof(buffer));
        if(r == -1){
            perror("Error occured while reading");
            return 1;
        }
        if(r == 0){
            return 0;
        }
        ssize_t w = write(STDOUT_FILENO, buffer, r);
        if(w == -1){
            perror("Error occured while writing");
            return 1;
        }
    }  
    return 0;
}