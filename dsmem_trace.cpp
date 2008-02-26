#include "address_hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdexcept>
#include <errno.h>
#include <assert.h>
#include <stack>

#include "trace.h"
#include "memlocdata.h"
#include "binaryconstant.h"
#include "stringconstant.h"
#include "datatype.h"
#include "xref.h"
#include "instruction.h"

//#include "memtags.h"
#include "architecture.h"
#include "memsegment.h"

#define ASSERT_RESOLVE(addr) assert(readByte(addr, NULL))
Trace::Trace(Architecture * arch) : m_arch(arch)
{
	m_last_segment = NULL;
	m_cdat = m_arch->createDataType(this);
}

void Trace::analyze(address_t start)
{
	ASSERT_RESOLVE(start);
	//follow_code_build_insns(start);
	build_xrefs();
	build_xref_syms();
	
	/** \todo notifications need to be reworked, a global notification is a bit of a hack */
	notifyMemlocChange(NULL, HOOK_MODIFY);
}

/* Factor out */
bool Trace::add_segment(MemSegment * m)
{
	assert(m);
	
	/** \todo notifications need to be reworked, a global notification is a bit of a hack. Also, a dedicated memory segment cat would be good */
	notifyMemlocChange(NULL, HOOK_MODIFY);
	
	address_t start = m->get_start(), end = start + m->get_length();

	memseglist_ci ci = m_mem_segments.begin(),
			endi = m_mem_segments.end();
	for (; ci != endi; ++ci) {
		MemSegment *mi = *ci;
		address_t cstart = mi->get_start();
		address_t cend = cstart + mi->get_length();
		if ((cstart > start && cstart < end) ||
		    (cend > start && cend < end))
			throw (std::out_of_range("Memory segments overlap."));
	}

	m_mem_segments.insert(m);
	return true;
}


MemlocData * Trace::createMemlocDataAt(DataType * d, address_t addr)
{
	assert(d);
	MemlocData * i = d->instantiate(addr);
	if (i)
		insert_memlocd(i);
	return i;
}

Trace::~Trace()
{
}

/* Symbol accessors */
SymbolList::symname_ci Trace::sym_begin_name() const
{
	return m_symlist.begin_name();
}

SymbolList::symname_ci Trace::sym_end_name() const
{
	return m_symlist.end_name();
}

SymbolList::symaddr_ci Trace::sym_begin_addr() const
{
	return m_symlist.begin_addr();
}

SymbolList::symaddr_ci Trace::sym_end_addr() const
{
	return m_symlist.end_addr();
}

Symbol * Trace::find_ordered_symbol(uint32_t index, SymbolList::symsortorder_e order) const
{
	return m_symlist.find_ordered_symbol(index, order);
}

uint32_t Trace::get_symbol_count(void) const
{
	return  m_symlist.get_symbol_count();
}

const Symbol * Trace::lookup_symbol(const std::string & symname) const
{
	const Symbol *sy = m_symlist.get_symbol(symname);
	return sy;
}

const Symbol * Trace::lookup_symbol(address_t addr) const
{
	const Symbol *sy = m_symlist.get_symbol(addr);
	return sy;
}

void Trace::insert_memlocd(MemlocData * a)
{
	assert(a);
	
	address_t addr = a->get_addr();
	
	memloclist_ci ci = m_memdata.lower_bound(addr);
	memloclist_ci end = m_memdata.lower_bound(addr + a->get_length());
	for (; ci != end; ++ci) {
		address_t baddr = (*ci).first;
		m_memdata_hash.erase(baddr);
		m_memdata.erase(baddr);
	}
	MemlocData *ov = lookup_memloc(addr, false);
	if (ov)
		throw std::out_of_range("fixme: Overlapping memlocds.");

	// Todo: this is sorta hacky....  the memlocd's should insert their own xrefs, or provide a way to enumerate them
	Instruction * b = dynamic_cast<Instruction*>(a);
	
	// insert into both lookup tables
	m_memdata_hash[addr] = a;
	m_memdata[addr] = a;
	
	// Create Code XREF's if this memlocdata has any
	// The fn / jmp business is to avoid a gcc bug
	u32 fn = Xref::XR_TYPE_FNCALL;
	u32 jmp = Xref::XR_TYPE_JMP;
	if (b && (b->get_pcflags() & Instruction::PCFLAG_LOCMASK) == Instruction::PCFLAG_DIRLOC)
		create_xref(b->get_addr(), b->get_direct_jump_addr(), b->get_pcflags() & Instruction::PCFLAG_DIRLK ? fn : jmp);
	
	if (b->get_pcflags() & Instruction::PCFLAG_DREF)
		create_xref(b->get_addr(), b->get_data_ref_addr(), Xref::XR_TYPE_DATA);
	
	notifyMemlocChange(a, HOOK_MODIFY);
}

