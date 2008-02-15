/*
 *  document.cpp
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "document.h"
#include "localpythoninterpreter.h"

// TODO: remove this
#include "arch/arm/arm.h"
#include "guiglue.h"

Document::Document()
{
	m_trace = new Trace(new ARMArchitecture());
	m_pyInterpreter = new LocalPythonInterpreter(this);
	m_docgui = setupDocumentGui(*this);
}

Trace * Document::getTrace()
{
	return m_trace;
}

const std::string & Document::getName()
{
	return m_name;
}


LocalPythonInterpreter * Document::getPythonInterpreter()
{
	return m_pyInterpreter;
}