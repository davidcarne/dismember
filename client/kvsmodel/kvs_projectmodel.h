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

#ifndef _KVS_PROJECTMODEL_H_
#define _KVS_PROJECTMODEL_H_

#include <list>
#include <string>
#include <stdint.h>
#include <boost/noncopyable.hpp>
#include <boost/serialization/utility.hpp>

#include "i_kvs.h"
#include "i_projectmodel.h"

// Forward Declarations to reduce compile times
class MemlocData;
class MemoryBackedProjectModel;
class DataType;
class Symbol;
class Xref;

#include "kvs_memsegmentmanager.h"

#include "i_projectmodel.h"
#include "types.h"

class KVSBackedProjectModel : public I_ProjectModel, private I_KVS_attribproxy
{	
public:
#pragma mark Constructor/Destructor
	/*################ Constructor / Destructor ################*/
	KVSBackedProjectModel(I_KVS * root_node);
	virtual ~KVSBackedProjectModel();
	
#pragma mark Comment Management		
	/*################ Comment Management ################*/
	virtual I_Comment *create_comment(std::string comment, address_t addr);
	virtual I_Comment *lookup_comment(address_t addr) const;
	
#pragma mark Xref Management		
	/*################ Xref Management ################*/
	virtual Xref *create_xref(address_t src, address_t dst, xref_type_e type);

#pragma mark Memloc Management	
	/*################ Memory location management ################*/
	virtual I_MemlocData * createMemlocDataAt(DataType * d, address_t addr);
	virtual I_MemlocData * lookup_memloc(address_t addr, bool exactmatch=true) const;
	virtual memloc_addr_pair_ci memloc_list_begin() const;
	virtual memloc_addr_pair_ci memloc_list_end() const;
	virtual void remove_memloc(address_t addr);

#pragma mark Symbol Management	
	/*################ Symbol Managment ################*/
	virtual Symbol *create_sym(const std::string & name, address_t addr);
	virtual const Symbol * lookup_symbol(const std::string & symname) const;
	virtual const Symbol * lookup_symbol(address_t addr) const;
	virtual symbol_ci sym_begin_name() const;
	virtual symbol_ci sym_end_name() const;
	virtual symbol_ci sym_begin_addr() const;
	virtual symbol_ci sym_end_addr() const;
	virtual Symbol * find_ordered_symbol(uint32_t index, symsortorder_e order) const;
	virtual uint32_t get_symbol_count(void) const;
	

#pragma mark Memsegment Management		
	/*################ Memsegment managment ################*/
	virtual sp_I_MemSegment addSegment(paddr_t base, psize_t len,  const std::string & name, void * buffer = NULL, psize_t initLength = 0);
	virtual memsegment_ci memsegs_begin() const;
	virtual memsegment_ci memsegs_end() const;
	virtual address_t locateAddress(paddr_t address) const;
	
#pragma mark Datatype Management	
	/*################ Datatype management ################*/
	virtual datatype_ci getDataTypeBegin() const;
	virtual datatype_ci getDataTypeEnd() const;
	virtual sp_DataType lookupDataType(std::string name) const;
	virtual void addDataType(sp_DataType d);
	
	
#pragma mark Callback Management
	/*################ Callback management ################*/
	virtual void registerMemlocHook(CallbackBase<I_MemlocData *> *);
	virtual void registerXrefHook(CallbackBase<Xref *> *);
	virtual void registerSymbolHook(CallbackBase<Symbol *> *);
	
	virtual void unregisterMemlocHook(CallbackBase<I_MemlocData *> *);
	virtual void unregisterXrefHook(CallbackBase<Xref *> *);
	virtual void unregisterSymbolHook(CallbackBase<Symbol *> *);
	
	BRING_IN_KVS_ATTRIBS

	
private:
	KVS_MemSegmentManager m_memsegmentmanager;
	address_t addressFromString(std::string);
	
#pragma mark Internals 
	/*################ Callback Hacks; need to decide how to persist these - could be tricky ################*/
	/* Callback stuff - do not serialize */
	typedef std::list<CallbackBase<I_MemlocData *> *> memloc_hook_list;
	typedef std::list<CallbackBase<Xref *> *> xref_hook_list;
	typedef std::list<CallbackBase<Symbol *> *> symbol_hook_list;
	memloc_hook_list memloc_hooks;
	xref_hook_list   xref_hooks;
	symbol_hook_list symbol_hooks;
	void notifyMemlocChange(I_MemlocData *, HookChange);
	void notifyXrefChange(Xref *, HookChange);
	void notifySymbolChange(Symbol *, HookChange);
};

#endif
