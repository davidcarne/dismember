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
