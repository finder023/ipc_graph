#ifndef __QUEUE_H
#define __QUEUE_H

#include "requires.h"
#include "message.h"
#include "graph.h"
#include "ipc_opr.h"

typedef struct Queue {
    char name[MAXLINE];
    int fd_rd[MAX_RD_NUM];
    sem_t *sem_rd[MAX_RD_NUM];
    sem_t *sem_wr[MAX_WR_NUM];
    int fd_wr[MAX_WR_NUM];
    int rd_num;
    int wr_num;
    int msg_num;

    Connector *net;
    IpcOpr opr;
} Queue;


void queue_opr_register(Queue *, IpcOpr *);


#endif


