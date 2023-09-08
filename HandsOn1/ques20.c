// Ques - 20 :: Find out the priority of your running program. Modify the priority with nice command.

#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

int main(int argc, char const *argv[])
{
    pid_t pid = getpid();
    int my_prio = getpriority(PRIO_PROCESS, pid);
    printf("The nice value of my process is - %d", my_prio);
    nice(10);
    my_prio = getpriority(PRIO_PROCESS, pid);
    printf("The updated nice value of my process is - %d", my_prio);
    return 0;
}

