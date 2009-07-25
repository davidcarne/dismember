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

#include "memsegment.h"
#include "address.h"

#include <string.h>
#include <stdio.h>

MemSegment::MemSegment(paddr_t base, psize_t len, void * buffer, psize_t flength, const std::string & name)
 : m_base(base), m_len(len), m_data(NULL), m_name(name)
{
	init(buffer, flength);
}

MemSegment::MemSegment(paddr_t base, psize_t len, void * buffer, const std::string & name)
 : m_base(base), m_len(len), m_data(NULL), m_name(name)
{
	init(buffer, len);
}

MemSegment::MemSegment(paddr_t base, psize_t len, const std::string & name)
 : m_base(base), m_len(len), m_data(NULL), m_name(name)
{
	init(NULL, 0);
}

void MemSegment::init(void *data, psize_t initLength)
{
	if (initLength > 0) {
		// TODO:, removeme, +1 to hack around a boost bug
		m_data = new uint8_t[m_len+1];
		bzero(m_data, m_len);
		
		if (initLength != m_len)
			memcpy(m_data, data, initLength);
		else
			memcpy(m_data, data, m_len);
	}
}

MemSegment::~MemSegment()
{
	if (m_data)
		delete m_data;
}

paddr_t MemSegment::get_start() const
{
	return m_base;
}

address_t MemSegment::getBaseAddress() const
{

	return address_t(0, 32, this);
}

psize_t MemSegment::get_length() const
{
	return m_len;
}

bool MemSegment::get_bytes(address_t addr, psize_t length, void * dest) const
{
	if (!m_data)
		return false;

	if (!can_resolve(addr))
		return false;

	paddr_t offs = addr.getOffset();

	memcpy(dest, m_data + offs, (size_t)length);
	return true;
}

bool MemSegment::can_resolve(paddr_t addr) const
{
	return (addr >= m_base) && (addr < m_base + m_len);
}


bool MemSegment::can_resolve(address_t addr) const
{
	return addr.isValid() && addr.m_memsegment == this;
}

bool MemSegment::is_defined() const
{
	return m_data != NULL;
}


const uint8_t * MemSegment::data_ptr() const
{
	return m_data;
}

const std::string & MemSegment::getName() const
{
	return m_name;
}

bool MemSegment::less::operator()(MemSegment *a, MemSegment *b) const
{
	return a->get_start() < b->get_start();
}
