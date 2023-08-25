#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define RECORD_SIZE sizeof(struct Record)

struct Record {
    int id;
    char data[50];
};

int main() {
    const char *filename = "records_file.dat";

    int fd = open(filename, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    
    int record_offset = 0;  
    struct Record record;

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = record_offset * RECORD_SIZE;
    lock.l_len = RECORD_SIZE;

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error acquiring write lock");
        close(fd);
        return 1;
    }

    lseek(fd, record_offset * RECORD_SIZE, SEEK_SET);
    read(fd, &record, RECORD_SIZE);

    record.id++;
    snprintf(record.data, sizeof(record.data), "Modified Data");

    lseek(fd, record_offset * RECORD_SIZE, SEEK_SET);
    write(fd, &record, RECORD_SIZE);

    lock.l_type = F_UNLCK;

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error releasing write lock");
        close(fd);
        return 1;
    }

    printf("Record %d updated.\n", record.id);

    close(fd);

    return 0;
}