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

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <climits> // workaround for boost < 1.38

#include "i_memlocdata.h"
#include "i_projectmodel.h"
#include "memorybackedprojectmodel.h"
#include "document.h"
#include "localpythoninterpreter.h"

// TODO: remove this
#include "arch/arm/arm.h"
#include "guiglue.h"
#include "run_queue.h"

Workspace::Workspace()
{
	// Create all the constituent parts of the Workspace
	m_trace = new MemoryBackedProjectModel(new ARMArchitecture());
	m_pyInterpreter = new LocalPythonInterpreter(this);
	m_docgui = setupWorkspaceGui(*this);
	m_runQueue = createRunQueueControl();
	
	// Setup the new Workspace
	m_pyInterpreter->exec("dismemauto.newWorkspaceSetup(doc)");
	
	postGuiUpdate();
	
}

I_ProjectModel * Workspace::getProjectModel()
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

