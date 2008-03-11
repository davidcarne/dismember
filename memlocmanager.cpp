/*
 *  memlocmanager.cpp
 *  dismember
 *
 *  Created by David Carne on 05/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "address_hash.h"

#include "memlocmanager.h"

#include "memlocdata.h"

void MemlocManager::insertMemloc(MemlocData * a)
{
	assert(a);
	
	address_t addr = a->get_addr();
	
	memloclist_ci ci = m_memdata.lower_bound(addr);
	memloclist_ci end = m_memdata.lower_bound(addr + a->get_length());
	for (; ci != end; ++ci) {
		address_t baddr = (*ci).first;
		m_memdata_hash.erase(baddr);
		m_memdata.erase(baddr);
	}
	MemlocData *ov = findMemloc(addr, false);
	if (ov)
		throw std::out_of_range("fixme: Overlapping memlocds.");
	
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
			if (d->get_addr() <= addr && d->get_addr() + d->get_length() > addr)
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

