/*
============================================================================
Name : 29
Author : Shashank Mittra
Description : Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO, SCHED_RR).
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <errno.h>

void find_curr_poli(int curr_schedu_poli){
    switch (curr_schedu_poli) {
        case SCHED_FIFO:
            printf("FIFO\n");
            break;
        case SCHED_RR:
            printf("RR\n");
            break;
        case SCHED_OTHER:
            printf("OTHER\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }
}

int main() {
    int pid = getpid();
    int curr_schedu_poli = sched_getscheduler(pid); 

    if (curr_schedu_poli == -1) {
        perror("Error: sched_getscheduler");
        exit(EXIT_FAILURE);
    }

    printf("Current scheduling policy: ");
    find_curr_poli(curr_schedu_poli);

    if (sched_setscheduler(pid, SCHED_RR, &(struct sched_param){.sched_priority = 99}) == -1) {
        perror("Error: sched_setscheduler");
        exit(EXIT_FAILURE);
    }
    pid = getpid();
    curr_schedu_poli = sched_getscheduler(pid);
    
    printf("\nScheduling policy changed Successfully\n\n");
    
    if (curr_schedu_poli == -1) {
        perror("Error: sched_getscheduler");
        exit(EXIT_FAILURE);
    }

    printf("New scheduling policy: ");
    find_curr_poli(curr_schedu_poli);

    return 0;
}