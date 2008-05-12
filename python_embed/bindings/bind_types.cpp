/*
 *  bind_types.cpp
 *  dismember
 *
 *  Created by David Carne on 24/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
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
