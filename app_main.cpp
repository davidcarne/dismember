/*
 *  app_main.cpp
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
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
		
		gblinterp->init();
	
	
		gblinterp->exec("import sys;\nsys.path.append(\"./pythonsrc\")");

		gblinterp->import("dismember");
		gblinterp->import("dismemauto");
		
	} catch(.../* boost::python::error_already_set*/ ) {
		PyErr_Print();
	}
	
	// Create an initial document
}