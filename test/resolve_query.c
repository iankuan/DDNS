#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


#include "protocol/message.h"
#include "debug.h"
#include "core/dns.h"

#define BUF_SIZE 2000

char dns_server[10][100];
int dns_server_count = 0;


///FUNCTION
void dns_get_host_ip(char *host, RR_QTYPE_t qtype);
/**
 * TODO: IMPORTANT, but why
 * This would convert www.google.com to 3www6google3com
 */
static inline
void ch_to_dns_name(char *host, char *dns)
{
    for(int i = 0, count = 0; i < strlen(host) + 1; i++)
    {
        if(host[i] == '.' || host[i] == '\0')
        {
            *dns++ = i - count;
            while(count < i)
                *dns++ = host[count++];

            count++;
        }
    }
    *dns = '\0';
}

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
    char buf[BUF_SIZE];
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
                           (u16_t) getpid(),/*     id*/
                                          0,/*     qr*/
                                          0,/* opcode*/
                                 _STD_QUERY,/*     aa*/
                                          0,/*     tc*/
                                          1,/*     rd*/
                                          0,/*     ra*/
                                          0,/*      z*/
                                   _NOERROR,/*  rcode*/
                                          1,/*qdcount*/
                                          0,/*ancount*/
                                          0,/*nscount*/
                                          0)/*arcount*/;

    ///DNS Header Assign
    /*dns_header_assign(     *hdr,
        (u16_t) htons(getpid()),
                              0,
                              0,
              htons(_STD_QUERY),
                              0,
                       htons(1),
                              0,
                 htons(_NO_ERR),
                 htons(qdcount),
                              0,
                              0,
                              0);*/
    ///DNS Question
    //DNS_QUESTION_ptr_t *q_list = (DNS_QUESTION_ptr_t *) malloc(qdcount * sizeof(DNS_QUESTION_ptr_t));
    DNS_QUESTION_ptr_t **q_list = (DNS_QUESTION_ptr_t **) malloc(qdcount * sizeof(DNS_QUESTION_ptr_t *));
    
    for(int i = 0; i < qdcount; i++)
    {
        q_list[i] = (DNS_QUESTION_ptr_t *) malloc(sizeof(DNS_QUESTION_ptr_t));
    }

    ///DNS Question Assign
    //dns_question_locate(q_list[0], &buf[locate]);

    char *_tmp = (char *) malloc(30 *sizeof(char));
    ch_to_dns_name(host, _tmp);
    /*locate += strlen(q_list[0].qname) + 1;

    q_list[0].question = (DNS_QUESTION_t *) &buf[locate];
    q_list[0].question->qtype = htons(qtype);
    q_list[0].question->qclass = htons(_IN);

    locate += sizeof(*(q_list[0].question));*/
    locate += dns_question_locate_assign(q_list[0], &buf[locate], _tmp, qtype, _IN);


    ///Sendto
    printf("Sending Packet\n");
    
    syserr(sendto(sockfd, buf, locate, 0, (struct sockaddr *) &dest,
            sizeof(dest)) < 0, "sendto\n");
    printf("DONE\n");

    ///Receive
    printf("Receiving Answer\n");

    size_t len = sizeof(dest);
    syserr((nBytes = recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *) &dest,
            (socklen_t *) &len)) < 0, "sendto\n");
    printf("DONE\n");

    dns_header_show(hdr);

    //dns_header_locate(hdr, buf);

    /*printf("\n %u Question", ntohs(hdr->qdcount));
    printf("\n %u Answer", ntohs(hdr->ancount));
    printf("\n %u Authority", ntohs(hdr->nscount));
    printf("\n %u Additional\n", ntohs(hdr->arcount));

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
