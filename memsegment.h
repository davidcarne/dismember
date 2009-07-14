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
	 * \brief Initialize a new memory segment.
	 * @param base base address of the new memory segment
	 * @param len length of the new memory segment
	 * @param buffer data buffer to source contents from. May be released after buffer is created
	 * @param initLength length of the initialized data for the segment [ie, for a data segment]
	 */
	MemSegment(uint64_t base, uint64_t len, void * buffer = NULL, uint64_t initLength = -1);
	
	/** Destructor for a memory segment */
	~MemSegment();
	
	/** @return the start address of the memory segment */
	uint64_t get_start() const;

	address_t getBaseAddress() const;
	
	/** @return the length of the memory segment */
	uint64_t get_length() const;	
	
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
	bool get_bytes(address_t addr, uint64_t length, void * dest) const;
	
	/**
	 * @param addr address to check
	 * @return is addr valid within this block
	 */
	bool can_resolve(uint64_t addr) const;

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
		bool operator()(MemSegment *a, MemSegment *b) const
		{ return a->get_start() < b->get_start(); }
	};

private:
	
	uint64_t m_base;
	uint64_t m_len;
	uint8_t * m_data;
};

#endif

