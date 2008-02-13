#ifndef _DATATYPE_H_
#define _DATATYPE_H_

#include "types.h"
#include "trace.h"
#include "memlocdata.h"
#include <stdexcept>
/**
 * \brief DataType represents the concept of a datatype.
 *
 * DataType exists for easy instantiation of Data + Code types, by name / arch
 * DataType is an interface, IE, it cannot be instantiated.
 */
class DataType {
	public:
	// This doesn't work - always references the base class getName. bah.
	DataType(Trace * t) : m_ctx(t){
	};
	// { t->insertDataType(this->getName(),this); };
	
	/**
	 * \brief Get the name of the datatype [for display]
	 *
	 * @return the name of the dattatype
	 */
	virtual const std::string getName() const {
		return "inval-err";
	};
	

	/**
	 * \brief get the size that this datatype could take up
	 *
	 * @return the size of the datatype, 0 for variable
	 */
	virtual address_t getElemSize() const = 0;
	
	virtual void rename(std::string newname) = 0;
	virtual bool isMutable() const = 0;
	
	// need to add an "editor popup display"
	
	protected:
		
	/**
	* \brief instantiate a new instance of the datatype
	* @param t The trace to which the instance will belong. This does not insert it into the trace, but merely
	*	uses the trace to setup the datatype.
	* @param addr The start address
	* @return the instantiated object, NULL on error
	*/
	virtual MemlocData * instantiateForTraceAndAddress(address_t addr) const = 0;
	
	friend class Trace;
	
	Trace * m_ctx;
	
};

#endif
