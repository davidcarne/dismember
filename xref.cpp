#include "dsmem_trace.h"
#include "xref.h"

address_t Xref::get_src_addr() const
{
	return m_srcaddr;
}

address_t Xref::get_dst_addr() const
{
	return m_dstaddr;
}

const Xref::xr_mtype * Xref::get_src_inst() const
{
	return m_lookuptrace->lookup_memloc(m_srcaddr);
}

const Xref::xr_mtype * Xref::get_dst_inst() const
{
	return m_lookuptrace->lookup_memloc(m_dstaddr);
}

u32 Xref::get_type() const
{
	return m_type;
}

Xref::Xref(Trace * lookupsrc, address_t srcaddr, address_t dstaddr, xref_type_e type):
	m_srcaddr(srcaddr), m_dstaddr(dstaddr), m_type(type), m_lookuptrace(lookupsrc)
{
	
}

bool Xref::operator==(const Xref & x)
{
	return (x.m_srcaddr == m_srcaddr) && (x.m_dstaddr == m_dstaddr) && (x.m_type == m_type);
}

