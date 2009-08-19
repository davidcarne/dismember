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

#include <sstream>
#include "types.h"

std::string convertToKVS(const endian_e & value)
{
	switch (value)
	{
		case ENDIAN_BIG: return "BIG";
		case ENDIAN_LITTLE: return "LITTLE";
	}
	return "";
}

std::string convertToKVS(const nparse_e & value)
{
	switch (value)
	{
		case BTYPE_PLAIN: return "PLAIN";
		case BTYPE_FIXEDAT: return "FIXED"; // see fixed_place
		case BTYPE_FLOAT: return "FLOAT";
	}
	return "";
}

std::string convertToKVS(const nparse_sign_e & value)
{
	switch (value)
	{
		case BSIGN_NONE: return "NONE";
		case BSIGN_SIGN_AND_MAG: return "SIGN_AND_MAG";
		case BSIGN_ONES_COMP: return "ONES_COMP";
		case BSIGN_TWOS_COMP: return "TWOS_COMP";
		case BSIGN_EXCESS_N: return "EXCESS_N"; // see sign_excess
	}
	return "";
}

#define GENERIC_STRINGBUF_SERDES_PAIR(type) \
	std::string convertToKVS(const type & value) \
	{ \
		std::stringstream v; \
		v << value; \
		return v.str(); \
	} \
	type convertToKVS_##type(const std::string & value) \
	{ \
		type rv; \
		std::stringstream v(value); \
		v >> rv; \
		return rv; \
	}


GENERIC_STRINGBUF_SERDES_PAIR(uint64_t);
GENERIC_STRINGBUF_SERDES_PAIR(int64_t);
GENERIC_STRINGBUF_SERDES_PAIR(int32_t);
GENERIC_STRINGBUF_SERDES_PAIR(uint32_t);

std::string convertToKVS(const address_t & value)
{
	// TODO: implement me
	return "";
}

endian_e convertFromKVS_endian_e(const std::string & value)
{
	if (value == "BIG") return ENDIAN_BIG;
	if (value == "LITTLE") return ENDIAN_LITTLE;
	return ENDIAN_LITTLE;
}

nparse_e convertFromKVS_nparse_e(const std::string & value)
{
	if (value == "PLAIN") return BTYPE_PLAIN;
	if (value == "FIXED") return BTYPE_FIXEDAT;
	if (value == "FLOAT") return BTYPE_FLOAT;
	return BTYPE_PLAIN;
}

nparse_sign_e convertFromKVS_nparse_sign_e(const std::string & value)
{
	if (value == "NONE") return BSIGN_NONE;
	if (value == "SIGN_AND_MAG") return BSIGN_SIGN_AND_MAG;
	if (value == "ONES_COMP") return BSIGN_ONES_COMP;
	if (value == "TWOS_COMP") return BSIGN_TWOS_COMP;
	if (value == "EXCESS_N") return BSIGN_EXCESS_N;
	return BSIGN_NONE;
}

address_t convertToKVS_address_t(const std::string & value)
{
	return address_t();
}