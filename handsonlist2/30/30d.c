/*
============================================================================
Name : 30d.c
Author : Shashank Mittra
Description :Write a program to create a shared memory.    a. write some data to the shared memory    b. attach with O_RDONLY and check whether you are able to overwrite.    c. detach the shared memory    d. remove the shared memory
Date: 6th Oct, 2023.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>

void main() {
    key_t key = ftok("file", 97);
    int shmid = shmget(key, 1024, 0);
    char *str = (char *)shmat(shmid, (void *)0, 0);
    printf("Data : %s\n", str);
    shmctl(shmid, IPC_RMID, NULL);
    printf("Destroyed shared memory... \n");
}