#include "abstractdata.h"
#include <stdexcept>
#include <typeinfo>

GenericAbstractData::GenericAbstractData(int num)
 : m_type(NUMBER) { data.number = num; }
GenericAbstractData::GenericAbstractData(bool b)
 : m_type(BOOLEAN) { data.boolean = b; }
GenericAbstractData::GenericAbstractData(std::string str)
 : m_type(TEXT) { data.text = str; }

GenericAbstractData::~GenericAbstractData() {}

GenericAbstractData::DataType GenericAbstractData::type()
{ return m_type; }

std::string GenericAbstractData::text()
{
	if (m_type != TEXT)
		throw std::bad_cast();
	return data.text;
}
int GenericAbstractData::number()
{
	if (m_type != NUMBER)
		throw std::bad_cast();
	return data.number;
}

bool GenericAbstractData::boolean()
{
	if (m_type != BOOLEAN)
		throw std::bad_cast();
	return data.boolean;
}

