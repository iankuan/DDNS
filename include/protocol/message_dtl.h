#include "rr.h"
#include "macro.h"

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

///OPCODE
typedef enum {
    _STD_QUERY,
    _INV_QUERY,
    _STATUS_QUERY,
} OPCODE_t;

const char const *opcode[16] = {
    "_STD_QUERY",
    "_INV_QUERY",
    "_STATUS_QUERY",
    [3 ... 15] = "\0",
};

///RCODE
typedef enum {
    _NO_ERR,
    _SERV_FAIL,
    _NAME_ERR,
    _NOT_IMPL,
    _REFUSE,
} RCODE_t;

const char const *rcode[16] = {
    "_NO_ERR",
    "_SERV_FAIL",
    "_NAME_ERR",
    "_NOT_IMPL",
    "_REFUSE",
    [6 ... 15] = "\0",
};

typedef struct _dns_header {
    u16_t       id;
    u32_t       qr: 1;
    u32_t   opcode: 4;
    u32_t       aa: 1;
    u32_t       tc: 1;
    u32_t       rd: 1;
    u32_t       ra: 1;
    u32_t        z: 3;
    u32_t    rcode: 4;
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
    protocol_struct_member_assign(var,     qr,     _qr, htonl);\
    protocol_struct_member_assign(var, opcode, _opcode, htonl);\
    protocol_struct_member_assign(var,     aa,     _aa, htonl);\
    protocol_struct_member_assign(var,     tc,     _tc, htonl);\
    protocol_struct_member_assign(var,     rd,     _rd, htonl);\
    protocol_struct_member_assign(var,     ra,     _ra, htonl);\
    protocol_struct_member_assign(var,      z,      _z, htonl);\
    protocol_struct_member_assign(var,  rcode,  _rcode, htonl);\
    protocol_struct_member_assign(var, qdcount, _qdcount, htons);\
    protocol_struct_member_assign(var, ancount, _ancount, htons);\
    protocol_struct_member_assign(var, nscount, _nscount, htons);\
    protocol_struct_member_assign(var, arcount, _arcount, htons);\
    \
    _s;})

#define dns_header_init(var, locate, _id, _qr, _opcode, _aa, _tc,\
                            _rd, _ra, _z, _rcode, _qdcount,\
                            _ancount, _nscount, _arcount)\
    ({\
    dns_header_declare(var);\
    dns_header_locate(var, locate);\
    size_t _s = dns_header_assign(var, _id, _qr, _opcode, _aa, _tc,\
                            _rd, _ra, _z, _rcode, _qdcount,\
                            _ancount, _nscount, _arcount);\
    _s;})

#define dns_header_member(_struct, member)\
    _struct->member

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
typedef struct _DNS_QUESTION {
    RR_QTYPE_t     qtype;
    RR_QCLASS_t    qclass;
} DNS_QUESTION_t;

typedef struct _DNS_QUESTION_ptr {
    char             *qname;
    DNS_QUESTION_t *question;
} DNS_QUESTION_ptr_t;

#define dns_question_declare(var) _declare(DNS_QUESTION_ptr_t *, var)

#define dns_question_locate(var, locate) _locate(var, locate)

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
    
#define dns_question_init(var, locate, _qname, _qtype, _qclass)\
    ({\
    dns_question_declare(var);\
    dns_question_locate(var, locate);\
    size_t _s = dns_question_assign(var, _id, _qr, _opcode, _aa, _tc,\
                            _rd, _ra, _z, _rcode, _qdcount,\
                            _ancount, _nscount, _arcount);\
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
#define DNS_ANSWER     RR
#define DNS_AUTHORITY  RR 
#define DNS_ADDITIONAL RR

#define dns_answer     rr
#define dns_authority  rr 
#define dns_additional rr

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
