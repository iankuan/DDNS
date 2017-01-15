/**
 * **DNS Protocol Limit**
 * 
 * labels          63 octets or less
 * 
 * names           255 octets or less
 * 
 * TTL             positive values of a signed 32 bit number.
 * 
 * UDP messages    512 octets or less
*/

#define LABEL_LIMIT  63
#define NAME_LIMIT  255
#define TTL_LIMIT    32
#define UDP_LIMIT   512

#define BUF_SIZE 10000
/**
 * **FILE** 
 */
#define FNAME_LIMIT 50
#define PATH_LIMIT 200
