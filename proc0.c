#include "include/requires.h"
#include "include/conf.h"
#include "include/graph.h"
#include "include/queue.h"
#include "include/opr_impl.h"
#include "include/gen_main.h"

const char *demo = "demo";
void user_func(Message *msg_in, int in_len, Message *msg_out, int out_len) {
    assert(in_len > 0 && out_len > 0);
    (void)in_len;
    for (int i=0; i<out_len; ++i) {
        memcpy(&msg_out[i], &msg_in[0], sizeof(Message));
        printf("recvied msg: %s", msg_in[0].msg_data);
    }
}        

int main() {
    Graph *g = (Graph*)malloc(sizeof(Graph));
    Queue *q = (Queue*)malloc(sizeof(Queue));
    Message *msg_in, *msg_out;
    
    BUILD_GRAPH(Queue, g, q);
    ALLOC_MSG(msg_in, msg_out, q, demo, strlen(demo) + 1);

    for (int i=0; i<q->rd_num; ++i) {
        q->opr.recv(q, &msg_in[i], i);
    }

    // user  process
    user_func(msg_in, q->rd_num, msg_out, q->wr_num);

    for (int i=0; i<q->wr_num; ++i) {
        q->opr.send(q, &msg_out[0], i);    
    }
    
    FREE_MSG(msg_in, msg_out);
    FREE_GRAPH(g, q);
}
