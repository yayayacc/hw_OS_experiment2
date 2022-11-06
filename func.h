#ifndef _FUNC_H
#define _FUNC_H

#include"type.h"
#include"const.h"
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

struct free_block_type * init_free_block(int mem_size);
void display_menu();
int set_mem_size();
void set_algorithm();
void rearrange(int algorithm);
int rearrange_FF();//
int rearrange_BF();//
int rearrange_WF();//
int new_process();
int allocate_mem(struct allocated_block *ab);//
void kill_process();
int free_mem(struct allocated_block *ab);//
int dispose(struct allocated_block *free_ab);
int display_mem_usage();
struct allocated_block * find_process(int pid);//
void do_exit();


struct free_block_type * init_free_block(int mem_size){
    struct free_block_type *fb;
    fb=(struct free_block_type *)malloc(sizeof(struct free_block_type));
    if(fb==NULL){
        printf("No mem\n");
        return NULL;
        }
    fb->size = mem_size;
    fb->start_addr = DEFAULT_MEM_START;
    fb->next = NULL;
    return fb;

}

void display_menu(){
    printf("\n");
    printf("1 - Set memory size (default=%d)\n", DEFAULT_MEM_SIZE);
    printf("2 - Select memory allocation algorithm\n");
    printf("3 - New process \n");
    printf("4 - Terminate a process \n");
    printf("5 - Display memory usage \n");
    printf("0 - Exit\n");

}

int set_mem_size(){
    int size;
    if(flag!=0){  //防止重复设置
        printf("Cannot set memory size again\n");
        return 0;
    }
    printf("Total memory size =");
    scanf("%d", &size);
    if(size>0) {
        mem_size = size;
        free_block->size = mem_size;
    }
    flag=1;  
    return 1;
}

void set_algorithm(){
    int algorithm;
    printf("\t1 - First Fit\n");
    printf("\t2 - Best Fit \n");
    printf("\t3 - Worst Fit \n");
    scanf("%d", &algorithm);
    if(algorithm>=1 && algorithm <=3)  
              ma_algorithm=algorithm;
	//按指定算法重新排列空闲区链表
    rearrange(ma_algorithm); 
}

void rearrange(int algorithm){
     switch(algorithm){
        case MA_FF:rearrange_FF(); break;
        case MA_BF:rearrange_BF(); break;
        case MA_WF:rearrange_WF(); break;
        }

}

int new_process(){
    struct allocated_block *ab;
    int size;    int ret;
    ab=(struct allocated_block *)malloc(sizeof(struct allocated_block));
    if(!ab) exit(-5);
    ab->next = NULL;
    pid++;//静态变量，所有allocated_block公用一个
    sprintf(ab->process_name, "PROCESS-%02d", pid);
    ab->pid = pid;    
    printf("Memory for %s:", ab->process_name);
    scanf("%d", &size);
    if(size>0) ab->size=size;
    ret = allocate_mem(ab);  /* 从空闲区分配内存，ret==1表示分配ok*/
    /*如果此时allocated_block_head尚未赋值，则赋值*/
    if((ret==1) &&(allocated_block_head == NULL)){ 
        allocated_block_head=ab;
        return 1;        }
    /*分配成功，将该已分配块的描述插入已分配链表*/
    else if (ret==1) {
        ab->next=allocated_block_head;
        allocated_block_head=ab;
        return 2;        }//永远把新分配的节点放在第一个
    else if(ret==-1){ /*分配不成功*/
        printf("Allocation fail\n");
        free(ab);
        return -1;       
     }
    return 3;
}

int allocate_mem(struct allocated_block *ab){//finish this
    struct free_block_type *fbt, *pre;
    int request_size=ab->size;
    fbt = pre = free_block;
    //根据当前算法在空闲分区链表中搜索合适空闲分区进行分配，分配时注意以下情况：
    // 1. 找到可满足空闲分区且分配后剩余空间足够大，则分割
    // 2. 找到可满足空闲分区且但分配后剩余空间比较小，则一起分配
    // 3. 找不可满足需要的空闲分区但空闲分区之和能满足需要，则采用内存紧缩技术，进行空闲分区的合并，然后再分配
    // 4. 在成功分配内存后，应保持空闲分区按照相应算法有序
    // 5. 分配成功则返回1，否则返回-1
    //请自行补充。。。。。
    
}

