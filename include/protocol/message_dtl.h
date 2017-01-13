#include "rr.h"
#include "macro.h"

#include "utility/bitops.h"

/**
 * 4.1.1. Header section format
 * 
 * The header contains the following fields:
 * 
 *                                     1  1  1  1  1  1
 *       0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                      ID                       |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    QDCOUNT                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    ANCOUNT                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    NSCOUNT                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                    ARCOUNT                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where:
 * 
 * ID              A 16 bit identifier assigned by the program that
 *                 generates any kind of query.  This identifier is copied
 *                 the corresponding reply and can be used by the requester
 *                 to match up replies to outstanding queries.
 * 
 * QR              A one bit field that specifies whether this message is a
 *                 query (0), or a response (1).
 * 
 * OPCODE          A four bit field that specifies kind of query in this
 *                 message.  This value is set by the originator of a query
 *                 and copied into the response.  The values are:
 * 
 *                 0               a standard query (QUERY)
 * 
 *                 1               an inverse query (IQUERY)
 * 
 *                 2               a server status request (STATUS)
 * 
 *                 3-15            reserved for future use
 * 
 * AA              Authoritative Answer - this bit is valid in responses,
 *                 and specifies that the responding name server is an
 *                 authority for the domain name in question section.
 * 
 *                 Note that the contents of the answer section may have
 *                 multiple owner names because of aliases.  The AA bit
 *                 corresponds to the name which matches the query name, or
 *                 the first owner name in the answer section.
 * 
 * TC              TrunCation - specifies that this message was truncated
 *                 due to length greater than that permitted on the
 *                 transmission channel.
 * 
 * RD              Recursion Desired - this bit may be set in a query and
 *                 is copied into the response.  If RD is set, it directs
 *                 the name server to pursue the query recursively.
 *                 Recursive query support is optional.
 * 
 * RA              Recursion Available - this be is set or cleared in a
 *                 response, and denotes whether recursive query support is
 *                 available in the name server.
 * 
 * Z               Reserved for future use.  Must be zero in all queries
 *                 and responses.
 * 
 * RCODE           Response code - this 4 bit field is set as part of
 *                 responses.  The values have the following
 *                 interpretation:
 * 
 *                 0               No error condition
 * 
 *                 1               Format error - The name server was
 *                                 unable to interpret the query.
 * 
 *                 2               Server failure - The name server was
 *                                 unable to process this query due to a
 *                                 problem with the name server.
 * 
 *                 3               Name Error - Meaningful only for
 *                                 responses from an authoritative name
 *                                 server, this code signifies that the
 *                                 domain name referenced in the query does
 *                                 not exist.
 * 
 *                 4               Not Implemented - The name server does
 *                                 not support the requested kind of query.
 * 
 *                 5               Refused - The name server refuses to
 *                                 perform the specified operation for
 *                                 policy reasons.  For example, a name
 *                                 server may not wish to provide the
 *                                 information to the particular requester,
 *                                 or a name server may not wish to perform
 *                                 a particular operation (e.g., zone
 *                                 transfer) for particular data.
 * 
 *                 6-15            Reserved for future use.
 * 
 * QDCOUNT         an unsigned 16 bit integer specifying the number of
 *                 entries in the question section.
 * 
 * ANCOUNT         an unsigned 16 bit integer specifying the number of
 *                 resource records in the answer section.
 * 
 * NSCOUNT         an unsigned 16 bit integer specifying the number of name
 *                 server resource records in the authority records
 *                 section.
 * 
 * ARCOUNT         an unsigned 16 bit integer specifying the number of
 *                 resource records in the additional records section.
 * 
 */

