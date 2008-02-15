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


#ifndef _GLOBALPYTHON_H_
#define _GLOBALPYTHON_H_

#include <boost/python.hpp>
#include <string>

using namespace boost;


/**
 * \brief Global python interpreter - manages the main namespace.
 */
class GlobalPythonInterpreter {
public:
	GlobalPythonInterpreter();
	
	/**
	 * \brief Callback type called during initial import of a module
	 */
	typedef void (*moduleInitFunc)(void);
	
	/**
	 * \brief Add a module to the global interpreter module table
	 * Add a module to the global interpreter module table. 
	 * \throws std::runtime_error if the interpreter has been inited
	 */
	void addModule(std::string name, moduleInitFunc modinit);
	
	/**
	 * \brief import a module into the global namespace
	 */
	void import(std::string modname);

	/**
	 * \brief Initialize the interpreter 
	 */
	void init();
	
	/**
	 * Get a pointer to the global interpreter
	 */
	static GlobalPythonInterpreter * getGlobalInterpreter();
	
private:
	python::object getMainNamespace();
	
	friend class LocalPythonInterpreter;
	
	static GlobalPythonInterpreter * s_interpreter;
	python::object m_main_namespace;
	python::object m_main_module;
	
	bool m_is_init;
};

#endif
