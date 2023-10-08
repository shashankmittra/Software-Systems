/*
============================================================================
Name : 30
Author : Shashank Mittra
Description : Write a program to run a script at a specific time using a Daemon process.
Date: 28th Aug, 2023.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<time.h>


int main(void)
{
	FILE *fp = fopen("example.txt", "w");
	if(!fork())
	{
		setsid();
		umask(0);
		chdir("/");
		while (1)
		{
			/* code */
			fprintf(fp, "Shashank\n");
			fflush(fp);
			sleep(5);
		}
	}
	else
	{
		exit(0);
	}
	return 0;
}