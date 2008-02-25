/*
 *  bindings.cpp
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "bindings.h"

#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <binaryconstant.h>

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
}

BOOST_PYTHON_MODULE(_dismember_datatype_builtins)
{
	bindBinaryConstantDataTypes();
}