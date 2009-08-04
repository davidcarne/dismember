#
#  dismemauto.py
#  dismember
#
#  Created by David Carne on 24/02/08.
#  Copyright (c) 2008 __MyCompanyName__. All rights reserved.
#
import dismember;
from dismember.datatype_builtins import *;

def newWorkspaceSetup(theworkspace):
	setupWorkspaceDefaultDataTypes(theworkspace)
	
	
def setupWorkspaceDefaultDataTypes(theworkspace):
	
	trce = theworkspace.trace;
	
	# Unsigned 8 bit int
	x = BinaryConstantDataType(trce, "u8_le", dismember.nparse.BTYPE_PLAIN, 1, False, dismember.nparse_sign.BSIGN_NONE)
	trce.addDataType(x)
	
	# Unsigned 16 bit int
	x = BinaryConstantDataType(trce, "u16_le", dismember.nparse.BTYPE_PLAIN, 2, False, dismember.nparse_sign.BSIGN_NONE)
	trce.addDataType(x)
	
	# Unsigned 32 bit int
	x = BinaryConstantDataType(trce, "u32_le", dismember.nparse.BTYPE_PLAIN, 4, False, dismember.nparse_sign.BSIGN_NONE)
	trce.addDataType(x)
	
	# Unsigned 64 bit int
	x = BinaryConstantDataType(trce, "u64_le", dismember.nparse.BTYPE_PLAIN, 8, False, dismember.nparse_sign.BSIGN_NONE)
	trce.addDataType(x)
	
	# Signed 8 bit int
	x = BinaryConstantDataType(trce, "s8_le", dismember.nparse.BTYPE_PLAIN, 1, False, dismember.nparse_sign.BSIGN_STD)
	trce.addDataType(x)
	
	# Signed 16 bit int
	x = BinaryConstantDataType(trce, "s16_le", dismember.nparse.BTYPE_PLAIN, 2, False, dismember.nparse_sign.BSIGN_STD)
	trce.addDataType(x)
	
	# Signed 32 bit int
	x = BinaryConstantDataType(trce, "s32_le", dismember.nparse.BTYPE_PLAIN, 4, False, dismember.nparse_sign.BSIGN_STD)
	trce.addDataType(x)

	# Signed 64 bit int
	x = BinaryConstantDataType(trce, "s64_le", dismember.nparse.BTYPE_PLAIN, 8, False, dismember.nparse_sign.BSIGN_STD)
	trce.addDataType(x)

	# Ascii String
	x = StringConstantDataType(trce)
	trce.addDataType(x)