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

#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include "binaryconstant.h"
#include "stringconstant.h"

/*** NOTE - These are all split out to greatly reduce compile times. C++ template metaprogramming isn't terribly fast
 * and its much more pleasant if only the bare necessities are rebuilt
 */
BOOST_PYTHON_MODULE(_dismember_core)
{
	bind_document();
	bind_trace();
	bind_codeblocks();
	bind_datatype();
	bind_types();
	bind_arch();
	bind_runqueue();
}

BOOST_PYTHON_MODULE(_dismember_datatype_builtins)
{
	bindBinaryConstantDataTypes();	
	bindStringConstantDataType();
}
/*
BOOST_PYTHON_MODULE(_dismember_arch_builtins)
{
	bindBinaryConstantDataTypes();	
	bindStringConstantDataType();
}*/

BOOST_PYTHON_MODULE(_dismember_analysis_builtins)
{
	bind_analysis();
}
