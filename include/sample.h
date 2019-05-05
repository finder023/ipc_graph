#ifndef __SMAPLE__H
#define __SMAPLE__H

#include "requires.h"
#include "message.h"

typedef enum PortStatus {
    EMPTY,
    FULL
} PortStatus;

typedef struct Sample {
    char name[MAXLINE];
    int fd;
    void *ptr;
    sem_t *sem;
    ssize_t mem_len;
    PortStatus status;
} Sample;

#define O_FLAG (O_RDWR | O_CREAT)

int init_sample(Sample *, const char *);
int send_sample(Sample *, Message *);
int recv_sample(Sample *, Message*);


#endif
