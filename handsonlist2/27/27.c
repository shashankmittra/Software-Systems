/*
============================================================================
Name : 27.c
Author : Shashank Mittra
Description : Write a program to receive messages from the message queue.    a. with 0 as a flag    b. with IPC_NOWAIT as a flag
Date: 6th Oct, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX 10
struct msg_buffer {
    long msg_type;
    char msg_text[100];
}msg;

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;

    key = ftok("progfile", 65);

    msgid = msgget(key, 0666 | IPC_CREAT);
    
    strcpy(msg.msg_text, "0");
    msg.msg_type = 1;

    msgsnd(msgid, &msg, sizeof(msg), 0);


    msgrcv(msgid, &message, sizeof(message), 1, 0);
    printf("Received message (0 flag): %s\n", message.msg_text);

    strcpy(msg.msg_text, "IPC_NOWAIT");
    msg.msg_type = 1;

    msgsnd(msgid, &msg, sizeof(msg), 0);

    if (msgrcv(msgid, &message, sizeof(message), 1, IPC_NOWAIT) == -1) {
        perror("msgrcv (IPC_NOWAIT)");
        exit(EXIT_FAILURE);
    } else {
        printf("Received message (IPC_NOWAIT flag): %s\n", message.msg_text);
    }

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
