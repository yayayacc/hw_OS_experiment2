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
    int flag1 = 0;//判断是否分配成功
    /*switch (ma_algorithm)
    {
    case MA_FF://前两种情况
        while(fbt){
            if(fbt->size > ab->size){//2
                if(fbt->size - ab->size <MIN_SLICE){
                    ab->start_addr = fbt->start_addr;
                    flag1 = 1;
                    if(fbt == free_block) free_block = fbt->next;
                    else pre->next = fbt->next;
                    fbt = NULL;
                    pre = NULL;
                    break;
                }
                else{//1
                    struct free_block_type *ptem;
                    ptem = (struct free_block_type *)malloc(sizeof(struct free_block_type));
                    ptem->size = fbt->size - ab->size;
                    ptem->start_addr = fbt->start_addr + ab->size;
                    ptem->next = fbt->next;
                    flag1 = 1;
                    ab->start_addr = fbt->start_addr;
                    if(fbt == free_block) free_block = ptem;
                    else pre->next = ptem;         
                    fbt = NULL;
                    pre = NULL;
                    break;
                }
            }
            pre = fbt;
            fbt = fbt->next;
        }
        break;
    case MA_BF:
        while(fbt){
            if(fbt->size > ab->size){//2
                if(fbt->size - ab->size <MIN_SLICE){
                    ab->start_addr = fbt->start_addr;
                    flag1 = 1;
                    if(fbt == free_block) free_block = fbt->next;
                    else pre->next = fbt->next;
                    fbt = NULL;
                    pre = NULL;
                    break;
                }
                else{//1
                    struct free_block_type *ptem;
                    ptem = (struct free_block_type *)malloc(sizeof(struct free_block_type));
                    ptem->size = fbt->size - ab->size;
                    ptem->start_addr = fbt->start_addr + ab->size;
                    ptem->next = fbt->next;
                    flag1 = 1;
                    ab->start_addr = fbt->start_addr;
                    if(fbt == free_block) free_block = ptem;
                    else pre->next = ptem;         
                    fbt = NULL;
                    pre = NULL;
                    break;
                }
            }
            pre = fbt;
            fbt = fbt->next;
        }
        break;
    case MA_WF:
            if(fbt->size > ab->size){//2
                if(fbt->size - ab->size <MIN_SLICE){
                    ab->start_addr = fbt->start_addr;
                    flag1 = 1;
                    free_block = fbt->next;
                    fbt = NULL;
                    pre = NULL;
                    break;
                }
                else{//1
                    struct free_block_type *ptem;
                    ptem = (struct free_block_type *)malloc(sizeof(struct free_block_type));
                    ptem->size = fbt->size - ab->size;
                    ptem->start_addr = fbt->start_addr + ab->size;
                    ptem->next = fbt->next;
                    flag1 = 1;
                    ab->start_addr = fbt->start_addr;
                    free_block = ptem;        
                    fbt = NULL;
                    pre = NULL;
                    break;
                }
            }
    default:
        break;
    }
    //上边switch为前两种情况，下边为内存压缩技术
    if(!flag1){
        int flag2 = 0;//判断是否形成连续的一大片
        int total = 0;
        int times1 = 0;//开始位置，当找到满足要求的碎片的开始点时的迭代次数
        int times2 = 0;//结束位置,总的迭代次数
        fbt = free_block;
        while(fbt&&fbt->next){//寻找是否有一大连续片加起来可以容纳进程
             if(!flag2){
                times1 = times2;
                total = fbt->size;
             }
             if(fbt->start_addr+fbt->size == fbt->next->start_addr){
                total += fbt->next->size;
                flag2 = 1;
             }
             else flag2 = 0;
             times2++;
             if(total >= ab->size){
                flag1 = 1;//找到可以连续分配的一大片地方了
                break;
             }
             fbt = fbt->next;
        }
        if(flag1){//找到后开始把这一片划归为连续的一大片
            pre = free_block;
            fbt = free_block;
            for(int i = 0;i < times2 + 1 ;i++){
                fbt = fbt->next;
            }
            for(int i = 0;i < times1 ;i++){
                pre = pre->next;
            }
            struct free_block_type * p = free_block;
            for(int i = 0;i < times1 - 1 ; i++){
                p = p->next;
            }
            //判断是否分配的过多
            if(total - ab->size < MIN_SLICE){//不过多
                ab->start_addr = pre->start_addr;
                if(times1 == 0){
                    free_block = fbt;
                }
                else{
                    p->next = fbt;
                }
            }
            else{//过多了，需要将大片裁剪一下
                    struct free_block_type * ptem;
                    ptem = (struct free_block_type *)malloc(sizeof(struct free_block_type));
                    ptem->size = total - ab->size;
                    ptem->start_addr = pre->start_addr + ab->size;
                    ptem->next = fbt;
                    ab->start_addr = pre->start_addr;
                    if(times1 == 0) free_block = ptem;
                    else p->next = ptem;         
                    fbt = NULL;
                    pre = NULL;
            }
        }
    }*/

    //下边是另一种实现方法
    while(fbt){
        if(fbt->size > request_size){
            if(fbt->size - request_size < MIN_SLICE){//剩余空间太小，直接分配
                //pre = fbt->next;
                ab->start_addr = fbt->start_addr;
                mem_size -= fbt->size;
                ab->size = fbt->size;
                if(fbt == free_block){
                    free_block = fbt->next;
                }
                else{
                    pre->next = fbt->next;
                }
                //fbt->start_addr +=fbt->size;
                free(fbt);
            }
            else{
                mem_size -= request_size;
                fbt->size -=request_size;
                ab->start_addr = fbt->start_addr;
                fbt->start_addr += request_size;
            }
            flag1 = 1;
            break;
        }
        pre = fbt;
        fbt = fbt->next;
    }
    if(!flag1){//没有一整个可以满足要求的,进行内存压缩
        pre = free_block;
        fbt = free_block->next;
        while(pre){
            fbt = pre->next;
            if(fbt&&(pre->start_addr + pre->size == fbt->start_addr)){
                pre->size += fbt->size;
                pre->next = fbt->next;
                continue;
            }
            pre = pre->next;
        }
        fbt = free_block;
        while(fbt){//再次进行内存分配
            if(fbt->size > request_size){
                if(fbt->size - request_size < MIN_SLICE){//剩余空间太小，直接分配
                    //pre = fbt->next;
                    ab->start_addr = fbt->start_addr;
                    mem_size -= fbt->size;
                    fbt->start_addr +=fbt->size;
                    //free(fbt);
                }
                else{
                    mem_size -= request_size;
                    fbt->size -=request_size;
                    ab->start_addr = fbt->start_addr;
                    fbt->start_addr += request_size;
                }
                flag1 = 1;
                break;
            }
            fbt = fbt->next;
        }
    }
    




    //1,2,3步骤结束
    if(flag1){
        switch (ma_algorithm)
        {
        case MA_FF:
            rearrange_FF();
            break;
        case MA_BF:
            rearrange_BF();
        case MA_WF:
            rearrange_WF();
        default:
            break;
        }
        return 1;
    }
    else return -1;
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
    //if(fbt==NULL) return(-1);
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

