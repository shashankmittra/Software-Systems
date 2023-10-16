/*
============================================================================
Name : 312c.c
Author : Shashank Mittra
Description : Write a program to implement semaphore to protect any critical section.    a. rewrite the ticket number creation program using semaphore    b. protect shared memory from concurrent write access    c. protect multiple pseudo resources ( may be two) using counting semaphore    d. remove the created semaphore  
Date: 6th Oct, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define SHM_KEY 1234
#define SHM_SIZE 100

int shmid;
char *shm_ptr;
sem_t mutex;

void* writeToSharedMemory(void* arg) {
    char* message = (char*)arg;

    sem_wait(&mutex); // Wait for the semaphore

    // Write data to the shared memory
    strcpy(shm_ptr, message);

    sem_post(&mutex); // Release the semaphore

    pthread_exit(NULL);
}

int main() {
    // Create a shared memory segment
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);

    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment
    shm_ptr = shmat(shmid, NULL, 0);

    if (shm_ptr == (char *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Initialize the semaphore
    sem_init(&mutex, 0, 1);

    // Create threads to write to shared memory
    pthread_t thread1, thread2;
    char* message1 = "Message from Thread 1";
    char* message2 = "Message from Thread 2";

    pthread_create(&thread1, NULL, writeToSharedMemory, (void*)message1);
    printf("Content of shared memory: %s\n", shm_ptr);

    pthread_create(&thread2, NULL, writeToSharedMemory, (void*)message2);
    printf("Content of shared memory: %s\n", shm_ptr);


    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Print the content of the shared memory
    printf("Content of shared memory: %s\n", shm_ptr);

    // Detach the shared memory
    shmdt(shm_ptr);

    return 0;
}