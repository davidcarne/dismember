#include "arm.h"
#include "../../memlocdata.h"
#include "arm_instruction.h"
#include "datatype.h"

#include <boost/python.hpp>

ARMArchitecture::ARMArchitecture()
{
	m_adt = NULL;
}

std::string ARMArchitecture::get_short_name() const
{
	return "ARM";
}



class ARMInstructionDataType : public DataType {
public:

	virtual const std::string getName() const
	{
		return "ARMInstruction";
	}

	virtual u32 getElemSize() const 
	{
		return 0;
	}

	virtual bool isMutable() const
	{
		return false;
	}
	
	virtual ~ARMInstructionDataType() {
	};

	// TODO: Remove hack
	bool ldw(address_t addr, u32 * data) const
	{
		u8 dataar[4];
		if (!getProjectModelContext()->readBytes(addr, 4, dataar))
			return false;
		*data = dataar[3];
		*data <<= 8;
		*data |= dataar[2];
		*data <<= 8;
		*data |= dataar[1];
		*data <<= 8;
		*data |= dataar[0];
		return true;
	}
	
	/**
	 * \brief Default constructor for the datatype class - cannot fully construct a datatype.
	 */
	ARMInstructionDataType(ProjectModel * t) : DataType(t) {
	};

	/**
	 * \brief instantiate a new instance of the datatype
	 * @param addr The start address
	 * @return the instantiated object, NULL on error
	 */
	virtual MemlocData * instantiate(address_t addr) const {
		u32 data;
		if (ldw(addr, &data) )
			return new ARMInstruction(getProjectModelContext(), addr, data);
		
		return NULL;
	};
	
};

void ARMArchitecture::bind_type()
{
	/*boost::python::class_<ARMArchitecture, boost::python::bases<Architecture>,  boost::shared_ptr<ARMArchitecture> >
	 ("ARMArchitecture");
	 */
	boost::python::class_<ARMInstructionDataType, boost::python::bases<DataType>,  boost::shared_ptr<ARMInstructionDataType> >
	("ARMInstructionDataType", boost::python::init<ProjectModel *>());
}

DataType * ARMArchitecture::getDataType(ProjectModel *t) const
{
	if (!m_adt)
		m_adt = new ARMInstructionDataType(t);
	
	return m_adt;
}
	
/*
Instruction * ARMArchitecture::create_instruction(ProjectModel * ctx, address_t addr)
{
	return new ARMInstruction(ctx, addr, ctx->ldw(addr));
}*/
/*
endian_e ARMArchitecture::getArchEndian(void)
{
	// HACK HACK HACK - this should be queried on creation
	return ENDIAN_LITTLE;
}*/
