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

#ifndef _instruction_H_
#define _instruction_H_
#include "memlocdata.h"


/** 
 * \brief Virtual subclass of memlocdata for dealing with executable memory locations
 */
class Instruction : public MemlocData {
protected:
	
	/**
	 * \brief Create a new Instruction
	 * \todo Need to pass the datatype
	 */
	Instruction(const I_ProjectModel * ctx, address_t addr, u32 length);
	
public:
	/**
	 * \brief Type of pcflags bitfield
	 */
	typedef u32 pcflags_type;
	
	/**
	 *	\brief Flag bit indicating if the Instruction continues on to the next contiguous
	 *  \sa MemlocData::getNextContiguous()
	 */
	static const pcflags_type PCFLAG_CONTINUE	= 0x1;
	
	/**
	 * \brief Mask for the pcflags jump target
	 */
	static const pcflags_type PCFLAG_LOCMASK	= 0x6;
	
	/**
	 * \brief PCflags bit indicating jump target is direct
	 * \sa PCFLAG_LOCMASK
	 */
	static const pcflags_type PCFLAG_DIRLOC		= 0x2;
	
	/**
	 * \brief PCflags bit indicating jump target is indirect
	 * \sa PCFLAG_LOCMASK
	 */
	static const pcflags_type PCFLAG_INDLOC		= 0x6;
	
	/**
	 * \brief PCflags bit indicating that its a direct jump to another function
	 */
	static const pcflags_type PCFLAG_DIRLK		= 0x8;
	
	/**
	 * \brief PCflags bit indicating that this is a function entry
	 */
	static const pcflags_type PCFLAG_FNENT		= 0x10;
	
	/**
	 * \brief PCflags bit indicating that this instruction has a data reference
	 */
	static const pcflags_type PCFLAG_DREF		= 0x20;
	
	/**
	 * \brief PCflags mask for the datatype of a possible data reference
	 */
	static const pcflags_type PCFLAG_DSMASK		= 0x1C0;
	
	/**
	 * \brief PCflags bits indicating possible data reference is a byte
	 */
	static const pcflags_type PCFLAG_DSBYTE		= 0x000;
	
	/**
	 * \brief PCflags bits indicating possible data reference is a 16 bit integer
	 */
	static const pcflags_type PCFLAG_DSSHORT	= 0x040;
	
	/**
	 * \brief PCflags bits indicating possible data reference is a 32 bit integer
	 */
	static const pcflags_type PCFLAG_DSWORD		= 0x080;
	
	/**
	 * \brief PCflags bits indicating possible data reference is a 64 bit integer
	 */
	static const pcflags_type PCFLAG_DSQUAD		= 0x0C0;
	
	/**
	 * \brief PCflags bits indicating possible data reference is a calculated address
	 */
	static const pcflags_type PCFLAG_DCALC		= 0x100;
	
	/**
	 * \brief PCflags bit indicating possible data reference read/write direction
	 */
	static const pcflags_type PCFLAG_DRW		= 0x200;
	
	/**
	 * \brief Mark this memory location as being the entrance to a function
	 * \todo this should take the actual function block
	 */
	virtual void mark_fn_ent(bool fnent) = 0;
	
	/**
	 * \brief the location in memory that this can directly jump to
	 * \sa get_pcflags
	 * \deprecated this needs to be better represented
	 */
	virtual address_t DEPRECATED(get_direct_jump_addr() const) = 0;
	
	/**
	 * \brief get the address of data this function references
	 * \sa get_pcflags
	 * \deprecated this needs to be better represented
	 */
	virtual address_t DEPRECATED(get_data_ref_addr() const) = 0;
	
	/**
	 * \brief get flags about the execution flow of this memory location
	 * \deprecated this needs to be better represented
	 */
	virtual pcflags_type DEPRECATED(get_pcflags() const) = 0;
	
	virtual bool logically_continues() const;
};

#endif


