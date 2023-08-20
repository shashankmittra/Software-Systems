//Ques - 15 : Write a program to display the environmental variable of the user (use environ).

#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main() {
    // Iterate through the environ array and print each environmental variable
    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }

    return 0;
}
