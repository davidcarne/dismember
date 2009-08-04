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

#include "i_projectmodel.h"
#include "i_memlocdata.h"

#include "arm_instruction.h"

u32 ARMInstruction::get_length() const
{
	return 4;
}

u32 ARMInstruction::get_opcode() const
{
	return m_opcode;
}

address_t ARMInstruction::get_direct_jump_addr() const
{
	return m_ddest;
}

u32	ARMInstruction::get_pcflags() const
{
	return m_pcflags;
}

ARMInstruction::ARMInstruction(const I_ProjectModel * ctx, address_t address, u32 opcode) : Instruction(ctx, address, 4), m_opcode(opcode), m_pcflags(0)
{
	decode_pcflow();
	decode_data_refs();
}

bool ARMInstruction::is_executable() const
{
	return true;
}

address_t	ARMInstruction::get_data_ref_addr() const
{
	return m_data_ref_addr;
}

void ARMInstruction::mark_fn_ent(bool fnent)
{
	m_pcflags &= ~PCFLAG_FNENT;
	
	if (fnent)
		m_pcflags |= PCFLAG_FNENT;
}

