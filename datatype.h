#ifndef _DATATYPE_H_
#define _DATATYPE_H_

#include <stdexcept>

#include "types.h"

#include <boost/shared_ptr.hpp>

class Trace;
class MemlocData;

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
	virtual address_t getElemSize() const = 0;
	
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
	const Trace * getTraceContext() const {
		return m_ctx;
	}
	
	virtual ~DataType() {
	};
	protected:
	/**
	 * \brief Default constructor for the datatype class - cannot fully construct a datatype.
	 */
	DataType(Trace * t) : m_ctx(t) {
	};
	
	/**
	* \brief instantiate a new instance of the datatype
	* @param addr The start address
	* @return the instantiated object, NULL on error
	*/
	virtual MemlocData * instantiate(address_t addr) const = 0;
	
private:
	friend class Trace;
	Trace * m_ctx;
	std::string m_displayname;
	
};

typedef boost::shared_ptr<DataType> sp_DataType;
#endif
