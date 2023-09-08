// ques : 27 - Write a program to execute ls -Rl by the following system calls a. execl
// b. execlp c. execle
// d. execv e. execvp

#include <stdio.h>
#include <unistd.h>

int main() {
  char *argv[] = {"ls", "-Rl", NULL};

  execl("/bin/ls", "ls", "-Rl", NULL);

  execlp("ls", "ls", "-Rl", NULL);

  execle("/bin/ls", "ls", "-Rl", NULL, NULL);

  execv("/bin/ls", argv);

  execvp("ls", argv);

  printf("Failed to execute ls -Rl\n");
  return 1;
}
