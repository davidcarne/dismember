#include "../../dsmem_trace.h"
#include "../../memlocdata.h"

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

ARMInstruction::ARMInstruction(const Trace * ctx, address_t address, u32 opcode) : Instruction(ctx, address, 4), m_opcode(opcode), m_pcflags(0), m_ddest(0)
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

