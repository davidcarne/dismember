#ifndef _SYMLIST_H_
#define _SYMLIST_H_
#include "types.h"
#include "abstractdata.h"

#include <ext/hash_map>
//#include <ext/hash_fun.h>
#include <list>
#include <string>
#include <set>
#include <vector>
#include <algorithm>

// Hybrid datatype allowing for fast lookups, but also maintains sorted
class SymbolList;

#include "string_hash.h"

/**
 * \brief representation of a Symbol
 */
class Symbol {
public:
	/**
	 * @return the name of the symbol
	 */
	const std::string & get_name() const;
	
	/**
	 * @return the address of the symbol
	 */
	const address_t get_addr() const;

	/**
	 * \brief set a property of the symbol.
	 * \todo add this to another class that this can inherit from
	 */
	void set_property(std::string key, AbstractData *value);
	
	/**
	 * \brief get a property of the symbol. This may be reworked
	 * \todo add this to another class that this can inherit from
	 */
	AbstractData *get_property(std::string key) const;
protected:
	/**
	 * \brief allocate a new symbol. protected as trace owns
	 */
	Symbol(address_t addr, std::string name);
private:
	friend class SymbolList;
	std::string m_name;
	address_t m_addr;

	typedef __gnu_cxx::hash_map<std::string, AbstractData *> propertymap_t;
	typedef propertymap_t::iterator propertymap_i;	
	typedef propertymap_t::const_iterator propertymap_ci;
	propertymap_t properties;
};

/** 
 * Sortable caching list of symbols
 */
class SymbolList
{
private:
	struct ltsym_name
	{
		bool operator()(const Symbol * s1, const Symbol * s2) const
		{
			return s1->get_name() < s2->get_name();
		}
	};
	
	
	struct ltsym_addr
	{
		bool operator()(const Symbol * s1, const Symbol * s2) const
		{
			return s1->get_addr() < s2->get_addr();
		}
	};
	

	
	typedef __gnu_cxx::hash_map<std::string, Symbol *> symnamemap_t;
	typedef symnamemap_t::iterator symnamemap_i;	
	typedef symnamemap_t::const_iterator symnamemap_ci;
	
	typedef __gnu_cxx::hash_map< address_t, Symbol * > symaddrmap_t;
	typedef symaddrmap_t::iterator symaddrmap_i;	
	typedef symaddrmap_t::const_iterator symaddrmap_ci;
	

	typedef std::vector<Symbol *> symordering_t;
	

public:
	/// Symbol order const-iterator by name
	typedef symordering_t::const_iterator symname_ci;
	/// Symbol order const-iterator by addr
	typedef symordering_t::const_iterator symaddr_ci;
	
	/// Symbol order specifier
	typedef enum  {
		SYMORDER_NAME,
		SYMORDER_ADDR
	} symsortorder_e;
	
	SymbolList();
		
	/** all purpose tool for manipulating symbols
	 *
	 * creates a symbol at address. If symbol at address already exists
	 * nukes that symbol, creates new
	 * Call with empty name to delete symbol
	 *
	 * @param addr Address to set symbol at
	 * @param name string to set symbol to. "" to erase symbol
	 * @return pointer to created symbol. NULL if symbol was deleted
	 */
	Symbol * set_symbol(address_t addr, std::string name);
	
	/** 
	 * Lookup a symbol by address
	 * @param addr address to lookup
	 * @return pointer to found symbol, or NULL if no symbol found.
	 */
	const Symbol * get_symbol(address_t addr) const;
	
	/** 
	 * Lookup a symbol by name
	 * @param name name to lookup
	 * @return pointer to found symbol, or NULL if no symbol found.
	 */
	const Symbol * get_symbol(const std::string & name) const;
	
	
	/** 
	 * Lookup a symbol by index into an ordering
	 * @param index to lookup
	 * @param order ordering to use
	 * @return pointer to found symbol, or NULL if no symbol found.
	 */
	Symbol * find_ordered_symbol(uint32_t index, symsortorder_e order) const;
	
	/** @return count of symbols in list */
	uint32_t get_symbol_count(void) const;
	
	/** @return begin const iterator across name-ordered symbols */
	symname_ci begin_name() const;
	
	/** @return end const iterator across name-ordered symbols */
	symname_ci end_name() const;
	
	/** @return begin const iterator across addr-ordered symbols */
	symaddr_ci begin_addr() const;
	
	/** @return end const iterator across addr-ordered symbols */
	symaddr_ci end_addr() const;
	
private:
	symnamemap_t m_name_lookup;
	symaddrmap_t m_addr_lookup;

	// These are caches, and therefore mutable
	mutable bool m_vectors_dirty;
	mutable symordering_t m_name_order;
	mutable symordering_t m_addr_order;
	
	// Only touches caches, therefore const
	void updateDirtyVectors() const;
};


#endif
