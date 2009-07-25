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

#ifndef _MEMSEGMENT_H_
#define _MEMSEGMENT_H_
#include <stdio.h>
#include <stdint.h>
#include "address.h"
#include "types.h"

/**
 * \brief Represents a memory segment, optionally backed by a stretch of data
 */
class MemSegment {
public:
	/**
	 * \brief Create a new fully initialized memory segment.
	 * @param base base address of the new memory segment
	 * @param len length of the new memory segment
	 * @param buffer data buffer to source contents from. May be released after buffer is created
	 * @param name name of memory segment [eg: RAM, ROM, BSS, TXT]
	 */
	MemSegment(paddr_t base, psize_t len, void * buffer, const std::string & name = "");

	/**
	 * \brief Create a new memory segment with specified initialized length.
	 * @param base base address of the new memory segment
	 * @param len length of the new memory segment
	 * @param buffer data buffer to source contents from. May be released after buffer is created
	 * @param initLength length of the initialized data for the segment [ie, for a data segment]
	 * @param name name of memory segment [eg: RAM, ROM, BSS, TXT]
	 */
	MemSegment(paddr_t base, psize_t len, void * buffer, psize_t initLength, const std::string & name = "");

	/**
	 * \brief Create a new uninitialized memory segment.
	 * @param base base address of the new memory segment
	 * @param len length of the new memory segment
	 * @param name name of memory segment [eg: RAM, ROM, BSS, TXT]
	 */
	MemSegment(paddr_t base, psize_t len, const std::string & name = "");
	
	/** Destructor for a memory segment */
	~MemSegment();
	
	/** @return the start address of the memory segment */
	paddr_t get_start() const;

	address_t getBaseAddress() const;
	
	/** @return the length of the memory segment */
	psize_t get_length() const;	
	
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
	bool get_bytes(address_t addr, psize_t length, void * dest) const;
	
	/** 
	 * @return the name of the memory section
	 */
	const std::string & getName() const;
	
	/**
	 * @param addr address to check
	 * @return is addr valid within this block
	 */
	bool can_resolve(paddr_t addr) const;

	/**
	 * @param addr address to check
	 * @return is addr valid within this block
	 */
	bool can_resolve(address_t addr) const;
	
	/**
	 * @return if the memory block has data, or if its virtual.
	 */
	bool is_defined() const;
	
	/**
	 * \deprecated this function will go away after the trace load/save refactoring
	 * @return a pointer to the data inside the datablock.
	 */
	
	const uint8_t * DEPRECATED(data_ptr() const);

	/** Less comparison functor for MemSegment */
	struct less {
		/** Less comparison operator for MemSegment */
		bool operator()(MemSegment *a, MemSegment *b) const;
	};

private:
	void init(void *data, psize_t initLength);
	
	paddr_t m_base;
	psize_t m_len;
	uint8_t * m_data;
	std::string m_name;
};

#endif

