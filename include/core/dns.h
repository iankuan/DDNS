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

/**
 * TODO: IMPORTANT, but why
 * This would convert "www.google.com" to "3www6google3com"
 */
static inline
void host_to_dns_name(char *dns, char *host)
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

static inline
void dns_to_host_name(char *host, uchar *buf, size_t *locate)
{
    u16_t offset = 0xFFFF;
    if( is_compressive(&buf[*locate])) {
        offset = ((((u16_t) buf[*locate]) << 8) + buf[*locate + 1]) & compression_mask;
        *locate += 2;
    }
    else
        *locate += strlen((char *) &buf[*locate]) - 1;

    uchar *dn = offset != 0xFFFF ? &buf[offset]: &buf[*locate];

    int i = 0;
    //Convert 3www6google3com0 to www.google.com
    ///FIXME: Is it possible for >10

    for(uchar count = dn[0]; count != 0; count = dn[i])
    {
        while(count-- != 0) {
            host[i] = dn[i + 1];
            i++;
        }
        host[i++] = '.';
    }
    host[i - 1] = '\0';
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

void rr_show(RR_ptr_t *rr, uchar *buf, size_t *locate)
{
    printf("**RR_ptr_t %p**\n", rr);

    size_t tmp = *locate;

    char *host = (char *) malloc(NAME_LIMIT * sizeof(char));
    dns_to_host_name(host, buf, &tmp);

    printf("RR->name = %s\n", host);
    printf("RR->rr->type = %hu(%s)\n", rr_member(rr, type, ntohs), _RR_TYPE[rr_member(rr, type, ntohs)]);
    printf("RR->rr->class = %hu(%s)\n", rr_member(rr, class, ntohs), _RR_CLASS[rr_member(rr, class, ntohs)]);
    printf("RR->rr->ttl = %u\n", rr_member(rr, ttl, ntohl));

    //rd_show();
   
}
