#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    const int numFiles = 5;

    for (int i = 1; i <= numFiles; i++) {
        char filename[20];
        sprintf(filename, "file%d.txt", i);

        FILE *file = fopen(filename, "w");

        if (file == NULL) {
            printf("Failed to create %s.\n", filename);
            return 1; 
        }

        fclose(file);
    }

    printf("Files created successfully.\n");

    return 0; 
}