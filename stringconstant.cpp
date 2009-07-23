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

#include "datatype.h"
#include "i_memlocdata.h"
#include "memlocdata.h"
#include "i_projectmodel.h"

class StringConstantDataType : public DataType {
public:
	
	StringConstantDataType(I_ProjectModel * t);
	
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
		virtual I_MemlocData * instantiate(address_t addr) const
	{
		address_t stadr = addr;
		
		uint64_t len = 0;
		std::string contents;
		
		uint8_t unitchar;
		bool done = false;

		contents += "\"";
		
		while (!done)
		{
			if (!getI_ProjectModelContext()->readByte(addr, &unitchar))
			{
				break;
			}
			
			if (unitchar == 0)
				done = true;
			
			if (isprint(unitchar))
				contents += unitchar;
			else{
				switch (unitchar)
				{
					case '\"':
						contents += "\\\"";
						break;
					case '\n':
						contents += "\\n";
						break;
						
					case '\r':
						contents += "\\r";
						break;
						
					case '\t':
						contents += "\\t";
						break;
					case 0:
						break;
					default:
						char buf[8];
						snprintf(buf,7,"\\x%02x",unitchar); 
						contents += buf;
				}
			}
			addr++;
			len++;
		}
		contents += "\",0";
		
		return new StringConstant(this, getI_ProjectModelContext(), stadr, len, contents);
	}
	
private:
		
		class StringConstant : public MemlocData {
public:
			
			
			virtual ~StringConstant() {};
			
			virtual u32	get_length() const
			{
				return m_len;
			}
			
			virtual bool is_executable() const {
				return false;
			};
			virtual bool logically_continues() const {
				return false;
			};
			
			/* The slist requirement will go away later when this returns tokens */
			virtual const std::string get_textual()
			{
				return m_data;
			}
			
protected:
			StringConstant(const DataType * creator, const I_ProjectModel * ctx, address_t address, uint64_t len, std::string data) : MemlocData(creator, ctx, address, m_len), m_len(len), m_data(data)
			{}
			
			uint64_t m_len;
			std::string m_data;
			friend class StringConstantDataType;	
private:
		};
};

StringConstantDataType::StringConstantDataType(I_ProjectModel * t) : DataType(t)
{
}

const std::string StringConstantDataType::getName() const
{
	return "ascii_string";
}

u32 StringConstantDataType::getElemSize() const
{
	return 0;
}

void StringConstantDataType::rename(std::string newname)
{
	return;
}
bool StringConstantDataType::isMutable() const
{
	return false;
}
#include <memory>

#include <boost/python.hpp>
void bindStringConstantDataType()
{
	boost::python::class_<StringConstantDataType, boost::python::bases<DataType>,  boost::shared_ptr<StringConstantDataType> >
	("StringConstantDataType", boost::python::init<I_ProjectModel *>());
}
