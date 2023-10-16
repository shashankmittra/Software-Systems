/*
============================================================================
Name : 26.c
Author : Shashank Mittra
Description : Write a program to send messages to the message queue. Check $ipcs -q 
Date: 6th Oct, 2023.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define MAX 10
struct msg_buffer {
    long msg_type;
    char msg_text[100];
}msg;

int main() {
    key_t key;
    int msgid;
    key = ftok(".", 97);
    msgid = msgget(key, 0666 | IPC_CREAT);
    msg.msg_type = 1;
    printf("Enter Data : ");
    fgets(msg.msg_text, MAX, stdin);
    msgsnd(msgid, &msg, sizeof(msg), 0);
    printf("Data sent to the message queue : %s \n", msg.msg_text);
    execlp("ipcs", "ipcs", "-q", NULL);
    return 0;
}