#include "memsegment.h"

#include <string.h>
#include <stdio.h>

MemSegment::MemSegment(address_t base, address_t len, void * buffer, address_t flength) : m_base(base), m_len(len)
{
	if (buffer)
	{
		m_data = new uint8_t[len];
		bzero(m_data, len);
		
		if (flength != (address_t)-1)
			memcpy(m_data, buffer, flength);
		else
			memcpy(m_data, buffer, len);
		
	} else
		m_data = NULL;
}

MemSegment::~MemSegment()
{
	if (m_data)
		delete m_data;
}

address_t MemSegment::get_start() const
{
	return m_base;
}

address_t MemSegment::get_length() const
{
	return m_len;
}

bool MemSegment::get_bytes(address_t addr, address_t length, void * dest) const
{
	if (!m_data)
		return false;

	if (addr < m_base)
		return false;
	
	address_t offs = addr - m_base;
	
	if (offs + length > m_len)
		return false;
	
	memcpy(dest, m_data + offs, (size_t)length);
	return true;
}

bool MemSegment::can_resolve(address_t addr) const
{
	return (addr >= m_base) && (addr < m_base + m_len);
}

bool MemSegment::is_defined() const
{
	return m_data != NULL;
}


const uint8_t * MemSegment::data_ptr() const
{
	return m_data;
}