/**
 *  DDNS-extension
 *
 *    2.2 - Message Header
 * 
 *    The header of the DNS Message Format is defined by [RFC 1035 4.1].
 *    Not all opcodes define the same set of flag bits, though as a
 *    practical matter most of the bits defined for QUERY (in [ibid]) are
 *    identically defined by the other opcodes.  UPDATE uses only one flag
 *    bit (QR).
 * 
 *    The DNS Message Format specifies record counts for its four sections
 *    (Question, Answer, Authority, and Additional).  UPDATE uses the same
 *    fields, and the same section formats, but the naming and use of these
 *    sections differs as shown in the following modified header, after
 *    [RFC1035 4.1.1]:
 * 
 *                                       1  1  1  1  1  1
 *         0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *       |                      ID                       |
 *       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *       |QR|   Opcode  |          Z         |   RCODE   |
 *       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *       |                    ZOCOUNT                    |
 *       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *       |                    PRCOUNT                    |
 *       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *       |                    UPCOUNT                    |
 *       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *       |                    ADCOUNT                    |
 *       +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * 
 *    These fields are used as follows:
 * 
 *    ID      A 16-bit identifier assigned by the entity that generates any
 *            kind of request.  This identifier is copied in the
 *            corresponding reply and can be used by the requestor to match
 *            replies to outstanding requests, or by the server to detect
 *            duplicated requests from some requestor.
 * 
 *    QR      A one bit field that specifies whether this message is a
 *            request (0), or a response (1).
 * 
 *    Opcode  A four bit field that specifies the kind of request in this
 *            message.  This value is set by the originator of a request
 *            and copied into the response.  The Opcode value that
 *            identifies an UPDATE message is five (5).
 * 
 *    Z       Reserved for future use.  Should be zero (0) in all requests
 *            and responses.  A non-zero Z field should be ignored by
 *            implementations of this specification.
 * 
 *    RCODE   Response code - this four bit field is undefined in requests
 *            and set in responses.  The values and meanings of this field
 *            within responses are as follows:
 * 
 *               Mneumonic   Value   Description
 *               ------------------------------------------------------------
 *               NOERROR     0       No error condition.
 *               FORMERR     1       The name server was unable to interpret
 *                                   the request due to a format error.
 *               SERVFAIL    2       The name server encountered an internal
 *                                   failure while processing this request,
 *                                   for example an operating system error
 *                                   or a forwarding timeout.
 *               NXDOMAIN    3       Some name that ought to exist,
 *                                   does not exist.
 *               NOTIMP      4       The name server does not support
 *                                   the specified Opcode.
 *               REFUSED     5       The name server refuses to perform the
 *                                   specified operation for policy or
 *                                   security reasons.
 *               YXDOMAIN    6       Some name that ought not to exist,
 *                                   does exist.
 *               YXRRSET     7       Some RRset that ought not to exist,
 *                                   does exist.
 *               NXRRSET     8       Some RRset that ought to exist,
 *                                   does not exist.
 * 
 *               NOTAUTH     9       The server is not authoritative for
 *                                   the zone named in the Zone Section.
 *               NOTZONE     10      A name used in the Prerequisite or
 *                                   Update Section is not within the
 *                                   zone denoted by the Zone Section.
 * 
 *    ZOCOUNT The number of RRs in the Zone Section.
 * 
 *    PRCOUNT The number of RRs in the Prerequisite Section.
 * 
 *    UPCOUNT The number of RRs in the Update Section.
 * 
 *    ADCOUNT The number of RRs in the Additional Data Section.
 * 
 */

///OPCODE
typedef enum {
    _STD_QUERY,
    _INV_QUERY,
    _STATUS_QUERY,
    _UPDATE = 5,
} OPCODE_t;

const char const *_OPCODE[16] = {
    "_STD_QUERY",
    "_INV_QUERY",
    "_STATUS_QUERY",
    "\0",
    "_UPDATE",
    [6 ... 15] = "\0",
};

///RCODE
typedef enum {
    _NOERROR,
    _FORMERR,
    _SERVFAIL,
    _NXDOMAIN,
    _NOTIMP,
    _REFUSED, 
    _YXDOMAIN,
    _YXRRSET,
    _NXRRSET,
    _NOTAUTH,
    _NOTZONE,
} RCODE_t;

const char const *_RCODE[16] = {
    "_NOERROR",
    "_FORMERR",
    "_SERVFAIL",
    "_NXDOMAIN",
    "_NOTIMP",
    "_REFUSED",
    "_YXDOMAIN",
    "_YXRRSET",
    "_NXRRSET",
    "_NOTAUTH",
    "_NOTZONE",
    [11 ... 15] = "\0",
};

