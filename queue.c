#include "include/queue.h"

typedef enum FdType {
    RD,
    WR
} FdType;


void init_queue(Queue *q, const char *name) {
    assert(q && name);    

    strcpy(q->name, name);
    q->rd_num = 0;
    q->fd_wr = -1;
    q->msg_num = 0;
}

void init_queue_input(Queue *q, Connector *c) {
    assert(q && c);

    char buf[MAX_NAME_LEN];
    strcpy(buf, "./tmp/"); strcat(buf, c->name); strcat(buf, ".fifo");
    strcpy(q->name, buf);
    q->rd_num = c->rd_num;
    q->fd_wr = -1;

    q->msg_num = 0;
    
    q->net = c;
}


int send_queue(Queue *q, Message *msg) {
    assert(q && msg);

    assert(q->fd_wr != -1); 
    int remain_len = MSGHEADSIZE;
    ssize_t n;

    while (remain_len > 0) {
        n = write(q->fd_wr, msg, remain_len);
        remain_len -= n;
    }

    remain_len = msg->msg_len;
    while (remain_len > 0) {
        if ((n = write(q->fd_wr, msg->msg_data, remain_len)) < 0) {
            err_quit("write error");
        }
        remain_len -= n;
    }

    return 0;
}

int recv_queue(Queue *q, Message *msg, int _n) {
    assert(q && msg);
    assert(_n < q->rd_num);
    
    int remain_len = MSGHEADSIZE;
    // printf("remain len: %d\n", remain_len);
    ssize_t n;
    while (remain_len > 0) {
        if ((n = read(q->fd_rd[_n], msg, remain_len)) < 0) 
            err_quit("read header error");
        // printf("%d\n", (int)n);
        remain_len -= n;
    }

    remain_len = msg->msg_len;
    // printf("remain len: %d\n", remain_len);
    while (remain_len > 0) {
        if ((n = read(q->fd_rd[_n], msg->msg_data, remain_len)) < 0) {
            err_quit("read body error");
        }
        // printf("%d\n", (int)n);
        remain_len -= n;
    }

    return 0;
}
