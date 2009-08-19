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

#ifndef _ARM_H_
#define _ARM_H_

#include "i_projectmodel.h"
#include "../../architecture.h"

class ARMInstructionDataType;

/**
 * Arm architecture, implements arm specific architecture.
 */
class ARMArchitecture : public Architecture {
public:
	ARMArchitecture();
	
	virtual std::string get_short_name() const; 
	
	static void bind_type();
	virtual DataType * getDataType(const I_ProjectModel *) const;
private:
	mutable ARMInstructionDataType * m_adt;
	
	//virtual Instruction * create_instruction(I_ProjectModel * ctx, address_t addr);
	//virtual endian_e getArchEndian(void);
	// also here we have the create from serializer factory method
};

#define  CPSR_T	0x20
#define  CPSR_F	0x40
#define  CPSR_I	0x80
#define  CPSR_flags	0xf0000000
#define  CPSR_V	0x10000000
#define  CPSR_C	0x20000000
#define  CPSR_Z 0x40000000
#define  CPSR_N	0x80000000
#define  CPSR_Vbits	28
#define  CPSR_Cbits	29
#define  CPSR_Zbits	30
#define  CPSR_Nbits	31
#define SPSR -1

#endif

