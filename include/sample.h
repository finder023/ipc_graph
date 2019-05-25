#ifndef __SMAPLE__H
#define __SMAPLE__H

#include "requires.h"
#include "message.h"
#include "graph.h"

typedef enum PortStatus {
    EMPTY,
    FULL
} PortStatus;

typedef struct Sample {
    char name[MAXLINE];
    int wr_fd;
    int rd_num;
    int rd_fd_set[MAX_RD_NUM];
    void *wr_ptr;
    void *rd_ptr_set[MAX_RD_NUM];
    sem_t *sem;
    ssize_t mem_len;
    PortStatus status;

    Connector *net;
} Sample;

#define O_FLAG (O_RDWR )

int init_sample(Sample *, Connector*);
int send_sample(Sample *, Message *);
int recv_sample(Sample *, Message*, int);


#endif
