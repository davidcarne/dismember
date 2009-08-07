/*
 *  Copyright 2009 David Carne
 *
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

#ifndef _I_PROJECTMODEL_H_
#define _I_PROJECTMODEL_H_


#include <list>
#include <string>
#include <stdint.h>
#include <boost/noncopyable.hpp>


// Forward Declarations to reduce compile times
class I_MemlocData;
class Instruction;
class DataType;
class Symbol;
class Comment;
class Architecture;
class Xref;

#include "callback.h"
#include "datatypereg.h"
#include "xref.h"
#include "memlocmanager.h"
#include "symlist.h"
#include "memsegmentmanager.h"
#include "xrefmanager.h"

class I_ProjectModel : boost::noncopyable
{	
public:
	/*################ Hacks ################*/
	// Todo: This needs to be rethought
	virtual DataType * getCodeDataType() const = 0;
	
#pragma mark Constructor/Destructor
	/*################ Constructor / Destructor ################*/
	virtual ~I_ProjectModel() = 0;
	
#pragma mark Comment Management		
	/*################ Comment Management ################*/
	virtual Comment *create_comment(std::string comment, address_t addr) = 0;
	virtual Comment *lookup_comment(address_t addr) const = 0;
	
#pragma mark Xref Management		
	/*################ Xref Management ################*/
	virtual Xref *create_xref(address_t src, address_t dst, Xref::xref_type_e type) = 0;
	
#pragma mark Memloc Management	
	/*################ Memory location management ################*/
	virtual I_MemlocData * createMemlocDataAt(DataType * d, address_t addr) = 0;
	virtual I_MemlocData * lookup_memloc(address_t addr, bool exactmatch=true) const = 0;
	virtual MemlocManager::memloclist_ci memloc_list_begin() const = 0;
	virtual MemlocManager::memloclist_ci memloc_list_end() const = 0;
	virtual void remove_memloc(address_t addr) = 0;
	
#pragma mark Symbol Management	
	/*################ Symbol Managment ################*/
	virtual Symbol *create_sym(const std::string & name, address_t addr) = 0;
	virtual const Symbol * lookup_symbol(const std::string & symname) const = 0;
	virtual const Symbol * lookup_symbol(address_t addr) const = 0;
	virtual SymbolList::symname_ci sym_begin_name() const = 0;
	virtual SymbolList::symname_ci sym_end_name() const = 0;
	virtual SymbolList::symaddr_ci sym_begin_addr() const = 0;
	virtual SymbolList::symaddr_ci sym_end_addr() const = 0;
	virtual Symbol * find_ordered_symbol(uint32_t index, SymbolList::symsortorder_e order) const = 0;
	virtual uint32_t get_symbol_count(void) const = 0;
	
	
#pragma mark Memsegment Management		
	/*################ Memsegment managment ################*/
	virtual bool addSegment(MemSegment * m) = 0;
	virtual bool readByte(address_t taddr, uint8_t * data) const = 0;
	virtual bool readBytes(address_t, u8, u8*) const = 0;
	virtual MemSegmentManager::memseglist_ci memsegs_begin() const = 0;
	virtual MemSegmentManager::memseglist_ci memsegs_end() const = 0;
	virtual address_t locateAddress(uint64_t address) const = 0;
	
#pragma mark Datatype Management	
	/*################ Datatype management ################*/
	virtual DataTypeReg::datatypereg_ci getDataTypeBegin() const = 0;
	virtual DataTypeReg::datatypereg_ci getDataTypeEnd() const = 0;
	virtual sp_DataType lookupDataType(std::string name) const = 0;
	virtual void addDataType(sp_DataType d) = 0;
	
	
#pragma mark Callback Management
	/*################ Callback management ################*/
	virtual void registerMemlocHook(CallbackBase<I_MemlocData *> *) = 0;
	virtual void registerXrefHook(CallbackBase<Xref *> *) = 0;
	virtual void registerSymbolHook(CallbackBase<Symbol *> *) = 0;
	
	virtual void unregisterMemlocHook(CallbackBase<I_MemlocData *> *) = 0;
	virtual void unregisterXrefHook(CallbackBase<Xref *> *) = 0;
	virtual void unregisterSymbolHook(CallbackBase<Symbol *> *) = 0;
	
};

#endif