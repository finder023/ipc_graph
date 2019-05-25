#include "include/requires.h"
#include "include/conf.h"
#include "include/ipc_init.h"
#include "include/graph.h"

int main() {
    Graph g;
    Connector *net;
    build_graph(&g);
    net = dispatch_proc_net(&g);  
    

    destroy_graph(&g);
}
