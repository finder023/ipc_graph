#include "include/requires.h"
#include "include/proc_init.h"
#include "include/queue.h"
#include "include/proc_ipc.h"

const char *proc_name = "proc4";

int main() {
    ProcIpc ipc; 
    Connector net;
    strcpy(net.name, proc_name);
    proc_ipc_init(&net, &ipc);

    Message msg;
    sem_t *sem_rd;
    const char *rd_name = "proc3_2_proc4";
    
    if ((sem_rd = sem_open(rd_name, O_RDWR)) == SEM_FAILED) {
        err_ret("proc4 open sem_rd fail");
    }

    int val;
    
    sem_wait(sem_rd);
    sem_getvalue(sem_rd, &val);
    // printf("%s | %s : %d\n", proc_name, rd_name, val);

    recv_queue(&ipc._queue, &msg, 0);

    printf("%s: recv message | %s\n", proc_name, msg.msg_data);

    printf("proc4 exit\n");
}
