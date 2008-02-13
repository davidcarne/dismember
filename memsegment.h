#include <stdint.h>
#include "types.h"

#ifndef _MEMSEGMENT_H_
#define _MEMSEGMENT_H_

class MemSegment {
public:
	MemSegment(address_t base, address_t len, void * buffer, address_t flength = -1);
	~MemSegment();
	
	address_t get_start() const;
	address_t get_length() const;	
	bool get_bytes(address_t addr, address_t length, void * dest) const;
	
	bool can_resolve(address_t addr) const;
	bool is_defined() const;
	
	// DON'T USE ME - only here until traceloadsave is refactored
	const uint8_t * data_ptr() const;

	struct less {
		bool operator()(MemSegment *a, MemSegment *b) const
		{ return a->get_start() < b->get_start(); }
	};

private:
	address_t m_base;
	address_t m_len;
	uint8_t * m_data;
};

#endif

