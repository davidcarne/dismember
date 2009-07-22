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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "memsegment.h"
#include "../loaderfactory.h"

#include <stack>

class S19Loader : public FileLoaderMaker
{
 public:
	S19Loader();

	virtual bool loadFromFile(FILE *file, ProjectModel *ctx);
	virtual int matchToFile(FILE *file) const;
} registerS19Loader;

#define S0   0x0
#define S1   0x1
#define S2   0x2
#define S3   0x3
#define S5   0x5
#define S7   0x7
#define S8   0x8
#define S9   0x9

static uint8_t read4(const char *p, const char **e)
{
	uint8_t ret = 0;
	if (*p >= '0' && *p <= '9')
		ret += *p - '0';
	else if (*p >= 'a' && *p <= 'f')
		ret += (*p - 'a') + 0xa;
	else if (*p >= 'A' && *p <= 'F')
		ret += (*p - 'A') + 0xa;
	*e = p + 1;
	return ret;
}

static uint8_t read8(const char *p, const char **e)
{
	uint8_t m = read4(p, &p);
	uint8_t l = read4(p, e);
	return (uint8_t)(m << 4) | l;
}

static uint16_t read16(const char *p, const char **e)
{
	uint8_t m = read8(p, &p);
	uint8_t l = read8(p, e);
	return (uint16_t)(m << 8) | l;
}

static uint32_t read24(const char *p, const char **e)
{
	uint8_t u = read8(p, &p);
	uint8_t m = read8(p, &p);
	uint8_t l = read8(p, e);
	return (uint32_t)(u << 16) | (m << 8) | l;
}

static uint32_t read32(const char *p, const char **e)
{
	uint16_t m = read16(p, &p);
	uint16_t l = read16(p, e);
	return (uint32_t)(m << 16) | l;
}

static uint8_t *readBytes(const char *p, const char **e, uint8_t len)
{
	uint8_t *ret = (uint8_t *)malloc(len);
	for (uint8_t i = 0; i < len; ++i)
		ret[i] = read8(p, &p);
	*e = p;
	return ret;
}

S19Loader::S19Loader()
 : FileLoaderMaker("S19")
{ }

class S19Segment
{
 public:
	S19Segment()
	 : m_addr(0), m_data(0), m_lowaddr(0xffffffff),
	 		m_hiaddr(0), m_lastaddr(0), m_length(0)
	{ }

	~S19Segment()
	{
		if (m_data)
			free(m_data);
	}

	bool rangeIncludes(uint32_t addr)
	{
		if (addr > m_lastaddr)
			return (addr - m_lastaddr) < 0x100;
		return (m_lastaddr - addr) < 0x100;
	}

	void map()
	{	
		if (!m_data) {
			m_lowaddr = 0;
			m_hiaddr = 0;
			m_length = 0;
		} else {
			m_length = m_hiaddr + 1;
			m_data = (uint8_t *)realloc(m_data, m_length);
		}
	}

	void setData(uint32_t addr, uint8_t data)
	{
		uint32_t raddr = addr - m_addr;
		while (!m_data || raddr >= m_length) {
			if (!m_data) {
				m_addr = addr;
				raddr = addr - m_addr;
			}
			m_length = (m_length == 0) ? 4096 : (m_length * 2);
			m_data = (uint8_t *)realloc(m_data, m_length);
		}
		m_data[raddr] = data;
		if (raddr < m_lowaddr)
			m_lowaddr = raddr;
		if (raddr > m_hiaddr)
			m_hiaddr = raddr;
		m_lastaddr = addr;
	}

	uint32_t length() const
	{
		return m_length - m_lowaddr;
	}

	uint32_t address() const
	{
		return m_addr + m_lowaddr;
	}

	uint8_t *data() const
	{
		return m_data + m_lowaddr;
	}

 private:
	uint32_t m_addr;
	uint8_t *m_data;
	uint32_t m_lowaddr;
	uint32_t m_hiaddr;
	uint32_t m_lastaddr;
	uint32_t m_length;
};

class S19File
{
 public:
	S19File()
	 : start(0), error(0), count(0), m_segment(0)
	{ }

	~S19File()
	{
		while (!m_segments.empty()) {
			S19Segment *hs = m_segments.top();
			m_segments.pop();
			delete hs;
		}
	}

