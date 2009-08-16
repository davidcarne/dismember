#define CXXTEST_ABORT_TEST_ON_FAIL

#include <cxxtest/TestSuite.h>

#include "kvs/kvs_node.h"
#include "kvs/i_kvs.h"
#include "kvs/localkvsstore.h"

class MyTestSuite : public CxxTest::TestSuite 
{
	
public:
	void testDanglingAttribute( void )
	{
		LocalKVSStore s;
		
		I_KVS  & i_s = s;
		
		TS_ASSERT(i_s.createDanglingAttributes());
	}
	
	void testNonExistantGetNode( void )
	{
		LocalKVSStore s;
		
		I_KVS  & i_s = s;
		
		TS_ASSERT(i_s.getNode("/foobar"));
	}
	
	void testSetGetOffRoot( void )
	{
		LocalKVSStore s;
		
		I_KVS  & i_s = s;
		
		i_s.setValue("/foobar", "hello");
		TS_ASSERT_EQUALS(i_s.getValue("/foobar"), "hello");
	}
	
	void testNodePaths( void )
	{
		LocalKVSStore s;
		
		I_KVS  & i_s = s;
		
		i_s.setValue("/foobar", "hello");
		
		sp_I_KVS_node fbnode;
		TS_ASSERT(fbnode = i_s.getNode("/foobar"));
		
		TS_ASSERT_EQUALS(fbnode->getPath(), "/foobar");
		TS_ASSERT_EQUALS(fbnode->getKey(), "foobar");
	}
	
	void testRootNodeAttributes( void )
	{
		LocalKVSStore s;
		
		I_KVS  & i_s = s;
		CxxTest::setAbortTestOnFail( true );
		
		i_s.setValue("/foobar", "hello");
		
		sp_I_KVS_node fbnode;
		TS_ASSERT(fbnode = i_s.getNode("/"));
		
		sp_I_KVS_attributes fb_attribs;
		TS_ASSERT(fb_attribs = fbnode->getAttributesReference());
		
		TS_ASSERT_EQUALS(fb_attribs->getAttrib("foobar"), "hello");
	}
	
};


