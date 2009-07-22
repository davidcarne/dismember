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
