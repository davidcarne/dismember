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

#include <string.h>
#include "symlist.h"

Symbol::Symbol(address_t addr, std::string name) : m_name(name), m_addr(addr)
{
}

const std::string & Symbol::get_name() const
{
	return m_name;
}


const address_t Symbol::get_addr() const
{
	return m_addr;
}

void Symbol::set_property(std::string key, AbstractData *value)
{
	properties[key.c_str()] = value;
}

AbstractData *Symbol::get_property(std::string key) const
{
	propertymap_ci i = properties.find(key.c_str());
	if (i == properties.end())
		return NULL;
	return (*i).second;
}


SymbolList::SymbolList(): m_vectors_dirty(false)
{

}

Symbol * SymbolList::set_symbol(address_t addr, std::string name)
{
	// already have a symbol by that name
	symnamemap_i name_found = m_name_lookup.find(name.c_str());
	if (name_found != m_name_lookup.end())
		return NULL;


	symaddrmap_i i = m_addr_lookup.find(addr);
	
	if (i != m_addr_lookup.end())
	{
		Symbol * del = (*i).second;
		m_name_lookup.erase(del->get_name().c_str());
		m_addr_lookup.erase(addr);
		
		delete del;
	}
	
	Symbol *sym = 0;
	if (name.length() > 0) {
		sym = new Symbol(addr, name.c_str());
		m_name_lookup[name.c_str()] = sym;
		m_addr_lookup[addr] = sym;
	}

	m_vectors_dirty = true;
	return sym;
}

const Symbol * SymbolList::get_symbol(address_t addr) const
{
	symaddrmap_ci i = m_addr_lookup.find(addr);
	if (i != m_addr_lookup.end())
		return (*i).second;
	return NULL;
}

const Symbol * SymbolList::get_symbol(const std::string & name) const
{
	symnamemap_ci i = m_name_lookup.find(name.c_str());
	if (i != m_name_lookup.end())
		return (*i).second;
	return NULL;
}


SymbolList::symname_ci SymbolList::begin_name() const
{
	updateDirtyVectors();
	return m_name_order.begin();
}

SymbolList::symname_ci SymbolList::end_name() const
{
	updateDirtyVectors();
	return m_name_order.end();
}

SymbolList::symname_ci SymbolList::begin_addr() const
{
	updateDirtyVectors();
	return m_addr_order.begin();
}

SymbolList::symname_ci SymbolList::end_addr() const
{
	updateDirtyVectors();
	return m_addr_order.end();
}


Symbol * SymbolList::find_ordered_symbol(uint32_t index, symsortorder_e order) const
{
	updateDirtyVectors();
	//printf("Looking up index %d, maxsize %d\n", index, m_name_order.size());
	switch (order)
	{
		case SYMORDER_NAME:
			return m_name_order[index];
		case SYMORDER_ADDR:
			return m_addr_order[index];
	}
	
	return NULL;
}

uint32_t SymbolList::get_symbol_count(void) const
{
	return m_name_lookup.size();
}

struct nameSort
{
	bool operator()(Symbol *  const  & a, Symbol*  const & b)
	{
		return strcmp(a->get_name().c_str(), b->get_name().c_str()) < 0;
	}
};

struct addrSort
{
	bool operator()(Symbol * const & a, Symbol*  const & b)
	{
		return a->get_addr() <  b->get_addr();
	}
};

void SymbolList::updateDirtyVectors() const
{
	if (!m_vectors_dirty)
		return;
	
	m_name_order.clear();
	m_addr_order.clear();
	
	
	m_addr_order.reserve(m_name_lookup.size());
	// Populate vectors
	for (symnamemap_ci i = m_name_lookup.begin(); i != m_name_lookup.end(); i++)
	{
		m_addr_order.push_back((*i).second);
	}
	
	m_name_order.reserve(m_addr_order.size());
	for (symnamemap_ci i = m_name_lookup.begin(); i != m_name_lookup.end(); i++)
	{
		m_name_order.push_back((*i).second);
	}
	/*
	std::copy(m_addr_order.begin(), m_addr_order.end(), m_name_order.begin());
	*/
	// Sort vectors
	std::sort(m_addr_order.begin(), m_addr_order.end(), addrSort());
	std::sort(m_name_order.begin(), m_name_order.end(), nameSort());
	
	m_vectors_dirty = false;
}
