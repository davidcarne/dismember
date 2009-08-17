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
		
		TS_ASSERT(!i_s.getNode("/foobar"));
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
	
	void testMultiSectionPath (void)
	{
		LocalKVSStore s;
		I_KVS & i_s = s;
		CxxTest::setAbortTestOnFail( true );
		
		i_s.setValue("/foobar/frobber", "hello");
		TS_ASSERT_EQUALS(i_s.getValue("/foobar"), "");
		TS_ASSERT_EQUALS(i_s.getValue("/foobar/frobber"), "hello");
		
		sp_I_KVS_node fbnode;
		sp_I_KVS_attributes fb_attribs;
		
		TS_ASSERT(fbnode = i_s.getNode("/"));
		TS_ASSERT(fb_attribs = fbnode->getAttributesReference());
		TS_ASSERT_EQUALS(fb_attribs->getAttrib("foobar"), "");
		
		i_s.setValue("/foobar", "boo");
		
		TS_ASSERT(fbnode = i_s.getNode("/"));
		TS_ASSERT(fb_attribs = fbnode->getAttributesReference());
		TS_ASSERT_EQUALS(fb_attribs->getAttrib("foobar"), "boo");
		
		TS_ASSERT(fbnode = i_s.getNode("/foobar"));
		TS_ASSERT(fb_attribs = fbnode->getAttributesReference());
		TS_ASSERT_EQUALS(fb_attribs->getAttrib("frobber"), "hello");
	}
	
	
	void testSerialize(void)
	{
		LocalKVSStore s;
		I_KVS & i_s = s;
		i_s.setValue("/meta/description", 
					 "iPod binary. Pay no attention to the plaintext crypto key @ <a href=\"dsm://ROM/0xDEADBEEF\">0xDEADBEEF</a>");
		i_s.setValue("/meta/author/array/1/name", "David Carne");
		i_s.setValue("/meta/author/array/1/email", "davidcarne@gmail.com");
		i_s.setValue("/meta/author/array/1/website", "carnelec.com");
		i_s.setValue("/meta/author/array/1/company", "Carne Electronics Ltd.");
		i_s.setValue("/meta/author/array/2/name", "Courtney Cavin");
		i_s.setValue("/meta/author/array/2/email", "ccavin@gmail.com");
		
		
		TS_ASSERT_EQUALS(i_s.getValue("/meta/description"), 
						 "iPod binary. Pay no attention to the plaintext crypto key @ <a href=\"dsm://ROM/0xDEADBEEF\">0xDEADBEEF</a>");
		TS_ASSERT_EQUALS(i_s.getValue("/meta/author/array/1/name"), "David Carne");
		TS_ASSERT_EQUALS(i_s.getValue("/meta/author/array/1/email"), "davidcarne@gmail.com");
		TS_ASSERT_EQUALS(i_s.getValue("/meta/author/array/1/website"), "carnelec.com");
		TS_ASSERT_EQUALS(i_s.getValue("/meta/author/array/1/company"), "Carne Electronics Ltd.");
		TS_ASSERT_EQUALS(i_s.getValue("/meta/author/array/2/name"), "Courtney Cavin");
		TS_ASSERT_EQUALS(i_s.getValue("/meta/author/array/2/email"), "ccavin@gmail.com");
		s.serializeTo("testfile.out");
	}
};


