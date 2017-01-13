#ifndef BITOPS_H
#define BITOPS_H

#include <stdbool.h>

#define clear_bit(bit, ptr)\
	*(ptr) &= (typeof(*(ptr))) ~(1 << bit)

#define set_bit(bit, ptr)\
	*(ptr) |= (typeof(*(ptr))) (1 << bit)

#define get_bit(bit, ptr)\
    ({\
	    *(ptr) & (typeof(*(ptr)))(1 << bit)? true: false;\
    })

#endif /* BITOPS_H */
