#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

typedef uint8_t kbool_t;
typedef uint32_t size_t;

#define true ((kbool_t)1)
#define false ((kbool_t)0)
#define NULL ((void *)0)


#define _PAIR(name, k, v)\
            struct {\
            __typeof__(k) key;\
            __typeof__(v) value;\
            } name = { .key = (k), .value = (v) }


#define _MAP(name, size, ktype, vtype)\
            typedef struct {\
                ktype key;\
                vtype value;\
                kbool_t used;\
            } _entry;\
            typedef struct {\
            _entry entries[size];\
            } name;


#endif
