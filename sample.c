#include "include/sample.h"
#include <sys/mman.h>


int init_sample(Sample *sm, const char *n) {
    assert(sm && n);
    strcpy(sm->name, n);
    sm->mem_len = MAXLINE;
    // create shared memory
    if ((sm->fd = shm_open(sm->name, O_FLAG, FILE_MODE)) < 0) {
        err_ret("create shm %s fail", sm->name);
        return 1;
    }
    // mmap
    sm->ptr = mmap(NULL, sm->mem_len, PROT_READ | PROT_WRITE, MAP_SHARED, sm->fd, 0);
    if (!sm->ptr) {
        err_ret("mmap shm %s fail", sm->name);
        return 2;
    }

    // create semaphore
    char sem_name_buff[MAXLINE];
    strcpy(sem_name_buff, sm->name);
    strcat(sem_name_buff, ".sem");

    sm->sem = sem_open(sem_name_buff, O_CREAT, FILE_MODE, 1);
    if (!sm->sem) {
        err_ret("create semaphore  %s fail", sem_name_buff);
        return 3;
    }
    sm->status = EMPTY;
    return 0;
}

int send_sample(Sample *sm, Message *msg) {
    assert(sm && sm->fd && sm->ptr && sm->sem);

    sem_wait(sm->sem);
    if (sm->status == EMPTY)
        sm->status = FULL;
    memcpy(sm->ptr, msg, msg->msg_len + MSGHEADSIZE);
    sem_post(sm->sem);
    return msg->msg_len + MSGHEADSIZE;
}


int recv_sample(Sample *sm, Message *msg) {
    assert(sm && sm->fd && sm->ptr && sm->sem);

    // wait mutex
    sem_wait(sm->sem);
    // wait while shm empty, 1s equals 1000000ms
    while (sm->status == EMPTY) {
        sem_post(sm->sem);
        usleep(1000);
        sem_wait(sm->sem);
    }

    // read message header
    memcpy(msg, sm->ptr, MSGHEADSIZE);
    // read message body
    memcpy(msg->msg_data, sm->ptr + MSGHEADSIZE, msg->msg_len);
    // signal mutex
    sem_post(sm->sem);
    return msg->msg_len + MSGHEADSIZE;
}
