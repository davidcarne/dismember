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
