#include <stdio.h>
#include <arpa/inet.h>

#include "message.h"

#define BUF_SIZE 10000

char dns_server[10][100];
int dns_server_count = 0;

int main(int argc, char **argv)
{
    char hostname[100];

    //Get DNS servers from resolv.conf file
    //TODO: automatical not maunal
    dns_server[0] = "8.8.8.8";
    
    syserr(argc < 1, "argument error");

    //Get the ip of this host, A record


    return 0;
}

void dns_get_host_ip(char *host, RR_QTYPE_t qtype)
{
    char buf[BUF_SIZE], *qname, *reader;

    struct sockaddr_in addr;

    int sockfd;

    struct ;//RR_RECORD
    struct sockaddr_in dest;

    //init sockaddr
    dest.sin_family = AF_INET;
    dest.sin_port = htons(53);///TODO: what's htons(), inet_addr
    dest.sin_addr.s_addr = inet_addr(dns_server[0]);

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);///TODO: flag
    syserr(sockfd == -1, "socket\n");

    ///just declaration
    declare_dns_message(dns, 1, 0, 0, 0);

    ///init for struct assigment
    init_dns_header(_tmp_header,
                      ,//id
                              0,
                              0,
                     _STD_QUERY,
                              0,
                              1,
                              0,
                              0,
                              0,
                              0,
                        _NO_ERR,
                              1,//qdcount
                              0,
                              0,
                              0);
                                 

    QUESTION quest = {
    };

    ///Assign dns message
    dns->header = _tmp_header;

