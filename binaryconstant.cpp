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

#include "dsmem_trace.h"
#include "memlocdata.h"
#include "binaryconstant.h"
#include "datatype.h"


#include <memory>

class BinaryConstantDataType : public DataType {
public:
	
	BinaryConstantDataType(ProjectModel * t, std::string name, nparse_e type, u32 len, bool big_endian, 
						   nparse_sign_e is_signed, u32 excessoffs = 0);
	
	virtual const std::string getName() const;
	virtual u32 getElemSize() const;
	
	virtual void rename(std::string newname);
	virtual bool isMutable() const;
	// need to add an "editor popup display"
	
protected:
		
		/**
		* \brief instantiate a new instance of the datatype
		 * @param t The trace to which the instance will belong. This does not insert it into the trace, but merely
		 *	uses the trace to setup the datatype.
		 * @param addr The start address
		 * @return the instantiated object, NULL on error
		 */
		virtual MemlocData * instantiate(address_t addr) const;
	
private:
		
		class BinaryConstant : public MemlocData {
public:
			
			
			virtual ~BinaryConstant() {};
			
			virtual u32	get_length() const;
			
			virtual bool is_executable() const;
			virtual bool logically_continues() const;
			
			/* The slist requirement will go away later when this returns tokens */
			virtual const std::string get_textual();
			
			
protected:
				BinaryConstant(const DataType * creator, const ProjectModel * ctx, address_t address, u32 elemsize, 
							enum endian_e endian = ENDIAN_LITTLE);
			
			friend class BinaryConstantDataType;	
private:
				u32 m_elemsize;
			
			enum endian_e m_endian;
		};
	
	
	std::string m_name;
	size_t m_elemsize;
};


const DataType * datatype_u8_le = NULL;
const DataType * datatype_s8_le = NULL;

const DataType * datatype_u16_le = NULL;
const DataType * datatype_s16_le = NULL;

const DataType * datatype_u32_le = NULL;
const DataType * datatype_s32_le = NULL;

const DataType * datatype_u64_le = NULL;
const DataType * datatype_s64_le = NULL;

/**
 * \brief Create a new BinaryConstantDataType
 */
BinaryConstantDataType::BinaryConstantDataType(ProjectModel * t, std::string name, nparse_e type, 
											   u32 len, bool big_endian, nparse_sign_e is_signed, u32 excessoffs) : 
	DataType(t), m_name(name), m_elemsize(len)
{
}

const std::string BinaryConstantDataType::getName() const
{
	return m_name;
}

u32 BinaryConstantDataType::getElemSize() const
{
	return m_elemsize;
}


void BinaryConstantDataType::rename(std::string newname)
{
	m_name = newname;
}

bool BinaryConstantDataType::isMutable() const
{
	return false;
}

MemlocData * BinaryConstantDataType::instantiate(address_t addr) const
{
	return new BinaryConstant(this, getProjectModelContext(), addr, m_elemsize);
}

BinaryConstantDataType::BinaryConstant::BinaryConstant(const DataType * creator, 
													   const ProjectModel * ctx, address_t address, 
													   u32 elemsize, enum endian_e endian) : 
	MemlocData(creator, ctx, address, elemsize), m_elemsize(elemsize), m_endian(endian)
{
}


u32	BinaryConstantDataType::BinaryConstant::get_length() const
{
	return m_elemsize;
}

bool BinaryConstantDataType::BinaryConstant::is_executable() const
{
	return false;
}

/* The slist requirement will go away later when this returns tokens */
const std::string BinaryConstantDataType::BinaryConstant::get_textual()
{
	std::string res="";
	switch (m_elemsize)
	{
		case 1:
			res += ".db";
			break;
			
		case 2:
			res += ".dh";
			break;
			
		case 4:
			res += ".dw";
			break;
		
		case 8:
			res += ".dq";
			break;
			
		default:
			res += ".d?";
	}
	res += "\t";

	res += "0x";
	for (u32 bind = 0; bind< m_elemsize; bind++)
	{
		char bytebuf[3]="??";
		
		address_t addr;
		
		if (m_endian == ENDIAN_LITTLE)
			addr = get_addr() + (m_elemsize - bind - 1);
		else
			addr = get_addr() + bind;
		
		
		uint8_t data;
		
		if (get_ctx()->readByte(addr, &data))
			snprintf(bytebuf, 3, "%02x", data);
		else
			snprintf(bytebuf, 3, "??");
		res += bytebuf;
	}

	
	return res;
}


bool BinaryConstantDataType::BinaryConstant::logically_continues() const {
	return false;
}
#include <memory>

#include <boost/python.hpp>
void bindBinaryConstantDataTypes()
{//BinaryConstantDataType(ProjectModel * t, std::string name, nparse_e type, u32 len, bool big_endian, nparse_sign_e is_signed, u32 excessoffs = 0);
	boost::python::class_<BinaryConstantDataType, boost::python::bases<DataType>,  boost::shared_ptr<BinaryConstantDataType> >
	("BinaryConstantDataType", boost::python::init<ProjectModel *, std::string, nparse_e, u32, bool, nparse_sign_e>());
	
}

// HACK HACK HACK
/*
void createBinaryConstantDataTypes(ProjectModel * t)
{
	if (!datatype_u8_le) datatype_u8_le = new BinaryConstantDataType(t, "u8_le", BTYPE_PLAIN, 1, false, BSIGN_NONE);
	if (!datatype_u16_le) datatype_u16_le = new BinaryConstantDataType(t, "u16_le", BTYPE_PLAIN, 2, false, BSIGN_NONE);
	if (!datatype_u32_le) datatype_u32_le = new BinaryConstantDataType(t, "u32_le", BTYPE_PLAIN, 4, false, BSIGN_NONE);
	if (!datatype_u64_le) datatype_u64_le = new BinaryConstantDataType(t, "u64_le", BTYPE_PLAIN, 8, false, BSIGN_NONE);
	
	if (!datatype_s8_le) datatype_s8_le = new BinaryConstantDataType(t, "s8_le", BTYPE_PLAIN, 1, false, BSIGN_STD);
	if (!datatype_s16_le) datatype_s16_le = new BinaryConstantDataType(t, "s16_le", BTYPE_PLAIN, 2, false, BSIGN_STD);
	if (!datatype_s32_le) datatype_s32_le = new BinaryConstantDataType(t, "s32_le", BTYPE_PLAIN, 4, false, BSIGN_STD);
	if (!datatype_s64_le) datatype_s64_le = new BinaryConstantDataType(t, "s64_le", BTYPE_PLAIN, 8, false, BSIGN_STD);
}*/
