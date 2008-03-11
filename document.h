/*
 *  document.h
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _document_H_
#define _document_H_

#include <string>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/tracking.hpp>

// This has to be included for serialization deps.

class Trace;
class LocalPythonInterpreter;
class DocumentGui;
class IRunQueueControl;

/**
 * \brief Document encapsulates the entire state of a Dismember document
 */
class Document {
public: 
	/**
	 * Instantiate a new document - this will go away once the document manager is introduced
	 */
	Document();
	
	/**
	 * Get the trace associated with the document
	 * @return the trace associated with the document
	 */
	Trace * getTrace();
	
	/**
	 * Get the name of the document
	 * @return the name of the document
	 */
	const std::string & getName();
	
	/**
	 * Get the python interpreter for the document
	 * @return the python interpreter for the doc
	 */
	LocalPythonInterpreter * getPythonInterpreter();
	
	/**
	 * Get the run-queue controller
	 */
	IRunQueueControl * getRunQueue();
	
	/**
	 * Post an update notification to the gui 
	 */
	void postGuiUpdate();
	
	/**
	 * Save the document to a file
	 */
	void saveTo(const std::string & filename);

private:
	
	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive & ar,  const unsigned int)
	{
		ar & m_trace & m_name;
	}
	
	IRunQueueControl * m_runQueue;
	DocumentGui * m_docgui;
	std::string m_name;
	Trace * m_trace;
	LocalPythonInterpreter * m_pyInterpreter;
};

BOOST_CLASS_VERSION(Document, 1)
BOOST_CLASS_TRACKING(Document, boost::serialization::track_never)


#endif


