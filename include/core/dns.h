/**
 * The domain system provides:
 * 
 *    - Standard formats for resource data.
 * 
 *    - Standard methods for querying the database.
 * 
 *    - Standard methods for name servers to refresh local data from
 *      foreign name servers.
 */
#include "protocol/message.h"
#include "dns_util.h"
#include "dns_impl.h"
#include "debug.h"

#define SERV_PORT 8008

///We use the Object-Oriened concept to build the interface of the dns server.
struct DNS {
    //.init
    //.init_database
    int (*init_service)(int domain, int type, int protocol, struct sockaddr *addr, socklen_t len);
    //.query
    ssize_t (*listen)(int sk_fd, uchar buf[], struct sockaddr *addr, socklen_t *len);
    void (*respond)(int sk_fd, uchar buf[], ssize_t buf_len, struct sockaddr * addr, socklen_t len);
};

void dns_header_show(DNS_HEADER_t *hdr)
{
    printf("**DNS_Header_t %p**\n", hdr);
    
    printf("Header->id = %hu\n", dns_header_member(hdr, id, ntohs));
    printf("Header->qr = %hu\n", dns_header_member(hdr, qr));
    printf("Header->opcode = %hu(%s)\n", dns_header_member(hdr, opcode), _OPCODE[dns_header_member(hdr, opcode)]);
    printf("Header->aa = %hu\n", dns_header_member(hdr, aa));
    printf("Header->tc = %hu\n", dns_header_member(hdr, tc));
    printf("Header->rd = %hu\n", dns_header_member(hdr, rd));
    printf("Header->ra = %hu\n", dns_header_member(hdr, ra));
    printf("Header-> z = %hu\n", dns_header_member(hdr,  z));
    printf("Header->rcode = %hu(%s)\n", dns_header_member(hdr, rcode), _RCODE[dns_header_member(hdr, rcode)]);

    printf("Header->qdcount = %hu\n", dns_header_member(hdr, qdcount, ntohs));
    printf("Header->ancount = %hu\n", dns_header_member(hdr, ancount, ntohs));
    printf("Header->nscount = %hu\n", dns_header_member(hdr, nscount, ntohs));
    printf("Header->nscount = %hu\n", dns_header_member(hdr, nscount, ntohs));
}

/*void rr_show(RR_ptr_t *rr)
{
    printf("**RR_ptr_t %p**\n", rr);
    
    printf("Header->id = %hu\n", dns_header_member(hdr, id, ntohs));
    printf("Header->qr = %hu\n", dns_header_member(hdr, qr));
    printf("Header->opcode = %hu(%s)\n", dns_header_member(hdr, opcode), _OPCODE[dns_header_member(hdr, opcode)]);
    printf("Header->aa = %hu\n", dns_header_member(hdr, aa));
    printf("Header->tc = %hu\n", dns_header_member(hdr, tc));
    printf("Header->rd = %hu\n", dns_header_member(hdr, rd));
    printf("Header->ra = %hu\n", dns_header_member(hdr, ra));
    printf("Header-> z = %hu\n", dns_header_member(hdr,  z));
    printf("Header->rcode = %hu(%s)\n", dns_header_member(hdr, rcode), _RCODE[dns_header_member(hdr, rcode)]);

    printf("Header->qdcount = %hu\n", dns_header_member(hdr, qdcount, ntohs));
    printf("Header->ancount = %hu\n", dns_header_member(hdr, ancount, ntohs));
    printf("Header->nscount = %hu\n", dns_header_member(hdr, nscount, ntohs));
    printf("Header->nscount = %hu\n", dns_header_member(hdr, nscount, ntohs));
}*/


