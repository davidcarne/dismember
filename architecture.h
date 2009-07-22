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

#ifndef _architecture_H_
#define _architecture_H_
class DataType;
class I_ProjectModel;

/**
 * Architecture is an abstract base class that provides the interface through which architecture specific details can be queried
 */
class Architecture {
public:
	virtual ~Architecture() {};
	/** Get the short [display] name for the architecture */
	virtual std::string get_short_name() const = 0; 
	
	// HACK HACK
	/** get the datatype that will instantiate an instruction of this architecture. needs rethinking */
	virtual DataType * getDataType(const I_ProjectModel * t) const = 0;
	
	/** Create an instruction for the architecture at address addr */
	//virtual Instruction * create_instruction(Trace * ctx, address_t addr) = 0;
	
	//virtual endian_e getArchEndian(void) = 0;
	// also here we have the create from serializer factory method
};

#endif


