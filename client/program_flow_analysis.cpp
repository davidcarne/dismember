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

#include <stack>
#include <stdlib.h>
#include <stdexcept>


#include "i_projectmodel.h"
#include "i_memlocdata.h"
#include "instruction.h"
#include "architecture.h"
#include "program_flow_analysis.h"
#include "xref.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>

sp_RunQueueJob ProgramFlowAnalysis::createAnalysisJob(Workspace * d, DataType * dtcreate, address_t start)
{
	FunctorRunQueueJob::jobfun_t jb = boost::bind(&ProgramFlowAnalysis::analyze, d, dtcreate, start);
	return createFunctorRunQueueJob("code flow analysis", 20, jb);
}

void ProgramFlowAnalysis::submitAnalysisJob(Workspace * d, DataType * dtcreate, address_t start)
{
	d->getRunQueue()->submit(createAnalysisJob(d, dtcreate, start));
}

sp_RunQueueJob ProgramFlowAnalysis::createUndefineJob(Workspace * d, address_t start)
{
	FunctorRunQueueJob::jobfun_t jb = boost::bind(&ProgramFlowAnalysis::undefine, d, start);
	return createFunctorRunQueueJob("undefine", 0, jb);
}

void ProgramFlowAnalysis::submitUndefineJob(Workspace * d, address_t start)
{
	d->getRunQueue()->submit(createUndefineJob(d, start));
}


bool ProgramFlowAnalysis::undefine(Workspace * d, address_t start)
{
	I_ProjectModel * t = d->getProjectModel();

	std::stack<address_t> analysis_addrs;
	analysis_addrs.push(start);
	
	bool first = true;
	
	while (!analysis_addrs.empty())
	{
		address_t addr = analysis_addrs.top();
		analysis_addrs.pop();
		if (!addr.isValid())
			continue;
		
		I_MemlocData * m = t->lookup_memloc(addr);
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
	d->postGuiUpdate();
	return true;
}


bool ProgramFlowAnalysis::analyze(Workspace * d, DataType * dtcreate, address_t start)
{
	I_ProjectModel * t = d->getProjectModel();
	
	address_t addr;
	addr = start;
	
	bool first = true;
	
	while (addr.isValid()) {
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
