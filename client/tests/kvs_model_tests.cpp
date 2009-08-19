
#include <unistd.h>

#define BOOST_TEST_MODULE LocalKVSStore_Tests


#include <boost/test/unit_test.hpp>

#include "kvs/kvs_node.h"
#include "kvs/i_kvs.h"
#include "kvs/localkvsstore.h"

BOOST_AUTO_TEST_CASE(CreateDanglingAttribute)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	BOOST_CHECK(i_s.createDanglingAttributes());
}




