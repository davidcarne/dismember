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

#ifndef _KVS_MEMSEGMENT_H_
#define _KVS_MEMSEGMENT_H_
#include <stdio.h>
#include <stdint.h>
#include "address.h"
#include "types.h"
#include "i_memsegment.h"
#include "i_kvs.h"
#include "kvs_proxies.h"

class KVS_MemSegment;

typedef boost::shared_ptr<KVS_MemSegment> sp_KVS_MemSegment;
typedef boost::weak_ptr<KVS_MemSegment> wp_KVS_MemSegment;

/**
 * \brief Represents a memory segment, optionally backed by a stretch of data
 */
class KVS_MemSegment : public I_MemSegment, private I_KVS_attribproxy{
public:
	static sp_KVS_MemSegment createKVS_MemSegment(paddr_t base, psize_t len, void * buffer, psize_t initLength, const std::string & name = "");
	
	static sp_KVS_MemSegment createKVS_MemSegment(sp_I_KVS_attributes attribs);
	
	/** Destructor for a memory segment */
	~KVS_MemSegment();
	
	/** @return the start address of the memory segment */
	virtual paddr_t get_start() const;

	virtual address_t getBaseAddress() const;
	
	/** @return the length of the memory segment */
	virtual psize_t get_length() const;	
	
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
	virtual bool get_bytes(const address_t & addr, psize_t length, void * dest) const ;
	
	/** 
	 * @return the name of the memory section
	 */
	virtual const std::string & getName() const;
	
	/**
	 * @param addr address to check
	 * @return is addr valid within this block
	 */
	virtual bool can_resolve(const paddr_t & addr) const;

	
	/**
	 * @return if the memory block has data, or if its virtual.
	 */
	bool is_defined() const;

	/** Less comparison functor for MemSegment */
	struct less {
		/** Less comparison operator for MemSegment */
		bool operator()(sp_KVS_MemSegment a, sp_KVS_MemSegment b) const;
	};

private:
	
	/**
	 * \brief Create a new memory segment with specified initialized length.
	 * @param base base address of the new memory segment
	 * @param len length of the new memory segment
	 * @param buffer data buffer to source contents from. May be released after buffer is created
	 * @param initLength length of the initialized data for the segment [ie, for a data segment]
	 * @param name name of memory segment [eg: RAM, ROM, BSS, TXT]
	 */
	KVS_MemSegment(sp_I_KVS_attributes noderef, paddr_t base, psize_t len, void * buffer, psize_t initLength, const std::string & name = "");

	KVS_MemSegment(sp_I_KVS_attributes noderef);

	wp_KVS_MemSegment m_me;
	
	void init(void *data, psize_t initLength);
	
	paddr_t m_base;
	psize_t m_len;
	uint8_t * m_data;
	std::string m_name;
};

#endif

