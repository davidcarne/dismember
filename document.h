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

// Predeclare to avoid forever compile times, since this is included in lots of places
class Trace;
class LocalPythonInterpreter;
class DocumentGui;

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
	
private:
	DocumentGui * m_docgui;
	std::string m_name;
	Trace * m_trace;
	LocalPythonInterpreter * m_pyInterpreter;
};


#endif


