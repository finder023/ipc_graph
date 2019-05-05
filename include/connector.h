#ifndef __CONNECTOR_H
#define __CONNECTOR_H

#include "requires.h"

#define MAX_RD_NUM 5
#define MAX_WR_NUM 5

#define MAX_NAME_LEN 50

typedef struct Connector {
    char name[MAX_NAME_LEN];
    int rd_num;
    int wr_num;
    char rd_name_set[MAX_RD_NUM][MAX_NAME_LEN];
    char wr_name_set[MAX_WR_NUM][MAX_NAME_LEN];
} Connector;



#endif
