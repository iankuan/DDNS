#include "ud.h"

int main(int argc, char **argv)
{
    struct sockaddr_in ser_addr, cli_addr;

    int ser_fd;
    socklen_t len;
    char buf[BUF_SIZE];
    ssize_t nBytes;

    char cli_addr_str[INET_ADDRSTRLEN];

    ser_fd = socket(AF_INET, SOCK_DGRAM, 0);
    syserr(ser_fd == -1, "socket\n");

    //init
    memset(&ser_addr, 0, sizeof(struct sockaddr_in));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(PORT_NUM);
    syserr(inet_pton(AF_INET, argv[1], &ser_addr.sin_addr) <= 0, "serv rec ip\n");

    ///bind
    ///FIXME: second argument should take care
    //syserr(bind(ser_fd, &ser_addr, sizeof(struct sockaddr_un)) == -1, "bind\n");
    syserr(bind(ser_fd, (struct sockaddr *) &ser_addr, sizeof(struct sockaddr_in)) == -1, "serv bind\n");

    ///work
    while(1)
    {
        len = sizeof(struct sockaddr_in);
        nBytes = recvfrom(ser_fd, buf, BUF_SIZE, 0, (struct sockaddr *) &cli_addr, &len);
        syserr(nBytes == -1,  "serv recvfrom\n");

        syserr(inet_ntop(AF_INET, &cli_addr.sin_addr, cli_addr_str, INET_ADDRSTRLEN) <= 0, "server: ip transform fail\n");
        printf("Server recived %ld bytes from (%s, %u)\n", (long) nBytes, cli_addr_str, ntohs(cli_addr.sin_port));

        for(int i = 0; i < nBytes; i++)
            buf[i] = toupper(buf[i]);

        syserr(sendto(ser_fd, buf, nBytes, 0, (struct sockaddr *) &cli_addr, len) != nBytes, "serv sendto\n");
    }

    syserr(1,"out of while loop\n");
}
