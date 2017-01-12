#include "ud.h"

int main(int argc, char **argv)
{
    struct sockaddr_in ser_addr, cli_addr;

    int cli_fd;
    socklen_t len;
    size_t msgLen;
    char resp[BUF_SIZE];
    ssize_t nBytes;

    syserr(argc < 3, "argc < 3");

    cli_fd = socket(AF_INET, SOCK_DGRAM, 0);
    syserr(cli_fd == -1, "socket\n");

    //construct server
    memset(&ser_addr, 0, sizeof(struct sockaddr_in));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(PORT_NUM);
    syserr(inet_pton(AF_INET, argv[1], &ser_addr.sin_addr) <= 0, "inet_pton(ip)\n");
 
    //work
    for(int i = 2; i < argc; i++) {
        msgLen = strlen(argv[i]);
        len = sizeof(struct sockaddr_in);
        
        syserr(sendto(cli_fd, argv[i], msgLen, 0, (struct sockaddr *) &ser_addr, len) != msgLen, "client sendto\n");

        nBytes  = recvfrom(cli_fd, resp, BUF_SIZE, 0, NULL, NULL);
        syserr(nBytes == -1,  "cleint recvfrom\n");
        
        printf("Response %d: %.*s\n", i, (int) nBytes, resp);
    }
}
