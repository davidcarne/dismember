/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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


// Physical address, size + offset types
typedef uint64_t paddr_t;
typedef uint64_t psize_t;
typedef int64_t  poffset_t;

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

/**
 * Xref type enum
 */
typedef enum {
	/**
	 * \brief Xref of type jump
	 */
	XR_TYPE_JMP,
	
	/**
	 * \brief Xref of type function call
	 */
	XR_TYPE_FNCALL,
	
	/**
	 * \brief Xref of type data reference. See source pcflags for more info
	 */
	XR_TYPE_DATA
} xref_type_e;

/// Symbol order specifier
typedef enum  {
	SYMORDER_NAME,
	SYMORDER_ADDR
} symsortorder_e;

#include "address.h"

#define BSIGN_STD BSIGN_TWOS_COMP

#define DEPRECATED(x) x __attribute__((deprecated))
#endif
