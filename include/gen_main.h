#ifndef __GEN_MAIN_H
#define __GEN_MAIN_H

#include "requires.h"

#define DO_IPC_INIT do { \
    ProcIpc ipc; \
    Connector ct; \
    strcpy(ct.name, proc_name); \
    proc_ipc_init(&ct, &ipc); \
} while(0)

#define RECV_PROC(net, commu, msg) do { \
    for (int i=0; i<net.rd_num; ++i) { \
        
    }
}

#endif
