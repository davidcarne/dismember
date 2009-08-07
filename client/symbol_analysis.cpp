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
#include "instruction.h"
#include "binaryconstant.h"
#include "datatype.h"
#include "symbol_analysis.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>

sp_RunQueueJob SymbolAnalysis::createAnalysisJob(Workspace * d)
{
	FunctorRunQueueJob::jobfun_t jb =
			boost::bind(&SymbolAnalysis::analyze, d);
	return createFunctorRunQueueJob("symbol analysis", 0, jb);
}

void SymbolAnalysis::submitAnalysisJob(Workspace * d)
{
	d->getRunQueue()->submit(createAnalysisJob(d));
}

bool SymbolAnalysis::analyze(Workspace *d)
{
	I_ProjectModel *t = d->getProjectModel();
	char namebuf[128];
	char type[16];
	int size;
	bool subroutine = false;

	MemlocManager::memloclist_ci it = t->memloc_list_begin();
	MemlocManager::memloclist_ci end = t->memloc_list_end();
	
	for (; it != end; ++it) {
		I_MemlocData * id = (*it).second;
		if (!id || !id->has_xrefs_to())
			continue;
		if (id->get_symbol()) {
			const Symbol *sym = id->get_symbol();
			AbstractData *ad = sym->get_property("generated");
			if (!ad || !boost::get<bool>(*ad))
				continue;
		}

		Instruction * aid = dynamic_cast<Instruction *>(id);

		address_t addr = id->get_addr();
		std::string saddr_l = addr.toString();
		const char *saddr = saddr_l.c_str();
		if (!strncmp(saddr, "0x", 2))
			saddr += 2;
		if (!id->is_executable()) {
			sprintf(type, "data");
			switch ((size = id->get_length())) {
			case 4:
				sprintf(namebuf, "_dword_%s", saddr);
				break;
			case 2:
				sprintf(namebuf, "_hword_%s", saddr);
				break;
			case 1:
				sprintf(namebuf, "_byte_%s", saddr);
				break;
			}
		}
		else if (aid && aid->get_pcflags() & Instruction::PCFLAG_FNENT) {
			sprintf(type, "code");
			subroutine = true;
			sprintf(namebuf, "_sub_%s", saddr);
		}
		else {
			sprintf(type, "unk");
			sprintf(namebuf, "_loc_%s", saddr);
		}
		if (id->get_symbol() && !strcmp(id->get_symbol()->get_name().c_str(), namebuf))
			continue;
		
		Symbol *sym = t->create_sym(namebuf, addr);
		if (!sym) // FIXME: something bad happened, but what?
			continue;
		sym->set_property("type", new AbstractData(std::string(type)));
		if (!id->is_executable())
			sym->set_property("size", new AbstractData(size));
		sym->set_property("subroutine", new AbstractData(subroutine));
		sym->set_property("generated", new AbstractData(true));
	}

	d->postGuiUpdate();
	return true;
}
