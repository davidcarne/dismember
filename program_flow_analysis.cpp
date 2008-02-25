#include <stack>
#include <stdlib.h>
#include <stdexcept>


#include "trace.h"
#include "memlocdata.h"
#include "instruction.h"
#include "architecture.h"
#include "program_flow_analysis.h"

bool ProgramFlowAnalysis::undefine(Trace * t, address_t start)
{
	std::stack<address_t> analysis_addrs;
	analysis_addrs.push(start);
	
	bool first = true;
	
	while (!analysis_addrs.empty())
	{
		address_t addr = analysis_addrs.top();
		analysis_addrs.pop();
		
		MemlocData * m = t->lookup_memloc(addr);
		if (!m)
			continue;
		
		if (m->get_explicit() && !first)
			continue;
		
		if (m->logically_continues())
			analysis_addrs.push(m->get_addr() + m->get_length());
		
		analysis_addrs.push(start);
		t->remove_memloc(addr);
		
		first = false;
	}
	return true;
}


bool ProgramFlowAnalysis::analyze(Trace * t, address_t start)
{/*
	address_t addr;
	std::stack<address_t> analysis_addrs;
	addr = start;
	
	bool done = false;
	bool first = true;
	
	// HACK
	while (!done) {
		Instruction * curr = t->m_arch->create_instruction(t, addr);
		if (first)
			curr->mark_explicit(true);
		
		first = false;
		t->insert_memlocd(curr);
	
		//fprintf(stderr, "%p %x %x %p %p: %x %x\n", curr, addr, curr->get_addr(), instrns[addr], instrns[addr-4],  curr->get_pcflags(), curr->get_direct_jump_addr());
		// Decided where to look next
		if (curr->get_pcflags() & Instruction::PCFLAG_CONTINUE)
		{
			// easy call if its an incremental instr
			addr += curr->get_length();
			
			// If this instruction could also branch, save it
			if ((curr->get_pcflags() & Instruction::PCFLAG_LOCMASK) == Instruction::PCFLAG_DIRLOC)
				analysis_addrs.push(curr->get_direct_jump_addr());
			
		}
		else if ((curr->get_pcflags() & Instruction::PCFLAG_LOCMASK) == Instruction::PCFLAG_DIRLOC)
		{
			addr = curr->get_direct_jump_addr();
			//printf("following dirjump %x\n", addr);
		}
		else if (!analysis_addrs.empty())
		{
			addr = analysis_addrs.top();
			//printf("following popped addy %x\n", addr);
			analysis_addrs.pop();
		}
		else
		{
			break;
		}
		
		while (t->lookup_memloc(addr))
		{
			if (analysis_addrs.empty())
			{
				done = true;
				break;
			}
			else {
				addr = analysis_addrs.top();
				
				//printf("_following popped addy %x\n", addr);
				analysis_addrs.pop();
			}
		}
	}*/
	return true;
}
