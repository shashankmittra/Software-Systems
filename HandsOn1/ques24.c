// ques : 24 - Write a program to create an orphan process.

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t pid = fork();
    printf("pid - %d", pid);
    if (pid > 0)
    {
        // This is parent process and we have to exit this before
        // the child
        return 0;
    }
    else
    {
        printf("ppid - %d", getppid());
        // Since the parent exits before the child.
        // Henceforth it will become an orphan process.
    }
    return 0;
}
