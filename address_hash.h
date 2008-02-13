/*
 * This file was created because the cleanup of trace.h caused all sorts of problems with not being able to hash an address_t
 * include this file to fix weird hash operator errors on address_t's
 */
#ifndef _ADDRESS_HASH_H_
#define _ADDRESS_HASH_H_

#include <ext/hash_map>
#include "types.h"

namespace __gnu_cxx {
	/** hash functor for hashing an address_t */
	template<> struct hash<address_t>
{ 
	/** Stupidly simple hash functor */
	size_t operator()(const address_t & __x) const
{
	return __x ^ (__x >> 32);
}
};
}

#endif
