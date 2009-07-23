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

#ifndef _MemoryBackedProjectModel_H_
#define _MemoryBackedProjectModel_H_

#include <list>
#include <string>
#include <stdint.h>
#include <boost/noncopyable.hpp>
#include <boost/serialization/utility.hpp>

// Forward Declarations to reduce compile times
class MemlocData;
class MemoryBackedProjectModel;
class Instruction;
class DataType;
class Symbol;
class Comment;
class Architecture;
class Xref;

#include "i_projectmodel.h"
#include "types.h"
#include "memsegment.h"
#include "symlist.h"
#include "comment.h"
#include "callback.h"
#include "datatypereg.h"
#include "xrefmanager.h"
#include "memlocmanager.h"
#include "memsegmentmanager.h"

class MemoryBackedProjectModel : public I_ProjectModel
{	
public:
#pragma mark Hacks	
	/*################ Hacks ################*/
	// Todo: This needs to be rethought
	virtual DataType * getCodeDataType() const;
	
#pragma mark Constructor/Destructor
	/*################ Constructor / Destructor ################*/
	MemoryBackedProjectModel(Architecture * arch);
	virtual ~MemoryBackedProjectModel();
	
#pragma mark Comment Management		
	/*################ Comment Management ################*/
	virtual Comment *create_comment(std::string comment, address_t addr);
	virtual Comment *lookup_comment(address_t addr) const;
	
#pragma mark Xref Management		
	/*################ Xref Management ################*/
	virtual Xref *create_xref(address_t src, address_t dst, Xref::xref_type_e type);

#pragma mark Memloc Management	
	/*################ Memory location management ################*/
	virtual I_MemlocData * createMemlocDataAt(DataType * d, address_t addr);
	virtual I_MemlocData * lookup_memloc(address_t addr, bool exactmatch=true) const;
	virtual MemlocManager::memloclist_ci memloc_list_begin() const;
	virtual MemlocManager::memloclist_ci memloc_list_end() const;
	virtual void remove_memloc(address_t addr);

#pragma mark Symbol Management	
	/*################ Symbol Managment ################*/
	virtual Symbol *create_sym(const std::string & name, address_t addr);
	virtual const Symbol * lookup_symbol(const std::string & symname) const;
	virtual const Symbol * lookup_symbol(address_t addr) const;
	virtual SymbolList::symname_ci sym_begin_name() const;
	virtual SymbolList::symname_ci sym_end_name() const;
	virtual SymbolList::symaddr_ci sym_begin_addr() const;
	virtual SymbolList::symaddr_ci sym_end_addr() const;
	virtual Symbol * find_ordered_symbol(uint32_t index, SymbolList::symsortorder_e order) const;
	virtual uint32_t get_symbol_count(void) const;
	

#pragma mark Memsegment Management		
	/*################ Memsegment managment ################*/
	virtual bool addSegment(MemSegment * m);
	virtual bool readByte(address_t taddr, uint8_t * data) const;
	virtual bool readBytes(address_t, u8, u8*) const;
	virtual MemSegmentManager::memseglist_ci memsegs_begin() const;
	virtual MemSegmentManager::memseglist_ci memsegs_end() const;
	virtual address_t locateAddress(uint64_t address) const;
	
#pragma mark Datatype Management	
	/*################ Datatype management ################*/
	virtual DataTypeReg::datatypereg_ci getDataTypeBegin() const;
	virtual DataTypeReg::datatypereg_ci getDataTypeEnd() const;
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
	
	
protected:

	
private:
#pragma mark Serialization 
	/*################ Serialization Enablers ################*/
	
	template<class Archive> void serialize(Archive & ar, const unsigned int)
	{
		//ar & m_arch;
		//ar & m_commentlist;
		//ar & m_datatypelist;
		//ar & m_memlocmanager;
		//ar & m_symlist;
		//ar & m_xrefmanager;
		//ar & m_memsegmentmanager;
	}
	
	friend class boost::serialization::access;
	friend class MemlocData;
	
#pragma mark Internals 
	/*################ Sub Managers ################*/
	/* Serialize by name */
	Architecture * m_arch;

	/* Needs serializing */
	CommentList m_commentlist;
	
	/* Needs serializing */
	DataTypeReg m_datatypelist;
	
	/* Needs serializing */
	MemlocManager m_memlocmanager;
	
	/* Needs serializing */
	SymbolList m_symlist;

	/* Needs serializing */
	XrefManager m_xrefmanager;
	
	/* Needs serializing */
	MemSegmentManager m_memsegmentmanager;
	
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
