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
	 * Convert address to printable string, obeying size.
	 */
	std::string toString() const;

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
