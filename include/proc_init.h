#ifndef __PROC_INIT_H
#define __PROC_INIT_H

#include "requires.h"
#include "commu_def.h"
#include "queue.h"
#include "proc_ipc.h"
#include "graph.h"
#include "sample.h"

#define NUM_PROC 5

void proc_fifo_rd_open(Queue*);

void proc_fifo_wr_open(Queue*);

void proc_ipc_init(Connector *c, ProcIpc *ipc);

// this func requires different implementation depending on the graph
void init_connector(Connector *);

#endif
