
#include <unistd.h>
#include <boost/test/unit_test.hpp>

#include "kvs/kvs_node.h"
#include "kvs/i_kvs.h"
#include "kvs/localkvsstore.h"

BOOST_AUTO_TEST_CASE(NonExistantGetNode)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	BOOST_CHECK(!i_s.getNode("/foobar"));
}

BOOST_AUTO_TEST_CASE(SetValue)
{
	
	LocalKVSStore s;
	
	I_KVS  & i_s = s;
	
	sp_I_KVS_node n = i_s.setValue("/foobar","test");
	BOOST_CHECK_EQUAL(i_s.getNode("/foobar"),n);
	BOOST_CHECK_EQUAL(i_s.getValue("/foobar"),"test");
	
}

BOOST_AUTO_TEST_CASE(GetSetNodeValue)
{
	
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	i_s.setValue("/foobar", "hello");
	BOOST_CHECK_EQUAL(i_s.getValue("/foobar"), "hello");
	
	sp_I_KVS_node node = i_s.getNode("/foobar");
	BOOST_CHECK(node);
	BOOST_CHECK_EQUAL(node->getValue(), "hello");
	node->setValue("goodbye");
	BOOST_CHECK_EQUAL(node->getValue(), "goodbye");
	BOOST_CHECK_EQUAL(i_s.getValue("/foobar"), "goodbye");
}

BOOST_AUTO_TEST_CASE(SetGetOffRoot)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	i_s.setValue("/foobar", "hello");
	BOOST_CHECK_EQUAL(i_s.getValue("/foobar"), "hello");
}

BOOST_AUTO_TEST_CASE(NodePaths)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	i_s.setValue("/foobar", "hello");
	
	sp_I_KVS_node fbnode;
	BOOST_CHECK(fbnode = i_s.getNode("/foobar"));
	
	BOOST_CHECK_EQUAL(fbnode->getPath(), "/foobar");
	BOOST_CHECK_EQUAL(fbnode->getKey(), "foobar");
	BOOST_CHECK_EQUAL(fbnode->getValue(), "hello");
}

BOOST_AUTO_TEST_CASE(RootNodeAttributes)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	i_s.setValue("/foobar", "hello");
	
	sp_I_KVS_node fbnode;
	BOOST_CHECK(fbnode = i_s.getNode("/"));
	
	sp_I_KVS_attributes fb_attribs;
	BOOST_CHECK(fb_attribs = fbnode->getAttributesReference());
	
	BOOST_CHECK_EQUAL(fb_attribs->getAttrib("foobar"), "hello");
}

BOOST_AUTO_TEST_CASE(testMultiSectionPath)
{
	LocalKVSStore s;
	I_KVS & i_s = s;
	
	
	i_s.setValue("/foobar/frobber", "hello");
	BOOST_CHECK_EQUAL(i_s.getValue("/foobar"), "");
	BOOST_CHECK_EQUAL(i_s.getValue("/foobar/frobber"), "hello");
	
	sp_I_KVS_node fbnode;
	sp_I_KVS_attributes fb_attribs;
	
	BOOST_CHECK(fbnode = i_s.getNode("/"));
	BOOST_CHECK(fb_attribs = fbnode->getAttributesReference());
	BOOST_CHECK_EQUAL(fb_attribs->getAttrib("foobar"), "");
	
	i_s.setValue("/foobar", "boo");
	
	BOOST_CHECK(fbnode = i_s.getNode("/"));
	BOOST_CHECK(fb_attribs = fbnode->getAttributesReference());
	BOOST_CHECK_EQUAL(fb_attribs->getAttrib("foobar"), "boo");
	
	BOOST_CHECK(fbnode = i_s.getNode("/foobar"));
	BOOST_CHECK(fb_attribs = fbnode->getAttributesReference());
	BOOST_CHECK_EQUAL(fb_attribs->getAttrib("frobber"), "hello");
}


BOOST_AUTO_TEST_CASE(Serialize)
{
	
	char * filename = "/tmp/dismember.test";
	
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
		
		
		BOOST_CHECK_EQUAL(i_s.getValue("/meta/description"), 
						 "iPod binary. Pay no attention to the plaintext crypto key @ <a href=\"dsm://ROM/0xDEADBEEF\">0xDEADBEEF</a>");
		BOOST_CHECK_EQUAL(i_s.getValue("/meta/author/array/1/name"), "David Carne");
		BOOST_CHECK_EQUAL(i_s.getValue("/meta/author/array/1/email"), "davidcarne@gmail.com");
		BOOST_CHECK_EQUAL(i_s.getValue("/meta/author/array/1/website"), "carnelec.com");
		BOOST_CHECK_EQUAL(i_s.getValue("/meta/author/array/1/company"), "Carne Electronics Ltd.");
		BOOST_CHECK_EQUAL(i_s.getValue("/meta/author/array/2/name"), "Courtney Cavin");
		BOOST_CHECK_EQUAL(i_s.getValue("/meta/author/array/2/email"), "ccavin@gmail.com");
		s.serializeTo(filename);
	}
	sp_LocalKVSStore pt = LocalKVSStore::createFromFile(filename);
	sp_I_KVS i_pt = pt;
	
	BOOST_CHECK(i_pt);
	
	BOOST_CHECK_EQUAL(i_pt->getValue("/meta/description"), 
					 "iPod binary. Pay no attention to the plaintext crypto key @ <a href=\"dsm://ROM/0xDEADBEEF\">0xDEADBEEF</a>");
	BOOST_CHECK_EQUAL(i_pt->getValue("/meta/author/array/1/name"), "David Carne");
	BOOST_CHECK_EQUAL(i_pt->getValue("/meta/author/array/1/email"), "davidcarne@gmail.com");
	BOOST_CHECK_EQUAL(i_pt->getValue("/meta/author/array/1/website"), "carnelec.com");
	BOOST_CHECK_EQUAL(i_pt->getValue("/meta/author/array/1/company"), "Carne Electronics Ltd.");
	BOOST_CHECK_EQUAL(i_pt->getValue("/meta/author/array/2/name"), "Courtney Cavin");
	BOOST_CHECK_EQUAL(i_pt->getValue("/meta/author/array/2/email"), "ccavin@gmail.com");
	unlink(filename);
}
