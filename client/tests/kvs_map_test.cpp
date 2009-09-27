#include <boost/test/unit_test.hpp>
#include "kvs/i_kvs.h"
#include "kvs/localkvsstore.h"

#include "kvs/kvs_templates.h"

#include <iostream>


// This test case more for compile-time requirements checking
BOOST_AUTO_TEST_CASE(locking_iterator_casts)
{
	KVS_map<std::string, std::string> test_map;
	test_map["hello"] =   "world";
	test_map["goodbye"] = "moon";
	
	typedef KVS_map<std::string, std::string>::iterator i_t;
	typedef KVS_map<std::string, std::string>::const_iterator ci_t;

	i_t i_begin = test_map.begin();
	i_t i_end = test_map.end();
	
	ci_t ci_begin = test_map.begin();
	ci_t ci_end = test_map.end();
	
	BOOST_CHECK(i_begin == ci_begin);
	BOOST_CHECK(i_end == ci_end);
	BOOST_CHECK(i_begin != i_end);
	BOOST_CHECK(ci_begin != ci_end);
	
	i_begin++;
	ci_begin++;
	BOOST_CHECK(i_begin == ci_begin);
	BOOST_CHECK(ci_begin != ci_end);
	BOOST_CHECK(ci_begin != i_end);
	BOOST_CHECK(i_begin != ci_end);
	
	
	i_begin++;
	ci_begin++;
	BOOST_CHECK(i_begin == ci_begin);
	BOOST_CHECK(ci_begin == ci_end);
	BOOST_CHECK(i_begin == i_end);
}


BOOST_AUTO_TEST_CASE(basicMap)
{
	KVS_map<std::string, std::string> test_map;
	typedef KVS_map<std::string, std::string>::const_iterator ci_t;
	
	test_map["hello"] = "world";
	test_map["goodbye"] = "moon";
	
	for (ci_t ci = test_map.begin(); ci != test_map.end(); ci++)
	{
		std::cout << (*ci).first << " " << (*ci).second << std::endl;
	}
}