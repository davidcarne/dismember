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

#ifndef _xrefmanager_H_
#define _xrefmanager_H_
#include "xref.h"
#include <map>
#include <list>

class XrefManager {
public:
	
	XrefManager(I_ProjectModel * ctx);
	
	Xref * createXref(address_t srcaddr, address_t destaddr, Xref::xref_type_e type);
	
	typedef std::multimap<address_t, Xref *, address_t::less> xref_map;
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
	
	I_ProjectModel * m_ctx;
};


#endif


