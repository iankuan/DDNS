#ifndef DNS_IMPL_H
#define DNS_IMPL_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <netinet/in.h>
#include "debug.h"

static inline
int socket_config(int domain, int type, int protocol, struct sockaddr *addr, socklen_t len)
{
    int fd;

    fd = socket(domain, type, protocol);
    syserr(fd == -1, "socket_config: socket()\n");

    syserr(
        bind(fd, addr,len) < 0,
        "socket_config: bind()\n");

    return fd;
}

static inline
ssize_t socket_recvfrom(int sk_fd, uchar buf[], struct sockaddr *addr, socklen_t *len)
{
    ssize_t nBytes;

    syserr(
        (nBytes = recvfrom(sk_fd, buf, BUF_SIZE, 0, addr, len)) < 0,
        "socket_recvfrom: recvfrom()\n");

    return nBytes;
}

static inline
void socket_sendto(int sk_fd, uchar buf[], ssize_t buf_len, struct sockaddr * addr, socklen_t len)
{
    syserr(
        sendto(sk_fd, buf, buf_len, 0, addr, len) < 0,
        "socket_sendto()\n");
}

#endif ///DNS_IMPL_H
