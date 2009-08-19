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

#include "i_projectmodel.h"
#include <stdint.h>
#include <list>
#include <map>

#ifndef _GUIPROXY_H_
#define _GUIPROXY_H_

#define GPROX_BLOCK_SIZE 1024

typedef uint32_t line_ind_t;

/**
 * \brief Generalized caching layer for line->address mapping.
 *		Eventually will proxy all interface to trace object.
 *
 *
 * Theory of Operation: todo
 */
class GuiProxy {
	public:
	/**
	 * \brief Instantiate a new gui proxy for the given trace/document
	 */
	GuiProxy(I_ProjectModel * ctx);
	
	/**
	 * \brief Get the number of lines in the display
	 * @return number of lines to be displayed
	 */
	line_ind_t getLineCount();
	
	/**
	 * \brief Get the number of text rows for a given line
	 * @param line the line to get rows for
	 * @return number of text rows for the given line
	 */	
	uint8_t getLineRows(line_ind_t line);
	
	/**
	 * \brief Get the address for a given line
	 * @param line the line to get address for
	 * @return address
	 */	
	address_t getLineAddr(line_ind_t line);
	
	/**
	 * \brief Get the line for a given address
	 * @param addr the address to get a line for
	 * @return line
	 */	
	line_ind_t getLineAtAddr(address_t addr);
	
	
	/**
	 * \brief update the internal state of the guiproxy
	 */
	void update(void);
private:
	
	I_ProjectModel * m_ctx;
	
	struct addrblock {
		address_t start;
		uint64_t len;
		
		uint32_t lc;
	};
	
	void updateMemloc(I_MemlocData *s, HookChange hc);
	
	bool m_dirty;
	std::list<struct addrblock *> m_blocks;
	typedef std::list<struct addrblock *>::iterator blk_i;
	
	typedef std::map<size_t, struct addrblock *> lclookup_t;
	typedef lclookup_t::iterator lclookup_i;
	
	lclookup_t m_lclookup;
	
	line_ind_t m_lc;
	
	bool m_last_look_set;
	line_ind_t m_last_look_line;
	address_t m_last_look_addr;
};

#endif
