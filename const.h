#ifndef _CONST_H
#define _CONST_H

#define PROCESS_NAME_LEN 32   /*进程名长度*/
#define MIN_SLICE    10             /*最小碎片的大小*/
#define DEFAULT_MEM_SIZE 1024     /*内存大小*/
#define DEFAULT_MEM_START 0       /*起始位置*/

#define MA_FF 1
#define MA_BF 2
#define MA_WF 3
int mem_size=DEFAULT_MEM_SIZE; /*内存大小*/
int ma_algorithm = MA_FF;           /*当前分配算法*/
static int pid = 0;                                      /*初始pid*/
int flag = 0;                            /*设置内存大小标志*/

#endif