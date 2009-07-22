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
#include "dsmem_trace.h"
#include "datatype.h"
#include "program_flow_analysis.h"

using namespace boost::python;

// Make sure we retain ownership for this object
void addDataType_wrap(ProjectModel & t, boost::shared_ptr<DataType> a)
{
    t.addDataType(a);
}


void bind_trace()
{
	class_<ProjectModel, boost::noncopyable>("ProjectModel", no_init)
	//.def("undefine", &ProjectModel::undefine)
	//.def("createMemlocDataAt", &ProjectModel::createMemlocDataAt)
	.add_property("defdatatype", make_function(&ProjectModel::getCodeDataType, return_internal_reference<>() ))
	.def("addDataType", &addDataType_wrap);
}
