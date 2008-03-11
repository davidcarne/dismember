/*
 *  memlocmanager.h
 *  dismember
 *
 *  Created by David Carne on 05/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _memlocmanager_H_
#define _memlocmanager_H_

#include <list>
#include <ext/hash_map>
#include <map>
#include "types.h"

class MemlocData;

class MemlocManager {
public:
	typedef std::map<address_t, MemlocData * > memloclist_t;
	typedef memloclist_t::const_iterator memloclist_ci;
	
	typedef __gnu_cxx::hash_map<address_t, MemlocData * > memlochash_t;
	typedef memlochash_t::const_iterator memlochash_ci;
	
	void insertMemloc(MemlocData * a);
	MemlocData * findMemloc(address_t addr, bool exactmatch) const;
	
	void removeMemloc(MemlocData * a);
	
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


