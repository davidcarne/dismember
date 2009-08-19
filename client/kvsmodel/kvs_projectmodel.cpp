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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdexcept>
#include <errno.h>
#include <assert.h>
#include <stack>

#include "kvs_projectmodel.h"
#include "kvs_memsegment.h"


address_t KVSBackedProjectModel::addressFromString(std::string k)
{/*
	std::string segname = k.substr(0, k.find("!"));
	std::string addr = k.substr(k.find("!"), k.size());
	int addri = atoi(addr.c_str());
	
	for (memsegment_ci it = m_memsegmentmanager.memsegs_begin(); it != m_memsegmentmanager.memsegs_end(); it++)
	{
		if ((*it)->getName() == segname)
			return (*it)->getBaseAddress() + addri;
	}
	return address_t();*/
}

KVSBackedProjectModel::KVSBackedProjectModel(I_KVS * root_node) : 
	I_KVS_attribproxy(root_node->getPathNode("/")->getAttributesReference())
{}

sp_I_MemSegment KVSBackedProjectModel::addSegment(paddr_t base, psize_t len,  const std::string & name, void * buffer, psize_t initLength)
{
	sp_KVS_MemSegment seg =  KVS_MemSegment::createKVS_MemSegment(base, len, buffer, initLength, name);
	if (m_memsegmentmanager.addSegment(seg))
		return seg;
	return sp_KVS_MemSegment();
}



memsegment_ci KVSBackedProjectModel::memsegs_begin() const
{
	memsegment_ci i;
	i = m_memsegmentmanager.memsegs_begin();
	return i;
}

memsegment_ci KVSBackedProjectModel::memsegs_end() const
{
	memsegment_ci i;
	i = m_memsegmentmanager.memsegs_end();
	return i;
}

address_t KVSBackedProjectModel::locateAddress(paddr_t address) const
{
	return m_memsegmentmanager.locateAddress(address);
}

I_MemlocData * KVSBackedProjectModel::createMemlocDataAt(DataType * d, address_t addr)
{
}

KVSBackedProjectModel::~KVSBackedProjectModel()
{
}

/* Symbol accessors */
symbol_ci KVSBackedProjectModel::sym_begin_name() const
{
}

symbol_ci KVSBackedProjectModel::sym_end_name() const
{
}

symbol_ci KVSBackedProjectModel::sym_begin_addr() const
{
}

symbol_ci KVSBackedProjectModel::sym_end_addr() const
{
}

Symbol * KVSBackedProjectModel::find_ordered_symbol(uint32_t index, symsortorder_e order) const
{
}

uint32_t KVSBackedProjectModel::get_symbol_count(void) const
{
}

const Symbol * KVSBackedProjectModel::lookup_symbol(const std::string & symname) const
{
}

const Symbol * KVSBackedProjectModel::lookup_symbol(address_t addr) const
{
}


I_MemlocData * KVSBackedProjectModel::lookup_memloc(address_t addr, bool exactmatch) const
{	
}

void KVSBackedProjectModel::remove_memloc(address_t addr)
{
}

memloc_addr_pair_ci KVSBackedProjectModel::memloc_list_begin() const
{
}

memloc_addr_pair_ci KVSBackedProjectModel::memloc_list_end() const
{
}

I_Comment *KVSBackedProjectModel::create_comment(std::string cmt, address_t addr)
{
}

I_Comment *KVSBackedProjectModel::lookup_comment(address_t addr) const
{
}

Symbol *KVSBackedProjectModel::create_sym(const std::string & name, address_t addr)
{
}


datatype_ci KVSBackedProjectModel::getDataTypeBegin() const
{
}

datatype_ci KVSBackedProjectModel::getDataTypeEnd() const
{
}
	
sp_DataType KVSBackedProjectModel::lookupDataType(std::string name) const
{
}

void KVSBackedProjectModel::addDataType(sp_DataType d)
{
}


// Xrefs should be an analysis pass!
Xref *KVSBackedProjectModel::create_xref(address_t srcaddr, address_t dstaddr, xref_type_e type)
{
}


void KVSBackedProjectModel::registerMemlocHook(CallbackBase<I_MemlocData *> *cb)
{ memloc_hooks.push_back(cb); }
void KVSBackedProjectModel::registerXrefHook(CallbackBase<Xref *> *cb)
{ xref_hooks.push_back(cb); }
void KVSBackedProjectModel::registerSymbolHook(CallbackBase<Symbol *> *cb)
{ symbol_hooks.push_back(cb); }

void KVSBackedProjectModel::unregisterMemlocHook(CallbackBase<I_MemlocData *> *cb)
{ memloc_hooks.remove(cb); }
void KVSBackedProjectModel::unregisterXrefHook(CallbackBase<Xref *> *cb)
{ xref_hooks.remove(cb); }
void KVSBackedProjectModel::unregisterSymbolHook(CallbackBase<Symbol *> *cb)
{ symbol_hooks.remove(cb); }


#define NOTIFY_MACRO(a,b,c) \
a##_hook_list::iterator i = a##_hooks.begin(); \
for (; i != a##_hooks.end(); ++i) \
(*(i))->callback(b, c);

void KVSBackedProjectModel::notifyMemlocChange(I_MemlocData *data, HookChange ch)
{ NOTIFY_MACRO(memloc, data, ch) }
void KVSBackedProjectModel::notifyXrefChange(Xref *data, HookChange ch)
{ NOTIFY_MACRO(xref, data, ch) }
void KVSBackedProjectModel::notifySymbolChange(Symbol *data, HookChange ch)
{ NOTIFY_MACRO(symbol, data, ch) }

