#ifndef __SMAPLE__H
#define __SMAPLE__H

#include "requires.h"
#include "message.h"
#include "graph.h"
#include "ipc_opr.h"

typedef enum PortStatus {
    EMPTY,
    FULL
} PortStatus;

typedef struct Sample {
    char name[MAXLINE];
    int fd_rd[MAX_RD_NUM];
    int fd_wr[MAX_WR_NUM];
    sem_t *sem_rd[MAX_RD_NUM];
    sem_t *sem_wr[MAX_WR_NUM];
    void *wr_ptr_set[MAX_WR_NUM];
    void *rd_ptr_set[MAX_RD_NUM];
    int rd_num;
    int wr_num;
    sem_t *sem;
    ssize_t mem_len;
    PortStatus status;

    Connector *net;
    IpcOpr opr;
} Sample;

#define O_FLAG (O_RDWR )


#endif
