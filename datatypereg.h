/**
 * \brief Data Type Registry
 *
 */

#ifndef _DATATYPEREG_H_
#define _DATATYPEREG_H_

#include <map>
#include <string>

#include "datatype.h"

/**
 * Data type registry, list of all datatypes known to the analysis engine
 */
class DataTypeReg {
	public:
	
	/**
	 * Datatype map type
	 */
	typedef std::map<std::string, sp_DataType> datatypemap_t;
	
	/**
	 * Datatype public iterator type
	 */
	typedef datatypemap_t::const_iterator datatypereg_ci;
	
	/**
	 * Get the begin iterator
	 */
	datatypereg_ci getBegin() const;
	
	/**
	 * Get the end iterator
	 */
	datatypereg_ci getEnd() const;
	
	/**
	 * lookup a datatype given a datatype name
	 */
	sp_DataType lookupDataType(std::string name) const;
	
	/**
	 * insert a datatype given a datatype name
	 */
	void insertDataType(std::string name, sp_DataType);
	
	/**
	 * Default constructor
	 */
	DataTypeReg();
	
	private:
	datatypemap_t m_datatypes;
};

#endif
