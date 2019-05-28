#include "include/requires.h"
#include "include/conf.h"
#include "include/graph.h"
#include "include/queue.h"
#include "include/opr_impl.h"
#include "include/gen_main.h"

const char *demo = "demo";
void user_func(Message *msg_in, int in_len, Message *msg_out, int out_len) {
    USER_FUNC_BODY(msg_in, in_len, msg_out, out_len);
}        

int main() {
    Graph *g = (Graph*)malloc(sizeof(Graph));
    Queue *q = (Queue*)malloc(sizeof(Queue));
    Message *msg_in, *msg_out;
    
    BUILD_GRAPH(Queue, g, q);
    ALLOC_MSG(&msg_in, &msg_out, q, demo, strlen(demo) + 1);
    
    for (int i=0; i<q->rd_num; ++i) {
        q->opr.recv(q, &msg_in[i], i);
    }

    // user  process
    printf("\t%s: ", q->name);
    user_func(msg_in, q->rd_num, msg_out, q->wr_num);

    for (int i=0; i<q->wr_num; ++i) {
        q->opr.send(q, &msg_out[0], i);    
    }
    
    FREE_MSG(msg_in, msg_out);
    FREE_GRAPH(g, q);
}
