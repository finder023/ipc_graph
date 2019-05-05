#include "include/buffer.h"
#include <string.h>
#include <stdbool.h>

inline static bool _buffer_empty(Buffer *bf) {
    return bf->st_index == bf->ed_index;
}

void init_buffer(Buffer *bf) {
    assert(bf);
   
    pthread_mutex_init(&bf->mutex, NULL);
    
    bf->st_index = bf->ed_index = bf->size = 0;
    for (int i=0; i<MAX_MSG_SIZE; ++i) {
        bf->buff[i] = (Message*)malloc(sizeof(Message));
    }

}

void send_buffer(Buffer *bf, Message *msg) {
    assert(bf && msg);

    pthread_mutex_lock(&bf->mutex);
    while (bf->size == MAX_MSG_SIZE) {
        pthread_mutex_unlock(&bf->mutex);
        usleep(1000);
        pthread_mutex_lock(&bf->mutex);
    }

    memcpy(bf->buff[bf->ed_index++], msg, msg->msg_len + MSGHEADSIZE);
    if (bf->ed_index == MAX_MSG_SIZE)
        bf->ed_index = 0;
    bf->size++;
    pthread_mutex_unlock(&bf->mutex);
}

void recv_buffer(Buffer *bf, Message *msg) {
    assert(bf && msg);
    
    pthread_mutex_lock(&bf->mutex);
    while (_buffer_empty(bf)) {
        pthread_mutex_unlock(&bf->mutex);
        usleep(1000);
        pthread_mutex_lock(&bf->mutex);
    }

    memcpy(msg, bf->buff[bf->st_index], MSGHEADSIZE);
    memcpy(msg + MSGHEADSIZE, bf->buff[bf->st_index] + MSGHEADSIZE, msg->msg_len);
    if (++bf->st_index == MAX_MSG_SIZE)
        bf->st_index = 0;
    bf->size--;
    pthread_mutex_unlock(&bf->mutex);
}