typedef struct _dns_header {
///FIXME: IMPORTANT BIT would be affected by BIG ENDIAN
    u16_t       id;
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    u16_t       qr: 1;
    u16_t   opcode: 4;
    u16_t       aa: 1;
    u16_t       tc: 1;
    u16_t       rd: 1;

    u16_t       ra: 1;
    u16_t        z: 3;
    u16_t    rcode: 4;
#else
    u16_t       rd: 1;
    u16_t       tc: 1;
    u16_t       aa: 1;
    u16_t   opcode: 4;
    u16_t       qr: 1;

    u16_t    rcode: 4;
    u16_t        z: 3;
    u16_t       ra: 1;
#endif
    u16_t   qdcount;
    u16_t   ancount;
    u16_t   nscount;
    u16_t   arcount;
} DNS_HEADER_t;

#define dns_header_declare(var) _declare(DNS_HEADER_t *, var)

#define dns_header_locate(var, locate) _locate(var, locate)

#define dns_header_new(var) _new(DNS_HEADER_t *, var)

#define dns_header_assign(var, _id, _qr, _opcode, _aa, _tc,\
                            _rd, _ra, _z, _rcode, _qdcount,\
                            _ancount, _nscount, _arcount)\
    ({\
    size_t _s = sizeof(DNS_HEADER_t);\
    \
    protocol_struct_member_assign(var,     id,     _id, htons);\
    protocol_struct_member_assign(var,     qr,     _qr);\
    protocol_struct_member_assign(var, opcode, _opcode);\
    protocol_struct_member_assign(var,     aa,     _aa);\
    protocol_struct_member_assign(var,     tc,     _tc);\
    protocol_struct_member_assign(var,     rd,     _rd);\
    protocol_struct_member_assign(var,     ra,     _ra);\
    protocol_struct_member_assign(var,      z,      _z);\
    protocol_struct_member_assign(var,  rcode,  _rcode);\
    protocol_struct_member_assign(var, qdcount, _qdcount, htons);\
    protocol_struct_member_assign(var, ancount, _ancount, htons);\
    protocol_struct_member_assign(var, nscount, _nscount, htons);\
    protocol_struct_member_assign(var, arcount, _arcount, htons);\
    \
    _s;})

#define dns_header_locate_assign(var, locate, _id, _qr, _opcode, _aa, _tc,\
                            _rd, _ra, _z, _rcode, _qdcount,\
                            _ancount, _nscount, _arcount)\
    ({\
    dns_header_locate(var, locate);\
    size_t _s = dns_header_assign(var, _id, _qr, _opcode, _aa, _tc,\
                            _rd, _ra, _z, _rcode, _qdcount,\
                            _ancount, _nscount, _arcount);\
    _s;})

#define dns_header_member(_struct, member, ...)\
    __VA_ARGS__(_struct->member)

/**
 * 4.1.2. Question section format
 * 
 * The question section is used to carry the "question" in most queries,
 * i.e., the parameters that define what is being asked.  The section
 * contains QDCOUNT (usually 1) entries, each of the following format:
 * 
 *                                     1  1  1  1  1  1
 *       0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                                               |
 *     /                     QNAME                     /
 *     /                                               /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                     QTYPE                     |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                     QCLASS                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where:
 * 
 * QNAME           a domain name represented as a sequence of labels, where
 *                 each label consists of a length octet followed by that
 *                 number of octets.  The domain name terminates with the
 *                 zero length octet for the null label of the root.  Note
 *                 that this field may be an odd number of octets; no
 *                 padding is used.
 * 
 * QTYPE           a two octet code which specifies the type of the query.
 *                 The values for this field include all codes valid for a
 *                 TYPE field, together with some more general codes which
 *                 can match more than one type of RR.
 * 
 */

/**
 *  DDNS-extension
 *
 *   2.3 - Zone Section
 *
 *   The Zone Section has the same format as that specified in [RFC1035
 *   4.1.2], with the fields redefined as follows:
 *
 *                                      1  1  1  1  1  1
 *        0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *      |                                               |
 *      /                     ZNAME                     /
 *      /                                               /
 *      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *      |                     ZTYPE                     |
 *      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *      |                     ZCLASS                    |
 *      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 *   UPDATE uses this section to denote the zone of the records being
 *   updated.  All records to be updated must be in the same zone, and
 *   therefore the Zone Section is allowed to contain exactly one record.
 *   The ZNAME is the zone name, the ZTYPE must be SOA, and the ZCLASS is
 *   the zone's class.
 */


