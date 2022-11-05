#ifndef _TYPE_H
#define _TYPE_H

#include"const.h"
#define NULL 0
struct free_block_type{
    int size;
    int start_addr;
    struct free_block_type *next;
};  

/*指向内存中空闲块链表的首指针*/
struct free_block_type *free_block;


/*每个进程分配到的内存块的描述*/
struct allocated_block{
    int pid;    int size;
    int start_addr;
    char process_name[PROCESS_NAME_LEN];
    struct allocated_block *next;
    };
/*进程分配内存块链表的首指针*/
struct allocated_block *allocated_block_head = NULL;

#endif