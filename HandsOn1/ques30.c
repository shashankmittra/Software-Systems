#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<time.h>


int main(void)
{

    struct tm exectime;
    exectime.tm_hour = 1;
	exectime.tm_min = 38;
	
	if(fork()==0){
		setsid();
		umask(0);
		chdir("/");
		while(1){
		    time_t timevalue = time(NULL);
		    struct tm* currtime = localtime(&timevalue);

		    if(currtime->tm_hour == exectime.tm_hour && currtime->tm_min == exectime.tm_min){

                int result = system("/Users/shashankmittra/Desktop/IIITB/Sem-1/SoftwareSystems/practise/script.sh");
                if (result == -1) {
                    perror("system");
                }
                return 0;
                break;
		    }
	    }
	}else{
		exit(0);
	}
    return 0;
}