// ques - 22 :: Write a program, open a file, call fork, and then write to the file by both the child as well as the parent processes. 
// Check output of the file

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    int fd = open("example.txt", O_WRONLY);
    pid_t pid = fork();
    if(pid > 0)
    {
        char buff[] = "Hi I am parent";
        write(fd, buff, strlen(buff));
    }
    else
    {
        char buff[] = "Hi I am child";
        write(fd, buff, strlen(buff));
    }
    close(fd);
    return 0;
}
