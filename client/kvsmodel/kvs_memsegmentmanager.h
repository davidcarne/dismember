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

#ifndef _KVS_MEMSEGMENTMANAGER_H_
#define _KVS_MEMSEGMENTMANAGER_H_
#include <set>

#include "i_memsegment.h"
#include "kvs_memsegment.h"
#include "kvs_proxies.h"
class KVS_MemSegmentManager : private I_KVS_nodeproxy {
public:
	KVS_MemSegmentManager(sp_I_KVS_node noderef);
	
	typedef std::set<sp_KVS_MemSegment, KVS_MemSegment::less> memseglist_t;
	typedef memseglist_t::const_iterator memseglist_ci;
	
	bool addSegment(sp_KVS_MemSegment m);
	memseglist_ci memsegs_begin() const;
	memseglist_ci memsegs_end() const;

	address_t locateAddress(uint64_t address) const;

private:
	memseglist_t m_mem_segments;
};

#endif


