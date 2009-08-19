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


#include "../loaderfactory.h"

#include <stack>

class HexLoader : public FileLoaderMaker
{
 public:
	HexLoader();

	virtual bool loadFromFile(FILE *file, I_ProjectModel *ctx);
	virtual int matchToFile(FILE *file) const;
} registerHexLoader;

#define DAT   0x00
#define END   0x01
#define ESA   0x02
#define SSA   0x03
#define ELA   0x04
#define SLA   0x05

static uint8_t read8(const char *p, const char **e)
{
	uint8_t ret = 0;
	for (int i = 0; i < 2; ++i) {
		ret <<= 4;
		if (*p >= '0' && *p <= '9')
			ret += *p - '0';
		else if (*p >= 'a' && *p <= 'f')
			ret += (*p - 'a') + 0xa;
		else if (*p >= 'A' && *p <= 'F')
			ret += (*p - 'A') + 0xa;
		++p;
	}
	*e = p;
	return ret;
}

static uint16_t read16(const char *p, const char **e)
{
	uint8_t m = read8(p, &p);
	uint8_t l = read8(p, e);
	return (uint16_t)(m << 8) | l;
}

static uint8_t *readBytes(const char *p, const char **e, uint8_t len)
{
	uint8_t *ret = (uint8_t *)malloc(len);
	for (uint8_t i = 0; i < len; ++i)
		ret[i] = read8(p, &p);
	*e = p;
	return ret;
}

HexLoader::HexLoader()
 : FileLoaderMaker("Hex")
{ }

class HexSegment
{
 public:
	HexSegment(uint32_t addr = 0)
	 : m_addr(addr),m_data(0),m_lowaddr(0xffffffff),m_hiaddr(0),m_length(0)
	{ }

	~HexSegment()
	{
		if (m_data)
			free(m_data);
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
		while (addr >= m_length) {
			m_length = (m_length == 0) ? 4096 : (m_length * 2);
			m_data = (uint8_t *)realloc(m_data, m_length);
		}
		m_data[addr] = data;
		if (addr < m_lowaddr)
			m_lowaddr = addr;
		if (addr > m_hiaddr)
			m_hiaddr = addr;
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
	uint32_t m_length;
};

class HexFile
{
 public:
	HexFile()
	 : error(0), reg_eip(0), reg_cs(0), reg_ip(0)
	{ }
	const char *error;
	uint32_t reg_eip;
	uint16_t reg_cs;
	uint16_t reg_ip;
	std::stack<struct HexSegment *> segments;
};

int parseLine(const char *line, HexFile &hf, bool pars = true)
{
	struct {
		uint8_t byteCount;
		uint16_t address;
		uint8_t recordType;
		uint8_t *data;
		uint8_t checksum;
	} l;
	
	l.data = NULL; // Compiler warning of data uninited otherwise
	
	const char *p = line + 1;
	uint8_t sum;
	if (*line != ':') {
		hf.error = "Incorrectly formatted record!";
		return 1;
	}
	
	l.byteCount = read8(p, &p);
	if (((strlen(line) - 1)/2 - 5) != l.byteCount) {
		hf.error = "Incorrect record length!";
		return 1;
	}
	l.address = read16(p, &p);
	l.recordType = read8(p, &p);
	if (l.byteCount > 0)
		l.data = readBytes(p, &p, l.byteCount);
	l.checksum = read8(p, &p);

	sum = l.byteCount + (l.address & 0xff) + ((l.address >> 8) & 0xff)
		+ l.recordType;
	for (uint8_t i = 0; i < l.byteCount; ++i)
		sum += l.data[i];
	sum = (0x100 - sum);
	if (sum != l.checksum) {
		hf.error = "Bad checksum!";
		goto err;
	}
	switch (l.recordType) {
	case END:
		break;
	case ESA:
		if (l.byteCount != 2 || l.address != 0) {
			hf.error = "Strange extended segment address record!";
			goto err;
		} else if (pars) {
			uint32_t addr = ((l.data[0] << 8) | l.data[1]) << 4;
			HexSegment *hs = new HexSegment(addr);
			hf.segments.push(hs);
		}
		break;
	case ELA:
		if (l.byteCount != 2 || l.address != 0) {
			hf.error = "Strange extended linear address record!";
			goto err;
		} else if (pars) {
			uint32_t addr = ((l.data[0] << 8) | l.data[1]) << 16;
			HexSegment *hs = new HexSegment(addr);
			hf.segments.push(hs);
		}
		break;
	case SSA:
		if (l.byteCount != 4 || l.address != 0) {
			hf.error = "Strange start segment address record!";
			goto err;
		}
		hf.reg_cs = (l.data[0] << 8) | l.data[1];
		hf.reg_ip = (l.data[2] << 8) | l.data[3];
		break;
	case SLA:
		if (l.byteCount != 4 || l.address != 0) {
			hf.error = "Strange start linear address record!";
			goto err;
		}
		hf.reg_eip =    (l.data[0] << 24) | (l.data[1] << 16) |
				(l.data[2] <<  8) | (l.data[3]      );
		break;
	case DAT:
		if (pars) {
			if (hf.segments.empty())
				hf.segments.push(new HexSegment());
			uint32_t addr = l.address;
			for (uint8_t i = 0; i < l.byteCount; ++i)
				hf.segments.top()->setData(addr++, l.data[i]);
		}
		break;
	}
	if (l.byteCount)
		free(l.data);
	return 0;

 err:
	if (l.byteCount)
		free(l.data);
	while (!hf.segments.empty()) {
		HexSegment *hs = hf.segments.top();
		hf.segments.pop();
		delete hs;
	}
	return 1;
}


int HexLoader::matchToFile(FILE *fp) const
{
	char line[256];
	rewind(fp);
	HexFile dummy;
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

bool HexLoader::loadFromFile(FILE *fp, I_ProjectModel *ctx)
{
	char line[2048];

	HexFile file;

	rewind(fp);
	while (fgets(line, 2048, fp)) {
		line[strlen(line) - 1] = 0;
		if (parseLine(line, file)) {
			fprintf(stderr, "%s\n", file.error);
			return false;
		}
	}
	while (!file.segments.empty()) {
		HexSegment *hs = file.segments.top();
		hs->map();
		MemSegment *ms = new MemSegment(hs->address(), hs->length(), hs->data());
		ctx->addSegment(ms);
		file.segments.pop();
		delete hs;
	}
	return true;
}
