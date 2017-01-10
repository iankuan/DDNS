/**
 * 3.3. Standard RRs
 * 
 * The following RR definitions are expected to occur, at least
 * potentially, in all classes.  In particular, NS, SOA, CNAME, and PTR
 * will be used in all classes, and have the same format in all classes.
 * Because their RDATA format is known, all domain names in the RDATA
 * section of these RRs may be compressed.
 * 
 * <domain-name> is a domain name represented as a series of labels, and
 * terminated by a label with zero length.  <character-string> is a single
 * length octet followed by that number of characters.  <character-string>
 * is treated as binary information, and can be up to 256 characters in
 * length (including the length octet).
 */

#include "type.h"

/**
 * 3.4.1 Address (A) RRs
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
 * 3.3.11. Name Server (NS) RR
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
 typedef char *NN_t;

/**
 * 3.3.12. Pointer (PTR) RR
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
typedef char *PTRDNAME_t;

/**
 * 3.3.13. Start of Authority (SOA) RR
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
/*typedef struct _SOA {
    char   *mname;
    char   *rname;
    u32_t sperial;
    u32_t refresh;
    u32_t retry;
    u32_t expire;
    u32_t minimu;
} SOA_t;*/

typedef struct _SOA {
    u32_t sperial;
    u32_t refresh;
    u32_t retry;
    u32_t expire;
    u32_t minimu;
} SOA_t;

typedef struct _SOA_ptr {
    char        *mname;
    char        *rname;
    SOA_t*         soa;
} SOA_ptr_t;

/**
 * 3.3.1. Canonical Name (CNAME) RR format
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
typedef char *CNAME_t;

/**
 * 3.3.2. HINFO RDATA format
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                      CPU                      /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                       OS                      /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where:
 * 
 * CPU             A <character-string> which specifies the CPU type.
 * 
 * OS              A <character-string> which specifies the operating
 *                 system type.
 * 
 * Standard values for CPU and OS can be found in [RFC-1010].
 * 
 * HINFO records are used to acquire general information about a host.  The
 * main use is for protocols such as FTP that can use special procedures
 * when talking between machines or operating systems of the same type.
 */
typedef struct _HINFO_ptr {
    char *cpu;
    char  *os;
} HINFO_ptr_t;

/**
 * 3.3.3. MB RDATA format (EXPERIMENTAL)
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                   MADNAME                     /
 *     /                                               /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where:
 * 
 * MADNAME         A <domain-name> which specifies a host which has the
 *                 specified mailbox.
 * MB records cause additional section processing which looks up an A type
 * RRs corresponding to MADNAME.
 *
 */
typedef char *MADNAME_t;

/**
 * 3.3.6. MG RDATA format (EXPERIMENTAL)
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                   MGMNAME                     /
 *     /                                               /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where:
 * 
 * MGMNAME         A <domain-name> which specifies a mailbox which is a
 *                 member of the mail groupe 
 * 
 * MG records cause no additional section processing.
 */
typedef char *MGMNAME_t;

/**
 * 3.3.7. MINFO RDATA format (EXPERIMENTAL)
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                    RMAILBX                    /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                    EMAILBX                    /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where:
 * 
 * RMAILBX         A <domain-name> which specifies a mailbox which is
 *                 responsible for the mailing list or mailbox.  If this
 *                 domain name names the root, the owner of the MINFO RR is
 *                 responsible for itself.  Note that many existing mailing
 *                 lists use a mailbox X-request for the RMAILBX field of
 *                 mailing list X, e.g., Msgroup-request for Msgroup.  This
 *                 field provides a more general mechanism.
 * 
 * 
 * EMAILBX         A <domain-name> which specifies a mailbox which is to
 *                 receive error messages related to the mailing list or
 *                 mailbox specified by the owner of the MINFO RR (similar
 *                 to the ERRORS-TO: field which has been proposed).  If
 *                 this domain name names the root, errors should be
 *                 returned to the sender of the message.
 * 
 * MINFO records cause no additional section processing.  Although these
 * records can be associated with a simple mailbox, they are usually used
 * with a mailing list.
 */
