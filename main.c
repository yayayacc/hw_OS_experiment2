#include"const.h"
#include"func.h"
#include"type.h"
#include<stdio.h>
#include<stdlib.h>
int main()
{
    char choice = 'a';      pid=0;
    free_block = init_free_block(mem_size); //初始化空闲区/*初始化空闲块，默认为一块，可以指定大小及起始地址*/

    while(1) {
        
        fflush(stdin);//将输入缓冲区的东西丢弃
        if(choice < '0' || choice > '5') display_menu();	//显示菜单
        choice=getchar();//printf("+++++%c-------\n",choice);	//获取用户输入
        switch(choice){
            case '1': set_mem_size(); break; 	//设置内存大小,仅改变了内存大小，起始地点没改变
            case '2': set_algorithm();flag=1; break;//设置算法，并且用算法重排空闲区链表
            case '3': new_process(); flag=1; break;//创建新进程，并且将其放在已分配内存块的pcb链表的第一个
            case '4': kill_process(); flag=1;   break;//删除进程，释放内存，并且从链表中删除
            case '5': display_mem_usage();    flag=1; break;	//显示内存使用
            case '0': do_exit(); exit(0);	//释放链表并退出
            default: break;      
        }
        fflush(stdin);    
    } 
}