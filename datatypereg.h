/**
 * \brief Data Type Registry
 *
 */

#ifndef _DATATYPEREG_H_
#define _DATATYPEREG_H_

#include <map>
#include <string>
class DataType;

class DataTypeReg {
	public:
	typedef std::map<std::string, DataType *> datatypemap_t;
	typedef datatypemap_t::const_iterator datatypereg_ci;
	
	datatypereg_ci getBegin() const;
	datatypereg_ci getEnd() const;
	
	DataType * lookupDataType(std::string name) const;
	void insertDataType(std::string name, DataType * d);
	
	DataTypeReg();
	
	private:
	datatypemap_t m_datatypes;
};

#endif
