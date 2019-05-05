#include "include/message.h"
#include "include/requires.h"


ssize_t mesg_sendn(int fd, Message *msg) {
    return write(fd, msg, msg->mesg_len + HEADERSIZE);
}

void mesg_send(int fd, Message *msg) {
    int send_len = 0;
    int left_len = msg->mesg_len + HEADERSIZE;
    while ((send_len = write(fd, msg + send_len, left_len)) < left_len) {
        left_len -= send_len;
    }
}


ssize_t mesg_recv(int fd, Message *msg) {
    ssize_t n;
    long msg_len;
    again:
    if ((n = read(fd, msg, HEADERSIZE)) == 0)
        goto again;
    else if (n != HEADERSIZE) {
        err_ret("mesg_recv: mesg header err!");
        return n;
    }
    if ((msg_len = msg->mesg_len) > 0) {
        if ((n = read(fd, msg->mesg_data, msg_len)) != msg_len) {
            err_ret("mesg_recv: mesg body err!");
            return HEADERSIZE + n;
        }
    }
    return msg_len;
}
