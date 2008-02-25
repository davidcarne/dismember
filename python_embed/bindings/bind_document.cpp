/*
 *  document.cpp
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "document.h"
#include <boost/python.hpp>
#include "trace.h"
#include "bindings.h"

using namespace boost::python;

void bind_document()
{
	class_<Document>("Document", no_init)
		.add_property("trace", make_function(&Document::getTrace, return_internal_reference<>() ))
		.add_property("name", make_function(&Document::getName, return_internal_reference<>() ))
		.def("postGuiUpdate", &Document::postGuiUpdate);
}