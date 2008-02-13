#include "types.h"
#include "abstractdata.h"
#include <map>

class HintList;

class Hint {
	
	
protected:
	Hint(address_t addr, std::string key, AbstractData & value);
	
	friend class HintList;
private:
	address_t m_addr;
	std::string m_key;
	bool m_thisonly;
	AbstractData & value;
};

class HintList {
	
	void insertHint();

private:
	std::multimap<address_t, Hint *> m_hints;
};