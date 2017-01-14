#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "core/dns.h"
#include "debug.h"
#include "type.h"

char dns_server[10][100];
int dns_server_count = 0;


///FUNCTION
void dns_get_host_ip(char *host, RR_QTYPE_t qtype);

int main(int argc, char **argv)
{
    char host[] = "www.google.com";

    //Get DNS servers from resolv.conf file
    //TODO: automatical not maunal
    strcpy(dns_server[0], "127.0.1.1");

    //Get the ip of this host, A record
    dns_get_host_ip(host, _A);

    return 0;
}

void dns_get_host_ip(char *host, RR_QTYPE_t qtype)
{
    uchar buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);

    size_t locate = 0;
    int sockfd;
    ssize_t nBytes;
    //struct sockaddr_in addr;

    struct sockaddr_in dest;

    //init sockaddr
    dest.sin_family = AF_INET;
    dest.sin_port = htons(53);///TODO: what's htons(), inet_addr
    dest.sin_addr.s_addr = inet_addr(dns_server[0]);

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);///TODO: flag
    syserr(sockfd == -1, "socket\n");

    /**
     * Construct MESSAGE
     */
    int qdcount = 1;

    ///declare DNS MESSAGE
    ///DNS_MESSAGE_ptr_t msg_p;
    
    ///DNS Header
    //DNS_HEADER_t *hdr = (DNS_HEADER_t *) buf;
    dns_header_declare(hdr);
    locate += dns_header_locate_assign( hdr,
                                        buf,
                           (u16_t) getpid(),
                                          0,
                                          0,
                                 _STD_QUERY,
                                          0,
                                          1,
                                          0,
                                          0,
                                   _NOERROR,
                                          1,
                                          0,
                                          0,
                                          0);

    ///DNS Question
    DNS_QUESTION_ptr_t **q_list = (DNS_QUESTION_ptr_t **) malloc(qdcount * sizeof(DNS_QUESTION_ptr_t *));
    
    for(int i = 0; i < qdcount; i++)
    {
        q_list[i] = (DNS_QUESTION_ptr_t *) malloc(sizeof(DNS_QUESTION_ptr_t));
    }

    ///DNS Question Assign
    char *_tmp = (char *) malloc(30 *sizeof(char));
    host_to_dns_name(_tmp, host);

    locate += dns_question_locate_assign(q_list[0], &buf[locate], _tmp, qtype, _IN);

    dns_question_member(q_list[0], qtype) = qtype;
    dns_question_member(q_list[0], qname) = "name";

    ///Sendto
    printf("Sending Packet\n");
    
    syserr(sendto(sockfd, buf, locate, 0, (struct sockaddr *) &dest,
            sizeof(dest)) < 0, "sendto\n");
    printf("DONE\n");

    ///Receive
    printf("Receiving Answer\n");

    socklen_t len = sizeof(dest);
    syserr((nBytes = recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *) &dest,
            &len)) < 0, "sendto\n");
    printf("DONE\n");

    dns_header_show(hdr);

    //locate = sizeof(hdr);
    
    if( is_compressive(&buf[locate]))
        printf("\nCOMPRESSIVE\n");
    else
        printf("\nNOT COMPRESSIVE\n");

    printf("\n%u\n", ((((u16_t) buf[locate]) << 8) + buf[locate + 1]) & compression_mask);
    printf("\n%s\n", &buf[((((u16_t) buf[locate]) << 8) + buf[locate + 1]) & compression_mask]);

    char *ht = (char *) malloc(50 * sizeof(char));

    dns_to_host_name(ht, buf, &locate);

    printf("\n%s\n", ht);

    /*
    dns_answer_declare(ans[20]);
    char *reader = &buf[locate];
    u16_t offset = 0;

    for(int i = 0; i < ntohs(hdr->ancount); i++)
    {
        if((*reader & 0xC0) == 0xC0) {//compression
            offset = (((u16_t) *reader & 0x3F) << 8) + *(reader + 1);
            printf("\n offset = %u\n", offset);
            ans[i]->name = &buf[offset];
        }
        else
            dns_answer_locate(ans[i], reader);
        printf("name = %s\n", ans[i]->name);
    }*/
}
