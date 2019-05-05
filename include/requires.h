#ifndef __REQUIRES_H
#define __REQUIRES_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <semaphore.h>
#include <assert.h>
#include <string.h>

#define DEBUG 0

#define MAXLINE 4096
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define CONF_PREFIX "graph_module"

#define likely(x)    __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)

// Nonfatal error, print err msg and return
void err_ret(const char *, ...);
// fatal error, print err msg and exit
void err_quit(const char *, ...);

// signal
//typedef void sigfunc(int);

//void sig_chld(int);
//sigfunc *signal(int, sigfunc *);


#endif
