/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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
	/*class_<CodeBlock>("CodeBlock", no_init)
	.def("getStart", &CodeBlock::getStart)
		.def("getEnd", &CodeBlock::getEnd);
	
	class_<CodeBlockManager>("CodeBlockManager", no_init)
		.def("codeBlockAnalysisPassAt", &CodeBlockManager::codeBlockAnalysisPassAt)
		.def("codeBlockAt", &CodeBlockManager::codeBlockAt, return_internal_reference<>());*/
}
