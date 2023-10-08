/*
============================================================================
Name : 13
Author : Shashank Mittra
Description : Write a program to wait for a STDIN for 10 seconds using select. 
Write a proper print statement to verify whether the data is available within 10 seconds or not (check in $man 2 select).
Date: 28th Aug, 2023.
============================================================================
*/

#include <sys/time.h>  
#include <sys/types.h> 
#include <unistd.h>    
#include <stdio.h>    

void main()
{
    fd_set readFDSet, writeFDSet;

    struct timeval time;

    int returnVal;

    FD_SET(STDIN_FILENO, &readFDSet); 
    FD_SET(STDOUT_FILENO, &writeFDSet); 

    time.tv_sec = 10;
    time.tv_usec = 0;

    returnVal = select(1, &readFDSet, &writeFDSet, NULL, &time);

    if (returnVal == -1)
        perror("ERROR ");
    else if (returnVal)
        printf("The data is now available\n");
    else
        printf("No data was given for 10 seconds\n");
}