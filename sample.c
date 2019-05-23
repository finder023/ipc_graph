#include "include/sample.h"
#include <sys/mman.h>


int init_sample(Sample *sm, Connector *net) {
    assert(sm && net);
    sm->mem_len = MAXLINE;

    strcpy(sm->name, net->name);
    strcat(sm->name, ".shdm");

    sm->net = net;
    // create shared memory
    if ((sm->wr_fd = shm_open(sm->name, O_FLAG, FILE_MODE)) < 0) {
        err_ret("open shm %s fail", sm->name);
        return 1;
    }
    // mmap
    sm->wr_ptr = mmap(NULL, sm->mem_len, PROT_READ | PROT_WRITE, MAP_SHARED,
                      sm->wr_fd, 0);
    if (!sm->wr_ptr) {
        err_quit("mmap shm %s fail", sm->name);
        return 2;
    }
    
    int fd;
    char buf[MAX_NAME_LEN];
    for (int i=0; i<sm->rd_num; ++i) {
        strcpy(buf, net->rd_name_set[i]); strcat(buf, ".shdm");
        if ((fd = shm_open(buf, O_FLAG, FILE_MODE)) < 0) {
            err_quit("open rd shm %s fail", buf);
        }
        sm->rd_fd_set[i] = fd;

        sm->rd_ptr_set[i] = mmap(NULL, sm->mem_len, PROT_READ | PROT_WRITE, 
                                 MAP_SHARED, sm->rd_fd_set[i], 0);
        if (!sm->rd_ptr_set[i]) {
            err_quit("mmap shm fail: %s", net->rd_name_set[i]);
        }
    }

    // create semaphore
    char sem_name_buff[MAXLINE];
    strcpy(sem_name_buff, sm->name);
    strcat(sem_name_buff, ".shdm.sem");

    sm->sem = sem_open(sem_name_buff, O_CREAT, FILE_MODE, 1);
    if (!sm->sem) {
        err_quit("create semaphore  %s fail", sem_name_buff);
        return 3;
    }
    sm->status = EMPTY;
    return 0;
}

int send_sample(Sample *sm, Message *msg) {
    assert(sm && sm->wr_fd && sm->wr_ptr && sm->sem);

    sem_wait(sm->sem);
    if (sm->status == EMPTY)
        sm->status = FULL;
    memcpy(sm->wr_ptr, msg, msg->msg_len + MSGHEADSIZE);
    sem_post(sm->sem);
    return msg->msg_len + MSGHEADSIZE;
}


int recv_sample(Sample *sm, Message *msg, int _n) {
    assert(sm && msg);

    // wait mutex
    sem_wait(sm->sem);
    // wait while shm empty, 1s equals 1000000ms
    while (sm->status == EMPTY) {
        sem_post(sm->sem);
        usleep(1000);
        sem_wait(sm->sem);
    }

    // read message header
    memcpy(msg, sm->rd_ptr_set[_n], MSGHEADSIZE);
    // read message body
    memcpy(msg->msg_data, (char*)sm->rd_ptr_set[_n] + MSGHEADSIZE, msg->msg_len);
    // signal mutex
    sem_post(sm->sem);
    return msg->msg_len + MSGHEADSIZE;
}
