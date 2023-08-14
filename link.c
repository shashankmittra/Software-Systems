#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    printf("Enter the file name - ");
    int v = symlink("dest", "destSL");
    if(v < 0){
        perror("Failed");
        return 1;
    }
    int f = link("dest", "destHL");
    if(f<0){
        perror("Failed");
        return 1;
    }
    int e = mknod("destFIFO", S_IFIFO, 0);
    if(e < 0){
        perror("Failed");
    }
    return 0;
}

