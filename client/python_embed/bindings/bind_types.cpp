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

#include "types.h"


#include <boost/python.hpp>
using namespace boost::python;

void bind_types(void)
{
	enum_<endian_e>("endian")
	.value("ENDIAN_BIG", ENDIAN_BIG)
	.value("ENDIAN_LITTLE", ENDIAN_LITTLE)
	;
	
	enum_<nparse_e>("nparse")
	.value("BTYPE_PLAIN", BTYPE_PLAIN)
	.value("BTYPE_FIXEDAT", BTYPE_FIXEDAT)
	.value("BTYPE_FLOAT", BTYPE_FLOAT)
	;
	
	enum_<nparse_sign_e>("nparse_sign")
	.value("BSIGN_NONE", BSIGN_NONE)
	.value("BSIGN_STD", BSIGN_TWOS_COMP)
	.value("BSIGN_SIGN_AND_MAG", BSIGN_SIGN_AND_MAG)
	.value("BSIGN_ONES_COMP", BSIGN_ONES_COMP)
	.value("BSIGN_TWOS_COMP", BSIGN_TWOS_COMP)
	.value("BSIGN_EXCESS_N", BSIGN_EXCESS_N)
	;
}
