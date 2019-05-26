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
#include <stdarg.h>
#include <sys/mman.h>
#include "conf.h"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


#define likely(x)    __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)

#if DEBUG
    #define dprintf(f_, ...) printf((f_), ##__VA_ARGS__)
#else
    #define dprintf(f_, ...)
#endif

// Nonfatal error, print err msg and return
void err_ret(const char *, ...);
// fatal error, print err msg and exit
void err_quit(const char *, ...);

// signal
//typedef void sigfunc(int);

//void sig_chld(int);
//sigfunc *signal(int, sigfunc *);


#endif
