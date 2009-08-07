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

#include "arm.h"
#include "i_memlocdata.h"
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


const std::string decodeText(const I_ProjectModel * ctx, u32 instr, const address_t & addr);

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

	
	
	/**
	 * \brief Default constructor for the datatype class - cannot fully construct a datatype.
	 */
	ARMInstructionDataType(const I_ProjectModel * t) : DataType(t) {
	};

	/**
	 * \brief instantiate a new instance of the datatype
	 * @param addr The start address
	 * @return the instantiated object, NULL on error
	 */
	virtual const DataTypeDecoding decode(const address_t & addr) const {
		u32 opcode;
		
		if (ldw(addr, &opcode) )
			// TODO: needs work, it won't decode worth shite.
			return DataTypeDecoding(decodeText(getProjectModelContext(), opcode, addr), 4, 0, NULL);
		
		// If we can't decode, return nothing
		return DataTypeDecoding();
	};
	
private:
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
	
};

void ARMArchitecture::bind_type()
{
	/*boost::python::class_<ARMArchitecture, boost::python::bases<Architecture>,  boost::shared_ptr<ARMArchitecture> >
	 ("ARMArchitecture");
	 */
	boost::python::class_<ARMInstructionDataType, boost::python::bases<DataType>,  boost::shared_ptr<ARMInstructionDataType> >
	("ARMInstructionDataType", boost::python::init<I_ProjectModel *>());
}

DataType * ARMArchitecture::getDataType(const I_ProjectModel *t) const
{
	if (!m_adt)
		m_adt = new ARMInstructionDataType(t);
	
	return m_adt;
}
	
/*
Instruction * ARMArchitecture::create_instruction(I_ProjectModel * ctx, address_t addr)
{
	return new ARMInstruction(ctx, addr, ctx->ldw(addr));
}*/
/*
endian_e ARMArchitecture::getArchEndian(void)
{
	// HACK HACK HACK - this should be queried on creation
	return ENDIAN_LITTLE;
}*/
