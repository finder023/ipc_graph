#include "include/requires.h"
#include "include/conf.h"
#include "include/graph.h"
#include "include/queue.h"
#include "include/opr_impl.h"
#include "include/gen_main.h"

const char *demo = "demo";

int main() {
    Graph *g = (Graph*)malloc(sizeof(Graph));
    Queue *q = (Queue*)malloc(sizeof(Queue));
    Message *msg_in, *msg_out;
    
    BUILD_GRAPH(Queue, g, q);
    ALLOC_MSG(msg_in, msg_out, q, demo, strlen(demo) + 1);

    for (int i=0; i<q->rd_num; ++i) {
        q->opr.recv(q, &msg_in[i], i);
    }

    // process
    for (int i=0; i<q->wr_num; ++i) {
        q->opr.send(q, &msg_out[0], i);    
    }
    
    FREE_MSG(msg_in, msg_out);
    FREE_GRAPH(g, q);
}
