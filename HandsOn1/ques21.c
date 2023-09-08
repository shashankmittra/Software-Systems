// ques 21 - Write a program, call fork and print the parent and child process id.

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
