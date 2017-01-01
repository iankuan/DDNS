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
 typedef _rr {
    char    name[];
    TYPE    type;
    CLASS   class;
    u32_t   ttl;
    u16_t   rd_len;
    u16_t   rdata[];
} rr_t;
/**
 * TYPE values
 *
 * TYPE fields are used in resource records.
 * subset of QTYPEs.
 * Note that these types are a
 * TYPE value and meaning
 *
 *  A 1 a host address
 *  NS 2 an authoritative name server
 *  MD 3 a mail destination (Obsolete - use MX)
 *  MF 4 a mail forwarder (Obsolete - use MX)
 *  CNAME 5 the canonical name for an alias
 *  SOA 6 marks the start of a zone of authority
 *  MB 7 a mailbox domain name (EXPERIMENTAL)
 *  MG 8 a mail group member (EXPERIMENTAL)
 *  MR 9 a mail rename domain name (EXPERIMENTAL)
 *  NULL 10 a null RR (EXPERIMENTAL)
 *  WKS 11 a well known service description
 *  PTR 12 a domain name pointer
 *  HINFO 13 host information
 *  MINFO 14 mailbox or mail list information
 *  MX 15 mail exchange
 *  TXT 16 text strings
 */
typedef emun {
    _A       = 1;
    _NS      = 2;
    _MD      = 3;
    _MF      = 4;
    _CNAME   = 5;
    _SOA     = 6;
    _MB      = 7;
    _MG      = 8;
    _MR      = 9;
    _NULL    =10;
    _WKS     =11;
    _PTR     =12;
    _HINFO   =13;
    _MINFO   =14;
    _MX      =15;
    _TXT     =16;
} TYPE;

/**
 * QTYPE values
 *
 * QTYPE fields appear in the question part of a query.
 * superset of TYPEs, hence all TYPEs are valid QTYPEs.
 * following QTYPEs are defined:
 *
 * AXFR  252 A request for a transfer of an entire zone
 * MAILB 253 A request for mailbox-related records (MB, MG or MR)
 * MAILA 254 A request for mail agent RRs (Obsolete - see MX)
 * *     255 A request for all records
 */
typedef emun {
    _A       =  1;
    _NS      =  2;
    _MD      =  3;
    _MF      =  4;
    _CNAME   =  5;
    _SOA     =  6;
    _MB      =  7;
    _MG      =  8;
    _MR      =  9;
    _NULL    = 10;
    _WKS     = 11;
    _PTR     = 12;
    _HINFO   = 13;
    _MINFO   = 14;
    _MX      = 15;
    _TXT     = 16;
    _AXFR    =253;
    _MAILB   =253;
    _MAILA   =254;
    _*       =255;
} QTYPE;

/**
 * CLASS values
 *
 * CLASS fields appear in resource records.
 * and values are defined:
 * The following CLASS mnemonics
 *
 *  IN 1 the Internet
 *  CS 2 the CSNET class (Obsolete - used only for examples in
 *                          some obsolete RFCs)
 *  CH 3 the CHAOS class
 *  HS 4 Hesiod [Dyer 87]
 */
typedef enum {
    _IN = 1;
    _CS = 2;
    _CH = 3;
    _HS = 4;
} CLASS;

/**
 * QCLASS values
 *
 * QCLASS fields appear in the question section of a query. QCLASS values
 * are a superset of CLASS values; every CLASS is a valid QCLASS. In
 * addition to CLASS values, the following QCLASSes are defined:
 * 
 * *  255 any class
 */
typedef enum {
    _IN = 1;
    _CS = 2;
    _CH = 3;
    _HS = 4;
} QCLASS;

///TTL
