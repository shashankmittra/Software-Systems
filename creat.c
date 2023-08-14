#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int fd = creat("1.txt", 0644);
    if (fd == -1) {
        perror("Error creating file");
        return 1;
    }
    printf("File descriptor value: %d\n", fd);
    close(fd);  
    return 0;
}