void kill_process(){
    struct allocated_block *ab;
    int pid;
    printf("Kill Process, pid=");
    scanf("%d", &pid);
    ab=find_process(pid);
    if(ab!=NULL){
        free_mem(ab); /*释放ab所表示的分配区*/
        dispose(ab);  /*释放ab数据结构节点*/
        }
}
//need finish
int free_mem(struct allocated_block *ab){
     int algorithm = ma_algorithm;
    struct free_block_type *fbt, *pre, *work;
   fbt=(struct free_block_type*) malloc(sizeof(struct free_block_type));
    if(!fbt) return -1;
    // 进行可能的合并，基本策略如下
    // 1. 将新释放的结点插入到空闲分区队列末尾
    // 2. 对空闲链表按照地址有序排列
    // 3. 检查并合并相邻的空闲分区
    // 4. 将空闲链表重新按照当前算法排序
    //请自行补充……
    //return 1;
}

int dispose(struct allocated_block *free_ab){
    struct allocated_block *pre, *ab;
    if(free_ab == allocated_block_head) { /*如果要释放第一个节点*/
     allocated_block_head = allocated_block_head->next;
        free(free_ab);
        return 1;
    }
    pre = allocated_block_head;  
    ab = allocated_block_head->next;
    while(ab!=free_ab){ pre = ab;  ab = ab->next; }
    pre->next = ab->next;
    free(ab);
    return 2;

}

int display_mem_usage(){
    struct free_block_type *fbt=free_block;
    struct allocated_block *ab=allocated_block_head;
    if(fbt==NULL) return(-1);
    printf("----------------------------------------------------------\n");

    /* 显示空闲区 */
    printf("Free Memory:\n");
    printf("%20s %20s\n", "      start_addr", "       size");
    while(fbt!=NULL){
        printf("%20d %20d\n", fbt->start_addr, fbt->size);
        fbt=fbt->next;
    }
    /* 显示已分配区 */
    printf("\nUsed Memory:\n");
    printf("%10s %20s %10s %10s\n", "PID", "ProcessName", "start_addr", " size");
    while(ab!=NULL){
        printf("%10d %20s %10d %10d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
        ab=ab->next;
        }
    printf("----------------------------------------------------------\n");
    return 0;

}

int rearrange_FF(){
    if( free_block = NULL) return -1;
    else return 1;
}

int rearrange_BF(){
    struct free_block_type *pre,*pnext,*ptem = free_block;
    if(pre == NULL) return -1;
    else{
        pnext = pre->next;
        int num = 1;
        while(pnext){
            num++;
            pre = pnext;
            pnext = pre->next;
        }
        if(num == 1) return 1;
        else{
            for(int i = 0;i < num-1 ;i++){//冒泡排序
                int tem = num - i - 2;
                pre = free_block->next;
                pnext = pre->next;
                ptem = free_block;
                if(ptem->size > pre->size){
                    ptem->next = pre->next;
                    pre->next = ptem;
                    free_block = pre;
                }
                while(tem > 0){
                    if(pre->size > pnext->size){
                        pre->next = pnext->next;
                        pnext->next = pre;
                        ptem->next = pnext;
                    }
                    ptem = ptem->next;
                    pre = pre->next;
                    pnext = pnext->next;   
                    tem--;
                }
            }
        }
        return 1;
    }
}

int rearrange_WF(){
    struct free_block_type *pre,*pnext,*ptem = free_block;
    if(pre == NULL) return -1;
    else{
        pnext = pre->next;
        int num = 1;
        while(pnext){
            num++;
            pre = pnext;
            pnext = pre->next;
        }
        if(num == 1) return 1;
        else{
            for(int i = 0;i < num-1 ;i++){//冒泡排序
                int tem = num - i - 2;
                pre = free_block->next;
                pnext = pre->next;
                ptem = free_block;
                if(ptem->size < pre->size){
                    ptem->next = pre->next;
                    pre->next = ptem;
                    free_block = pre;
                }
                while(tem > 0){
                    if(pre->size < pnext->size){
                        pre->next = pnext->next;
                        pnext->next = pre;
                        ptem->next = pnext;
                    }
                    ptem = ptem->next;
                    pre = pre->next;
                    pnext = pnext->next;   
                    tem--;
                }
            }
        }
        return 1;
    }
}



//need finish
struct allocated_block * find_process(int pid){

}

void do_exit(){
    
}


#endif
