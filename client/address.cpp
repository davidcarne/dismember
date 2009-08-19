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

#include "exception.h"

#include "address.h"

// TODO: memsegment needs an interface.. this should use it.
#include "kvs_memsegment.h"

#define ASSERT_TRUE(x) \
  do { \
    if (!(x)) {\
      char __buf[512]; \
      snprintf(__buf, 512, #x " at " __FILE__ ":%d (" ")", __LINE__); \
      throw Exception(__buf); \
    } \
  } while (0)

/**
 * Construct invalid address.
 */
address_t::address_t()
 : m_offset(0), m_size(0), m_memsegment()
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
paddr_t address_t::getValue() const
{
	return m_memsegment.lock()->get_start() + m_offset;
}

/**
 * Get the offset value associated with this address.
 */
paddr_t address_t::getOffset() const
{
	return m_offset;
}

/**
 * Set the value associated with this address.
 */
void address_t::setValue(paddr_t val)
{
	m_offset = val - m_memsegment.lock()->get_start();
}

/**
 * Returns whether we're valid and associated with a memory segment.
 */
bool address_t::isValid() const
{
	if (!m_memsegment.expired()) {
		return (m_size > 0) &&
			(m_offset >= 0) &&
			((paddr_t)m_offset < m_memsegment.lock()->get_length());
	}
	return false;
}


/**
 * Returns whether we're associated to the same memory segment.
 */
bool address_t::comparableTo(const address_t &addr) const
{
	return addr.m_memsegment.lock() == m_memsegment.lock();
}

/**
 * Convert address to printable string, obeying size.
 */
std::string address_t::toString() const
{
	char fmt[16];
	if (!isValid())
		throw Exception("Invalid address!");
	snprintf(fmt, 16, "%%s%s%%0%u%s",
			m_memsegment.lock()->getName().length() ? ":":"0x", (u32)(m_size >> 2),
			(sizeof(unsigned long) == sizeof(paddr_t)) ? "lx" : "llx");

	char buf[100];
	snprintf(buf, 100, fmt, m_memsegment.lock()->getName().c_str(), getValue());
	return std::string(buf);
}

/**
 * Convert address to serial string
 */
std::string address_t::toSerialString() const
{
	if (!isValid())
		throw Exception("Invalid address!");
	
	char buf[100];
	snprintf(buf, 100, "%s!%x", m_memsegment.lock()->getName().c_str(), getValue());
	return std::string(buf);
}

wp_cI_MemSegment address_t::getSegment(void) const
{
	return m_memsegment;
}

bool address_t::readBytes(u8 bytes, u8 * buf) const
{
	return m_memsegment.lock()->get_bytes(*this, bytes, buf);
}

bool address_t::readByte(uint8_t * data) const
{
	uint8_t dummy;
	
	if (data)
		return m_memsegment.lock()->get_bytes(*this, 1, data);
	 
	return m_memsegment.lock()->get_bytes(*this, 1, &dummy);
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

address_t &address_t::operator+=(const poffset_t &r)
{
	m_offset += r;
	return *this;
}

address_t &address_t::operator-=(const poffset_t &r)
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

address_t & address_t::operator=(const address_t& in)
{
	m_offset = in.m_offset;
	m_size = in.m_size;
	m_memsegment = in.m_memsegment;
}


address_t::address_t(paddr_t offset, size_t size, const wp_cI_MemSegment segment)
 : m_offset(offset), m_size(size), m_memsegment(segment)
{ }


address_t operator+(const address_t &l, const address_t &r)
{
	ASSERT_TRUE(l.m_size == r.m_size);
	ASSERT_TRUE(l.m_memsegment.lock() == r.m_memsegment.lock());
	return address_t(l.m_offset + r.m_offset, l.m_size, l.m_memsegment);
}

address_t operator+(const address_t &l, const poffset_t &r)
{
	return address_t(l.m_offset + r, l.m_size, l.m_memsegment);
}

address_t operator+(const poffset_t &l, const address_t &r)
{
	return address_t(l + r.m_offset, r.m_size, r.m_memsegment);
}

address_t operator-(const address_t &l, const address_t &r)
{
	ASSERT_TRUE(l.m_size == r.m_size);
	ASSERT_TRUE(l.m_memsegment.lock() == r.m_memsegment.lock());
	return address_t(l.m_offset - r.m_offset, l.m_size, l.m_memsegment);
}

address_t operator-(const address_t &l, const poffset_t &r)
{
	return address_t(l.m_offset - r, l.m_size, l.m_memsegment);
}

address_t operator-(const poffset_t &l, const address_t &r)
{
	return address_t(l - r.m_offset, r.m_size, r.m_memsegment);
}

bool operator==(const address_t &l, const address_t &r)
{
	return (l.m_offset == r.m_offset) &&
		(l.m_size == r.m_size) &&
		(l.m_memsegment.lock() == r.m_memsegment.lock());
}

bool operator!=(const address_t &l, const address_t &r)
{
	return (l.m_offset != r.m_offset) ||
		(l.m_size != r.m_size) ||
		(l.m_memsegment.lock() != r.m_memsegment.lock());
}

bool operator> (const address_t &l, const address_t &r)
{
	ASSERT_TRUE(l.m_size == r.m_size);
	ASSERT_TRUE(l.m_memsegment.lock() == r.m_memsegment.lock());
	return l.m_offset > r.m_offset;
}

bool operator<= (const address_t &l, const address_t &r)
{
	ASSERT_TRUE(l.m_size == r.m_size);
	ASSERT_TRUE(l.m_memsegment.lock() == r.m_memsegment.lock());
	return l.m_offset <= r.m_offset;
}

bool operator< (const address_t &l, const address_t &r)
{
	ASSERT_TRUE(l.m_size == r.m_size);
	ASSERT_TRUE(l.m_memsegment.lock() == r.m_memsegment.lock());
	return l.m_offset < r.m_offset;
}

bool operator>= (const address_t &l, const address_t &r)
{
	ASSERT_TRUE(l.m_size == r.m_size);
	ASSERT_TRUE(l.m_memsegment.lock() == r.m_memsegment.lock());
	return l.m_offset >= r.m_offset;
}


bool operator==(const address_t &l, const paddr_t &r)
{
	return l.getValue() == r;
}

bool operator!=(const address_t &l, const paddr_t &r)
{
	return l.getValue() != r;
}

bool operator> (const address_t &l, const paddr_t &r)
{
	return l.getValue() > r;
}

bool operator<= (const address_t &l, const paddr_t &r)
{
	return l.getValue() <= r;
}

bool operator< (const address_t &l, const paddr_t &r)
{
	return l.getValue() < r;
}

bool operator>= (const address_t &l, const paddr_t &r)
{
	return l.getValue() >= r;
}

bool address_t::less::operator()(const address_t &l, const address_t &r) const
{
	return l.getValue() < r.getValue();
}
