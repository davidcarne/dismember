#include "datatype.h"

class StringConstantDataType : public DataType {
public:
	
	StringConstantDataType(Trace * t);
	
	virtual const std::string getName() const;
	virtual address_t getElemSize() const;
	
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
		virtual MemlocData * instantiateForTraceAndAddress(address_t addr) const
	{
		return new StringConstant(this, m_ctx, addr);
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
			StringConstant(const DataType * creator, Trace * ctx, address_t address) : MemlocData(creator, ctx, address)
			{
				m_len = 0;
				uint8_t data;
				bool done = false;
				while (!done)
				{
					if (!ctx->readByte(address, &data))
					{
						break;
					}
					
					if (data == 0)
						done = true;
					
					if (isprint(data))
						m_data += data;
					else{
						switch (data)
						{
							case '\n':
								m_data += "\\n";
								break;
								
							case '\r':
								m_data += "\\r";
								break;
								
							case '\t':
								m_data += "\\t";
								break;
							case 0:
								break;
							default:
								char buf[8];
								snprintf(buf,7,"\\x%02x",data); 
								m_data += buf;
						}
					}
					address++;
					m_len++;
				}
			}
			address_t m_len;
			std::string m_data;
			friend class StringConstantDataType;	
private:
		};
};

StringConstantDataType::StringConstantDataType(Trace * t) : DataType(t)
{
	t->insertDataType(this->getName(),this);
}

const std::string StringConstantDataType::getName() const
{
	return "ascii_string";
}

address_t StringConstantDataType::getElemSize() const
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
void createStringConstantDataTypes(Trace * t)
{
	new StringConstantDataType(t);
}
