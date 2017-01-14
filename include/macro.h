#ifndef MACRO_H
#define MACRO_H
/**
 * We use this file to implement utility.
 */

#define READ_ONCE(x)       (x)
#define WRITE_ONCE(x, val) ({ x = val; val; })

#define MIN(a,b) ({\
                    __typeof__(a) _a = a; \
                    __typeof__(b) _b = b; \
                    _a < _b ? _a : _b;})

#define MAX(a,b) ({\
                    __typeof__(a) _a = a; \
                    __typeof__(b) _b = b; \
                    _a > _b ? _a : _b;})
                    
#define ARRAY_SIZE(arr) \
	(sizeof(arr) / sizeof(*(arr)))

#define ARRAY_INDEX(elt, arr)				\
	({ unsigned int _elt = (unsigned int)(elt);	\
	   unsigned int _arr = (unsigned int)(arr);	\
	   (_elt - _arr) / sizeof(*(elt)); })

/**
 * offset series
 */
///offsetof
//#define offsetof(type, member) gcc_offsetof(type, member)
#define offsetof(type, member)  ((size_t)&((type *)0)->member)
#define gcc_offsetof(type, member) \
    __builtin_offsetof (type, member)

/**
 * offsetofend(TYPE, MEMBER)
 *
 * @TYPE: The type of the structure
 * @MEMBER: The member within the structure to get the end offset of
 */
#define offsetofend(type, member) \
    (offsetof(type, member) + sizeof(((type *) 0)->member))

///Container: structure begin
#define container_of(ptr, type, member) ({ \
        const __typeof__( ((type *)0)->member ) *__mptr = (ptr); \
        (type *)( (char *)__mptr - offsetof(type,member) );})


///Protocol Assignment
/*#define protocol_var_assign(var, val, ...)\
    var->val = __VA_ARGS__( _ ## val )*/

#define protocol_struct_member_assign(_struct, member, val, ...)\
    _struct->member = __VA_ARGS__(val)

#define _declare(type, var)\
    type var


#define _locate(var, locate)\
    ({\
    var = (__typeof__(var)) locate;\
    var;})

#define _new(type, var)\
    type var = (type) malloc(sizeof(*((type) 0)));\
    /*({\
    type _tmp;\
    type var = (type) malloc(sizeof(*_tmp));\
    var;})*/

#endif ///MACRO_H
