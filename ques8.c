// Ques - 8 : Write a program to open a file in read only mode, read line by line and display each line as it is read.
// Close the file when end of file is reached.

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    FILE *fp = fopen("file1.txt", "r");
    if(fp == NULL){
        perror("Error while opening the file");
        return 1;
    }
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    if (!feof(fp)) {
        perror("Error reading from file");
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}