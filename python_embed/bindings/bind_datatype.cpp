/*
 *  bind_datatype.cpp
 *  dismember
 *
 *  Created by David Carne on 24/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "datatype.h"

#include <boost/python.hpp>
using namespace boost::python;

void bind_datatype()
{
	class_<DataType, boost::noncopyable>("DataType", no_init)
	.add_property("name", make_function(&DataType::getName ))
	.add_property("displayname", make_function(&DataType::getDisplayName/*,  return_value_policy<copy_const_reference>()*/ ))
	.add_property("elemsize", &DataType::getElemSize)
	.add_property("ismutable", &DataType::isMutable);
}