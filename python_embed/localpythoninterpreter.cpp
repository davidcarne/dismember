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

#include "localpythoninterpreter.h"


LPIOutputStreamAcceptor::~LPIOutputStreamAcceptor()
{}

LocalPythonOutputRedirector::LocalPythonOutputRedirector(int x) : m_stream(x), m_dest(NULL) {
}

void LocalPythonOutputRedirector::setOutputAcceptor(LPIOutputStreamAcceptor * a)
{
	m_dest = a;
}

void LocalPythonOutputRedirector::write(std::string data)
{
	
	//printf("Write: this = %p\n", this);
	if (m_dest)
		m_dest->write(data, m_stream);
	else
		printf("emerg: ::%d:>>> %s <<<\n", m_stream, data.c_str());
}

LocalPythonInterpreter::LocalPythonInterpreter(Workspace * doc) : m_doc(doc),
	m_stdout_dest(new LocalPythonOutputRedirector(1)), m_stderr_dest(new LocalPythonOutputRedirector(2))
{
	m_gbl = GlobalPythonInterpreter::getGlobalInterpreter();
	m_globalNamespace = m_gbl->getMainNamespace();
	python::object sys_module = python::import("sys");
	
	m_localNamespace = python::dict();
	m_localNamespace["sys"] = sys_module;
	m_localNamespace["doc"] = python::ptr(m_doc);
	m_localNamespace["local"] = python::ptr(this);
	//printf("this = %p\n", this);
	//execsingle("print local");
}

python::object LocalPythonInterpreter::exec(python::str code)
{
	setupLocalNamespaceBindings();
	return python::exec(code, m_globalNamespace, m_localNamespace);
}

python::object LocalPythonInterpreter::eval(python::str code)
{
	setupLocalNamespaceBindings();
	return python::eval(code, m_globalNamespace, m_localNamespace);
}

python::object LocalPythonInterpreter::execsingle(python::str code)
{
	setupLocalNamespaceBindings();
	char *s = python::extract<char *>(code);
	PyObject* result = PyRun_String(s, Py_single_input, m_globalNamespace.ptr(), m_localNamespace.ptr());
	if (!result) python::throw_error_already_set();
	return python::object(python::detail::new_reference(result));
}

void LocalPythonInterpreter::setupLocalNamespaceBindings()
{
	m_localNamespace["sys"].attr("stdout") = m_localNamespace["local"].attr("stdout");
	m_localNamespace["sys"].attr("stderr") = m_localNamespace["local"].attr("stderr");
}



LocalPythonOutputRedirector * LocalPythonInterpreter::getStdoutRedirector()
{
	return m_stdout_dest;
}

LocalPythonOutputRedirector * LocalPythonInterpreter::getStderrRedirector()
{
	return m_stderr_dest;
}
void LocalPythonInterpreter::setupPythonClasses()
{
	// Tell python about the output redirector class
	python::class_<LocalPythonOutputRedirector>("LocalPythonOutputRedirector", python::no_init)
	.def("write",&LocalPythonOutputRedirector::write);
	
	// Tell python about the localinterpreter class
	python::class_<LocalPythonInterpreter>("LocalPythonInterpreter", python::no_init)
	.add_property("stdout", python::make_function(&LocalPythonInterpreter::getStdoutRedirector, python::return_internal_reference<>() ))
	.add_property("stderr", python::make_function(&LocalPythonInterpreter::getStderrRedirector, python::return_internal_reference<>() ));
}
