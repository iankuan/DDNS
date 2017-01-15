#ifndef DNS_UTIL_H
#define DNS_UTIL_H

#include <string.h>
#include <arpa/inet.h>

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

#endif ///DNS_UTIL_H
