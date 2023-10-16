/*
============================================================================
Name : 29.c
Author : Shashank Mittra
Description : Write a program to remove the message queue
Date: 6th Oct, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msgid;

    // ftok to generate unique key
    key = ftok("progfile", 65);

    // msgget creates a message queue
    msgid = msgget(key, 0666 | IPC_CREAT);

    printf("Message queue id = %d\n", msgid);


    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    printf("Message queue removed.\n");

    return 0;
}