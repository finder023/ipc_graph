#include "include/requires.h"
#include <sys/mman.h>
#include "include/sample.h"
#include "include/commu_def.h"
#include "include/ipc_init.h"
#include "include/graph.h"



static void _make_sem(const char *name, const int default_val) {
    assert(name);

    sem_t *sem;
    sem_unlink(name);
    if ((sem = sem_open(name, O_CREAT, FILE_MODE, default_val)) == SEM_FAILED) {
        err_quit("make fifo sem fail: %s", name);
    }
    sem_close(sem);
}

static void _make_shd_mem(Graph *g) {
    assert(g);

    char name_buff[MAXLINE];
    Connector *net;
    int wr_num;

    for (int i=0; i<g->size; ++i) {
        net = g->all_connector[i];
        wr_num = net->wr_num;

        for (int j=0; j<wr_num; ++j) {
            snprintf(name_buff, MAXLINE, "%s_2_%s.shm", net->name, \
                     net->wr_name_set[j]);
            shm_unlink(name_buff);
            if (shm_open(name_buff, O_CREAT | O_RDWR, FILE_MODE) < 0) {
                err_quit("make shd_mem fail: %s", name_buff);
            }
            
            // shd_mem sem for read and write
            snprintf(name_buff, MAXLINE, "%s_2_%s.shmsem", net->name,
                     net->wr_name_set[i]);
            _make_sem(name_buff, 0);
        }
    }
} 


static void _make_fifo(Graph *g) {
    assert(g);

    char name_buff[MAXLINE];
    Connector *con;
    int wr_num;
    for (int i=0; i<g->size; ++i) {
        con = g->all_connector[i];
        wr_num = con->wr_num;
        for (int j=0; j<wr_num; ++j) {
            snprintf(name_buff, MAXLINE, "./tmp/%s_2_%s.fifo", con->name, 
                     con->wr_name_set[j]);
            unlink(name_buff);
            if (mkfifo(name_buff, FILE_MODE) < 0 && errno != EEXIST) {
                err_quit("make fifo failed: %s", name_buff);
            }

            // semaphore
            snprintf(name_buff, MAXLINE, "%s_2_%s.fifosem", con->name, 
                     con->wr_name_set[i]);
            _make_sem(name_buff, 0);
        }
    }
}

static void _make_proc_id_sem(Graph *g) {
    assert(g);

    _make_sem(PROC_COUNT_SEM, g->size);
    _make_sem(PROC_COUNT_MUTEX, 1);
}

static void _make_fifo_rd_sem(Graph *g) {
    assert(g);
    _make_sem(FIFO_RD_COUNT_SEM, g->size);
    _make_sem(FIFO_WR_SEM, 0);
}

int main() {
    Graph g;
    build_graph(&g);

    _make_fifo(&g);
    _make_shd_mem(&g);
    _make_proc_id_sem(&g);

    destroy_graph(&g);
}

