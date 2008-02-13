#include "trace.h"
#include "memlocdata.h"
#include "binaryconstant.h"
#include "datatype.h"

Xref *Trace::create_xref(address_t srcaddr, address_t dstaddr, u32 type)
{
	
	MemlocData * src = lookup_memloc(srcaddr);
	MemlocData * dst = lookup_memloc(dstaddr);
	
	if (!src)
		return NULL;
	
	// Check if we already have an xref pointing to the destination
	for (xref_map_ci i = xref_from_lower_bound(srcaddr); i!= xref_from_upper_bound(srcaddr); i++)
		if ((*i).second->get_dst_addr() == dstaddr)
			return (*i).second;
	
	Xref * sx = new Xref(this, srcaddr, dstaddr, type);

	// Mark as a function call
	if (dst)
	{
		if (type == XR_TYPE_FNCALL)
		{
			Instruction * instdst;
			instdst = dynamic_cast<Instruction *>(dst);
			if (instdst)
				instdst->mark_fn_ent(true);
		}
			
	}
	
	// Insert into the master xref list
	m_xrefs_master.push_back(sx);
	
	// Add to the to/from caches
	m_xrefs_to.insert(std::pair<const address_t, Xref*>(dstaddr, sx));
	m_xrefs_from.insert(std::pair<const address_t, Xref*>(srcaddr, sx));

	return sx;
}

void Trace::build_xrefs()
{
	for (memloclist_i i = m_memdata.begin(); i != m_memdata.end(); i++) {
		Instruction * id = dynamic_cast<Instruction*>((*i).second);
		if (id)
		{
			if ((id->get_pcflags() & PCFLAG_LOCMASK) == PCFLAG_DIRLOC)
				create_xref(id->get_addr(), id->get_direct_jump_addr(), id->get_pcflags() & PCFLAG_DIRLK ? XR_TYPE_FNCALL : XR_TYPE_JMP);
			
			if (id->get_pcflags() & PCFLAG_DREF)
			{
				address_t daddr = id->get_data_ref_addr();
				
				MemlocData * ddst = lookup_memloc(daddr);
				
				u32 size = 0;
				if ((id->get_pcflags() & PCFLAG_DSMASK) == PCFLAG_DSWORD)
				{
					size = 4;
				}
				else if ((id->get_pcflags() & PCFLAG_DSMASK) == PCFLAG_DSBYTE)
				{	
					size = 1;
				}
				
				if (!ddst)
				{
					switch (size) {
						// HACK HACK HACK - changeme
						case 1:
							insert_memlocd(datatype_u8_le->instantiateForTraceAndAddress(daddr));
							break;
							
						case 4:
							insert_memlocd(datatype_u32_le->instantiateForTraceAndAddress(daddr));
							break;
					}
				}
				
				create_xref(id->get_addr(), daddr, XR_TYPE_DATA);
			}

		}
	}
}

void Trace::build_xref_syms()
{
	char namebuf[128];
	char type[16];
	int size;
	bool subroutine = false;
	
	for (memloclist_i i = m_memdata.begin(); i != m_memdata.end(); i++) {
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
			else if (aid && aid->get_pcflags() & PCFLAG_FNENT) {
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
