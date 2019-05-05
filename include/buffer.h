#ifndef __BUFFER_H
#define __BUFFER_H

#include "requires.h"
#include "message.h"

#define MAX_MSG_SIZE 512

typedef struct Buffer {
    Message *buff[MAX_MSG_SIZE];
    pthread_mutex_t mutex;
    int size;
    int st_index;
    int ed_index;
} Buffer;

void init_buffer(Buffer *);
void read_buffer(Buffer *, Message *);
void send_buffer(Buffer *, Message *);

#endif