int rearrange_FF(){//按照地址从小到大排列
   struct free_block_type * pre,*pnext;
   pre = free_block;
   while(pre){
        pnext = pre->next;
        while(pnext){
            if(pre->start_addr > pnext->start_addr){
                int tem = pnext->start_addr;
                pnext->start_addr = pre->start_addr;
                pre->start_addr = tem;
                tem = pre->size;
                pre->size = pnext->size;
                pnext->size = tem;
            }
            else pnext = pnext->next;
        }
        pre = pre->next;
   }
   return 1;
}

int rearrange_BF(){
    struct free_block_type *fbt,*pre;
    pre = free_block;
    while(pre){
        fbt = pre->next;
        while(fbt){
            if(fbt->size > pre->size){
                int tem;
                tem = fbt->size;
                fbt->size = pre->size;
                pre->size = tem;
                tem = fbt->start_addr;
                fbt->start_addr = pre->start_addr;
                pre->start_addr = fbt->start_addr;
            }
            else fbt = fbt->next;
        }
        pre = pre->next;
    }
    return 1;
}

int rearrange_WF(){
    struct free_block_type *fbt,*pre;
    pre = free_block;
    while(pre){
        fbt = pre->next;
        while(fbt){
            if(fbt->size > pre->size){
                int tem;
                tem = fbt->size;
                fbt->size = pre->size;
                pre->size = tem;
                tem = fbt->start_addr;
                fbt->start_addr = pre->start_addr;
                pre->start_addr = fbt->start_addr;
            }
            else fbt = fbt->next;
        }
        pre = pre->next;
    }
    return 1;
}



//need finish
struct allocated_block * find_process(int pid){

}

void do_exit(){
    
}


#endif
