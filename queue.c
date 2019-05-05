#include "include/queue.h"

typedef enum FdType {
    RD,
    WR
} FdType;


void init_queue(Queue *q, const char *name) {
    assert(q && name);    

    strcpy(q->name, name);
    q->input_num = 0;
    q->fd_wr = -1;
    q->msg_num = 0;
}

void init_queue_input(Queue *q, const char *name, char input_rd[MAX_INPUT][MAX_FIFO_NAME], int n) {
    assert(q && name && input_rd);

    strcpy(q->name, name);
    q->input_num = n;
    q->fd_wr = -1;

    q->msg_num = 0;

    for (int i=0; i<n; ++i) {
        strcpy(q->rd_name[i], input_rd[i]);
    }
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
    assert(_n < q->input_num);
    
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
