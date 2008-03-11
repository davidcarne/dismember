/*
 *  xrefmanager.h
 *  dismember
 *
 *  Created by David Carne on 05/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _xrefmanager_H_
#define _xrefmanager_H_
#include "xref.h"
#include <map>
#include <list>

class XrefManager {
public:
	
	XrefManager(Trace * ctx);
	
	Xref * createXref(address_t srcaddr, address_t destaddr, Xref::xref_type_e type);
	
	typedef std::multimap<address_t, Xref *> xref_map;
	typedef xref_map::iterator xref_map_i;
	typedef xref_map::const_iterator xref_map_ci;
	
	xref_map_ci xref_from_lower_bound(address_t addr) const;
	xref_map_ci xref_from_upper_bound(address_t addr) const;
	u32 xref_from_count(address_t addr) const;
	
	xref_map_ci xref_to_lower_bound(address_t addr) const;
	xref_map_ci xref_to_upper_bound(address_t addr) const;
	u32 xref_to_count(address_t addr) const;
	
private:
	/* Private xref types */
	typedef std::list<Xref *> xref_list;
	typedef xref_list::iterator xref_list_i;
	typedef xref_list::const_iterator xref_list_ci;
	
	/* Master xrefs - serialize */
	xref_list m_xrefs_master;
	
	/* sub xrefs list - do not serialize */
	xref_map m_xrefs_to;
	xref_map m_xrefs_from;
	
	Trace * m_ctx;
};


#endif


