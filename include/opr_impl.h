#ifndef __OPR_IMPL_H
#define __OPR_IMPL_H

#include "conf.h"
#include "requires.h"
#include "queue.h"

// queue opr impl
void init_queue_input(Queue *, Connector*);
int send_queue(Queue *, Message *);
int recv_queue(Queue *, Message *, int);



#endif



