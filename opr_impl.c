#include "include/opr_impl.h"

// fifo helper func
static void _proc_fifo_rd_open(Queue *q) {
    assert(q);

    sem_t *rd_sem, *wr_sem;
    if ((rd_sem = sem_open(FIFO_RD_COUNT_SEM, O_RDWR)) == SEM_FAILED) {
        err_quit("proc open rd sem fail: %s", FIFO_RD_COUNT_SEM);
    }
    if ((wr_sem = sem_open(FIFO_WR_SEM, O_RDWR)) == SEM_FAILED) {
        err_quit("proc open wr sem fail: %s", FIFO_WR_SEM);
    }

    int val;
    sem_wait(rd_sem);

    // open read fifo
    char buf[MAXLINE];
    for (int i=0; i<q->rd_num; ++i) {
        int fd;
        snprintf(buf, MAXLINE, "./tmp/%s_2_%s.fifo", q->net->rd_name_set[i],
                 q->name);
        if ((fd = open(buf, O_RDONLY | O_NONBLOCK)) < 0) {
            err_quit("open rd fifo fail: %s", buf);
        }
        q->fd_rd[i] = fd;
    }

    // signal proc that wait for fifo write
    sem_getvalue(rd_sem, &val);
    if (val == 0) {
        // only one proc will call sem_post func
        for (int i=0; i<q->net->net_num; ++i) {
            sem_post(wr_sem);
        }
    }

    sem_close(rd_sem);
    sem_close(wr_sem);
}

static void _proc_fifo_wr_open(Queue *q) {
    assert(q);

    char buf[MAXLINE];
    sem_t *sem;

    if ((sem = sem_open(FIFO_WR_SEM, O_RDWR)) == SEM_FAILED) {
        err_quit("proc open wr sem fail: %s", FIFO_WR_SEM);
    }

    sem_wait(sem);

    // open and record wr_fd
    for (int i=0; i<q->wr_num; ++i) {
        int fd;
        snprintf(buf, MAXLINE, "./tmp/%s_2%s.fifo", q->name, 
                 q->net->wr_name_set[i]);
        if ((fd = open(buf, O_WRONLY | O_NONBLOCK)) < 0) {
            err_quit("proc open wr fifo fail: %s", buf);
        }
        q->fd_wr[i] = fd;
    }

    sem_close(sem);

}

void init_queue_input(Queue *q, Connector *net) {
    assert(q && net);

    memset(q, 0, sizeof(Queue));

    strcpy(q->name, net->name);
    q->net = net;
    q->wr_num = net->wr_num;
    q->rd_num = net->rd_num;
    
    // wr fd open
    _proc_fifo_rd_open(q);
    _proc_fifo_wr_open(q);
}


int send_queue(Queue *q, Message *msg, int id);
int recv_queue(Queue *, Message *, int id);


