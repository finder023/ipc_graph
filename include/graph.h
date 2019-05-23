#ifndef __CONNECTOR_H
#define __CONNECTOR_H

#include "requires.h"
#include "conf.h"

typedef struct Connector {
    char name[MAX_NAME_LEN];
    int rd_num;
    int wr_num;
    char rd_name_set[MAX_RD_NUM][MAX_NAME_LEN];
    char wr_name_set[MAX_WR_NUM][MAX_NAME_LEN];
} Connector;

typedef struct Graph {
    Connector *all_connector[MAX_GRAPH_MODULE];
    int size;
} Graph;

void build_graph(Graph *);
void destroy_graph(Graph *);

#endif
