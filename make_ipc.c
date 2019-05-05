#include "include/requires.h"
#include <sys/mman.h>
#include "include/sample.h"
#include "include/commu_def.h"
#include "include/proc_init.h"

static void make_fifo() {
    char buf[MAXLINE];
    
    strcpy(buf, "./tmp/");

    int len = strlen(buf);
    for (int i=0; i<NUM_PROC; ++i) {
        snprintf(buf + len, MAXLINE - len - 1,  "proc%d.fifo", i);
        unlink(buf);
        if (mkfifo(buf, FILE_MODE) < 0 && errno != EEXIST) {
            err_quit("make fifo failed: %s", buf);
        }
    }
}


static void make_shd_mem() {
    char buf[MAXLINE];

    for (int i=0; i<NUM_PROC; ++i) {
        snprintf(buf, MAXLINE - 1, "proc%d.shdm", i);
        if (shm_open(buf, O_FLAG, FILE_MODE) < 0) {
            err_quit("create shm fail: %s", buf);
        }
    }
}

static void make_semaphore() {
    int flag = O_CREAT | O_RDWR;
    sem_t *sem;

    sem_unlink(FIFO_RD_SEM);
    if ((sem = sem_open(FIFO_RD_SEM, flag, FILE_MODE, NUM_PROC)) == SEM_FAILED) {
        err_quit("sem create fail: %s", FIFO_RD_SEM);
    }
    int val;
    sem_getvalue(sem, &val);
    printf("sem val: %d\n", val);
    sem_close(sem);

    sem_unlink(FIFO_WR_SEM);
    if ((sem = sem_open(FIFO_WR_SEM, flag, FILE_MODE, 0)) == SEM_FAILED) {
        err_quit("sem create fail: %s", FIFO_WR_SEM);
    }
    sem_close(sem);
    
    sem_unlink(FIFO_RD_COUNT_SEM);
    if ((sem = sem_open(FIFO_RD_COUNT_SEM, flag, FILE_MODE, 1)) == SEM_FAILED) {
        err_quit("sem create fail: %s", FIFO_RD_COUNT_SEM);
    }
    sem_close(sem);

}

static void make_proc_ipc_sem() {
    char sem_name[MAXLINE];
    sem_t *sem;
    for (int i=0; i<NUM_PROC-1; ++i) {
        snprintf(sem_name, MAXLINE-1, "proc%d_2_proc%d", i, i+1);

        sem_unlink(sem_name);
        if ((sem = sem_open(sem_name, O_CREAT | O_CREAT, FILE_MODE, 0)) == SEM_FAILED) {
            err_quit("make proc ipc sem fail: %s", sem_name);
        }
        sem_close(sem);
    }
}

int main() {
    make_fifo();
    make_shd_mem();
    make_semaphore();
    make_proc_ipc_sem();
}
