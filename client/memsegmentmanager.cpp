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

#include "memsegmentmanager.h"
#include <assert.h>
#include "exception.h"

MemSegmentManager::MemSegmentManager()
{
	
}
/* Factor out */
bool MemSegmentManager::addSegment(MemSegment * m)
{
	assert(m);
	
	paddr_t start = m->get_start(), end = start + m->get_length();
	
	memseglist_ci ci = m_mem_segments.begin(),
	endi = m_mem_segments.end();
	for (; ci != endi; ++ci) {
		MemSegment *mi = *ci;
		paddr_t cstart = mi->get_start();
		paddr_t cend = cstart + mi->get_length();
		if ((cstart > start && cstart < end) ||
		    (cend > start && cend < end))
			throw Exception("Memory segments overlap.");
	}
	
	m_mem_segments.insert(m);
	return true;
}

address_t MemSegmentManager::locateAddress(uint64_t address) const
{
	
	memseglist_ci ci = m_mem_segments.begin(),
	endi = m_mem_segments.end();
	for (; ci != endi; ++ci) {
		MemSegment *m = (*ci);
		if (m->can_resolve(address))
			return m->getBaseAddress() + (address - m->get_start());
	}
	return address_t();
}

bool MemSegmentManager::readBytes(const address_t & addr,u8 bytes, u8 * buf) const
{
	assert(buf);
	return addr.readBytes(bytes, buf);
}

bool MemSegmentManager::readByte(const address_t & taddr, uint8_t * data) const
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

