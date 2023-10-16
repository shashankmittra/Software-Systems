/*
============================================================================
Name : 31a.c
Author : Shashank Mittra
Description :Write a program to create a semaphore and initialize value to the semaphore.    a. create a binary semaphore    b. create a counting semaphore
Date: 6th Oct, 2023.
============================================================================
*/

#include <stdio.h>
#include <semaphore.h>

int main() {
    sem_t binary_semaphore;

    sem_init(&binary_semaphore, 0, 1);

    printf("Binary semaphore created and initialized.\n");

    sem_destroy(&binary_semaphore);

    return 0;
}