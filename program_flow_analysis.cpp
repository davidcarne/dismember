#include <stack>
#include <stdlib.h>
#include <stdexcept>


#include "dsmem_trace.h"
#include "memlocdata.h"
#include "instruction.h"
#include "architecture.h"
#include "program_flow_analysis.h"
#include "xref.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>

sp_RunQueueJob ProgramFlowAnalysis::createAnalysisJob(Document * d, DataType * dtcreate, address_t start)
{
	FunctorRunQueueJob::jobfun_t jb = boost::bind(&ProgramFlowAnalysis::analyze, d, dtcreate, start);
	return createFunctorRunQueueJob("code flow analysis..", jb);
}

void ProgramFlowAnalysis::submitAnalysisJob(Document * d, DataType * dtcreate, address_t start)
{
	d->getRunQueue()->submit(createAnalysisJob(d, dtcreate, start));
}

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


bool ProgramFlowAnalysis::analyze(Document * d, DataType * dtcreate, address_t start)
{
	Trace * t = d->getTrace();
	
	address_t addr;
	std::stack<address_t> analysis_addrs;
	addr = start;
	
	bool done = false;
	bool first = true;
	
	while (!done) {
		if (t->lookup_memloc(addr))
			break;
		
		Instruction * curr = dynamic_cast<Instruction *>(t->createMemlocDataAt(dtcreate, addr));
	
		if (curr && first)
			curr->mark_explicit(true);
		
		first = false;
	
		// If we jump anywhere, submit a new analysis job
		if (curr && (curr->get_pcflags() & Instruction::PCFLAG_LOCMASK) == Instruction::PCFLAG_DIRLOC)
			submitAnalysisJob(d, dtcreate, curr->get_direct_jump_addr());
		
		// If the instruction can continue
		if (curr && (curr->get_pcflags() & Instruction::PCFLAG_CONTINUE))
			addr += curr->get_length();
		else
			break;
		

	}
	
	d->postGuiUpdate();
	return true;
}
