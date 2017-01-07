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
typedef enum {
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
} RR_TYPE_t;

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
/*typedef enum {
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
} RR_QTYPE_t;*/

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
} RR_CLASS_t;

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
    _* =255;
} RR_QCLASS_t;

///TTL
/**
 * Address (A) RRs
 * 
 * Address (A) records match domain names to IP address, and are both the most important and the most mundane aspect of DNS. See RFC 1035 §3.4.1 for a more detailed description of the A RR, though there is really very little to describe. The data section consists entirely of a 32-bit IP address. Most DNS operations are queries for A records matching a given domain name. Since hosts can have multiple IP addresses, corresponding to multiple physical network interfaces, so it is permissible for multiple A records to match a given domain name. Normally, only the first one is used, so chose a host's most reliable IP address and put it first when constructing name server databases.
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    ADDRESS                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where: 
 * 
 * ADDRESS         A 32 bit Internet address.
 */
typedef u32_t A_t;

/**
 * Canonical Name (CNAME) RR
 * 
 * Canonical Names (CNAMEs) are the DNS equivalent of aliases or symbolic links. The data field contains another fully-qualified DNS name, which should be used as the target of another DNS operation to acquire the desired information. However, a second lookup is rarely required, since most name servers will provide the additional records as part of the reply. See RFC 1035 §3.3.1 for a more detailed description of the CNAME RR.
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                     CNAME                     /
 *     /                                               /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where: 
 * 
 * CNAME           A <domain-name> which specifies the canonical or primary
 *                 name for the owner.  The owner name is an alias.
 */
typedef char CNAME_t[NAME_LIMIT];


/**
 * Pointer (PTR) RR
 * 
 * Pointers (PTRs) are like CNAMEs in their format - the data area contains a domain name. The difference between CNAMEs and PTRs is purely one of semantics. A CNAME specifies an alias, a PTR merely points to another location in the domain name space. The most important use of PTRs is to construct the in-addr.arpa domain, used to convert IP addresses to DNS names (the reverse of the normal process). See RFC 1035 §3.3.12 for a more detailed description of the PTR RR, and RFC 1035 »3.5 for a explanation of the in-addr.arpa domain.
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                   PTRDNAME                    /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where: 
 * 
 * PTRDNAME        A <domain-name> which points to some location in the
 *                 domain name space.
 */

/**
 * Start of Authority (SOA) RR
 * 
 * A Start of Authority SOA RR marks the beginning of a DNS zone, and is typically seen as the first record in a name server for that domain. The encyclopedia's discussion of name servers explains the various fields. See RFC 1035 §3.3.13 for a more detailed description of the SOA RR.
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                     MNAME                     /
 *     /                                               /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                     RNAME                     /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    SERIAL                     |
 *     |                                               |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    REFRESH                    |
 *     |                                               |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                     RETRY                     |
 *     |                                               |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    EXPIRE                     |
 *     |                                               |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    MINIMUM                    |
 *     |                                               |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where: 
 * 
 * MNAME           The <domain-name> of the name server that was the
 *                 original or primary source of data for this zone.
 * 
 * RNAME           A <domain-name> which specifies the mailbox of the
 *                 person responsible for this zone.
 * 
 * SERIAL          The unsigned 32 bit version number of the original copy
 *                 of the zone.  Zone transfers preserve this value.  This
 *                 value wraps and should be compared using sequence space
 *                 arithmetic.
 * 
 * REFRESH         A 32 bit time interval before the zone should be
 *                 refreshed.
 * 
 * RETRY           A 32 bit time interval that should elapse before a
 *                 failed refresh should be retried.
 * 
 * EXPIRE          A 32 bit time value that specifies the upper limit on
 *                 the time interval that can elapse before the zone is no
 *                 longer authoritative.
 * 
 * MINIMUM         The unsigned 32 bit minimum TTL field that should be
 *                 exported with any RR from this zone.
 */



/**
 * Name Server (NS) RR
 * 
 * An NS RR marks the beginning of a DNS zone and supplies the domain name of a name server for that zone. It is typically seen in two places - at the top of a zone, just after the SOA; and at the start of a subzone, where an NS (and often a paired A) are all that is required to perform delegation. See RFC 1035 §3.3.11 for a more detailed description of the NS RR.
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                   NSDNAME                     /
 *     /                                               /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where: 
 * 
 * NSDNAME         A <domain-name> which specifies a host which should be
 *                 authoritative for the specified class and domain
 */

 typedef char _NN_t[NAME_LIMIT];
