/*
============================================================================
Name : 30b.c
Author : Shashank Mittra
Description :Write a program to create a shared memory.    a. write some data to the shared memory    b. attach with O_RDONLY and check whether you are able to overwrite.    c. detach the shared memory    d. remove the shared memory
Date: 6th Oct, 2023.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>

void main() {
    int key, shmid;
    char *data;
    key=ftok("file", 97);
    shmid=shmget(key, 1024, 0);
    data=(char *)shmat(shmid, NULL, SHM_RDONLY);
    printf("Shared memory input : %s\n", data);
}