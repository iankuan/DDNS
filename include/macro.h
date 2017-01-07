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
