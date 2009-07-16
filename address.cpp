#include "exception.h"
#include "memsegment.h"
#include "address.h"

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
paddr_t address_t::getValue() const
{
	return m_memsegment->get_start() + m_offset;
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
			((paddr_t)m_offset < m_memsegment->get_length());
	}
	return false;
}


/**
 * Returns whether we're associated to the same memory segment.
 */
bool address_t::comparableTo(const address_t &addr) const
{
	return addr.m_memsegment == m_memsegment;
}

/**
 * Convert address to printable string, obeying size.
 */
std::string address_t::toString() const
{
	char fmt[16];
	snprintf(fmt, 16, "%%s%s%%0%u%s",
			m_memsegment->getName().length() ? ":":"0x", (u32)(m_size >> 2),
			(sizeof(unsigned long) == sizeof(paddr_t)) ? "lx" : "llx");

	char buf[100];
	snprintf(buf, 100, fmt, m_memsegment->getName().c_str(), getValue());
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

address_t::address_t(paddr_t offset, size_t size, const MemSegment *segment)
 : m_offset(offset), m_size(size), m_memsegment(segment)
{ }


address_t operator+(const address_t &l, const address_t &r)
{
	ASSERT_TRUE(l.m_size == r.m_size);
	ASSERT_TRUE(l.m_memsegment == r.m_memsegment);
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
	ASSERT_TRUE(l.m_memsegment == r.m_memsegment);
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
	ASSERT_TRUE(l.m_size == r.m_size);
	ASSERT_TRUE(l.m_memsegment == r.m_memsegment);
	return l.m_offset > r.m_offset;
}

bool operator<= (const address_t &l, const address_t &r)
{
	ASSERT_TRUE(l.m_size == r.m_size);
	ASSERT_TRUE(l.m_memsegment == r.m_memsegment);
	return l.m_offset <= r.m_offset;
}

bool operator< (const address_t &l, const address_t &r)
{
	ASSERT_TRUE(l.m_size == r.m_size);
	ASSERT_TRUE(l.m_memsegment == r.m_memsegment);
	return l.m_offset < r.m_offset;
}

bool operator>= (const address_t &l, const address_t &r)
{
	ASSERT_TRUE(l.m_size == r.m_size);
	ASSERT_TRUE(l.m_memsegment == r.m_memsegment);
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
