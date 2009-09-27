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

#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_

#include <string>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/tracking.hpp>

// This has to be included for serialization deps.

class I_ProjectModel;
class LocalPythonInterpreter;
class WorkspaceGui;
class IRunQueueControl;

/**
 * \brief Workspace encapsulates the entire state of a Dismember Workspace
 */
class Workspace {
public: 
	/**
	 * Instantiate a new Workspace - this will go away once the Workspace manager is introduced
	 */
	Workspace();
	
	/**
	 * Get the trace associated with the Workspace
	 * @return the trace associated with the Workspace
	 */
	I_ProjectModel * getProjectModel();
	
	/**
	 * Get the name of the Workspace
	 * @return the name of the Workspace
	 */
	const std::string & getName();
	
	/**
	 * Get the python interpreter for the Workspace
	 * @return the python interpreter for the doc
	 */
	LocalPythonInterpreter * getPythonInterpreter();
	
	/**
	 * Get the run-queue controller
	 */
	IRunQueueControl * getRunQueue();
	
	/**
	 * Get the Workspace's gui
	 */
	WorkspaceGui * getWorkspaceGui();

	/**
	 * Post an update notification to the gui 
	 */
	void postGuiUpdate();
	
	/**
	 * Save the Workspace to a file
	 */
	void saveTo(const std::string & filename);

private:
	
	IRunQueueControl * m_runQueue;
	WorkspaceGui * m_docgui;
	std::string m_name;
	I_ProjectModel * m_trace;
	LocalPythonInterpreter * m_pyInterpreter;
};

#endif


