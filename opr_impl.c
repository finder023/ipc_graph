#include "include/opr_impl.h"

/**
 * @file opr_impl.c
 * @brief ipc commution operator implication
 * @author liuyaolong
 * @version 1.0
 * @date 2019-05-27
 */


/**
 * @brief process read fifo open func
 *
 * @param struct Queue in proc
 */
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

/**
 * @brief proess write fifo open function
 *
 * @param Queue in process
 */
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

/**
 * @brief process semaphore open function, sem that used for fifo sync
 *
 * @param q
 */
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

/**
 * @brief proess Queue init function
 *
 * @param q
 * @param net Connector that saves modules's input and output module name
 */
void init_Queue(Queue *q, Connector *net) {
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


/**
 * @brief Module's send msg function using Queue
 *
 * @param q
 * @param msg Message that contains data for sending
 * @param id index of the send_fifo in module
 *
 * @return sent bytes
 */
int send_Queue(Queue *q, Message *msg, int id) {
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

/**
 * @brief Recive function in module using Queue
 *
 * @param q
 * @param msg Empty Message used to save received data
 * @param id Index of the recv_fifo in module
 *
 * @return received data in bytes
 */
int recv_Queue(Queue *q, Message *msg, int id) {
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


/**
 * @brief 
 *
 * @param s
 * @param net
 */
void init_Sample(Sample *s, Connector *net) {
    assert(s && net);

    strcpy(s->name, net->name);
    s->wr_num = net->wr_num;
    s->rd_num = net->rd_num;
    s->net = net;
    s->mem_len = MAXLINE;

    int fd;
    void *ptr;
    char buf[MAXLINE];
    sem_t *sem;

    // sample shm mutex
    if (snprintf(buf, MAXLINE, "%s.shmsem", s->name) < 0)
        err_quit("snprintf error");
    if ((sem = sem_open(buf, O_RDWR)) == SEM_FAILED)
        err_quit("%s open shm shm fail", s->name);
    s->sem = sem;

    // rd shm open
    for (int i=0; i<s->rd_num; ++i) {
        if (snprintf(buf, MAXLINE, "%s_2_%s.shm", s->net->rd_name_set[i],
                    s->name) < 0)
            err_quit("snprinf error");
        if ((fd = shm_open(buf, O_RDWR, FILE_MODE)) < 0)
            err_quit("open shm fail %s", buf);
        s->fd_rd[i] = fd;

        if ((ptr = mmap(NULL, s->mem_len, PROT_READ | PROT_WRITE, MAP_SHARED,
            fd, 0)) == NULL)
            err_quit("mmap shm fail: %s", buf);
        s->rd_ptr_set[i] = ptr;

        // semaphore open
        if (snprintf(buf, MAXLINE, "%s_2_%s.shmsem", s->net->rd_name_set[i],
                    s->name) < 0)
            err_quit("snprinf error");
        if ((sem = sem_open(buf, O_RDWR)) == SEM_FAILED)
            err_quit("open shm sem fail: %s", buf);
        s->sem_rd[i] = sem;
    }
    // wr shm open
    for (int i=0; i<s->wr_num; ++i) {
        if (snprintf(buf, MAXLINE, "%s_2_%s.shm", s->name, 
                    s->net->rd_name_set[i]) < 0)
            err_quit("snprinf error");
        if ((fd = shm_open(buf, O_RDWR, FILE_MODE)) < 0)
            err_quit("open shm fail %s", buf);
        s->fd_wr[i] = fd;

        if ((ptr = mmap(NULL, s->mem_len, PROT_READ | PROT_WRITE, MAP_SHARED,
            fd, 0)) == NULL)
            err_quit("mmap shm fail: %s", buf);
        s->wr_ptr_set[i] = ptr;

        // semaphore open
        if (snprintf(buf, MAXLINE, "%s_2_%s.shmsem", s->name, 
            s->net->rd_name_set[i]) < 0)
            err_quit("snprinf error");
        if ((sem = sem_open(buf, O_RDWR)) == SEM_FAILED)
            err_quit("open shm sem fail: %s", buf);
        s->sem_wr[i] = sem;
    }

}

/**
 * @brief 
 *
 * @param s
 * @param msg
 * @param id
 *
 * @return 
 */
int send_Sample(Sample *s, Message *msg, int id) {
    assert(s && msg);

    dprintf("%s wait for rd sems\n", s->name);
    assert(id < s->wr_num);
    
    int remain_len = msg->msg_len + MSGHEADSIZE;
    // shm mutex
    sem_wait(s->sem);
    memcpy(s->wr_ptr_set[id], msg, remain_len);
    sem_post(s->sem);

    // signal proc that wait for the message
    sem_post(s->sem_wr[id]);
    dprintf("%s wr sem_post posted\n", s->name);
    return remain_len;
}

/**
 * @brief 
 *
 * @param s
 * @param msg
 * @param id
 *
 * @return 
 */
int recv_Sample(Sample *s, Message *msg, int id) {
    assert(s && msg);
    assert(id < s->rd_num);

    dprintf("%s wait for sem_rd\n", s->name);
    
    // wait until input proc write msg in shm
    sem_wait(s->sem_rd[id]);
    int remain_len = MSGHEADSIZE;
    
    // shm mutex
    sem_wait(s->sem);
    memcpy(msg, s->rd_ptr_set[id], remain_len);
    remain_len = msg->msg_len;
    memcpy(msg->msg_data, (char*)s->rd_ptr_set[id] + MSGHEADSIZE, remain_len);
    sem_post(s->sem);

    return MSGHEADSIZE + remain_len;
}
