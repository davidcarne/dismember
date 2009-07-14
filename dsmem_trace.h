#ifndef _TRACE_H_
#define _TRACE_H_

#include <list>
#include <string>
#include <stdint.h>
#include <boost/noncopyable.hpp>
#include <boost/serialization/utility.hpp>

// Forward Declarations to reduce compile times
class MemlocData;
class Trace;
class Instruction;
class DataType;
class Symbol;
class Comment;
class Architecture;
class Xref;

#include "types.h"
#include "memsegment.h"
#include "symlist.h"
#include "comment.h"
#include "callback.h"
#include "datatypereg.h"
#include "xrefmanager.h"
#include "memlocmanager.h"
#include "memsegmentmanager.h"

class Trace : boost::noncopyable
{	
public:
#pragma mark Hacks	
	/*################ Hacks ################*/
	// Todo: This needs to be rethought
	DataType * getCodeDataType();
	
#pragma mark Constructor/Destructor
	/*################ Constructor / Destructor ################*/
	Trace(Architecture * arch);
	~Trace();
	
#pragma mark Comment Management		
	/*################ Comment Management ################*/
	Comment *create_comment(std::string comment, address_t addr);
	Comment *lookup_comment(address_t addr) const;
	
#pragma mark Xref Management		
	/*################ Xref Management ################*/
	Xref *create_xref(address_t src, address_t dst, Xref::xref_type_e type);

#pragma mark Memloc Management	
	/*################ Memory location management ################*/
	MemlocData * createMemlocDataAt(DataType * d, address_t addr);
	MemlocData * lookup_memloc(address_t addr, bool exactmatch=true) const;
	MemlocManager::memloclist_ci memloc_list_begin() const;
	MemlocManager::memloclist_ci memloc_list_end() const;
	void remove_memloc(address_t addr);

#pragma mark Symbol Management	
	/*################ Symbol Managment ################*/
	Symbol *create_sym(const std::string & name, address_t addr);
	const Symbol * lookup_symbol(const std::string & symname) const;
	const Symbol * lookup_symbol(address_t addr) const;
	SymbolList::symname_ci sym_begin_name() const;
	SymbolList::symname_ci sym_end_name() const;
	SymbolList::symaddr_ci sym_begin_addr() const;
	SymbolList::symaddr_ci sym_end_addr() const;
	Symbol * find_ordered_symbol(uint32_t index, SymbolList::symsortorder_e order) const;
	uint32_t get_symbol_count(void) const;
	

#pragma mark Memsegment Management		
	/*################ Memsegment managment ################*/
	bool addSegment(MemSegment * m);
	bool readByte(address_t taddr, uint8_t * data) const;
	bool readBytes(address_t, u8, u8*) const;
	MemSegmentManager::memseglist_ci memsegs_begin() const;
	MemSegmentManager::memseglist_ci memsegs_end() const;
	address_t locateAddress(uint64_t address) const;
	
#pragma mark Datatype Management	
	/*################ Datatype management ################*/
	DataTypeReg::datatypereg_ci getDataTypeBegin() const;
	DataTypeReg::datatypereg_ci getDataTypeEnd() const;
	sp_DataType lookupDataType(std::string name) const;
	void addDataType(sp_DataType d);
	
	
#pragma mark Callback Management
	/*################ Callback management ################*/
	void registerMemlocHook(CallbackBase<MemlocData *> *);
	void registerXrefHook(CallbackBase<Xref *> *);
	void registerSymbolHook(CallbackBase<Symbol *> *);
	
	void unregisterMemlocHook(CallbackBase<MemlocData *> *);
	void unregisterXrefHook(CallbackBase<Xref *> *);
	void unregisterSymbolHook(CallbackBase<Symbol *> *);
	
	
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
	typedef std::list<CallbackBase<MemlocData *> *> memloc_hook_list;
	typedef std::list<CallbackBase<Xref *> *> xref_hook_list;
	typedef std::list<CallbackBase<Symbol *> *> symbol_hook_list;
	memloc_hook_list memloc_hooks;
	xref_hook_list   xref_hooks;
	symbol_hook_list symbol_hooks;
	void notifyMemlocChange(MemlocData *, HookChange);
	void notifyXrefChange(Xref *, HookChange);
	void notifySymbolChange(Symbol *, HookChange);
};

#endif
