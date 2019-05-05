#ifndef __BLACKBOARD_H
#define __BLACKBOARD_H

#include "requires.h"
#include "message.h"

typedef enum Status {
    EMPTY,
    FULL
} Status;


typedef struct Blackboard {
    char buff[MAXLINE];
    pthread_mutex_t mutex;
    Status status;
} Blackboard;


void init_blackboard(Blackboard *bd);
void send_blackboard(Blackboard *bd, Message *);
void recv_blackboard(Blackboard *bd, Message *);


#endif
