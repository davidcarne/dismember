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
class KVS_Node;

class DataTypeDecoding {
public:
	
	typedef enum {
		DT_IS_EXEC = 0x1,
		DT_CONTINUES = 0x2
	} e_datatype_decoding_flags;
	
	DataTypeDecoding() : m_flags(0), m_text_form(""), m_decoded_length(0), m_kvattribs(0) {
		
	}
	
	DataTypeDecoding(const std::string & text_form, psize_t declength, uint32_t flags = 0, KVS_Node * kvattribs = NULL) :
		m_flags(flags), m_text_form(text_form), m_decoded_length(declength), m_kvattribs(kvattribs) {
	}

	bool isExecutable() const {
		return m_flags & DT_IS_EXEC;
	};
	
	bool continues() const {
		return m_flags & DT_CONTINUES;
	};
	
	bool isValid() const {
		return m_decoded_length > 0;
	}
	
	psize_t getDecodedLength() const {
		return m_decoded_length;
	}
	
	const std::string & getDecodedText() const
	{
		return m_text_form;
	}
private:
	std::string m_text_form;
	psize_t m_decoded_length;
	
	uint32_t m_flags;
	
	KVS_Node * m_kvattribs;
};


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
	const I_ProjectModel * getProjectModelContext() const {
		return m_ctx;
	}
	
	virtual ~DataType() {
	};
	
	
	/**
	 * \brief instantiate a new instance of the datatype
	 * @param addr The start address
	 * @return the instantiated object, NULL on error
	 */
	virtual const DataTypeDecoding decode(const address_t & addr) const = 0;
	
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
