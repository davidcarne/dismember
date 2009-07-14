#include "dsmem_trace.h"

#include "memlocdata.h"

MemlocData::MemlocData(const DataType * creator, const Trace * ctx, address_t addr, u32 length)
: m_ctx(ctx), m_address(addr), m_creator(creator)
{
	m_next = ctx->lookup_memloc(m_address + length, true);
	if (m_next)
		m_next->m_prev = this;
	
	m_prev = ctx->lookup_memloc(m_address - 1, false);
	if (m_prev)
		m_prev->m_next = this;
	
}

MemlocData::~MemlocData()
{
	if (m_prev)
		m_prev->m_next = NULL;
	
	if (m_next)
		m_next->m_prev = NULL;
}

MemlocData * MemlocData::getPreviousContiguous()
{
	if (m_prev)
		return m_prev;
	
	m_prev = m_ctx->lookup_memloc(m_address -1, false);
	if (m_prev)
		m_prev->m_next = this;
	
	return m_prev;
}

MemlocData *  MemlocData::getNextContiguous()
{
	if (m_next)
		return m_next;
	
	m_next = m_ctx->lookup_memloc(m_address + get_length(), true);
	if (m_next)
		m_next->m_prev = this;
	
	return m_next;
}

address_t	MemlocData::get_addr() const
{
	return m_address;
}

const DataType * MemlocData::getCreatingDataType() const
{
	return m_creator;
}


XrefManager::xref_map_ci MemlocData::begin_xref_to() const
{
	return m_ctx->m_xrefmanager.xref_to_lower_bound(m_address);
}

XrefManager::xref_map_ci MemlocData::end_xref_to() const
{
	return m_ctx->m_xrefmanager.xref_to_upper_bound(m_address);
}

u32 MemlocData::count_xrefs_to() const
{
	return m_ctx->m_xrefmanager.xref_to_count(m_address);
}

bool MemlocData::has_xrefs_to() const
{
	return count_xrefs_to() != 0;
}

XrefManager::xref_map_ci MemlocData::begin_xref_from() const
{
	return m_ctx->m_xrefmanager.xref_from_lower_bound(m_address);
}

XrefManager::xref_map_ci MemlocData::end_xref_from() const
{
	return m_ctx->m_xrefmanager.xref_from_upper_bound(m_address);
}

u32 MemlocData::count_xrefs_from() const
{
	return m_ctx->m_xrefmanager.xref_from_count(m_address);
}

bool MemlocData::has_xrefs_from() const
{
	return count_xrefs_from() != 0;
}

const Symbol * MemlocData:: get_symbol() const
{
	return m_ctx->lookup_symbol(get_addr());
}

/*
void MemlocData::add_xref_to(Xref * xr)
{
	if (!xr)
		return;
	
	if (!m_xrefs_to)
		m_xrefs_to = new xref_list();
	
	m_xrefs_to->push_back(xr);
}

void MemlocData::add_xref_from(Xref * xr)
{
	if (!xr)
		return;
	
	if (!m_xrefs_from)
		m_xrefs_from = new xref_list();
	
	m_xrefs_from->push_back(xr);
}
*/

void MemlocData::mark_explicit(bool mark)
{
	m_explicit = mark;
}

bool MemlocData::get_explicit() const
{
	return m_explicit;
}


const Trace * MemlocData::get_ctx() const
{
	return m_ctx;
}

