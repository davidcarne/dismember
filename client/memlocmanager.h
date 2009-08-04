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

#ifndef _memlocmanager_H_
#define _memlocmanager_H_

#include <list>
#include "hash_map.h"
#include <map>
#include "types.h"
#include <boost/serialization/access.hpp>

class I_MemlocData;

class MemlocManager {
public:
	typedef std::map<address_t, I_MemlocData *, address_t::less> memloclist_t;
	typedef memloclist_t::const_iterator memloclist_ci;
	
	typedef std::hash_map<address_t, I_MemlocData * > memlochash_t;
	typedef memlochash_t::const_iterator memlochash_ci;
	
	void insertMemloc(I_MemlocData * a);
	I_MemlocData * findMemloc(address_t addr, bool exactmatch) const;
	
	void removeMemloc(I_MemlocData * a);
	
	memloclist_ci memloc_list_begin() const;
	memloclist_ci memloc_list_end() const;
	
private:
	
	template<class Archive> void serialize(Archive & ar, const unsigned int)
	{
		ar & m_memdata;
	}
	
	friend class boost::serialization::access;
	
	typedef memloclist_t::iterator memloclist_i;
	typedef memlochash_t::iterator memlochash_i;
	
	memloclist_t m_memdata;
	
	/* Cache, do not serialize */
	mutable memlochash_t m_memdata_hash;
};


#endif


