#ifndef _TRACE_H_
#define _TRACE_H_

#include "types.h"

#include <ext/hash_map>
#include <list>
#include <map>
#include <string>
#include <set>

// Forward Declarations to reduce compile times
class MemlocData;
class Trace;
class Instruction;
class DataType;
class Symbol;
class Comment;


#define XR_TYPE_JMP		0x0
#define XR_TYPE_FNCALL	0x1
#define XR_TYPE_DATA	0x2

#include <stdint.h>

struct comment {
	address_t addr;
	std::string text;
	int lines;
};

class Xref {
private:
	typedef MemlocData xr_mtype;
	
public:
	address_t get_src_addr() const;
	address_t get_dst_addr() const;
	const xr_mtype * get_src_inst() const;
	const xr_mtype * get_dst_inst() const;
	u32 get_type() const;
	
	bool operator==(const Xref & x);
	
protected:
	Xref(Trace * lookup, address_t srcaddr, address_t dstaddr, u32 type);
	friend class Trace;
	
private:
	address_t m_srcaddr;
	address_t m_dstaddr;
	
	u32 m_type;
	
	Trace * m_lookuptrace;
};

typedef std::multimap<address_t, Xref *> xref_map;
typedef xref_map::iterator xref_map_i;
typedef xref_map::const_iterator xref_map_ci;

// Instruction list type
// used to be __gnu_cxx::hash_map
// Change to a map - slightly slower, but we can lookup by lower bound
typedef std::map<address_t, MemlocData * > memloclist_t;
typedef memloclist_t::iterator memloclist_i;
typedef memloclist_t::const_iterator memloclist_ci;

typedef __gnu_cxx::hash_map<address_t, MemlocData * > memlochash_t;
typedef memlochash_t::iterator memlochash_i;
typedef memlochash_t::const_iterator memlochash_ci;

class Architecture {
public:
	virtual ~Architecture() {};
	virtual std::string get_short_name() const = 0; 
	virtual Instruction * create_instruction(Trace * ctx, address_t addr) = 0;
	virtual endian_e getArchEndian(void) = 0;
	// also here we have the create from serializer factory method
};

#include "memsegment.h"
#include "symlist.h"
#include "comment.h"
#include "callback.h"
#include "datatypereg.h"

class Trace
{	
public:
	
	typedef std::set<MemSegment *, MemSegment::less> memseglist_t;
	typedef memseglist_t::const_iterator memseglist_ci;
	
	Trace(Architecture * arch);
	
	~Trace();

	void analyze(address_t start);
	void undefine(address_t start);
	
	void createMemlocDataAt(DataType * d, address_t addr);
	
	Symbol *create_sym(const std::string & name, address_t addr);
	Xref *create_xref(address_t src, address_t dst, u32 type);
	Comment *create_comment(std::string comment, address_t addr);
	
	const Symbol * lookup_symbol(const std::string & symname);
	const Symbol * lookup_symbol(address_t addr);

	Comment *lookup_comment(address_t addr);
	
	MemlocData * lookup_memloc(address_t addr, bool exactmatch=true);
	memloclist_ci memloc_list_begin() const;
	memloclist_ci memloc_list_end() const;
	
	void remove_memloc(address_t addr);
	
	SymbolList::symname_ci sym_begin_name();
	SymbolList::symname_ci sym_end_name();
	SymbolList::symaddr_ci sym_begin_addr();
	SymbolList::symaddr_ci sym_end_addr();
	Symbol * find_ordered_symbol(uint32_t index, SymbolList::symsortorder_e order);
	uint32_t get_symbol_count(void) const;
		
	//bool load_binary(const char * filename, u32 base);
	
	// callbacks.cpp
	void registerMemlocHook(CallbackBase<MemlocData *> *);
	void registerXrefHook(CallbackBase<Xref *> *);
	void registerSymbolHook(CallbackBase<Symbol *> *);

