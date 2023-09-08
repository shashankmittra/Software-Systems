// Ques 19 - Write a program to find out time taken to execute getpid system call. Use time stamp counter.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char const *argv[])
{
    unsigned long long start, end, diff;
    start = __rdtsc();
    
    end = __rdtsc();
    return 0;
}
