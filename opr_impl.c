#include "include/opr_impl.h"

// fifo helper func
static void _proc_fifo_rd_open(Queue *q) {
    assert(q);

    if (q->rd_num == 0)
        return;

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
        if (snprintf(buf, MAXLINE, "./tmp/%s_2_%s.fifo", q->net->rd_name_set[i],
                 q->name) < 0)
            err_quit("snprintf error");
        if ((fd = open(buf, O_RDONLY | O_NONBLOCK)) < 0) {
            err_quit("open rd fifo fail: %s", buf);
        }
        q->fd_rd[i] = fd;
        dprintf("open rd fifo: %s\n", buf);
    }

    // signal proc that wait for fifo write
    sem_getvalue(rd_sem, &val);
    if (val == 0) {
        // only one proc will call sem_post func
        for (int i=0; i<q->net->net_num - 1; ++i) {
            sem_post(wr_sem);
            dprintf("post for wr fifo\n");
        }
    }

    sem_close(rd_sem);
    sem_close(wr_sem);
}

static void _proc_fifo_wr_open(Queue *q) {
    assert(q);

    if (q->wr_num == 0)
        return;

    char buf[MAXLINE];
    sem_t *sem;

    if ((sem = sem_open(FIFO_WR_SEM, O_RDWR)) == SEM_FAILED) {
        err_quit("proc open wr sem fail: %s", FIFO_WR_SEM);
    }

    dprintf("%s wait for wr fifo open\n", q->name);
    sem_wait(sem);

    // open and record wr_fd
    for (int i=0; i<q->wr_num; ++i) {
        int fd;
        if (snprintf(buf, MAXLINE, "./tmp/%s_2_%s.fifo", q->name, 
                 q->net->wr_name_set[i]) < 0)
            err_quit("snprintf error");
        if ((fd = open(buf, O_WRONLY | O_NONBLOCK)) < 0) {
            err_quit("proc open wr fifo fail: %s", buf);
        }
        q->fd_wr[i] = fd;
        dprintf("open wr fifo: %s\n", buf);
    }

    sem_close(sem);

}

static void _proc_fifo_sem_open(Queue *q) {
    assert(q);

    char buf[MAXLINE];
    sem_t *sem;

    for (int i=0; i<q->rd_num; ++i) {
        if (snprintf(buf, MAXLINE, "%s_2_%s.fifosem", q->net->rd_name_set[i],
                     q->name) < 0)
            err_quit("snprintf error");
        if ((sem = sem_open(buf, O_RDWR)) == SEM_FAILED)
            err_quit("fifo sem open fail");
        q->sem_rd[i] = sem;
    }

    for (int i=0; i<q->wr_num; ++i) {
        if (snprintf(buf, MAXLINE, "%s_2_%s.fifosem", q->name, 
                        q->net->wr_name_set[i]) < 0)
            err_quit("snprintf error");
        if ((sem = sem_open(buf, O_RDWR)) == SEM_FAILED)
            err_quit("fifo sem open fail");
        q->sem_wr[i] = sem;
    }
}

void init_queue(Queue *q, Connector *net) {
    assert(q && net);

    strcpy(q->name, net->name);
    q->net = net;
    q->wr_num = net->wr_num;
    q->rd_num = net->rd_num;
    
    // wr fd open
    _proc_fifo_rd_open(q);
    _proc_fifo_wr_open(q);
    _proc_fifo_sem_open(q);
}


int send_queue(Queue *q, Message *msg, int id) {
    assert(q && msg);
    assert(id < q->wr_num);
    int res = 0;
    int remain_len = MSGHEADSIZE;
    ssize_t n;

    while (remain_len > 0) {
        n = write(q->fd_wr[id], msg, remain_len);
        remain_len -= n;
    }
    res += MSGHEADSIZE;

    remain_len = msg->msg_len;
    while (remain_len > 0) {
        n = write(q->fd_wr[id], msg->msg_data, remain_len);
        remain_len -= n;
    }
    res += msg->msg_len;

    sem_post(q->sem_wr[id]);
    dprintf("wr sem_post posed\n");

    return res;
}

int recv_queue(Queue *q, Message *msg, int id) {
    assert(q && msg);
    assert(id < q->rd_num);
    
    int res = 0;
    int remain_len = MSGHEADSIZE;
    ssize_t n;

    dprintf("wait for sem_rd\n");
    sem_wait(q->sem_rd[id]);
    dprintf("sem_wait_done\n");
    while (remain_len > 0) {
        if ((n = read(q->fd_rd[id], msg, remain_len)) < 0)
            err_quit("read header error");
        remain_len -= n;
    }
    res += MSGHEADSIZE;

    remain_len = msg->msg_len;

    while (remain_len > 0) {
        if ((n = read(q->fd_rd[id], msg->msg_data, remain_len)) < 0)
            err_quit("read body error");
        remain_len -= n;
    }
    res += msg->msg_len;
    return res;
}


/*
opr register func
*/

void queue_opr_register(Queue *q, IpcOpr *opr) {
    assert(q);

    if (opr) {
        q->opr = *opr;
        return;
    }
    
    // default opr impl
    q->opr.init = (init_func_t)init_queue;
    q->opr.recv = (recv_func_t)recv_queue;
    q->opr.send = (send_func_t)send_queue;
    return;
}
