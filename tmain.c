#include "include/requires.h"
#include "include/conf.h"
#include "include/ipc_init.h"
#include "include/graph.h"
#include "include/queue.h"

int main() {
    Graph g;
    Connector *net;
    build_graph(&g);
    net = dispatch_proc_net(&g);

    Message msg;
    Queue q;
    queue_opr_register(&q, NULL);
    q.opr.init(&q, net);
    q.opr.recv(&q, &msg, 0);

    // process
    q.opr.send(&q, &msg, 0);    
    

    destroy_graph(&g);
}
