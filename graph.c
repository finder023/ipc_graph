#include "include/graph.h"
#include <string.h>

// private funcs

static int _read_line(int fd, char *buff) {
    assert(buff);
    int n = 0;

    for (;;) {
    again:
        if ((n = read(fd, buff, 1)) == 1) {
            if (*buff == EOF) 
                return 1;
            if (*buff == '\n')
                break;
            ++n; ++buff;
        } else {
            if (errno == EINTR)
               goto again;
            return 1;
        }
        if (unlikely(n >= MAXLINE - 1))
            break;
    }
    *buff = 0;
    return 0;
}

inline static void _skip_space(const char **s) {
    assert(s);
    while (*s && **s == ' ') ++*s;
}

static int _is_valid_line(const char **s) {
    assert(s && *s);
    size_t n;
    
    _skip_space(s);
    // comment line
    if (**s == '#')
        return 0;
    // cur line len less than len of prefix, invalid
    if ((n = strlen(*s)) < strlen(CONF_PREFIX))
        return 0;

    if (!strcmp(CONF_PREFIX, *s))
        return 0;

    *s += strlen(CONF_PREFIX);
    return 1;
}

static int _read_num(const char **s) {
    assert (s && *s);

    _skip_space(s);
    int res = atoi(*s);
    while (*s && **s != ' ')    ++*s;
    return res;
}

static int _read_str(const char **s, char *buf) {
    assert(s && *s && buf);

    _skip_space(s);
    int n = 0;
    const char *s_cp = *s;
    while (*s && **s != ' ')    {++n; ++*s;}
    memcpy(buf, s_cp, n);
    buf[n] = 0;
    return n;
}

// graph init funcs

static void _graph_init(Graph *g) {
    assert(g);
    g->size = 0;
    g->num_in_only = 0;
    g->num_out_only = 0;
    memset(g->all_connector, 0, sizeof(Connector*) * MAX_GRAPH_MODULE);
}

static int _build_connector_from_conf(Connector *c, char *line_buff) {
    assert(c && line_buff);

    char name_buff[MAXLINE];

    if (strlen(line_buff) == 0)
        return 1;
    
    const char *s = line_buff;
    if (!_is_valid_line(&s))
        return 2;

    if (_read_str(&s, name_buff) == 0) {
        err_ret("bad conf file: module name error");
        return 3;
    }

    strcpy(c->name, name_buff);

    if ((c->rd_num = _read_num(&s))) {
        assert(c->rd_num <= MAX_RD_NUM);
        for (int i=0; i<c->rd_num; ++i) {
            if (_read_str(&s, name_buff) == 0) {
                err_ret("bad conf file: rd name error");
                return 4;
            }
            strcpy(c->rd_name_set[i], name_buff);
        }
    }

    if ((c->wr_num = _read_num(&s))) {
        assert(c->wr_num <= MAX_WR_NUM);
        for (int i=0; i<c->wr_num; ++i) {
            if (_read_str(&s, name_buff) == 0) {
                err_ret("bad conf file : wr name error");
                return 5;
            }
            strcpy(c->wr_name_set[i], name_buff);
        }
    }
    return 0;
}

void build_graph(Graph *g) {
    _graph_init(g);
    
    int fd;
    char name_buff[MAXLINE];

    if ((fd = open(CONF_FILE, O_RDONLY)) <= 0) {
        err_quit("open conf file error");
    }

    for (;;) {
        if (_read_line(fd, name_buff))
            break;
        if (strlen(name_buff) == 0)
            continue;
        g->all_connector[g->size] = (Connector*)malloc(sizeof(Connector));
        if (_build_connector_from_conf(g->all_connector[g->size], name_buff) 
                                                                        != 0) {
            free(g->all_connector[g->size]);
            continue;
        }
        if (g->all_connector[g->size]->rd_num == 0)
            g->num_in_only++;
        if (g->all_connector[g->size]->wr_num == 0)
            g->num_out_only++;
        g->size++;
    }

    // write g->size to all connetor
    for (int i=0; i<g->size; ++i) {
        g->all_connector[i]->net_num = g->size;
    }
}

void destroy_graph(Graph *g) {
    assert(g);

    for (int i=0; i<g->size; ++i) {
        free(g->all_connector[i]);
    }
}


Connector *dispatch_proc_net(Graph *g) {
    assert(g);

    Connector *net;
    sem_t *count_sem, *mutex_sem;
    int val;

    if ((count_sem = sem_open(PROC_COUNT_SEM, O_RDWR)) == SEM_FAILED) {
        err_quit("dispatch net fail => sem_open fail: %s", PROC_COUNT_SEM);
    }
    if ((mutex_sem = sem_open(PROC_COUNT_MUTEX, O_RDWR)) == SEM_FAILED) {
        err_quit("dispatch net fail => sem open fail: %s", PROC_COUNT_MUTEX);
    }

    sem_wait(mutex_sem);
    
    sem_wait(count_sem);
    sem_getvalue(count_sem, &val);
    net = g->all_connector[val];  
    sem_post(mutex_sem);

    return net;
    
}


#if TEST

int main() {
    Graph g;
    build_graph(&g);
    printf("-------------------------------------------------\n");
    for (int i=0; i<g.size; ++i) {
        printf("%s\t%d\t", g.all_connector[i]->name, 
               g.all_connector[i]->rd_num);
        for (int j=0; j<g.all_connector[i]->rd_num; ++j) {
            printf("%s\t", g.all_connector[i]->rd_name_set[j]);
        }
        printf("%d\t", g.all_connector[i]->wr_num);
        for (int j=0; j<g.all_connector[i]->wr_num; ++j) {
            printf("%s\t", g.all_connector[i]->wr_name_set[j]);
        }
        printf("\n");
    }
    destroy_graph(&g);
}

#endif
