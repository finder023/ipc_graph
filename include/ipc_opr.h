#ifndef __IPC_OPR_H
#define __IPC_OPR_H

typedef struct IpcOpr {
    void (*init)(void*, void*);
    int (*send)(void*, void*, int);
    int (*recv)(void*, void*, int);
} IpcOpr;

// opr func type
typedef void (*init_func_t)(void*, void*);
typedef int (*send_func_t)(void*, void*, int);
typedef int (*recv_func_t)(void*, void*, int);

#endif

