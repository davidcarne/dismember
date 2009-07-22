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

#include "memlocmanager.h"
#include "exception.h"
#include "memlocdata.h"

void MemlocManager::insertMemloc(MemlocData * a)
{
	assert(a);
	
	address_t addr = a->get_addr();
	
	memloclist_ci ci = m_memdata.lower_bound(addr);
	memloclist_ci end = m_memdata.lower_bound(addr + a->get_length());
	while (ci != end) {
		address_t baddr = (*ci++).first;
		m_memdata_hash.erase(baddr);
		m_memdata.erase(baddr);
	}
	MemlocData *ov = findMemloc(addr, false);
	if (ov)
		throw Exception("fixme: Overlapping memlocds.");
	
	// insert into both lookup tables
	m_memdata_hash[addr] = a;
	m_memdata[addr] = a;
}


MemlocData * MemlocManager::findMemloc(address_t addr, bool exactmatch) const
{	
	// Try and do a fast hash lookup
	memlochash_ci i = m_memdata_hash.find(addr);
	if (i != m_memdata_hash.end())
		return (*i).second;
	
	if (!exactmatch)
	{
		// Slower "address inside data" lookup
		// Find the one after
		// and see if the one before contains the addr
		memloclist_ci ib = m_memdata.upper_bound(addr);
		if (ib != m_memdata.begin())
		{
			ib--;
			MemlocData * d = (*ib).second;
			address_t daddr = d->get_addr();
			if (addr.comparableTo(daddr) && daddr <= addr
				&& daddr + d->get_length() > addr)
				return d;
		}
	}
	
	return NULL;
}

void MemlocManager::removeMemloc(MemlocData * a)
{

	address_t addr = a->get_addr();
	
	m_memdata.erase(addr);
	
	memlochash_i ih = m_memdata_hash.find(addr);
	if (ih != m_memdata_hash.end())
		m_memdata_hash.erase(ih);
	
}

MemlocManager::memloclist_ci MemlocManager::memloc_list_begin() const
{
	return m_memdata.begin();
}

MemlocManager::memloclist_ci MemlocManager::memloc_list_end() const
{
	return m_memdata.end();
}

