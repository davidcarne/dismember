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


#ifndef _LOCAL_PYTHON_H_
#define _LOCAL_PYTHON_H_
#include <boost/python.hpp>
#include "globalpythoninterpreter.h"
#include <boost/utility.hpp>
#include <stdexcept>
#include "document.h"

/**
 * Interface for an output that the python output redirector can write to
 */
class LPIOutputStreamAcceptor {
public:
	/**
	 * Write to the Output stream acceptor
	 */
	virtual void write(const std::string &, int stream) = 0;
};

/**
 * Output redirector thats fed to python for sys.stdout + sys.stderr
 */
class LocalPythonOutputRedirector {

public:
	
	/**
	 * Set the output to which the incoming data will be written
	 */
	void setOutputAcceptor(LPIOutputStreamAcceptor * a);
	
	/**
	 * Write data to the output redirector
	 */
	void write(std::string data);
	
private:
	friend class LocalPythonInterpreter;
	LocalPythonOutputRedirector(int stream);
	int m_stream;
	LPIOutputStreamAcceptor * m_dest;
};

/**
 * Document specific python interpreter. Plays tricks with python state to make it seem local
 */
class LocalPythonInterpreter
{

	
public:
	/**
	 * 
	 */
	LocalPythonInterpreter(Document * doc);
	python::object exec(python::str code);
	python::object eval(python::str code);
	python::object execsingle(python::str code);
	
	LocalPythonOutputRedirector * getStdoutRedirector();
	LocalPythonOutputRedirector * getStderrRedirector();
	
private:
	Document * m_doc;
	
	friend class GlobalPythonInterpreter;
	static void setupPythonClasses();
	void setupLocalNamespaceBindings();
	GlobalPythonInterpreter * m_gbl;
	LocalPythonOutputRedirector * m_stdout_dest;
	LocalPythonOutputRedirector * m_stderr_dest;
	python::object m_localNamespace;
	python::object m_globalNamespace;
};
#endif

