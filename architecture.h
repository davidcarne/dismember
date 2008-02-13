/*
 *  architecture.h
 *  dismember
 *
 *  Created by David Carne on 13/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _architecture_H_
#define _architecture_H_

/**
 * Architecture is an abstract base class that provides the interface through which architecture specific details can be queried
 */
class Architecture {
public:
	virtual ~Architecture() {};
	/** Get the short [display] name for the architecture */
	virtual std::string get_short_name() const = 0; 
	
	/** Create an instruction for the architecture at address addr */
	virtual Instruction * create_instruction(Trace * ctx, address_t addr) = 0;
	
	//virtual endian_e getArchEndian(void) = 0;
	// also here we have the create from serializer factory method
};

#endif


