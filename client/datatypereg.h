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
