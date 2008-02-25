/*
 *  bind_codeblocks.cpp
 *  dismember
 *
 *  Created by David Carne on 15/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "bindings.h"
#include "codeblock.h"
#include <boost/python.hpp>

using namespace boost::python;

// BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (foo_overloads, foo, 1, 4)
BOOST_PYTHON_FUNCTION_OVERLOADS (codeBlockAnalysisPass_overloads, codeBlockAnalysisPass, 1, 2)
void bind_codeblocks()
{
	/*def("linkCodeBlocks", linkCodeBlocks);
	def("unlinkCodeBlocks", unlinkCodeBlocks);
	def("codeBlockAnalysisPass", codeBlockAnalysisPass, codeBlockAnalysisPass_overloads());*/
	class_<CodeBlock>("CodeBlock", no_init)
	.def("getStart", &CodeBlock::getStart)
		.def("getEnd", &CodeBlock::getEnd);
	
	class_<CodeBlockManager>("CodeBlockManager", no_init)
		.def("codeBlockAnalysisPassAt", &CodeBlockManager::codeBlockAnalysisPassAt)
		.def("codeBlockAt", &CodeBlockManager::codeBlockAt, return_internal_reference<>());
}
