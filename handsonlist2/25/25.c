/*
============================================================================
Name : 25.c
Author : Shashank Mittra
Description : Write a program to print a message queue's  (use msqid_ds and ipc_perm structures)    a. access permission    b. uid, gid    c. time of last message sent and received       d. time of last change in the message queue    d. size of the queue    f. number of messages in the queue     g. maximum number of bytes allowed     h. pid of the msgsnd and msgrcv
Date: 6th Oct, 2023.
============================================================================
*/
#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>

void main(void) {
    key_t key;
    int msqid;
    key = ftok(".", 97);
    msqid = msgget(key, 0666 | IPC_CREAT);
    struct msqid_ds ms_info;
    msgctl(msqid, IPC_STAT, &ms_info);
    struct ipc_perm permissions = ms_info.msg_perm;
    printf("access permission = %d\n", permissions.mode);
    printf("uid = %d ", permissions.uid);
    printf("gid = %d\n", permissions.gid);
    printf("time of last message sent = %ld\n", ms_info.msg_stime);
    printf("time of last message received = %ld\n", ms_info.msg_rtime);
    printf("time of last change in the message queue = %ld\n", ms_info.msg_ctime);
    printf("number of messages in the queue = %ld\n", ms_info.msg_qnum);
    printf("maximum number of bytes allowed= %ld\n", ms_info.msg_qbytes);
    printf("pid of the msgsnd = %d\n", ms_info.msg_lspid);
    printf("pid of the msgrcv = %d\n", ms_info.msg_lrpid);
}