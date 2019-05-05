#ifndef __QUEUE_H
#define __QUEUE_H

#include "requires.h"
#include "message.h"
#include "commu_def.h"
#include "connector.h"

typedef struct Queue {
    char name[MAXLINE];
    int fd_rd[MAX_RD_NUM];
    int fd_wr;
    int rd_num;
    int wr_num;
    int msg_num;

    Connector *net;
} Queue;

void init_queue(Queue *q, const char *);
void init_queue_input(Queue *, const char *, char [MAX_INPUT][MAX_FIFO_NAME], int);
int send_queue(Queue *, Message *);
int recv_queue(Queue *, Message *, int);

#endif


