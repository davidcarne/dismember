
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

#ifndef _I_MEMSEGMENT_H_
#define _I_MEMSEGMENT_H_
#include <stdio.h>
#include <stdint.h>
#include "address.h"
#include "types.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

/**
 * \brief Represents a memory segment, optionally backed by a stretch of data
 */
class I_MemSegment {
public:
	/** @return the start address of the memory segment */
	virtual paddr_t get_start() const = 0;

	virtual address_t getBaseAddress() const = 0;
	
	/** @return the length of the memory segment */
	virtual psize_t get_length() const = 0;	
	
	/**
	 * \brief retrieve length bytes from the memory segment into the dest buffer
	 * get_bytes attempts to retrieve length bytes from the memory segment and copy them into the
	 * destination buffer. If the destination buffer is NULL, get_bytes simply checks to see if the copy
	 * would have succeeded.
	 *
	 * @param addr virtual address [not an offset] of the data to retrieve
	 * @param length number of bytes to copy
	 * @param dest destination buffer. May be null.
	 * @return if the copy succeeded, or would have succeeded had dest been non-null
	 */
	virtual bool get_bytes(const address_t & addr, psize_t length, void * dest) const = 0;
	
	/** 
	 * @return the name of the memory section
	 */
	virtual const std::string & getName() const = 0;
	
	/**
	 * @param addr address to check
	 * @return is addr valid within this block
	 */
	virtual bool can_resolve(const paddr_t & addr) const = 0;
	
	/**
	 * @return if the memory block has data, or if its virtual.
	 */
	virtual bool is_defined() const = 0;

	virtual ~I_MemSegment() {};
};

typedef boost::shared_ptr<I_MemSegment> sp_I_MemSegment;
typedef boost::shared_ptr<const I_MemSegment> sp_cI_MemSegment;
typedef boost::weak_ptr<I_MemSegment> wp_I_MemSegment;
typedef boost::weak_ptr<const I_MemSegment> wp_cI_MemSegment;

#endif

