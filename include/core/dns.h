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
#include "debug.h"

#define BUF_SIZE 10000

///We use the Object-Oriened concept to build the interface of the dns server.
struct DNS {
    ///database

    ///function
    void (*dns_init)(char* cfg_dir);
    /*(*query)();
    (*response)();*/
};

void dns_header_show(DNS_HEADER_t *hdr)
{
    /**
     *  FIXME: YOU CANNOT TAKE THE ADDRESS of BITFILEDS
     *   *((u16_t *) (hdr + offsetof(DNS_HEADER_t, qr))) = ntohs(*((u16_t *) (hdr + offsetof(DNS_HEADER_t, qr))));
     */
    //*((u16_t *)((char *) hdr + offsetofend(DNS_HEADER_t, id))) = ntohs(*((u16_t *) ((char *) hdr + offsetofend(DNS_HEADER_t, id))));
    printf("**DNS_Header_t %p**\n", hdr);
    /*printf("Header->id = %hu\n", dns_header_member(hdr, id, ntohs));
    printf("Header->qr = %u\n", dns_header_member(hdr, qr, ntohl));
    printf("Header->opcode = %u(%s)\n", dns_header_member(hdr, opcode, ntohl), _OPCODE[dns_header_member(hdr, opcode, ntohl)]);
    printf("Header->aa = %u\n", dns_header_member(hdr, aa, ntohl) & 0x1);
    printf("Header->tc = %u\n", dns_header_member(hdr, tc, ntohl) & 0x1);
    printf("Header->rd = %u\n", dns_header_member(hdr, rd, ntohl);
    printf("Header->ra = %u\n", dns_header_member(hdr, ra, ntohl) & 0x1);
    printf("Header-> z = %u\n", dns_header_member(hdr,  z, ntohl));
    printf("Header->rcode = %u(%s)\n", dns_header_member(hdr, rcode, ntohl), _RCODE[dns_header_member(hdr,rcode, ntohl)]);*/
    printf("Header->id = %hu\n", dns_header_member(hdr, id, ntohs));
    printf("Header->qr = %u\n", dns_header_member(hdr, qr));
    printf("Header->opcode = %u(%s)\n", dns_header_member(hdr, opcode), _OPCODE[dns_header_member(hdr, opcode)]);
    printf("Header->aa = %u\n", dns_header_member(hdr, aa));
    printf("Header->tc = %u\n", dns_header_member(hdr, tc));
    printf("Header->rd = %u\n", dns_header_member(hdr, rd));
    printf("Header->ra = %u\n", dns_header_member(hdr, ra));
    printf("Header-> z = %u\n", dns_header_member(hdr,  z));
    printf("Header->rcode = %u(%s)\n", dns_header_member(hdr, rcode), _RCODE[dns_header_member(hdr, rcode)]);

    printf("Header->qdcount = %hu\n", dns_header_member(hdr, qdcount, ntohs));
    printf("Header->ancount = %hu\n", dns_header_member(hdr, ancount, ntohs));
    printf("Header->nscount = %hu\n", dns_header_member(hdr, nscount, ntohs));
    printf("Header->nscount = %hu\n", dns_header_member(hdr, nscount, ntohs));
}

/**
 * TODO: IMPORTANT, but why
 * This would convert "www.google.com" to "3www6google3com"
 */
static inline
void host_to_dns_name(char *host, char *dns)
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
