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
#include "dsmem_trace.h"
#include "bindings.h"
#include "run_queue.h"

using namespace boost::python;

void bind_document()
{
	class_<Workspace>("Workspace", no_init)
		.add_property("trace", make_function(&Workspace::getProjectModel, return_internal_reference<>() ))
		.add_property("name", make_function(&Workspace::getName, return_value_policy< copy_const_reference >() ))
		.add_property("runqueue", make_function(&Workspace::getRunQueue, return_internal_reference<>() ))
		.def("postGuiUpdate", &Workspace::postGuiUpdate);
}
