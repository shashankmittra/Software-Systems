/*
============================================================================
Name : 27.c
Author : Shashank Mittra
Description : Write a program to change the exiting message queue permission. (use msqid_ds structure)
Date: 6th Oct, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;
    struct msqid_ds msq_info;

    key = ftok("progfile", 65);

    msgid = msgget(key, 0666 | IPC_CREAT);

    msgctl(msgid, IPC_STAT, &msq_info);

    printf("Current permissions: %o\n", msq_info.msg_perm.mode);

    msq_info.msg_perm.mode = 0644;

    msgctl(msgid, IPC_SET, &msq_info);

    printf("Permissions changed to %o\n", msq_info.msg_perm.mode);

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}