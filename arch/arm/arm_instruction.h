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

#ifndef _ARM_INSTRUCTION_H_
#define _ARM_INSTRUCTION_H_
#include "../../dsmem_trace.h"

#include "../../instruction.h"

/**
 * \brief Arm specific subclass of Instruction memory type
 */
class ARMInstruction : public Instruction {
public:
	virtual ~ARMInstruction() {};
	/**
	 * Create a new arm instruction.
	 * \deprecated - should only be created through the appropriate datatype that doesn't exist yet
	 * \todo protect me
	 */
	ARMInstruction(const ProjectModel * ctx, address_t address, u32 opcode);
	
	virtual u32			get_length() const;
	
	virtual bool is_executable() const;
	
	/* The slist requirement will go away later when this returns tokens */
	virtual const std::string get_textual();
	
	virtual void mark_fn_ent(bool fnent);
	
	// Uncommon / deprecated interfaces
	virtual address_t	get_direct_jump_addr() const;
	virtual u32	get_pcflags() const;
	virtual address_t	get_data_ref_addr() const;
protected:
		
private:
		u32 m_opcode;
	
	u32 m_pcflags;
	
	// For direct locations - addr of instruction
	// already PC-8 compensated
	address_t m_ddest;
	
	address_t m_data_ref_addr;
	
	void decode_pcflow();
	void decode_data_refs();
	
	u32	get_opcode() const;
};

#endif
