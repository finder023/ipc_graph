#include "include/proc_init.h"
#include "include/queue.h"
#include "include/sample.h"

void proc_fifo_rd_open(Queue *q) {
    assert(q && strlen(q->name));

    sem_t *sem = NULL, *wr_sem = NULL;
    if ((sem = sem_open(FIFO_RD_SEM, O_RDWR)) == NULL) {
        err_quit("proc open sem fail: %s", FIFO_RD_SEM);
    }

    if ((wr_sem = sem_open(FIFO_WR_SEM, O_RDWR)) == NULL) {
        err_quit("proc open sem fail: %s", FIFO_WR_SEM);
    }

    int val;
    sem_wait(sem);

    // open read fifo
    char buf[MAX_NAME_LEN];
    for (int i=0; i<q->rd_num; ++i) {
        int fd;
        strcpy(buf, "./tmp/"); strcat(buf, q->net->rd_name_set[i]); strcat(buf, ".fifo");
        if ((fd = open(buf, O_RDONLY | O_NONBLOCK)) < 0) {
            err_quit("open rd fifo fail: %s", buf);
        }
        q->fd_rd[i] = fd;
    }

    // signal proc that wait for fifo write
    sem_getvalue(sem, &val);
    printf("proc get rd sem val: %d\n", val);
    // only one proc will call sem_post func
    if (val == 0) {
        for (int i=0; i<NUM_PROC; ++i)
            sem_post(wr_sem);
    }

    sem_close(sem);
    sem_close(wr_sem);
}

void proc_fifo_wr_open(Queue *q) {
    assert(q && strlen(q->name));
    
    sem_t *sem = NULL;

    if ((sem = sem_open(FIFO_WR_SEM, O_RDWR)) == NULL) {
        err_quit("open sem fail: %s", FIFO_WR_SEM);
    }

    sem_wait(sem);

    // open write fifo
    int fd;
    if ((fd = open(q->name, O_WRONLY | O_NONBLOCK)) < 0) {
        err_quit("open write fifo fail: %s", q->name);
    }

    q->fd_wr = fd;

    sem_close(sem);
}


void init_connector(Connector *c) {
    assert(c);

    const char *name = c->name;

    if (!strcmp(name, "proc0")) {
        c->rd_num = 0;
        c->wr_num = 1;
    } else if (!strcmp(name, "proc1")) {
        c->rd_num = 1;
        c->wr_num = 1;
        strcpy(c->rd_name_set[0], "proc0");
    } else if (!strcmp(name, "proc2")) {
        c->rd_num = 1;
        c->wr_num = 1;
        strcpy(c->rd_name_set[0], "proc1");
    } else if (!strcmp(name, "proc3")) {
        c->rd_num = 1;
        c->wr_num = 1;
        strcpy(c->rd_name_set[0], "proc2");
    } else if (!strcmp(name, "proc4")) {
        c->rd_num = 1;
        c->wr_num = 0;
        strcpy(c->rd_name_set[0], "proc3");
    } else {
        err_quit("invalid proc name: %s", name);
    }
} 

void proc_ipc_init(Connector *c, ProcIpc *ipc) {
    assert(c && ipc);

    init_connector(c);
    init_queue_input(&ipc->_queue, c);

    proc_fifo_rd_open(&ipc->_queue);
    if (strcmp(c->name, "proc4"))
        proc_fifo_wr_open(&ipc->_queue);

    printf("%s fifo wr done\n", c->name);

}


