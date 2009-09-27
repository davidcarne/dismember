/*  Copyright 2009 David Carne
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

#ifndef _KVS_SERIALIZERS_H_
#define _KVS_SERIALIZERS_H_

#include "types.h"
#include "address.h"

/*
 * Note - address_ts are packed into a single string
 * so they can be used as part of a key
 */

template<typename T> convertToKVS<T>(const T & src);

std::string convertToKVS(const endian_e & value);
std::string convertToKVS(const nparse_e & value);
std::string convertToKVS(const nparse_sign_e & value);
std::string convertToKVS(const uint64_t & value);
std::string convertToKVS(const int64_t & value);
std::string convertToKVS(const int32_t & value);
std::string convertToKVS(const uint32_t & value);
std::string convertToKVS(const address_t & value);

endian_e convertFromKVS_endian_e(const std::string &);
nparse_e convertFromKVS_nparse_e(const std::string &);
nparse_sign_e convertFromKVS_nparse_sign_e(const std::string &);
uint64_t convertToKVS_uint64_t(const std::string &);
int64_t convertToKVS_int64_t(const std::string &);
int32_t convertToKVS_int32_t(const std::string &);
uint32_t convertToKVS_uint32_t(const std::string &);
address_t convertToKVS_address_t(const std::string &);

#endif

