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

#include "workspace.h"
#include <boost/python.hpp>
#include "i_projectmodel.h"
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
