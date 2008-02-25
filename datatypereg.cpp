#include "datatypereg.h"

DataTypeReg::DataTypeReg()
{
	
}

DataTypeReg::datatypereg_ci DataTypeReg::getBegin() const
{
	return m_datatypes.begin();
}

DataTypeReg::datatypereg_ci DataTypeReg::getEnd() const
{
	return m_datatypes.end();
}
	
sp_DataType DataTypeReg::lookupDataType(std::string name) const
{
	datatypereg_ci i = m_datatypes.find(name);
	if (i != m_datatypes.end())
		return (*i).second;
	return sp_DataType();
}

void DataTypeReg::insertDataType(std::string name, sp_DataType d)
{
	m_datatypes[name] = d;
}
