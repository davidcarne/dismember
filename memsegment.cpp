#include "memsegment.h"
#include "address.h"

#include <string.h>
#include <stdio.h>

MemSegment::MemSegment(uint64_t base, uint64_t len, void * buffer, uint64_t flength) : m_base(base), m_len(len)
{
	if (buffer)
	{
		// TODO:, removeme, +1 to hack around a boost bug
		m_data = new uint8_t[len+1];
		bzero(m_data, len);
		
		if (flength != (uint64_t)-1)
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

uint64_t MemSegment::get_start() const
{
	return m_base;
}

address_t MemSegment::getBaseAddress() const
{

	return address_t(0, 32, this);
}

uint64_t MemSegment::get_length() const
{
	return m_len;
}

bool MemSegment::get_bytes(address_t addr, uint64_t length, void * dest) const
{
	if (!m_data)
		return false;

	if (!addr.isValid())
		return false;
	
	uint64_t offs = addr.getOffset();
	
	memcpy(dest, m_data + offs, (size_t)length);
	return true;
}

bool MemSegment::can_resolve(uint64_t addr) const
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
