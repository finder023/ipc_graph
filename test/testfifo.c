#include "include/requires.h"
#include "include/message.h"

int main() {
    int fd_rd, fd_wr;

    const char *name = "./tmp/proc0.fifo";
    

    if ((fd_wr = open(name, O_WRONLY | O_NONBLOCK)) < 0) {
        err_ret("open fifo failed: %s", name);
    }
    if ((fd_rd = open(name, O_RDONLY | O_NONBLOCK)) < 0) {
        err_ret("open fifo failed: %s", name);
    }
    const char *msgin = "hello world";
    write(fd_wr, msgin, strlen(msgin) + 1);

    char buff[MAXLINE];
    read(fd_rd, buff, strlen(msgin) + 1);
    printf("read %s\n", buff);
}
