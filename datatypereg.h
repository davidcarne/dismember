/**
 * \brief Data Type Registry
 *
 */

#ifndef _DATATYPEREG_H_
#define _DATATYPEREG_H_

#include <map>
#include <string>

#include "datatype.h"

class DataTypeReg {
	public:
	typedef std::map<std::string, sp_DataType> datatypemap_t;
	typedef datatypemap_t::const_iterator datatypereg_ci;
	
	datatypereg_ci getBegin() const;
	datatypereg_ci getEnd() const;
	
	sp_DataType lookupDataType(std::string name) const;
	void insertDataType(std::string name, sp_DataType);
	
	DataTypeReg();
	
	private:
	datatypemap_t m_datatypes;
};

#endif
