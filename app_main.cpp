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
	// Setup the global python interpreters
	GlobalPythonInterpreter * gblinterp = GlobalPythonInterpreter::getGlobalInterpreter();
	// TODO: here we need to add the object model funcs with
	gblinterp->addModule( "dismember", &initdismember );
	
	gblinterp->init();
	gblinterp->import("dismember");
					  
	// Create an initial document
}