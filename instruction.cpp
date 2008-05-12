/*
 *  instruction.cpp
 *  dismember
 *
 *  Created by David Carne on 25/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "instruction.h"



Instruction::Instruction(const Trace * ctx, address_t addr, u32 length) : MemlocData(NULL, ctx,  addr, length)
{

}
