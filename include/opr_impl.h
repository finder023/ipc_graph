#ifndef __OPR_IMPL_H
#define __OPR_IMPL_H

#include "conf.h"
#include "requires.h"
#include "queue.h"
#include "sample.h"

// queue opr impl
void init_Queue(Queue *, Connector*);
int send_Queue(Queue *, Message *, int);
int recv_Queue(Queue *, Message *, int);

// sample opr impl
void init_Sample(Sample*, Connector*);
int send_Sample(Sample*, Message*, int);
int recv_Sample(Sample*, Message*, int);


#endif



