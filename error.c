#include "include/requires.h"

#include <stdarg.h>
#include <syslog.h>
#include <string.h>

static void err_doit(int, const char *, va_list);

/* Nonfatal error relate to system call, print msg and return */
void err_ret(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    err_doit(0, fmt, ap);
    va_end(ap);
    return;
}

/* Fatal error relate to a system call, print msg and exit */
void err_quit(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    err_doit(1, fmt, ap);
    va_end(ap);
    exit(1);
}


static void err_doit(int errnoflag, const char *fmt, va_list ap) {
    int errno_save;
    ssize_t n;
    char buff[MAXLINE];

    errno_save = errno;
    vsnprintf(buff, sizeof(buff), fmt, ap);

    n = strlen(buff);
    if (errnoflag) {
        snprintf(buff+n, sizeof(buff)-n, ": %s", strerror(errno_save));
    }
    strcat(buff, "\n");

    fflush(stdout);
    fputs(buff, stderr);
    fflush(stderr);

}
