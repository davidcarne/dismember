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
	for length in [1,2,4,8]:
		for name_signed, signed in [ ("u", dismember.nparse_sign.BSIGN_NONE), ("s", dismember.nparse_sign.BSIGN_STD) ]:
			for name_endian, endian in [ ("le", dismember.endian.ENDIAN_LITTLE), ("be", dismember.endian.ENDIAN_LITTLE) ]:
				
				x = BinaryConstantDataType(trce, "%s%d_%s" %(name_signed, 8 * length, name_endian), 
					dismember.nparse.BTYPE_PLAIN, length, endian, signed)
				trce.addDataType(x)
			
	# Ascii String
	x = StringConstantDataType(trce)
	trce.addDataType(x)