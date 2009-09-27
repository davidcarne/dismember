/*
 *  Copyright 2009 David Carne
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

#ifndef _I_MEMLOCDATA_H_
#define _I_MEMLOCDATA_H_

class Symbol;
class address_t;
class DataType;
class I_ProjectModel;

#include "types.h"
#include "xrefmanager.h"

/**
 * \brief I_MemlocData is the interface to the a class that represents the data that the I_ProjectModel
 *			has on a location specified by an address_t
 *
 * Each analyzed memory location is associated with an instantiation of a subclass of MemlocData.
 * I_MemlocData is an interface and cannot be used directly
 */
class I_MemlocData {
public:
	virtual ~I_MemlocData() = 0;
	
	/**
	 * \brief Get the address of the start of this memory location information
	 * @return the start address of this data
	 */
	virtual address_t	get_addr() const = 0;
	
	/**
	 * \brief get a pointer to the previous contiguous I_MemlocData
	 * @return a pointer to the previous I_MemlocData, or NULL if none could be found
	 */
	virtual I_MemlocData *  getPreviousContiguous() = 0;
	
	/**
	 * \brief get a pointer to the next contiguous I_MemlocData
	 * @return a pointer to the next I_MemlocData, or NULL if none could be found
	 */
	virtual I_MemlocData *  getNextContiguous() = 0;
	
	/** 
	 * \brief Get the length of this memory location datablock
	 * @return the length of this memory location
	 */
	virtual u32	get_length() const = 0;
	
	/**
	 * @return whether the memloc continues to the next memlocdata [see getNextContiguous() ]
	 */
	virtual bool logically_continues() const = 0;
	
	/**
	 * @return if the memlocdata is executable
	 */
	virtual bool is_executable() const = 0;
	
	/**
	 * @return begin iterator of the xrefs to this memory location
	 */
	virtual XrefManager::xref_map_ci begin_xref_to() const = 0;
	
	/**
	 * @return end iterator of the xrefs to this memory location
	 */
	virtual XrefManager::xref_map_ci end_xref_to() const = 0;
	
	/**
	 * @return count of the xrefs to this memory location
	 */
	virtual u32			 count_xrefs_to() const = 0;
	
	/**
	 * @return if there are any xrefs to this memory location [may be faster than count_ ]
	 */
	virtual bool		 has_xrefs_to() const = 0;
	
	/**
	 * @return begin iterator of the xrefs from this memory location
	 */
	virtual XrefManager::xref_map_ci begin_xref_from() const = 0;
	
	/**
	 * @return end iterator of the xrefs from this memory location
	 */
	virtual XrefManager::xref_map_ci end_xref_from() const = 0;
	
	/**
	 * @return count of the xrefs from this memory location
	 */
	virtual u32			 count_xrefs_from() const = 0;
	
	/**
	 * @return if there are any xrefs from this memory location [may be faster than count_ ]
	 */
	virtual bool		 has_xrefs_from() const = 0;
	
	/**
	 * @return Pointer to the symbol of this memory location, NULL if none
	 */
	virtual const Symbol * get_symbol() const = 0;
	
	/**
	 *	@return a textual representation of this memory data
	 */
	virtual const std::string get_textual() = 0;
	
	/**
	 *	\brief mark this memory loc data as explicitly defined, aka, not autogenerated. This is so autogenerated data
	 * can be erased when the original creator is gone
	 */
	virtual void		mark_explicit(bool mark) = 0;
	
	/**
	 * @return if this memory loc was created explicitly
	 */
	virtual bool		get_explicit() const = 0;
	
	/**
	 * @return the DataType that this memory location is. may return null now, but not in the future
	 */
	virtual const DataType * getCreatingDataType() const = 0;
	
	/**
	 * @return the trace that this ctx belongs to
	 */
	virtual const I_ProjectModel * get_ctx() const = 0;
};

#endif
