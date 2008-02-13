#include "address_hash.h"

#include "trace.h"
#include "memlocdata.h"
#include "binaryconstant.h"
#include "stringconstant.h"
#include "datatype.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <stdexcept>
#include <errno.h>
#include <assert.h>
#include <stack>
//#include "memtags.h"

#include "memsegment.h"

#define ASSERT_RESOLVE(addr) assert(readByte(addr, NULL))
Trace::Trace(Architecture * arch) : m_arch(arch)
{
	m_last_segment = NULL;
	// HACK HACK HACK
	createBinaryConstantDataTypes(this);
	createStringConstantDataTypes(this);
}

void Trace::analyze(address_t start)
{
	ASSERT_RESOLVE(start);
	follow_code_build_insns(start);
	build_xrefs();
	build_xref_syms();
	
	// HACK< removeme
	notifyMemlocChange(NULL, HOOK_MODIFY);
}

/* Factor out */
bool Trace::add_segment(MemSegment * m)
{
	assert(m);
	
	// HACK< removeme
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


void Trace::createMemlocDataAt(DataType * d, address_t addr)
{
	assert(d);
	insert_memlocd(d->instantiateForTraceAndAddress(addr));
}

Trace::~Trace()
{
}

/* Symbol accessors */
SymbolList::symname_ci Trace::sym_begin_name()
{
	return m_symlist.begin_name();
}

SymbolList::symname_ci Trace::sym_end_name()
{
	return m_symlist.end_name();
}

SymbolList::symaddr_ci Trace::sym_begin_addr()
{
	return m_symlist.begin_addr();
}

SymbolList::symaddr_ci Trace::sym_end_addr()
{
	return m_symlist.end_addr();
}

Symbol * Trace::find_ordered_symbol(uint32_t index, SymbolList::symsortorder_e order)
{
	return m_symlist.find_ordered_symbol(index, order);
}

uint32_t Trace::get_symbol_count(void) const
{
	return  m_symlist.get_symbol_count();
}

const Symbol * Trace::lookup_symbol(const std::string & symname)
{
	const Symbol *sy = m_symlist.get_symbol(symname);
	return sy;
}

const Symbol * Trace::lookup_symbol(address_t addr)
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

	// insert into both lookup tables
	m_memdata_hash[addr] = a;
	m_memdata[addr] = a;
}

MemlocData * Trace::lookup_memloc(address_t addr, bool exactmatch)
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
		memloclist_i ib = m_memdata.upper_bound(addr);
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

Comment *Trace::lookup_comment(address_t addr)
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
	
DataType * Trace::lookupDataType(std::string name)
{
	return m_datatypelist.lookupDataType(name);
}

void Trace::insertDataType(std::string name, DataType * d)
{
	assert(d);
	assert(name.size() > 0);
	return m_datatypelist.insertDataType(name, d);
}
