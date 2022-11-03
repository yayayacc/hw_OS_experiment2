#include <unistd.h>
#include <stdio.h>
#include<sys/types.h>
int main()
{
int i=0;
 for(i=0;i<3;i++){
 pid_t fpid=fork();
 if(fpid==0)
printf("son     %d\n",getpid());
else{
 printf("father  %d \n",getpid());
}}
return 0;
}
