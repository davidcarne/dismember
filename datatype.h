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

#ifndef _DATATYPE_H_
#define _DATATYPE_H_

#include <stdexcept>

#include "types.h"

#include <boost/shared_ptr.hpp>


class I_ProjectModel;
class I_MemlocData;

/**
 * \brief DataType represents the concept of a datatype.
 *
 * DataType exists for easy instantiation of Data + Code types, by name / arch
 * DataType is an interface, IE, it cannot be instantiated.
 */
class DataType {
	public:
	/**
	 * \brief Get the name of the datatype [for display]
	 *
	 * @return the name of the dattatype
	 */
	virtual const std::string getName() const = 0;
	
	/**
	 * \brief Get the name of the datatype [for display]
	 *
	 * @return the name of the dattatype
	 */
	virtual const std::string getDisplayName() const
	{
		if (m_displayname.length() == 0)
			return getName();
		return m_displayname;
	}
	
	/**
	 * \brief get the size that this datatype could take up
	 *
	 * @return the size of the datatype, 0 for variable
	 */
	virtual u32 getElemSize() const = 0;
	
	/**
	 * \brief create an alias name for a datatype
	 */
	virtual void alias(std::string newname)
	{
		m_displayname = newname;
	}
	
	/**
	 * @return if the datatype is mutable
	 */
	virtual bool isMutable() const = 0;
	
	// need to add an "editor popup display"
	
	/**
	 * @return the trace context that this datatype belongs to
	 */
	const I_ProjectModel * getI_ProjectModelContext() const {
		return m_ctx;
	}
	
	virtual ~DataType() {
	};
	
	
	/**
	 * \brief instantiate a new instance of the datatype
	 * @param addr The start address
	 * @return the instantiated object, NULL on error
	 */
	virtual I_MemlocData * instantiate(address_t addr) const = 0;
	
	protected:
	/**
	 * \brief Default constructor for the datatype class - cannot fully construct a datatype.
	 */
	DataType(const I_ProjectModel * t) : m_ctx(t) {
	};
	
	
private:
	friend class I_ProjectModel;
	const I_ProjectModel * m_ctx;
	std::string m_displayname;
	
};

typedef boost::shared_ptr<DataType> sp_DataType;
#endif
