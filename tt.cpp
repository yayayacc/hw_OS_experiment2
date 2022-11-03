#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;
int p1,p2;
void kill(int a){  
   kill(p1,10);   //向P1发送中断信号10
   kill(p2,12);   //向P2发送中断信号12
}
void child_stop_1(int a) { 
   cout<<"Child Processl1 is Killed byParent!"<<endl; 
   exit(0); 
} 
void child_stop_2(int a) { 
   cout<<"Child Processl2 is Killed byParent!"<<endl; 
   exit(0); 
} 
int main(){
   p1 = fork();
   if ( p1 == 0 ) {
        signal(SIGINT,SIG_IGN);
        signal(10,child_stop_1);
        sleep(5);
   }
   else{
        p2 = fork();
        if ( p2 == 0 ){
            signal(SIGINT,SIG_IGN);
            signal(12,child_stop_2);
            sleep(5);
        }
        else{
            signal(SIGINT,kill);
            sleep(5);
            wait(0);      //等待子进程全部退出
            cout<<"This is parent process is Killed!"<<endl;
    	}
   }
}

