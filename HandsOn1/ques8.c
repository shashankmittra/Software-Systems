#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    
    int fd_read = open("read.txt",O_RDONLY);

    if(fd_read==-1){
        printf(":(");
    }

    while(1){
        char buf;
        int char_read = read(fd_read,&buf,1);
        if(!char_read){
            close(fd_read);
            break;
        }
        printf("%c",buf);
    }


    return 0;
}