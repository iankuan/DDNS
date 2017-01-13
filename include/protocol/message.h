#ifndef MESSAGE_H
#define MESSAGE_H

#include "message_dtl.h"

/**
 * 4. MESSAGES
 *
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


/**
 * Breif the context of the MESSAGE
 *
 * @breif:The header section is always present.  The header includes fields that
 * specify which of the remaining sections are present, and also specify
 * whether the message is a query or a response, a standard query or some
 * other opcode, etc.
 * 
 * The names of the sections after the header are derived from their use in
 * standard queries.  The question section contains fields that describe a
 * question to a name server.  These fields are a query type (QTYPE), a
 * query class (QCLASS), and a query domain name (QNAME).  The last three
 * sections have the same format: a possibly empty list of concatenated
 * resource records (RRs).  The answer section contains RRs that answer the
 * question; the authority section contains RRs that point toward an
 * authoritative name server; the additional records section contains RRs
 * which relate to the query, but are not strictly answers for the
 * question.
 */

/**
 * 2 - Update Message Format
 * 
 *    The DNS Message Format is defined by [RFC1035 4.1].  Some extensions
 *    are necessary (for example, more error codes are possible under
 *    UPDATE than under QUERY) and some fields must be overloaded (see
 *    description of CLASS fields below).
 * 
 *    The overall format of an UPDATE message is, following [ibid]:
 * 
 *       +---------------------+
 *       |        Header       |
 *       +---------------------+
 *       |         Zone        | specifies the zone to be updated
 *       +---------------------+
 *       |     Prerequisite    | RRs or RRsets which must (not) preexist
 *       +---------------------+
 *       |        Update       | RRs or RRsets to be added or deleted
 *       +---------------------+
 *       |   Additional Data   | additional data
 *       +---------------------+
 * 
 *    The Header Section specifies that this message is an UPDATE, and
 *    describes the size of the other sections.  The Zone Section names the
 *    zone that is to be updated by this message.  The Prerequisite Section
 *    specifies the starting invariants (in terms of zone content) required
 *    for this update.  The Update Section contains the edits to be made,
 *    and the Additional Data Section contains data which may be necessary
 *    to complete, but is not part of, this update.
 */


///FIXME: Use #define to do this.
/*typedef RR_ptr_t *DNS_ANSWER_ptr_t;
typedef RR_ptr_t *DNS_AUTHORITY_ptr_t;
typedef RR_ptr_t *DNS_ADDITION_ptr_t;*/

#define DNS_ZONE_ptr_t          DNS_QUESTION_ptr_t
#define DNS_PREREQUISITE_ptr_t  DNS_ANSWER_ptr_t
#define DNS_UPDATE_ptr_t        DNS_ANSWER_ptr_t

typedef struct _DNS_MESSAGE_PTR {
   DNS_HEADER_t             *header;
   DNS_QUESTION_ptr_t    *quest_ptr;
   DNS_ANSWER_ptr_t        *ans_ptr;
   DNS_ADDITIONAL_ptr_t    *add_ptr;
} DNS_MESSAGE_ptr_t;

#endif///MESSAGE_H
