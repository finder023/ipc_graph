#ifndef __PROC_IPC_H
#define __PROC_IPC_H

#include "requires.h"
#include "queue.h"
#include "sample.h"

typedef struct ProcIpc {
    Queue _queue;
    Sample _sample;
} ProcIpc;

#endif
