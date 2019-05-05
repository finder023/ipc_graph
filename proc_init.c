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
    for (int i=0; i<q->rd_num; ++i) {
        int fd;
        if ((fd = open(q->net->rd_name_set[i], O_RDONLY | O_NONBLOCK)) < 0) {
            err_quit("open rd fifo fail: %s", q->net->rd_name_set[i]);
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


void init_connector(const char *name, Connector *c) {
    assert(c && strlen(name));

    strcpy(c->name, name);

    if (!strcmp(name, "proc0")) {
        c->rd_num = 0;
        c->wr_num = 1;
    } else if (!strcpy(name, "proc1")) {
        c->rd_num = 1;
        c->wr_num = 1;
        strcpy(c->rd_name_set[0], "proc0");
    } else if (!strpcy)
} 

void proc_ipc_init(const char *name, ProcIpc *ipc) {
    assert(strlen(name));

    char buf[MAXLINE];
    strcpy(buf, "./tmp/");
    strcat(buf, name);
    strcat(buf, ".fifo");

    char rd_buf[MAXLINE];

    
    char input_rd[MAX_INPUT][MAX_FIFO_NAME];
    int input_num = 0;

    if (!strcmp(name, "proc0")) {
        input_num = 0;
    } else if (!strcmp(name, "proc1")) {
        input_num = 1;
        strcpy(rd_buf, "./tmp/"); 
        strcat(rd_buf, "proc0");
        strcat(rd_buf, ".fifo");
        strcpy(input_rd[0], rd_buf);
    } else if (!strcmp(name, "proc2")) {
        input_num = 1;
        strcpy(rd_buf, "./tmp/"); 
        strcat(rd_buf, "proc1");
        strcat(rd_buf, ".fifo");
        strcpy(input_rd[0], rd_buf);
    } else if (!strcmp(name, "proc3")) {
        input_num = 1;
        strcpy(rd_buf, "./tmp/"); 
        strcat(rd_buf, "proc2");
        strcat(rd_buf, ".fifo");
        strcpy(input_rd[0], rd_buf);
    } else if (!strcmp(name, "proc4")) {
        input_num = 1;
        strcpy(rd_buf, "./tmp/"); 
        strcat(rd_buf, "proc3");
        strcat(rd_buf, ".fifo");
        strcpy(input_rd[0], rd_buf);
    } else {
        err_quit("invalid proc name: %s", name);
    }

    init_queue_input(&ipc->_queue, buf, input_rd, input_num);

    proc_fifo_rd_open(&ipc->_queue);
    if (strcmp(name, "proc4"))
        proc_fifo_wr_open(&ipc->_queue);

    printf("%s fifo wr done\n", name);

}


