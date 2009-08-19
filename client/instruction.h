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

// Shell of a class - we don't need an instruction subtype right now.
class InstructionFlags {
private:
	InstructionFlags() {}
	
public:
	typedef u32 pcflags_type;
	
	/**
	 *	\brief Flag bit indicating if the Instruction continues on to the next contiguous
	 *  \sa I_MemlocData::getNextContiguous()
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
};

#endif


