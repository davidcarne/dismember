/*
 *  memsegmentmanager.cpp
 *  dismember
 *
 *  Created by David Carne on 05/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "memsegmentmanager.h"
#include <stdexcept>

MemSegmentManager::MemSegmentManager() : m_last_segment(NULL)
{
	
}
/* Factor out */
bool MemSegmentManager::addSegment(MemSegment * m)
{
	assert(m);
	
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


bool MemSegmentManager::readBytes(address_t addr,u8 bytes, u8 * buf) const
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

bool MemSegmentManager::readByte(address_t taddr, uint8_t * data) const
{
	uint8_t dummy;
	
	if (data)
		return readBytes(taddr,1, data);
	
	return readBytes(taddr,1, &dummy);
}


MemSegmentManager::memseglist_ci MemSegmentManager::memsegs_begin() const
{
	return m_mem_segments.begin();
}

MemSegmentManager::memseglist_ci MemSegmentManager::memsegs_end() const
{
	return m_mem_segments.end();
}

