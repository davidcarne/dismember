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

#include "app_main.h"
#include "globalpythoninterpreter.h"
#include "bindings.h"

void app_main()
{
	try {
		
		// Setup the global python interpreters
		GlobalPythonInterpreter * gblinterp = GlobalPythonInterpreter::getGlobalInterpreter();
		
		gblinterp->addModule( "_dismember_core", &init_dismember_core );
		gblinterp->addModule( "_dismember_datatype_builtins", &init_dismember_datatype_builtins );
		gblinterp->addModule( "_dismember_analysis_builtins", &init_dismember_analysis_builtins );
		
		gblinterp->init();
	
	
		gblinterp->exec("import sys;\nsys.path.append(\"./pythonsrc\")");

		gblinterp->import("dismember");
		gblinterp->import("dismemauto");
		
	} catch(.../* boost::python::error_already_set*/ ) {
		PyErr_Print();
	}
	
	// Create an initial document
}
