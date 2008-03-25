#include "dsmem_trace.h"
#include "memlocdata.h"
#include "instruction.h"
#include "binaryconstant.h"
#include "datatype.h"
#include "xref.h"

#if 0
void Trace::build_xref_syms()
{
	char namebuf[128];
	char type[16];
	int size;
	bool subroutine = false;
	
	for (MemlocManager::memloclist_ci i = m_memdata.begin(); i != m_memdata.end(); i++) {
		MemlocData * id = (*i).second;

		if (id && id->has_xrefs_to() && (!id->get_symbol() || id->get_symbol()->get_name().c_str()[0] == '_'))
		{
			Instruction * aid = dynamic_cast<Instruction *>(id);
			
			
			
			if (!id->is_executable())
			{
				sprintf(type, "data");
				switch ((size = id->get_length()))
				{
					case 4:
						sprintf(namebuf, "_dword_%08X", id->get_addr());
						break;
					case 2:
						sprintf(namebuf, "_hword_%08X", id->get_addr());
						break;
					case 1:
						sprintf(namebuf, "_byte_%08X", id->get_addr());
						break;
				}
			}
			else if (aid && aid->get_pcflags() & Instruction::PCFLAG_FNENT) {
				sprintf(type, "code");
				subroutine = true;
				sprintf(namebuf, "_sub_%08X", id->get_addr());
			}
			else {
				sprintf(type, "unk");
				sprintf(namebuf, "_loc_%08X", id->get_addr());
			}
			if (id->get_symbol() && !strcmp(id->get_symbol()->get_name().c_str(), namebuf))
				break;
			
			Symbol *sym = create_sym(namebuf, id->get_addr());
			sym->set_property("type",
					new GenericAbstractData(std::string(type)));
			if (!id->is_executable())
				sym->set_property("size",
						new GenericAbstractData(size));
			sym->set_property("subroutine",
					new GenericAbstractData(subroutine));
		}
	}
}
#endif

