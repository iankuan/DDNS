/**
 * DNS Lookups
 * 
 * Normal resource records lookups are done with UDP. An "intelligent retransmission" is to be used, though one is not specified in the protocol, resulting in a mix of poor strategies with good ones. The protocol itself is stateless; all the information needed is contained in a single message, fully documented in RFC 1035 §4.1, and having the following format:
 * 
 *     +---------------------+
 *     |        Header       |
 *     +---------------------+
 *     |       Question      | the question for the name server
 *     +---------------------+
 *     |        Answer       | RRs answering the question
 *     +---------------------+
 *     |      Authority      | RRs pointing toward an authority
 *     +---------------------+
 *     |      Additional     | RRs holding additional information
 *     +---------------------+
 * 
 * Questions are always Name, Type, Class tuples. For Internet applications, the Class is IN, the Type is a valid RR type, and the Name is a fully-qualified domain name, stored in a standard format. Names can't be wildcarded, but Types and Classes can be. In addition, special Types exist to wildcard mail records and to trigger zone transfers. The question is the only section included in a query message; the remaining sections being used for replies.
 * Answers are RRs that match the Name, Type, Class tuple. If any of the matching records are CNAME pointers leading to other records, the target records should also be included in the answer. There may be multiple answers, since there may be multiple RRs with the same labels.
 * Authority RRs are type NS records pointing to name servers closer to the target name in the naming hierarchy. This field is completely optional, but clients are encouraged to cache this information if further requests may be made in the same name hierarchy.
 * Additional RRs are records that the name server believes may be useful to the client. The most common use for this field is to supply A (address) records for the name servers listed in the Authority section.
 */

typedef struct _head_sec {
    u16_t       _ID;
    int         _QR: 1;
    int     _Opcode: 4;
    int         _AA: 1;
    int         _TC: 1;
    int         _RD: 1;
    int         _RA: 1;
    int          _Z: 3;
    int      _RCODE: 4;
    u16_t   _QDCOUNT;
    u16_t   _ANCOUNT;
    u16_t   _NSCOUNT;
    u16_t   _ARCOUNT;
} head_sec;
