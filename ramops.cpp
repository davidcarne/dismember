#include "trace.h"
#include "memsegment.h"

#include <stdexcept>


bool Trace::resolve(address_t addr,u8 bytes, u8 * buf) const
{
	assert(buf);
	
	if (m_last_segment && m_last_segment->get_bytes(addr, bytes, buf))
		return true;
	
	for (memseglist_ci i = m_mem_segments.begin(); i != m_mem_segments.end(); i++)
		if ((*i)->get_bytes(addr, bytes, buf))
		{
			m_last_segment = (*i);
			return true;
		}

	return false;
}

u32 Trace::ldw(address_t taddr) const
{
	u32 buf;
	if (!resolve(taddr,4, (u8*)&buf))
	{
		assert(false);
	}
	return endian_swap32(buf);
}

bool Trace::readByte(address_t taddr, uint8_t * data) const
{
	uint8_t dummy;
	
	if (data)
		return resolve(taddr,1, data);
	
	return resolve(taddr,1, &dummy);
}