	void pushData(uint32_t address, uint8_t data)
	{
		if (m_segment && m_segment->rangeIncludes(address))
			m_segment->setData(address, data);
		else {
			m_segment = new S19Segment();
			m_segments.push(m_segment);
			m_segment->setData(address, data);
		}
	}

	void map(ProjectModel *ctx)
	{
		while (!m_segments.empty()) {
			S19Segment *hs = m_segments.top();
			hs->map();
			MemSegment *ms = new MemSegment(hs->address(), hs->length(), hs->data());
			ctx->addSegment(ms);
			m_segments.pop();
			delete hs;
		}
	}

	uint32_t start;
	const char *error;
	uint16_t count;
 private:
	std::stack<S19Segment *> m_segments;
	S19Segment *m_segment;
};

static int parseLine(const char *line, S19File &hf, bool pars = true)
{
	struct {
		uint8_t byteCount;
		uint32_t address;
		uint8_t recordType;
		uint8_t *data;
		uint8_t checksum;
	} l;
	
	l.address = 0; // suppress unitialized data compiler warning
	l.data = NULL;
	
	const char *p = line + 1;
	uint8_t sum = 0;
	if (*line != 'S') {
		hf.error = "Incorrectly formatted record!";
		return 1;
	}
	
	l.recordType = read4(p, &p);

	l.byteCount = read8(p, &p);
	if ((strlen(line)/2 - 2) != l.byteCount) {
		hf.error = "Incorrect record length!";
		return 1;
	}
	sum += l.byteCount;

	// don't read in checksum
	l.byteCount--;

	switch (l.recordType) {
	case S0:
	case S1:
	case S5:
	case S9:
		l.address = read16(p, &p);
		l.byteCount -= 2;
		break;
	case S2:
	case S8:
		l.address = read24(p, &p);
		sum += (l.address >> 16) & 0xff;
		l.byteCount -= 3;
		break;
	case S3:
	case S7:
		l.address = read32(p, &p);
		sum += ((l.address >> 24) & 0xff) + ((l.address >> 16) & 0xff);
		l.byteCount -= 4;
		break;
	}
	sum += ((l.address >> 8) & 0xff) + (l.address & 0xff);

	if (l.byteCount > 0)
		l.data = readBytes(p, &p, l.byteCount);
	l.checksum = read8(p, &p);

	for (uint8_t i = 0; i < l.byteCount; ++i)
		sum += l.data[i];
	sum = ~sum;
	if (sum != l.checksum) {
		hf.error = "Bad checksum!";
		goto err;
	}
	switch (l.recordType) {
	case S0:
		// module name     : char[20]
		// version number  : char[2]
		// revision number : char[2]
		// description     : char[0-36]
		break;
	case S1:
	case S2:
	case S3:
		if (pars) {
			uint32_t addr = l.address;
			for (uint8_t i = 0; i < l.byteCount; ++i)
				hf.pushData(addr++, l.data[i]);
		}
		hf.count++;
		break;
	case S5:
		if (l.byteCount != 0) {
			hf.error = "Incorrect record format!";
			goto err;
		}
		if (l.address != hf.count) {
			hf.error = "Incorrect record count!";
			goto err;
		}
		break;
	case S7:
	case S8:
	case S9:
		if (l.byteCount != 0) {
			hf.error = "Incorrect record format!";
			goto err;
		}
		hf.start = l.address;
		break;
	}

	if (l.byteCount)
		free(l.data);
	return 0;

 err:
	if (l.byteCount)
		free(l.data);
	return 1;
}


int S19Loader::matchToFile(FILE *fp) const
{
	char line[516];
	rewind(fp);
	S19File dummy;
	int i;
	for (i = 0; i < 3; ++i) {
		if (fgets(line, 256, fp)) {
			line[strlen(line) - 1] = 0;
			if (parseLine(line, dummy, false))
				return -1;
		} else 
			break;
	}
	return i*25;
}

bool S19Loader::loadFromFile(FILE *fp, ProjectModel *ctx)
{
	char line[516];

	S19File file;

	rewind(fp);
	while (fgets(line, 2048, fp)) {
		line[strlen(line) - 1] = 0;
		if (parseLine(line, file)) {
			fprintf(stderr, "%s\n", file.error);
			return false;
		}
	}
	file.map(ctx);
	return true;
}
