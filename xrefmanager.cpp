/*
 *  xrefmanager.cpp
 *  dismember
 *
 *  Created by David Carne on 05/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "xrefmanager.h"

XrefManager::XrefManager(ProjectModel * ctx) : m_ctx(ctx)
{
	
}

XrefManager::xref_map_ci XrefManager::xref_from_lower_bound(address_t addr) const
{
	return m_xrefs_from.lower_bound(addr);
}

XrefManager::xref_map_ci XrefManager::xref_from_upper_bound(address_t addr) const
{
	return m_xrefs_from.upper_bound(addr);
}

u32 XrefManager::xref_from_count(address_t addr) const
{
	return m_xrefs_from.count(addr);
}


XrefManager::xref_map_ci XrefManager::xref_to_lower_bound(address_t addr) const
{
	return m_xrefs_to.lower_bound(addr);
}

XrefManager::xref_map_ci XrefManager::xref_to_upper_bound(address_t addr) const
{
	return m_xrefs_to.upper_bound(addr);
}

u32 XrefManager::xref_to_count(address_t addr) const
{
	return m_xrefs_to.count(addr);
}

Xref * XrefManager::createXref(address_t srcaddr, address_t destaddr, Xref::xref_type_e type)
{
	
	// Check if we already have an xref pointing to the destination
	for (xref_map_ci i = xref_from_lower_bound(srcaddr); i!= xref_from_upper_bound(srcaddr); i++)
		if ((*i).second->get_dst_addr() == destaddr)
			return (*i).second;
	
	Xref * sx = new Xref(m_ctx, srcaddr, destaddr, type);
	
	// Insert into the master xref list
	m_xrefs_master.push_back(sx);
	
	// Add to the to/from caches
	m_xrefs_to.insert(std::pair<const address_t, Xref*>(destaddr, sx));
	m_xrefs_from.insert(std::pair<const address_t, Xref*>(srcaddr, sx));
	return sx;
}