typedef struct _DNS_QUESTION {
    RR_QTYPE_t     qtype;
    RR_QCLASS_t    qclass;
} DNS_QUESTION_t;

typedef struct _DNS_QUESTION_ptr {
    char             *qname;
    DNS_QUESTION_t *question;
} DNS_QUESTION_ptr_t;

#define dns_question_declare(var) _declare(DNS_QUESTION_ptr_t *, var)

///TODO: Important it is not same as header file, because we should take a independent space to store the pointer to buf not directly store to buf.
#define dns_question_locate(var, locate) _locate(var->qname, locate)

#define dns_question_new(var) _new(DNS_QUESTION_ptr_t *, var)

#define dns_question_assign(var, _qname, _qtype, _qclass)\
    ({\
    size_t _s = strlen(_qname) + 1;\
    var->question = (DNS_QUESTION_t *)(var->qname + _s);\
    \
    strcpy(var->qname, _qname);\
    protocol_struct_member_assign(var->question,  qtype,  _qtype, htons);\
    protocol_struct_member_assign(var->question, qclass, _qclass, htons);\
    \
    _s += sizeof(DNS_QUESTION_t);\
    _s;})

#define dns_question_locate_assign(var, locate, _qname, _qtype, _qclass)\
    ({\
    dns_question_locate(var, locate);\
    size_t _s = dns_question_assign(var, _qname, _qtype, _qclass);\
    _s;})

