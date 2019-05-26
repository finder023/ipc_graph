#include "include/ipc_init.h"
#include "include/queue.h"
#include "include/sample.h"


Connector *dispatch_proc_net(Graph *g) {
    assert(g);

    Connector *net;
    sem_t *count_sem, *mutex_sem;
    int val;

    if ((count_sem = sem_open(PROC_COUNT_SEM, O_RDWR)) == SEM_FAILED) {
        err_quit("dispatch net fail => sem_open fail: %s", PROC_COUNT_SEM);
    }
    if ((mutex_sem = sem_open(PROC_COUNT_MUTEX, O_RDWR)) == SEM_FAILED) {
        err_quit("dispatch net fail => sem open fail: %s", PROC_COUNT_MUTEX);
    }

    sem_wait(mutex_sem);
    
    sem_wait(count_sem);
    sem_getvalue(count_sem, &val);
    net = g->all_connector[val];
    
    sem_post(mutex_sem);

    return net;
    
}

