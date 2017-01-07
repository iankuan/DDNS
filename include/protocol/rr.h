#include "config_limit.h"
#include "type.h"

/**
 *                                  1  1  1  1  1  1
 *    0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *    |                                               |
 *    /                                               /
 *    /                      NAME                     /
 *    |                                               |
 *    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *    |                      TYPE                     |
 *    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *    |                     CLASS                     |
 *    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *    |                      TTL                      |
 *    |                                               |
 *    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *    |                   RDLENGTH                    |
 *    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
 *    /                     RDATA                     /
 *    /                                               /
 *    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 *where:
 *  
 *  NAME            an owner name, i.e., the name of the node to which this
 *                  resource record pertains.
 *  
 *  TYPE            two octets containing one of the RR TYPE codes.
 *  
 *  CLASS           two octets containing one of the RR CLASS codes.
 *  
 *  TTL             a 32 bit signed integer that specifies the time interval
 *                  that the resource record may be cached before the source
 *                  of the information should again be consulted.  Zero
 *                  values are interpreted to mean that the RR can only be
 *                  used for the transaction in progress, and should not be
 *                  cached.  For example, SOA records are always distributed
 *                  with a zero TTL to prohibit caching.  Zero values can
 *                  also be used for extremely volatile data.
 *  
 *  RDLENGTH        an unsigned 16 bit integer that specifies the length in
 *                  octets of the RDATA field.
 */

///rr_format
typedef struct _RR {
    char    name[NAME_LIMIT];
    RR_TYPE_t     type;
    RR_CLASS_t   class;
    u32_t   ttl;
    u16_t   rd_len;
    u16_t   rdata[];
} RR_t;