#define dns_question_member(_struct, member)\
    ({\
    if(strcmp(#member, "qname"))\
        _struct->member;\
    else\
        _struct->question->member;\
    })
        
        

/**
 * 4.1.3. Resource record format
 * 
 * The answer, authority, and additional sections all share the same
 * format: a variable number of resource records, where the number of
 * records is specified in the corresponding count field in the header.
 * Each resource record has the following format:
 *                                     1  1  1  1  1  1
 *       0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                                               |
 *     /                                               /
 *     /                      NAME                     /
 *     |                                               |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                      TYPE                     |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                     CLASS                     |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                      TTL                      |
 *     |                                               |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     |                   RDLENGTH                    |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
 *     /                     RDATA                     /
 *     /                                               /
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * where:
 * 
 * NAME            a domain name to which this resource record pertains.
 * 
 * TYPE            two octets containing one of the RR type codes.  This
 *                 field specifies the meaning of the data in the RDATA
 *                 field.
 * 
 * CLASS           two octets which specify the class of the data in the
 *                 RDATA field.
 * 
 * TTL             a 32 bit unsigned integer that specifies the time
 *                 interval (in seconds) that the resource record may be
 *                 cached before it should be discarded.  Zero values are
 *                 interpreted to mean that the RR can only be used for the
 *                 transaction in progress, and should not be cached.
 * 
 * RDLENGTH        an unsigned 16 bit integer that specifies the length in
 *                 octets of the RDATA field.
 * 
 * RDATA           a variable length string of octets that describes the
 *                 resource.  The format of this information varies
 *                 according to the TYPE and CLASS of the resource record.
 *                 For example, the if the TYPE is A and the CLASS is IN,
 *                 the RDATA field is a 4 octet ARPA Internet address.
 */
///FIXME: We should take care! We reserve them for convenience.

///DNS_ANSWER_ptr_t
#define DNS_ANSWER_ptr_t     RR_ptr_t

#define dns_answer_declare(var) rr_declare(var)
#define dns_answer_locate(var, locate) rr_locate(var, locate)
#define dns_answer_new(var) rr_new(var)
#define dns_answer_assign(var, _name, _type, _class, _ttl, _rd_len, _rdata)\
    rr_assign(var, _name, _type, _class, _ttl, _rd_len, _rdata)
#define dns_answer_locate_assign(var, locate, _qname, _qtype, _qclass)\
    rr_locate_assign(var, locate, _qname, _qtype, _qclass)
#define dns_answer_init(var, locate, _qname, _qtype, _qclass)\
    rr_init(var, locate, _qname, _qtype, _qclass)
#define dns_answer_member(_struct, member)\
    rr_member(_struct, member)

///DNS_AUTHORITY_ptr_t
#define DNS_AUTHORITY_ptr_t  RR_ptr_t

#define dns_authority_declare(var) rr_declare(var)
#define dns_authority_locate(var, locate) rr_locate(var, locate)
#define dns_authority_new(var) rr_new(var)
#define dns_authority_assign(var, _name, _type, _class, _ttl, _rd_len, _rdata)\
    rr_assign(var, _name, _type, _class, _ttl, _rd_len, _rdata)
#define dns_authority_locate_assign(var, locate, _qname, _qtype, _qclass)\
    rr_locate_assign(var, locate, _qname, _qtype, _qclass)
#define dns_authority_member(_struct, member)\
    rr_member(_struct, member)

///DNS_ADDITIONAL_ptr_t
#define DNS_ADDITIONAL_ptr_t RR_ptr_t  

#define dns_additional_declare(var) rr_declare(var)
#define dns_additional_locate(var, locate) rr_locate(var, locate)
#define dns_additional_new(var) rr_new(var)
#define dns_additional_assign(var, _name, _type, _class, _ttl, _rd_len, _rdata)\
    rr_assign(var, _name, _type, _class, _ttl, _rd_len, _rdata)
#define dns_additional_locate_assign(var, locate, _qname, _qtype, _qclass)\
    rr_locate_assign(var, locate, _qname, _qtype, _qclass)
#define dns_additional_member(_struct, member)\
    rr_member(_struct, member)

/**
 * 4.1.4. Message compression
 * 
 * In order to reduce the size of messages, the domain system utilizes a
 * compression scheme which eliminates the repetition of domain names in a
 * message.  In this scheme, an entire domain name or a list of labels at
 * the end of a domain name is replaced with a pointer to a prior occurance
 * of the same name.
 * 
 * The pointer takes the form of a two octet sequence:
 * 
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     | 1  1|                OFFSET                   |
 *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * The first two bits are ones.  This allows a pointer to be distinguished
 * from a label, since the label must begin with two zero bits because
 * labels are restricted to 63 octets or less.  (The 10 and 01 combinations
 * are reserved for future use.)  The OFFSET field specifies an offset from
 * the start of the message (i.e., the first octet of the ID field in the
 * domain header).  A zero offset specifies the first byte of the ID field,
 * etc.
 * 
 * The compression scheme allows a domain name in a message to be
 * represented as either:
 * 
 *    - a sequence of labels ending in a zero octet
 * 
 *    - a pointer
 * 
 *    - a sequence of labels ending with a pointer
 * 
 * Pointers can only be used for occurances of a domain name where the
 * format is not class specific.  If this were not the case, a name server
 * or resolver would be required to know the format of all RRs it handled.
 * As yet, there are no such cases, but they may occur in future RDATA
 * formats.
 * 
 * If a domain name is contained in a part of the message subject to a
 * ed, the length of the compressed name is used in the length
 * calculation, rather than the length of the expanded name.
 * 
 * Programs are free to avoid using pointers in messages they generate,
 * although this will reduce datagram capacity, and may cause truncation.
 * However all programs are required to understand arriving messages that
 * contain pointers.
 * 
 * For example, a datagram might need to use the domain names F.ISI.ARPA,
 * FOO.F.ISI.ARPA, ARPA, and the root.  Ignoring the other fields of the
 * message, these domain names might be represented as:
 * 
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     20 |           1           |           F           |
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     22 |           3           |           I           |
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     24 |           S           |           I           |
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     26 |           4           |           A           |
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     28 |           R           |           P           |
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     30 |           A           |           0           |
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     40 |           3           |           F           |
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     42 |           O           |           O           |
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     44 | 1  1|                20                       |
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     64 | 1  1|                26                       |
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *     92 |           0           |                       |
 *        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 * The domain name for F.ISI.ARPA is shown at offset 20.  The domain name
 * FOO.F.ISI.ARPA is shown at offset 40; this definition uses a pointer to
 * concatenate a label for FOO to the previously defined F.ISI.ARPA.  The
 * domain name ARPA is defined at offset 64 using a pointer to the ARPA
 * component of the name F.ISI.ARPA at 20; note that this pointer relies on
 * ARPA being the last label in the string at 20.  The root domain nameength field (such as the RDATA section of an RR), and compression is
 * defined by a single octet of zeros at 92; the root domain name has no
 * labels.
 */
static inline
bool is_compressive(uchar *byte)
{
   return get_bit(7, byte) & get_bit(6, byte); 
}

#define compression_mask 0x3FFF
