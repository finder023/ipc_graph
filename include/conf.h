#ifndef __CONF_H
#define __CONF_H

// conf 
#define CONF_FILE           "graphConf"
#define CONF_PREFIX         "graph_module"

// process id dispacher
#define PROC_COUNT_SEM      "proc_count_sem"
#define PROC_COUNT_MUTEX    "proc_count_mutex"

#define FIFO_RD_COUNT_SEM   "fifo_rd_count_sem"
#define FIFO_WR_SEM         "fifo_wr_sem"

#define CONF_PREFIX         "graph_module"

// connector
#define MAX_RD_NUM 5
#define MAX_WR_NUM 5

#define MAX_NAME_LEN 50

#define MAX_GRAPH_MODULE 100

// common
#define MAXLINE 4096

#define MAXMSG 4000

#define SHM_LEN 4096

// test or not
#define TEST 0

// debug print func
#define DEBUG 0

#define IPC_T "Queue"

#endif