///FIXME: member rename
typedef struct _MINFO_ptr {
    char *rmailbx;
    char *emailbx;
} MINFO_ptr_t;

/**
 * 3.3.8. MR RDATA format (EXPERIMENTAL)
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                   NEWNAME                     /
 *     /                                               /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where:
 * 
 * NEWNAME         A <domain-name> which specifies a mailbox which is the
 *                 proper rename of the specified mailbox.
 * 
 * MR records cause no additional section processing.  The main use for MR
 * is as a forwarding entry for a user who has moved to a different
 * mailbox.
 */
typedef char *NEWNAME_t;
 
/** 
 * 3.3.9. MX RDATA format
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                  PREFERENCE                   |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                   EXCHANGE                    /
 *     /                                               /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where:
 * 
 * PREFERENCE      A 16 bit integer which specifies the preference given to
 *                 this RR among others at the same owner.  Lower values
 *                 are preferred.
 * 
 * EXCHANGE        A <domain-name> which specifies a host willing to act as
 *                 a mail exchange for the owner name.
 * 
 * MX records cause type A additional section processing for the host
 * specified by EXCHANGE.  The use of MX RRs is explained in detail in
 * [RFC-974].
 */
///TODO: Consider that we should use the uniform format(not zero-length array)
typedef struct MX {
    uint16_t preference;
    char exchange[0];
} MX_t;

/** 
 * 3.3.10. NULL RDATA format (EXPERIMENTAL)
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                  <anything>                   /
 *     /                                               /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * Anything at all may be in the RDATA field so long as it is 65535 octets
 * or less.
 * NULL records cause no additional section processing.  NULL RRs are not
 * allowed in master files.  NULLs are used as placeholders in some
 * experimental extensions of the DNS.
 */
///FIXME: I can't define it.


/**
 * 3.3.14. TXT RDATA format
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     /                   TXT-DATA                    /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where:
 * 
 * TXT-DATA        One or more <character-string>s.
 * 
 * TXT RRs are used to hold descriptive text.  The semantics of the text
 * depends on the domain where it is found.
 */
typedef char *TXT_t;

/**
 * 3.4.2 WKS RDATA format
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    ADDRESS                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |       PROTOCOL        |                       |
 *     +--+--+--+--+--+--+--+--+                       |
 *     |                                               |
 *     /                   <BIT MAP>                   /
 *     /                                               /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where:
 * 
 * ADDRESS         An 32 bit Internet address
 * 
 * PROTOCOL        An 8 bit IP protocol number
 * 
 * <BIT MAP>       A variable length bit map.  The bit map must be a
 *                 multiple of 8 bits long.
 * 
 * The WKS record is used to describe the well known services supported by
 * a particular protocol on a particular internet address.  The PROTOCOL
 * field specifies an IP protocol number, and the bit map has one bit per
 * port of the specified protocol.  The first bit corresponds to port 0,
 * the second to port 1, etc.  If the bit map does not include a bit for a
 * protocol of interest, that bit is assumed zero.  The appropriate values
 * and mnemonics for ports and protocols are specified in [RFC-1010].
 * 
 * For example, if PROTOCOL=TCP (6), the 26th bit corresponds to TCP port
 * 25 (SMTP).  If this bit is set, a SMTP server should be listening on TCP
 * port 25; if zero, SMTP service is not supported on the specified
 * address.
 * 
 * The purpose of WKS RRs is to provide availability information for
 * servers for TCP and UDP.  If a server supports both TCP and UDP, or has
 * multiple Internet addresses, then multiple WKS RRs are used.
 * 
 * WKS RRs cause no additional section processing.
 * 
 * In master files, both ports and protocols are expressed using mnemonics
 * or decimal numbers.
 */
typedef u8_t PROT_t;
typedef u8_t BMAP_t;

///FIXME: variable len of bit_map
typedef struct _WKS {
    A_t addr; 
    PROT_t prot;
    BMAP_t bmap[0];
} WKS_t;
