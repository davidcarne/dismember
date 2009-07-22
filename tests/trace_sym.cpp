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

#define BOOST_TEST_MODULE trace_sym test

#define BOOST_AUTO_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/included/unit_test_framework.hpp> 
#include "symlist.h"

BOOST_AUTO_TEST_CASE( sym_insert_test )
{
	SymbolList a;
	Symbol * b = a.set_symbol(0x100, "foo");
	BOOST_CHECK(b);
	BOOST_CHECK_EQUAL(b->get_name(), "foo");
	BOOST_CHECK_EQUAL(b->get_addr(), 0x100);
	
	const Symbol * c = a.get_symbol(0x100);
	const Symbol * d = a.get_symbol("foo");
	BOOST_CHECK_EQUAL(b, c);
	BOOST_CHECK_EQUAL(b, d);
	
	const Symbol * cx = a.get_symbol(0x1001);
	const Symbol * dx = a.get_symbol("food");
	BOOST_CHECK(!cx);
	BOOST_CHECK(!dx);
}
