/*
 *  bind_trace.cpp
 *  dismember
 *
 *  Created by David Carne on 15/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "bindings.h"

#include <boost/python.hpp>
#include "trace.h"
#include "datatype.h"

using namespace boost::python;

void bind_trace()
{
	class_<Trace>("Trace", no_init)
	.def("analyze", &Trace::analyze)
	.def("undefine", &Trace::undefine)
	.def("createMemlocDataAt", &Trace::createMemlocDataAt);
}