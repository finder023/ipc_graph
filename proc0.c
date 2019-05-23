#include "include/requires.h"
#include "include/proc_init.h"
#include "include/queue.h"
#include "include/proc_ipc.h"

const char *proc_name = "proc0";

int main() {
    ProcIpc ipc; 
    Connector ct;   
    strcpy(ct.name, proc_name);
    proc_ipc_init(&ct, &ipc);

    Message msg;
    const char *data = "demo";
    strcpy(msg.msg_data, data);
    msg.msg_len = strlen(data) + 1;
    const char *wr_name = "proc0_2_proc1";

    sem_t *sem;
    if ((sem = sem_open(wr_name, O_RDWR)) == SEM_FAILED) {
        err_ret("proc0 sem_open fail");
    }
    
    send_queue(&ipc._queue, &msg);
    sem_post(sem);
    
    int val;
    sem_getvalue(sem, &val);
    // printf("%s | %s : %d\n", proc_name, wr_name, val);
    printf("proc0 exit\n");

}
