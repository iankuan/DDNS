#include "config_limit.h"
#include "type.h"
#include "rr_dtl.h"

///Reference: https://www.ietf.org/rfc/rfc1035.txt

/**
 * 3. DOMAIN NAME SPACE AND RR DEFINITIONS
 * 
 * 3.1. Name space definitions
 * 
 * Domain names in messages are expressed in terms of a sequence of labels.
 * Each label is represented as a one octet length field followed by that
 * number of octets.  Since every domain name ends with the null label of
 * the root, a domain name is terminated by a length byte of zero.  The
 * high order two bits of every length octet must be zero, and the
 * remaining six bits of the length field limit the label to 63 octets or
 * less.
 * 
 * To simplify implementations, the total length of a domain name (i.e.,
 * label octets and label length octets) is restricted to 255 octets or
 * less.
 * 
 * Although labels can contain any 8 bit values in octets that make up a
 * label, it is strongly recommended that labels follow the preferred
 * syntax described elsewhere in this memo, which is compatible with
 * existing host naming conventions.  Name servers and resolvers must
 * compare labels in a case-insensitive manner (i.e., A=a), assuming ASCII
 * with zero parity.  Non-alphabetic codes must match exactly.
 */

/**
 * 3.2. RR definitions
 * 
 * 3.2.1. Format
 * 
 * All RRs have the same top level format shown below:
 *
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
typedef s32_t TTL_t;///ttl

///rr_format
/*typedef struct _RR {
    char    name[NAME_LIMIT];
    RR_TYPE_t     type;
    RR_CLASS_t   class;
    TTL_t   ttl;
    u16_t   rd_len;
    u32_t   rdata[0];
} RR_t;*/

typedef struct _RR {
    RR_TYPE_t     type;
    RR_CLASS_t   class;
    TTL_t   ttl;
    u16_t   rd_len;
    u32_t   rdata[0];
} RR_t;

typedef struct _RR_ptr {
    char    *name;
    RR_t      *rr;
} RR_ptr_t;

/**
 * Function
 */

 #define get_rr(rr, member) \
    (typeof(member)) (rr + offsetof(typeof(rr), type))
