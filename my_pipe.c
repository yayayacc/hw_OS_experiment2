#include  <unistd.h>                                                                                                       
#include  <signal.h>                                                                                                       
#include  <stdio.h>   
#include  <stdlib.h>   
#include  <sys/types.h>
#include  <wait.h>
int pid1,pid2;
int main()
{
    int fd[2];
    char InPipe[1000];
    char * c1 = "1" , * c2 = "2";
    pipe(fd);
    while((pid1 = fork()) == -1);
    if(pid1 == 0){
        //lockf(fd[1],1,0);
        //close(fd[0]);
        for(int i = 0;i < 5000;i++){
            write(fd[1],c1,1);
        }
        printf("Child 1 is sending message!\n");
        sleep(5);
        //lockf(fd[1],0,0);
        exit(0);
    }
    else{
        while((pid2 = fork()) == -1);
        if(pid2 == 0){
            //lockf(fd[1],1,0);
            //close(fd[0]);
            for(int i = 0;i < 5000 ;i++){
                write(fd[1] , c2 , 1);
            }
            printf("Child 2 is sending message!\n");
            sleep(5);
            //lockf(fd[1],0,0);
            exit(0);
        }
        else{
            wait(0);
            wait(0);
            for(int i = 0 ; i<10;i++){
                read(fd[0],InPipe,1000);
                printf(InPipe);
            }
            exit(0);
        }
    }
}