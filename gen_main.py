#! /usr/bin/python3

import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-i', '--ipc_type', dest='ipc_type', 
                    choices={'Queue', 'Sample'}, default='Queue')
args = parser.parse_args()

conf_file = 'graphConf'

headers = '''\
#include "include/requires.h"
#include "include/conf.h"
#include "include/graph.h"
#include "include/queue.h"
#include "include/opr_impl.h"
#include "include/gen_main.h"

const char *demo = "demo";
'''

user_funcs = '''\
void user_func(Message *msg_in, int in_len, Message *msg_out, int out_len) {
    USER_FUNC_BODY(msg_in, in_len, msg_out, out_len);
}        
'''

main_str = '''
int main() {
    Graph *g = (Graph*)malloc(sizeof(Graph));
    %s *q = (%s*)malloc(sizeof(%s));
    Message *msg_in, *msg_out;
    
    BUILD_GRAPH(%s, g, q);
    ALLOC_MSG(&msg_in, &msg_out, q, demo, strlen(demo) + 1);
    
    for (int i=0; i<q->rd_num; ++i) {
        q->opr.recv(q, &msg_in[i], i);
    }

    // user  process
    printf("\\t%%s: ", q->name);
    user_func(msg_in, q->rd_num, msg_out, q->wr_num);

    for (int i=0; i<q->wr_num; ++i) {
        q->opr.send(q, &msg_out[0], i);    
    }
    
    FREE_MSG(msg_in, msg_out);
    FREE_GRAPH(g, q);
}
''' % (args.ipc_type, args.ipc_type, args.ipc_type, args.ipc_type)

def read_conf():
    names = []
    with open(conf_file, 'r') as f:
        for line in f.readlines():
            if line.startswith('#'):
                continue
            if (not line.startswith('graph_module')):
                continue
            line = line.lstrip('graph_module')
            line = line.strip(' ')
            parts = line.split(' ')
            assert len(parts) > 1
            names.append(parts[0])
    return names

def main():
    names = read_conf()
    for name in names:
        with open(name + '.c', 'w') as f:
            f.write(headers)
            f.write(user_funcs)
            f.write(main_str)
    with open('start.sh', 'w') as f:
        f.write('#! /bin/bash\n\n')
        f.write('./build/make_ipc\n')
        for name in names:
            f.write('./build/' + name + ' &\n')

if __name__ == '__main__':
    main()
