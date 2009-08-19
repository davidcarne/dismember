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
#include "kvs_projectmodel.h"
#include "kvs_memlocdata.h"


MemlocData::MemlocData(const sp_LocalKVSNode sourceNode, const I_ProjectModel * ctx) : I_KVS_attribproxy(sourceNode->getAttributesReference())
{
	m_decoded_length = atoi(getAttrib("length").c_str());
}

MemlocData::MemlocData(sp_I_KVS_attributes attribs, const DataType * creator, const I_ProjectModel * ctx, address_t addr)
: m_ctx(ctx), m_address(addr), m_creator(creator),  I_KVS_attribproxy(attribs)
{
	const DataTypeDecoding & decoding = creator->decode(m_address);
	
	u32 length = m_decoded_length = decoding.getDecodedLength();
	
	m_next = static_cast<MemlocData*>(ctx->lookup_memloc(m_address + length, true));
	if (m_next)
		m_next->m_prev = this;
	
	m_prev = static_cast<MemlocData*>(ctx->lookup_memloc(m_address - 1, false));
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
	
	m_prev = static_cast<MemlocData*>(m_ctx->lookup_memloc(m_address -1, false));
	if (m_prev)
		m_prev->m_next = this;
	
	return m_prev;
}

MemlocData *  MemlocData::getNextContiguous()
{
	if (m_next)
		return m_next;
	
	m_next = static_cast<MemlocData*>(m_ctx->lookup_memloc(m_address + get_length(), true));
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
	
}

XrefManager::xref_map_ci MemlocData::end_xref_to() const
{
	
}

u32 MemlocData::count_xrefs_to() const
{
	
}

bool MemlocData::has_xrefs_to() const
{
	return count_xrefs_to() != 0;
}

XrefManager::xref_map_ci MemlocData::begin_xref_from() const
{
	
}

XrefManager::xref_map_ci MemlocData::end_xref_from() const
{
	
}

u32 MemlocData::count_xrefs_from() const
{
	
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


const std::string MemlocData::get_textual() const
{
	
	const DataTypeDecoding & decoding = m_creator->decode(m_address);
	return decoding.getDecodedText();
}

const I_ProjectModel * MemlocData::get_ctx() const
{
	return m_ctx;
}

u32 MemlocData::get_length() const
{
	return m_decoded_length;
}

bool MemlocData::logically_continues() const
{
	return false;
}

bool MemlocData::is_executable() const
{
	// TODO: Load this from attributes
	return false;
}