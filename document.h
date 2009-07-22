/*
 *  Workspace.h
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_

#include <string>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/tracking.hpp>

// This has to be included for serialization deps.

class ProjectModel;
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
	ProjectModel * getProjectModel();
	
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
	
	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive & ar,  const unsigned int)
	{
		ar & m_trace & m_name;
	}
	
	IRunQueueControl * m_runQueue;
	WorkspaceGui * m_docgui;
	std::string m_name;
	ProjectModel * m_trace;
	LocalPythonInterpreter * m_pyInterpreter;
};

BOOST_CLASS_VERSION(Workspace, 1)
BOOST_CLASS_TRACKING(Workspace, boost::serialization::track_never)


#endif


