#include "trace.h"

#include "memlocdata.h"

MemlocData::MemlocData(const DataType * creator, Trace * ctx, address_t addr)
 : m_creator(creator), m_address(addr), m_ctx(ctx), m_comment(0)
{
}

address_t	MemlocData::get_addr() const
{
	return m_address;
}

const DataType * MemlocData::getCreatingDataType() const
{
	return m_creator;
}


xref_map_ci MemlocData::begin_xref_to() const
{
	return m_ctx->xref_to_lower_bound(m_address);
}

xref_map_ci MemlocData::end_xref_to() const
{
	return m_ctx->xref_to_upper_bound(m_address);
}

u32 MemlocData::count_xrefs_to() const
{
	return m_ctx->xref_to_count(m_address);
}

bool MemlocData::has_xrefs_to() const
{
	return count_xrefs_to() != 0;
}

xref_map_ci MemlocData::begin_xref_from() const
{
	return m_ctx->xref_from_lower_bound(m_address);
}

xref_map_ci MemlocData::end_xref_from() const
{
	return m_ctx->xref_from_upper_bound(m_address);
}

u32 MemlocData::count_xrefs_from() const
{
	return m_ctx->xref_from_count(m_address);
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


Trace * MemlocData::get_ctx() const
{
	return m_ctx;
}

