#include "types.h"
#include "trace.h"

#ifndef _MEMLOCDATA_H_
#define _MEMLOCDATA_H_

#define PCFLAG_CONTINUE	0x1

#define PCFLAG_LOCMASK	0x6
// Jump location is determined directly
#define PCFLAG_DIRLOC	0x2
// Jump location is determined indirectly
#define PCFLAG_INDLOC	0x6

#define PCFLAG_DIRLK	0x8

#define PCFLAG_FNENT	0x10

#define PCFLAG_DREF		0x20

#define PCFLAG_DSMASK	0xC0
#define PCFLAG_DSBYTE	0x00
#define PCFLAG_DSSHORT	0x40
#define PCFLAG_DSWORD	0x80
#define PCFLAG_DSQUAD	0xC0

// 0 for read, 1 for write
#define PCFLAG_DRW		0x100

class Trace;

class MemlocData {
public:
	virtual ~MemlocData() {};
	
	// Get the address of the start of this memory location datablock
	virtual address_t	get_addr() const;
	
	// Get the length of this memory location datablock
	virtual u32	get_length() const = 0;
	
	virtual bool logically_continues() const = 0;
	
	virtual bool is_executable() const = 0;
	
	// Functions for xrefs to this datablock
	virtual xref_map_ci begin_xref_to() const;
	virtual xref_map_ci end_xref_to() const;
	virtual u32			 count_xrefs_to() const;
	virtual bool		 has_xrefs_to() const;
	
	// Iterator to list of xrefs "from" this addr, aka, originating here
	virtual xref_map_ci begin_xref_from() const;
	virtual xref_map_ci end_xref_from() const;
	virtual u32			 count_xrefs_from() const;
	virtual bool		 has_xrefs_from() const;
	
	virtual const Symbol * get_symbol() const;
	
	/* The slist requirement will go away later when this returns tokens */
	virtual const std::string get_textual() = 0;
	
	void		mark_explicit(bool mark);
	bool		get_explicit() const;
	const DataType * getCreatingDataType() const;
	
	Trace * get_ctx() const;

	typedef struct {
		std::string text;
		int lines;
	} comment;

	
	MemlocData(const DataType * creator, Trace * ctx, address_t addr);
protected:

	// This goes once we have a trace base class
	friend class Trace;
	
	Trace * m_ctx;
private:	
		
	// If the address has been explicitly defined
	bool m_explicit;
	
	// Common variables
	address_t m_address;

	const DataType * m_creator;
	comment *m_comment;
	
};

class Instruction : public MemlocData {
protected:
	// HACK HACK - Instruction creators should be datatypes
	Instruction(Trace * ctx, address_t addr) : MemlocData(NULL, ctx,  addr) {};
	
public:
	// Replace me with an actual pointer to the function
	virtual void mark_fn_ent(bool fnent) = 0;
	
	virtual address_t get_direct_jump_addr() const = 0;
	
	virtual address_t get_data_ref_addr() const = 0;
	
	// I haven't decided how to replace this yet, but its poor form
	virtual u32	DEPRECATED(get_pcflags() const) = 0;
	
	virtual bool logically_continues() const {
		return get_pcflags() & PCFLAG_CONTINUE;
	}
};

#endif

