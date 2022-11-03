#include<stdio.h>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>

typedef void (*signal_handler)(int);

int wait_flag = 1;
void stop(int tem) 
{
	wait_flag = 0;
}


int main() 
{
	pid_t pid1,pid2;
	wait_flag = 1;
  

	while((pid1 = fork()) == -1);
	if(pid1>0) 
	{
		while((pid2 = fork()) == -1);
		if(pid2 > 0) 
		{
			wait_flag = 1;
            signal(SIGQUIT,stop);//signal(SIGINT,stop);
			signal(SIGALRM,SIG_IGN);//signal(SIGALRM,stop);
			alarm(5);//sleep(5);
            while(wait_flag == 1);
			kill(pid1,16);
			kill(pid2,17);
			wait(0);
			wait(0);
			printf("Parent process is killed !!\n");
			exit(0);
		}
		else 
		{
			wait_flag = 1;
			signal(SIGQUIT,SIG_IGN);//signal(SIGINT,SIG_IGN);
			signal(17,stop);
			while(wait_flag == 1);
			printf("Child process 2 is killed by parent !!\n");
			exit(0);
		}
	}
    else 
    {
		wait_flag = 1;
		signal(16,stop);
		signal(SIGQUIT,SIG_IGN);//signal(SIGINT,SIG_IGN);
		while(wait_flag == 1);
		printf("Child process 1 is killed by parent !!\n");
		exit(0);
	}
}

