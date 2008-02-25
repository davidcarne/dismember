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
#include <boost/shared_ptr.hpp>
#include "trace.h"
#include "datatype.h"

using namespace boost::python;

// Make sure we retain ownership for this object
void addDataType_wrap(Trace & t, boost::shared_ptr<DataType> a)
{
    t.addDataType(a);
}


void bind_trace()
{
	class_<Trace, boost::noncopyable>("Trace", no_init)
	.def("analyze", &Trace::analyze)
	//.def("undefine", &Trace::undefine)
	.def("createMemlocDataAt", &Trace::createMemlocDataAt)
	.def("addDataType", &addDataType_wrap);
}