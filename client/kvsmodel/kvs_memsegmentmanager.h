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

#ifndef _memsegmentmanager_H_
#define _memsegmentmanager_H_
#include <set>
#include "memsegment.h"

#include <boost/serialization/utility.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/tracking.hpp>

class MemSegmentManager {
public:
	MemSegmentManager();
	
	typedef std::set<MemSegment *, MemSegment::less> memseglist_t;
	typedef memseglist_t::const_iterator memseglist_ci;
	
	bool addSegment(MemSegment * m);
	bool readByte(const address_t & taddr, uint8_t * data) const;
	bool readBytes(const address_t &, u8, u8*) const;
	memseglist_ci memsegs_begin() const;
	memseglist_ci memsegs_end() const;

	address_t locateAddress(uint64_t address) const;

private:
	
	template<class Archive> void serialize(Archive & ar, const unsigned int)
	{
		ar & m_mem_segments;
	}
	
	friend class boost::serialization::access;
	
	/* Needs serializing */
	memseglist_t m_mem_segments;
	
};

#endif


