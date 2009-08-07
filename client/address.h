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

#ifndef _ADDRESS_H_
#define _ADDRESS_H_
#include <string>
#include <stdint.h>
#include "types.h"

class MemSegment;

class address_t
{
 public:
	/**
	 * Construct invalid address.
	 */
 	address_t();

	/**
	 * Construct copy of address.
	 */
 	address_t(const address_t &c);

	/**
	 * Get the actual value associated with this address.
	 */
	paddr_t getValue() const;

	/**
	 * Get the offset value associated with this address.
	 */
	paddr_t getOffset() const;

	/**
	 * Set the value associated with this address.
	 * This is not the function you want.
	 */
	void setValue(paddr_t val);

	/**
	 * Returns whether we're valid and associated with a memory segment.
	 */
	bool isValid() const;

	/**
	 * Returns whether we're associated to the same memory segment.
	 */
	bool comparableTo(const address_t &addr) const;

	/**
	 * Convert address to printable string, obeying size.
	 */
	std::string toString() const;

	/**
	 * Get bytes at address
	 */
	bool readBytes(u8 bytes, u8 * buf) const;
	
	/*
	 * Get byte at address
	 * TODO: byte != 8 bits in all cases
	 */
	bool readByte(uint8_t * data) const;
	
	friend address_t operator+(const address_t &l, const address_t &r);
	friend address_t operator+(const address_t &l, const poffset_t &r);
	friend address_t operator+(const poffset_t &l, const address_t &r);
	friend address_t operator-(const address_t &l, const address_t &r);
	friend address_t operator-(const address_t &l, const poffset_t &r);
	friend address_t operator-(const poffset_t &l, const address_t &r);
	friend bool operator==(const address_t &l, const address_t &r);
	friend bool operator!=(const address_t &l, const address_t &r);
	friend bool operator> (const address_t &l, const address_t &r);
	friend bool operator<=(const address_t &l, const address_t &r);
	friend bool operator< (const address_t &l, const address_t &r);
	friend bool operator>=(const address_t &l, const address_t &r);
	friend bool operator==(const address_t &l, const paddr_t &r);
	friend bool operator!=(const address_t &l, const paddr_t &r);
	friend bool operator> (const address_t &l, const paddr_t &r);
	friend bool operator<=(const address_t &l, const paddr_t &r);
	friend bool operator< (const address_t &l, const paddr_t &r);
	friend bool operator>=(const address_t &l, const paddr_t &r);


	address_t &operator+=(const address_t &r);
	address_t &operator-=(const address_t &r);
	address_t &operator+=(const poffset_t &r);
	address_t &operator-=(const poffset_t &r);
	address_t &operator++();
	address_t &operator--();
	address_t operator++(int);
	address_t operator--(int);

	struct less {
		bool operator()(const address_t &l, const address_t &r) const;
	};

 protected:
 	/**
	 * Construct address from value and size.
	 * \param value value associated with address.
	 * \param size bit size of actual address.
	 * \param segment memory segment that the address is associated with
	 */
	address_t(paddr_t off, size_t size, const MemSegment *segment);

	friend class MemSegment;

	poffset_t m_offset;
	size_t m_size;
	const MemSegment *m_memsegment;
};

#endif
