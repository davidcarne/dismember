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

/* Hack hack hack.. stupid stl */
namespace __gnu_cxx
{
	template<> struct hash< std::string >
	{
		size_t operator()( const std::string& x ) const
	{
		return __stl_hash_string( x.c_str() );
	}
	};
}


class Symbol {
public:
	const std::string & get_name() const;
	const address_t get_addr() const;

	void set_property(std::string key, GenericAbstractData *value);
	GenericAbstractData *get_property(std::string key) const;
protected:
	Symbol(address_t addr, std::string name);
private:
	friend class SymbolList;
	std::string m_name;
	address_t m_addr;

	typedef __gnu_cxx::hash_map<std::string, GenericAbstractData *> propertymap_t;
	typedef propertymap_t::iterator propertymap_i;	
	typedef propertymap_t::const_iterator propertymap_ci;
	propertymap_t properties;
};


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
	typedef symordering_t::const_iterator symname_ci;
	typedef symordering_t::const_iterator symaddr_ci;
	
	typedef enum  {
		SYMORDER_NAME,
		SYMORDER_ADDR
	} symsortorder_e;
	
	SymbolList();
		
	// all purpose tool for manipulating symbols
	// creates a symbol at address. If symbol at address already exists
	// nukes that symbol, creates new
	// Call with empty name to delete symbol
	Symbol * set_symbol(address_t addr, std::string name);
	
	const Symbol * get_symbol(address_t addr) const;
	const Symbol * get_symbol(const std::string & name) const;
	
	Symbol * find_ordered_symbol(uint32_t index, symsortorder_e order);
	uint32_t get_symbol_count(void) const;
	
	symname_ci begin_name();
	symname_ci end_name();
	symaddr_ci begin_addr();
	symaddr_ci end_addr();
	
private:
	symnamemap_t m_name_lookup;
	symaddrmap_t m_addr_lookup;

	bool m_vectors_dirty;
	
	symordering_t m_name_order;
	symordering_t m_addr_order;
	
	void updateDirtyVectors();
};


#endif
