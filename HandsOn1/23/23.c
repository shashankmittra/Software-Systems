/*
============================================================================
Name : 23
Author :Shashank Mittra
Description : Write a program to create a Zombie state of the running program.
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t pid = fork();
    if(pid > 0)
    {
        // This is parent and we do not have to close it
        sleep(20);
    }
    else
    {
        // Here we are exiting from the child before the parent does.
        // Hence creatig a zombie process. The parent does not know about it.
        return 0;
    }
    return 0;
}
