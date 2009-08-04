/*
 * This code is Copyright David Carne, Licensed under the "Modified BSD license", also known as the 3-clause BSD license.
 * See terms and conditions below.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *	this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *	this list of conditions and the following disclaimer in the documentation 
 *	and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "globalpythoninterpreter.h"
#include "string.h"
#include "localpythoninterpreter.h"

GlobalPythonInterpreter * GlobalPythonInterpreter::s_interpreter = NULL;

GlobalPythonInterpreter::GlobalPythonInterpreter() : m_is_init(false)
{}


python::object GlobalPythonInterpreter::exec(python::str code)
{
	python::object templocal = python::dict();
	
	return python::exec(code, m_main_namespace, templocal);
}

void GlobalPythonInterpreter::addModule(std::string name, moduleInitFunc modinit)
{
	if (m_is_init)
		throw new std::runtime_error("cannot modify python inittab after init()");
	
	PyImport_AppendInittab( strdup(name.c_str()), modinit );
}

void GlobalPythonInterpreter::import(std::string modname)
{
	m_main_namespace[modname] = python::import(strdup(modname.c_str()));
}

python::object GlobalPythonInterpreter::getMainNamespace()
{
	if (!m_is_init)
		throw new std::runtime_error("cannot get namespace without init");
	
	return m_main_namespace;
}

GlobalPythonInterpreter * GlobalPythonInterpreter::getGlobalInterpreter()
{
	if (!s_interpreter)
	{
		s_interpreter = new GlobalPythonInterpreter();
	}
	
	return s_interpreter;
}

void GlobalPythonInterpreter::init()
{
	Py_Initialize();
	
	// Tell python about the local interpreter classes
	LocalPythonInterpreter::setupPythonClasses();
	
	m_main_module = python::import("__main__");
	m_main_namespace = m_main_module.attr("__dict__");
	m_is_init = true;
}
