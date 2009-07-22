/*
 *  Workspace.cpp
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <climits> // workaround for boost < 1.38

#include <boost/archive/tmpdir.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>

#include <boost/serialization/set.hpp>
#include <boost/serialization/map.hpp>

#include "memlocdata.h"
#include "dsmem_trace.h"
#include "document.h"
#include "localpythoninterpreter.h"

// TODO: remove this
#include "arch/arm/arm.h"
#include "guiglue.h"
#include "run_queue.h"

Workspace::Workspace()
{
	// Create all the constituent parts of the Workspace
	m_trace = new ProjectModel(new ARMArchitecture());
	m_pyInterpreter = new LocalPythonInterpreter(this);
	m_docgui = setupWorkspaceGui(*this);
	m_runQueue = createRunQueueControl();
	
	// Setup the new Workspace
	m_pyInterpreter->exec("dismemauto.newWorkspaceSetup(doc)");
	
	postGuiUpdate();
	
}

ProjectModel * Workspace::getProjectModel()
{
	return m_trace;
}

const std::string & Workspace::getName()
{
	return m_name;
}


LocalPythonInterpreter * Workspace::getPythonInterpreter()
{
	return m_pyInterpreter;
}


IRunQueueControl * Workspace::getRunQueue()
{
	return m_runQueue;
}

WorkspaceGui * Workspace::getWorkspaceGui()
{
	return m_docgui;
}

void Workspace::postGuiUpdate()
{
	
	m_docgui->postUpdate();
}

void Workspace::saveTo(const std::string & filename)
{
	std::ofstream ofs(filename.c_str(), std::ios::binary);
	boost::archive::text_oarchive oa(ofs);
	oa << this;
}
