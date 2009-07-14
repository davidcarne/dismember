#include <assert.h>

#include "exception.h"
#include "memsegment.h"
#include "address.h"

/**
 * Construct invalid address.
 */
address_t::address_t()
 : m_offset(0), m_size(0), m_memsegment(0)
{ }

/**
 * Construct copy of address.
 */
address_t::address_t(const address_t &c)
 : m_offset(c.m_offset), m_size(c.m_size), m_memsegment(c.m_memsegment)
{ }

/**
 * Get the actual value associated with this address.
 */
uint64_t address_t::getValue() const
{
	return m_memsegment->get_start() + m_offset;
}

/**
 * Get the offset value associated with this address.
 */
uint64_t address_t::getOffset() const
{
	return m_offset;
}

/**
 * Set the value associated with this address.
 */
void address_t::setValue(uint64_t val)
{
	m_offset = val - m_memsegment->get_start();
}

/**
 * Returns whether we're valid and associated with a memory segment.
 */
bool address_t::isValid() const
{
	if (m_memsegment != 0) {
		return (m_size > 0) &&
			(m_offset >= 0) &&
			((uint64_t)m_offset < m_memsegment->get_length());
	}
	return false;
}

/**
 * Convert address to printable string, obeying size.
 */
std::string address_t::toString() const
{
	char fmt[12];
	snprintf(fmt, 12, "0x%%0%lux", m_size >> 2);
	char buf[40];
	snprintf(buf, 40, fmt, getValue());
	return std::string(buf);
}

address_t &address_t::operator+=(const address_t &r)
{
	m_offset += r.m_offset;
	return *this;
}

address_t &address_t::operator-=(const address_t &r)
{
	m_offset -= r.m_offset;
	return *this;
}

address_t &address_t::operator+=(const uint64_t &r)
{
	m_offset += r;
	return *this;
}

address_t &address_t::operator-=(const uint64_t &r)
{
	m_offset -= r;
	return *this;
}

address_t &address_t::operator++()
{
	++m_offset;
	return *this;
}

address_t &address_t::operator--()
{
	--m_offset;
	return *this;
}

address_t address_t::operator++(int)
{
	address_t ret(*this);
	++(*this);
	return ret;
}
address_t address_t::operator--(int)
{
	address_t ret(*this);
	--(*this);
	return ret;
}

address_t::address_t(uint64_t offset, size_t size, const MemSegment *segment)
 : m_offset(offset), m_size(size), m_memsegment(segment)
{ }


address_t operator+(const address_t &l, const address_t &r)
{
	assert(l.m_size == r.m_size);
	assert(l.m_memsegment == r.m_memsegment);
	return address_t(l.m_offset + r.m_offset, l.m_size, l.m_memsegment);
}

address_t operator+(const address_t &l, const uint64_t &r)
{
	return address_t(l.m_offset + r, l.m_size, l.m_memsegment);
}

address_t operator+(const uint64_t &l, const address_t &r)
{
	return address_t(l + r.m_offset, r.m_size, r.m_memsegment);
}

address_t operator-(const address_t &l, const address_t &r)
{
	assert(l.m_size == r.m_size);
	assert(l.m_memsegment == r.m_memsegment);
	return address_t(l.m_offset - r.m_offset, l.m_size, l.m_memsegment);
}

address_t operator-(const address_t &l, const uint64_t &r)
{
	return address_t(l.m_offset - r, l.m_size, l.m_memsegment);
}

address_t operator-(const uint64_t &l, const address_t &r)
{
	return address_t(l - r.m_offset, r.m_size, r.m_memsegment);
}

bool operator==(const address_t &l, const address_t &r)
{
	return (l.m_offset == r.m_offset) &&
		(l.m_size == r.m_size) &&
		(l.m_memsegment == r.m_memsegment);
}

bool operator!=(const address_t &l, const address_t &r)
{
	return (l.m_offset != r.m_offset) ||
		(l.m_size != r.m_size) ||
		(l.m_memsegment != r.m_memsegment);
}

bool operator> (const address_t &l, const address_t &r)
{
	assert(l.m_size == r.m_size);
	assert(l.m_memsegment == r.m_memsegment);
	return l.m_offset > r.m_offset;
}

bool operator<= (const address_t &l, const address_t &r)
{
	assert(l.m_size == r.m_size);
	assert(l.m_memsegment == r.m_memsegment);
	return l.m_offset <= r.m_offset;
}

bool operator< (const address_t &l, const address_t &r)
{
	assert(l.m_size == r.m_size);
	assert(l.m_memsegment == r.m_memsegment);
	return l.m_offset < r.m_offset;
}

bool operator>= (const address_t &l, const address_t &r)
{
	assert(l.m_size == r.m_size);
	assert(l.m_memsegment == r.m_memsegment);
	return l.m_offset >= r.m_offset;
}


bool operator==(const address_t &l, const uint64_t &r)
{
	return l.getValue() == r;
}

bool operator!=(const address_t &l, const uint64_t &r)
{
	return l.getValue() != r;
}

bool operator> (const address_t &l, const uint64_t &r)
{
	return l.getValue() > r;
}

bool operator<= (const address_t &l, const uint64_t &r)
{
	return l.getValue() <= r;
}

bool operator< (const address_t &l, const uint64_t &r)
{
	return l.getValue() < r;
}

bool operator>= (const address_t &l, const uint64_t &r)
{
	return l.getValue() >= r;
}
