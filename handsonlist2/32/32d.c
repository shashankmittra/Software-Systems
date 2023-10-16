/*
============================================================================
Name : 32d.c
Author : Shashank Mittra
Description : Write a program to implement semaphore to protect any critical section.    a. rewrite the ticket number creation program using semaphore    b. protect shared memory from concurrent write access    c. protect multiple pseudo resources ( may be two) using counting semaphore    d. remove the created semaphore  
Date: 6th Oct, 2023.
============================================================================
*/

#include <stdio.h>
#include <semaphore.h>
#include <sys/sem.h>
sem_t semaphore;

int main() {
    int value;
    sem_init(&semaphore, 0, 1);
    sem_getvalue(&semaphore, &value);

    printf("Semphore created successfully : %d\n",value);

    sem_destroy(&semaphore);

    printf("Semphore destroyed successfully : %d\n",value);


    return 0;
}