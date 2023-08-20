// Ques - 7 : Write a program to copy file1 into file2 ($cp file1 file2)

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }
    const char *sourceFile = argv[1];
    const char *destFile = argv[2];
    int sourcefd = open(sourceFile, O_RDONLY);
    if(sourcefd == -1){
        perror("Error in opening source file");
        return 1;
    }
    int destfd = open(destFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(destfd == -1){
        perror("Error in opening destination file");
        close(sourcefd);
        return 1;
    }
    char buff[1024];
    ssize_t br;
    while((br = read(sourcefd, buff, sizeof(buff))) > 0){
        ssize_t bw = write(destfd, buff, br);
        if(bw == -1){
            perror("Error in writing to the file");
            close(sourcefd);
            close(destfd);
            return 1;
        }
    }
    if(br == -1){
        perror("Error in reading the file");
        close(sourcefd);
        close(destfd);
        return 1;
    }
    close(sourcefd);
    close(destfd);
    printf("File Copied Successfully");
    return 0;
}