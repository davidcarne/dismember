#include "../trace.h"
#include <stdint.h>
#include <list>

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

class CodeView;

class GuiProxy {
	public:
	GuiProxy(Trace * ctx, CodeView * gui );
	
	/**
	 * \brief Get the number of lines in the display
	 * @return number of lines to be displayed
	 */
	line_ind_t get_line_count();
	
	/**
	 * \brief Get the number of text rows for a given line
	 * @param line the line to get rows for
	 * @return number of text rows for the given line
	 */	
	uint8_t get_line_rows(line_ind_t line);
	
	/**
	 * \brief Get the address for a given line
	 * @param line the line to get address for
	 * @return address
	 */	
	address_t get_addr_for_line(line_ind_t line);
	
	/**
	 * \brief Get the line for a given address
	 * @param addr the address to get a line for
	 * @return line
	 */	
	line_ind_t get_line_for_addr(address_t addr);
private:
	
	Trace * m_ctx;
	CodeView * m_gui;
	
	struct addrblock {
		address_t start;
		address_t len;
		
		uint32_t lc;
	};
	
	void update(void);
	
	bool m_dirty;
	std::list<struct addrblock *> m_blocks;
	typedef std::list<struct addrblock *>::iterator blk_i;
	
	typedef std::map<size_t, struct addrblock *> lclookup_t;
	typedef lclookup_t::iterator lclookup_i;
	
	lclookup_t m_lclookup;
	
	line_ind_t m_lc;
	
	bool m_last_look_set;
	address_t m_last_look_line;
	address_t m_last_look_addr;
	
	void UpdateMemloc(MemlocData *s, HookChange hc);
};

#endif
