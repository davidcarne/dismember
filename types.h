#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned long ulong;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;


enum endian_e {
	ENDIAN_BIG,
	ENDIAN_LITTLE
};

typedef enum {
	BTYPE_PLAIN,
	BTYPE_FIXEDAT,
	BTYPE_FLOAT // Need to add new types for the various encoding
} nparse_e;

typedef enum {
	BSIGN_NONE,
	BSIGN_SIGN_AND_MAG,
	BSIGN_ONES_COMP,
	BSIGN_TWOS_COMP,
	BSIGN_EXCESS_N
} nparse_sign_e;


#include "address.h"

#define BSIGN_STD BSIGN_TWOS_COMP

#define DEPRECATED(x) x __attribute__((deprecated))
#endif
