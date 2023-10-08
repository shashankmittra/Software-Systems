/*
============================================================================
Name : 21
Author : Shashank Mittra
Description : Write a program, call fork and print the parent and child process id.
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t f = fork();
  if (f > 0) 
  {
    printf("Parent process ID: %d\n", getpid());
  } 
  else 
  {
    printf("Child process ID: %d\n", getpid());
  }

  return 0;
}
