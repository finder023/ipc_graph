#include "include/blackboard.h"
#include <string.h>

void init_balckboard(Blackboard *bd) {
    assert(bd);
    memset(bd->buff, 0, MAXLINE);
    pthread_mutex_init(&bd->mutex, NULL);

    bd->status = EMPTY;
}

void send_blackboard(Blackboard *bd, Message *msg) {
    assert(bd && msg);
    
    pthread_mutex_lock(&bd->mutex);
    if (bd->status == EMPTY)
        bd->status = FULL;

    memcpy(bd->buff, msg, msg->msg_len + MSGHEADSIZE);
    pthread_mutex_unlock(&bd->mutex);
}


void recv_blackboard(Blackboard *bd, Message *msg) {
    assert(bd && msg);

    pthread_mutex_lock(&bd->mutex);
    while (bd->status == EMPTY) {
        pthread_mutex_unlock(&bd->mutex);
        usleep(1000);
        pthread_mutex_lock(&bd->mutex);
    }

    memcpy(msg, bd->buff, MSGHEADSIZE);
    memcpy(msg->msg_data, bd->buff + MSGHEADSIZE, msg->msg_len);
    pthread_mutex_unlock(&bd->mutex);
}
