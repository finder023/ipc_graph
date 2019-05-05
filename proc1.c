#include "include/requires.h"
#include "include/proc_init.h"
#include "include/queue.h"
#include "include/proc_ipc.h"

const char *proc_name = "proc1";

int main() {
    ProcIpc ipc;
    Connector net;
    strcpy(net.name, proc_name);
    proc_ipc_init(&net, &ipc);

    Message msg;
    sem_t *sem_rd, *sem_wr;
    const char *rd_name = "proc0_2_proc1";
    const char *wr_name = "proc1_2_proc2";

    if ((sem_rd = sem_open(rd_name, O_RDWR)) == SEM_FAILED) {
        err_ret("proc1 open sem_rd fail");
    }
    if ((sem_wr = sem_open(wr_name, O_RDWR)) == SEM_FAILED) {
        err_ret("proc1 open sem_wr fail");
    }

    int val;
    
    sem_wait(sem_rd);
    sem_getvalue(sem_rd, &val);
    // printf("%s after wait | %s : %d\n", proc_name, rd_name, val);

    recv_queue(&ipc._queue, &msg, 0);

    printf("%s: recv message | %s\n", proc_name, msg.msg_data);

    send_queue(&ipc._queue, &msg);
    sem_post(sem_wr);
    sem_getvalue(sem_wr, &val);
    // printf("%s | %s : %d\n", proc_name, wr_name, val);
    printf("proc1 exit\n");
}