	void unregisterMemlocHook(CallbackBase<MemlocData *> *);
	void unregisterXrefHook(CallbackBase<Xref *> *);
	void unregisterSymbolHook(CallbackBase<Symbol *> *);
	
	bool add_segment(MemSegment * m);
	
	// ramops.cpp
	// Anything larger than a byte needs to be looked at thru the architecture
	// and eventually, we can have more than one arch per file. need to decide how to do this
	// but one global ldw/ldh handler isn't gonna work
	u32 ldw(address_t taddr) __attribute__((deprecated));
	
	// Read a byte from memory
	bool readByte(address_t taddr, uint8_t * data);
	
	// Factor out into a specific algorithm class..
	// this doesn't need internal knowledge of trace
	// code_follow.cpp
	void follow_code_build_insns(address_t start);

	// Factor out into a specific helper algorithm class..
	// doesn't need any internal trace knowledge
	// xref_build.cpp
	void build_xrefs();
	void build_xref_syms();

	void write_file(const char * filename) __attribute((deprecated));
	
	static Trace * load_project_file(const char * filename)__attribute((deprecated));
	
	xref_map_ci xref_from_lower_bound(address_t addr) const;
	xref_map_ci xref_from_upper_bound(address_t addr) const;
	u32 xref_from_count(address_t addr) const;
	
	xref_map_ci xref_to_lower_bound(address_t addr) const;
	xref_map_ci xref_to_upper_bound(address_t addr) const;
	u32 xref_to_count(address_t addr) const;
	
	memseglist_ci memsegs_begin() const;
	memseglist_ci memsegs_end() const;
	
	DataTypeReg::datatypereg_ci getDataTypeBegin() const;
	DataTypeReg::datatypereg_ci getDataTypeEnd() const;
	
	DataType * lookupDataType(std::string name);
	void insertDataType(std::string name, DataType * d);
private:
	/* Private xref types */
	typedef std::list<Xref *> xref_list;
	typedef xref_list::iterator xref_list_i;
	typedef xref_list::const_iterator xref_list_ci;
	
	void insert_memlocd(MemlocData * a);
	
	/* Serialize by name */
	Architecture * m_arch;
	
	/* do not serialize */
	MemSegment * m_last_segment;
	
	/* Needs serializing */
	CommentList m_commentlist;
	
	/* Needs serializing */
	DataTypeReg m_datatypelist;
	
	/* Needs serializing */
	memloclist_t m_memdata;
	
	/* Cache, do not serialize */
	memlochash_t m_memdata_hash;
	
	/* Needs serializing */
	SymbolList m_symlist;
	
	/* Needs serializing */
	memseglist_t m_mem_segments;
	
	/* Master xrefs - serialize */
	xref_list m_xrefs_master;
	
	/* sub xrefs list - do not serialize */
	xref_map m_xrefs_to;
	xref_map m_xrefs_from;

	/* This needs and the memseglist_t need to be pushed to another object */
	bool resolve(address_t, u8, u8*);

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


#ifdef _BIG_ENDIAN
#define endian_swap32( V )           \
( (((V) & 0x000000ff )<<24) | \
  (((V) & 0x0000ff00 )<<8) |  \
  (((V) & 0x00ff0000 )>>8) |  \
  (((V) & 0xff000000 )>>24) )
#define endian_swap16( V )   \
( (((V) & 0xff00) >> 8) | (((V) & 0x00ff) << 8)  )
#else
#define endian_swap32( V ) (V)
#define endian_swap16( V ) (V)
#endif


#define  CPSR_T	0x20
#define  CPSR_F	0x40
#define  CPSR_I	0x80
#define  CPSR_flags	0xf0000000
#define  CPSR_V	0x10000000
#define  CPSR_C	0x20000000
#define  CPSR_Z 0x40000000
#define  CPSR_N	0x80000000
#define  CPSR_Vbits	28
#define  CPSR_Cbits	29
#define  CPSR_Zbits	30
#define  CPSR_Nbits	31
#define SPSR -1

#endif