MemlocData * Trace::lookup_memloc(address_t addr, bool exactmatch) const
{	
	// Try and do a fast hash lookup
	memlochash_ci i = m_memdata_hash.find(addr);
	if (i != m_memdata_hash.end())
		return (*i).second;
	
	if (!exactmatch)
	{
		// Slower "address inside data" lookup
		// Find the one after
		// and see if the one before contains the addr
		memloclist_ci ib = m_memdata.upper_bound(addr);
		if (ib != m_memdata.begin())
		{
			ib--;
			MemlocData * d = (*ib).second;
			if (d->get_addr() <= addr && d->get_addr() + d->get_length() > addr)
				return d;
		}
	}

	return NULL;
}

void Trace::remove_memloc(address_t addr)
{
	memloclist_i i = m_memdata.find(addr);
	if (i == m_memdata.end())
		return;
	
	MemlocData * x = (*i).second;
	notifyMemlocChange(x, HOOK_DELETE);
	m_memdata.erase(i);
	
	memlochash_i ih = m_memdata_hash.find(addr);
	if (ih != m_memdata_hash.end())
		m_memdata_hash.erase(ih);
	
	delete x;
}

memloclist_ci Trace::memloc_list_begin() const
{
	return m_memdata.begin();
}

memloclist_ci Trace::memloc_list_end() const
{
	return m_memdata.end();
}

Comment *Trace::create_comment(std::string cmt, address_t addr)
{
	return m_commentlist.set_comment(addr, cmt);
}

Comment *Trace::lookup_comment(address_t addr) const
{
	return m_commentlist.get_comment(addr);
}

Symbol *Trace::create_sym(const std::string & name, address_t addr)
{

	//MemlocData * memd = lookup_memloc(addr);
	Symbol * sym = m_symlist.set_symbol(addr, name);
	if (sym != NULL)
		notifySymbolChange(sym, HOOK_CREATE);
	//if (memd)
	//	memd->set_symbol(sym);
	return sym;
}


xref_map_ci Trace::xref_from_lower_bound(address_t addr) const
{
	return m_xrefs_from.lower_bound(addr);
}

xref_map_ci Trace::xref_from_upper_bound(address_t addr) const
{
	return m_xrefs_from.upper_bound(addr);
}

u32 Trace::xref_from_count(address_t addr) const
{
	return m_xrefs_from.count(addr);
}


xref_map_ci Trace::xref_to_lower_bound(address_t addr) const
{
	return m_xrefs_to.lower_bound(addr);
}

xref_map_ci Trace::xref_to_upper_bound(address_t addr) const
{
	return m_xrefs_to.upper_bound(addr);
}

u32 Trace::xref_to_count(address_t addr) const
{
	return m_xrefs_to.count(addr);
}

Trace::memseglist_ci Trace::memsegs_begin() const
{
	return m_mem_segments.begin();
}

Trace::memseglist_ci Trace::memsegs_end() const
{
	return m_mem_segments.end();
}

DataTypeReg::datatypereg_ci Trace::getDataTypeBegin() const
{
	return m_datatypelist.getBegin();
}

DataTypeReg::datatypereg_ci Trace::getDataTypeEnd() const
{
	return m_datatypelist.getEnd();
}
	
sp_DataType Trace::lookupDataType(std::string name) const
{
	return m_datatypelist.lookupDataType(name);
}

void Trace::addDataType(sp_DataType d)
{
	assert(d);
	assert(d->getName().size() > 0);
	return m_datatypelist.insertDataType(d->getName(), d);
}


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
	
	// TODO: This should be attached as a notifier, maybe?
	// Build data if its a data xreef
	if (type == Xref::XR_TYPE_DATA)
	{
		Instruction * id  = dynamic_cast<Instruction *>(src);
		if (id)
			switch (id->get_pcflags() & Instruction::PCFLAG_DSMASK) {
					// HACK HACK HACK - changeme
				case Instruction::PCFLAG_DSBYTE:
					//	insert_memlocd(datatype_u8_le->instantiate(daddr));
					break;
				
				case Instruction::PCFLAG_DSWORD:
					//	insert_memlocd(datatype_u32_le->instantiate(daddr));
					break;
			}
	}
	
	// Mark as a function call
	if (dst)
	{
		if (type == Xref::XR_TYPE_FNCALL)
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