void dns_query(char *dns, uchar *buf, size_t len)
{
    int sfd;
    struct sockaddr_in dest;
    ssize_t nBytes;

    /**
     * SOCKET INIT
     */
    ///init sockaddr
    dest.sin_family = AF_INET;
    dest.sin_port = htons(53);
    dest.sin_addr.s_addr = inet_addr(dns);

    ///init socket
    sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    syserr(sfd == -1, "dns_query:socket\n");

    dlog("dns_query: Sending Packet\n");
    syserr(sendto(sfd, buf, len, 0, (struct sockaddr *) &dest,
            sizeof(dest)) < 0, "dns_query: sendto\n");
    dlog("dns_query: Done\n");

    dlog("dns_query: Receiving Answer\n");
    
    socklen_t slen = (socklen_t) sizeof(dest);
    syserr((nBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &dest,
            &slen)) < 0, "dns_query: recvfrom\n");
    dlog("dns_query: Done\n");
}

void resolve_message(uchar *buf)
{
    int locate = 0;
    dns_header_declare(header);
    dns_header_locate(header, buf);
    locate += (int) sizeof(header);
    dns_header_show(header);
}

void rr_rdate_show(RR_TYPE_t type, u16_t rdlen, uchar *rdata, uchar* buf, size_t *locate)
{
    size_t _tmp = 0;
    switch(type) {
        case _A:
            printf("rdate = %s\n", inet_ntoa(*((struct in_addr*) rdata)));
            break;
        case _NS:
        case _CNAME:
            /*uchar *host;
            if(is_compressive(rdata)) {
                size_t tmp = *locate;
                uchar *host = (uchar *) malloc(NAME_LIMIT);
                dns_to_host_name(host, buf, &tmp);
            }
            else {
                uchar *host = (uchar *) malloc(rdlen + 1);
                host[rdlen] = '\0';
                strncpy(host, rdata, rdlen);
            }*/

        {
            _tmp = *locate;

            char *_host = (char *) malloc(NAME_LIMIT * sizeof(char));
            //printf("rdate = %s\n", _host);
            dns_to_host_name(_host, buf, &_tmp);

            printf("rdate = %s\n", _host);


            /*
            char data;
            char *ptr;
                ptr = (&buf[((((u16_t) buf[locate]) << 8) + buf[locate + 1]) & compression_mask]);
            //data[rdlen] = '\0';

            ///printf("rdate = %s\n", rdata);
            printf("rdate = ");
            for(int i = 0; i < rdlen * 4; i++) putchar(*((char *) data + i));
            putchar('\n');*/

            //free(host);
            break;
        }
        /*
        case _MD:
        case _MF:
        case _SOA:
        case _MB:
        case _MG:
        case _MR:
        case _NULL:
        case _WKS:
        case _PTR:
        case _HINFO:
        case _MINFO:
        case _MX:
        case _TXT:
        case _AXFR:
        case _MAILB:
        case _MAILA:
        case _wildcard:*/
        default:
            dlog("Cannot find the RR_TYPE corresponded to rdata\n");
            break;
    }

    *locate += rdlen;
    ///return 0;
}

void rr_show(RR_ptr_t *rr, uchar *buf, size_t *locate)
{
    printf("**RR_ptr_t %p**\n", rr);

    size_t _tmp = *locate;

    char *_host = (char *) malloc(NAME_LIMIT * sizeof(char));
    dns_to_host_name(_host, buf, &_tmp);

    printf("RR->name = %s\n", _host);
    printf("RR->rr->type = %hu(%s)\n", rr_member(rr, type, ntohs), _RR_TYPE[rr_member(rr, type, ntohs)]);
    printf("RR->rr->class = %hu(%s)\n", rr_member(rr, class, ntohs), _RR_CLASS[rr_member(rr, class, ntohs)]);
    printf("RR->rr->ttl = %u\n", rr_member(rr, ttl, ntohl));
    printf("RR->rr->rdlen = %hu\n", rr_member(rr, rdlength, ntohs));

    _tmp += sizeof(RR_t);
    *locate = _tmp + rr_member(rr, rdlength, ntohs);

    rr_rdate_show(rr_member(rr, type, ntohs), rr_member(rr, rdlength, ntohs), rr_member(rr, rdata), buf, &_tmp);

    //*locate = *locate + sizeof(RR_t) + rr_member(rr, rdlength, ntohs);
}
