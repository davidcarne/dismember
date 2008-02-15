#include "localpythoninterpreter.h"

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

LocalPythonInterpreter::LocalPythonInterpreter(Document * doc) : m_doc(doc),
	m_stdout_dest(new LocalPythonOutputRedirector(1)), m_stderr_dest(new LocalPythonOutputRedirector(2))
{
	m_gbl = GlobalPythonInterpreter::getGlobalInterpreter();
	m_globalNamespace = m_gbl->getMainNamespace();
	python::object sys_module = python::import("sys");
	
	m_localNamespace = python::dict();
	m_localNamespace["sys"] = sys_module;
	m_localNamespace["doc"] = python::ptr(m_doc);
	m_localNamespace["local"] = python::ptr(this);
	printf("this = %p\n", this);
	execsingle("print local");
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