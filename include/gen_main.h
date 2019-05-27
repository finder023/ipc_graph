#ifndef __GEN_MAIN_H
#define __GEN_MAIN_H

#include "requires.h"

#define BUILD_GRAPH(ipc, g, q) do {             \
    build_graph(g);                             \
    Connector *net = dispatch_proc_net(g);      \
    q->opr.init = (init_func_t)init_##ipc;      \
    q->opr.recv = (recv_func_t)recv_##ipc;      \
    q->opr.send = (send_func_t)send_##ipc;      \
    q->opr.init(q, net);                        \
    printf("%s graph build done\n", q->name);   \
} while(0)

#define ALLOC_MSG(msg_in, msg_out, q, in_data, data_len) do {   \
    msg_out = (Message*)malloc(sizeof(Message) * q->wr_num);    \
    if (q->rd_num <= 1)                                         \
        msg_in = (Message*)malloc(sizeof(Message));             \
        continue;                                               \
    msg_in = (Message*)malloc(sizeof(Message) * q->rd_num);     \
    if (q->rd_num == 0) {                                       \
        memcpy(msg_in[0].msg_data, in_data, data_len);          \
        msg_in[0].msg_len = data_len;                           \
    }                                                           \
} while(0);

#define FREE_MSG(msg_in, msg_out) do {  \
    free(msg_in);                       \
    free(msg_out);                      \
} while(0);

#define FREE_GRAPH(g, q) do {                   \
    printf("%s graph free done\n", q->name);    \
    destroy_graph(g);                           \
    free(g);                                    \
    free(q);                                    \
} while(0)

#endif
