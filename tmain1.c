#include "include/requires.h"
#include "include/conf.h"
#include "include/ipc_init.h"
#include "include/graph.h"
#include "include/queue.h"
#include "include/opr_impl.h"



int main() {
    Graph *g = (Graph*)malloc(sizeof(Graph));
    Connector *net;
    Queue *q = (Queue*)malloc(sizeof(Queue));
    Message *msg = (Message*)malloc(sizeof(Message));    
    

    build_graph(g);
    net = dispatch_proc_net(g);
    
    //queue_opr_register(q, NULL);
    q->opr.init = (init_func_t)init_queue;
    q->opr.recv = (recv_func_t)recv_queue;
    q->opr.send = (send_func_t)send_queue;
    
    q->opr.init(q, net);
    dprintf("init done\n");

    if (q->rd_num == 0) {
        strcpy(msg->msg_data, "demo");
        msg->msg_len = strlen("demo") + 1;
    }

    for (int i=0; i<q->rd_num; ++i) {
        dprintf("before recv %d\n", i);
        q->opr.recv(q, msg, i);
        dprintf("recved msg: %s\n", msg->msg_data);
    }

    printf("recv msg: %s\n", msg->msg_data);
    // process
    for (int i=0; i<q->wr_num; ++i) {
        q->opr.send(q, msg, i);    
    }
    
    
    destroy_graph(g);
    free(g);
    free(q);
    free(msg);
}
