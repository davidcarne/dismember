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

Xref::Xref(ProjectModel * lookupsrc, address_t srcaddr, address_t dstaddr, xref_type_e type):
	m_srcaddr(srcaddr), m_dstaddr(dstaddr), m_type(type), m_lookuptrace(lookupsrc)
{
	
}

bool Xref::operator==(const Xref & x)
{
	return (x.m_srcaddr == m_srcaddr) && (x.m_dstaddr == m_dstaddr) && (x.m_type == m_type);
}

