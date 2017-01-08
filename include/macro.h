/**
 * We use this file to implement utility.
 */

#define MIN(a,b) ({\
                    typeof(a) _a = a; \
                    typeof(b) _b = b; \
                    _a < _b ? _a : _b;})

#define MAX(a,b) ({\
                    typeof(a) _a = a; \
                    typeof(b) _b = b; \
                    _a > _b ? _a : _b;})

#define offsetof(type, member) gcc_offsetof(type, member)

#define gcc_offsetof(type, member) __builtin_offsetof (type, member)
