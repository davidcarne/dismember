
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

BOOST_AUTO_TEST_CASE(GetSetOnDanglingAttributes)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	sp_I_KVS_attributes a = i_s.createDanglingAttributes();
	a->setAttrib("hello","frobbisher");
	a->setAttrib("no","way");
	BOOST_CHECK_EQUAL(a->getAttrib("hello"), "frobbisher");
	BOOST_CHECK_EQUAL(a->getAttrib("no"), "way");
	BOOST_CHECK_EQUAL(a->getAttrib("empty"), "");
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

BOOST_AUTO_TEST_CASE(OverlayDanglingAttributes)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	
	sp_I_KVS_attributes a = i_s.createDanglingAttributes();
	a->setAttrib("hello","frobbisher");
	a->setAttrib("no","way");
	
	i_s.setValue("/foo/bar","frob");
	i_s.setValue("/foo/bar/depth","ishere");
	sp_I_KVS_node node_foo = i_s.getNode("/foo");
	node_foo->overlayAttributes(a);
	
	
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/hello"), "frobbisher");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/no"), "way");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/bar"), "frob");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/bar/depth"), "ishere");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/empty"), "");
	
}



BOOST_AUTO_TEST_CASE(OverwriteDanglingAttributes)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	
	sp_I_KVS_attributes a = i_s.createDanglingAttributes();
	a->setAttrib("hello","frobbisher");
	a->setAttrib("no","way");
	
	i_s.setValue("/foo","value");
	i_s.setValue("/foo/bar","frob");
	i_s.setValue("/foo/bar/depth","ishere");
	sp_I_KVS_node node_foo = i_s.getNode("/foo");
	node_foo->overwriteAttributes(a);
	
	
	BOOST_CHECK_EQUAL(i_s.getValue("/foo"), "value");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/hello"), "frobbisher");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/no"), "way");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/bar"), "");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/bar/depth"), "");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/empty"), "");
	
}

BOOST_AUTO_TEST_CASE(SetValue)
{
	
	LocalKVSStore s;
	
	I_KVS  & i_s = s;
	
	sp_I_KVS_node n = i_s.setValue("/foobar","test");
	BOOST_CHECK_EQUAL(i_s.getNode("/foobar"),n);
	BOOST_CHECK_EQUAL(i_s.getValue("/foobar"),"test");
	
}
BOOST_AUTO_TEST_CASE(NonExistantGetNode)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	BOOST_CHECK(!i_s.getNode("/foobar"));
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

void testMultiSectionPath (void)
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

BOOST_AUTO_TEST_CASE(OverlayAttributes)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	sp_I_KVS_attributes a = i_s.createDanglingAttributes();
	a->setAttrib("hello","frobbisher");
	a->setAttrib("no","way");
	
	i_s.setValue("/foo","hasvalue");
	i_s.setValue("/foo/bar/depth","ishere");
	i_s.overlayNode("/foo", a);
	
	BOOST_CHECK_EQUAL(i_s.getValue("/foo"), "hasvalue");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/hello"), "frobbisher");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/no"), "way");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/bar/depth"), "ishere");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/empty"), "");
}


BOOST_AUTO_TEST_CASE(OverlayAttributes2)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	sp_I_KVS_attributes a = i_s.createDanglingAttributes();
	a->setAttrib("hello","frobbisher");
	a->setAttrib("no","way");
	
	i_s.setValue("/foo","hasvalue");
	i_s.setValue("/foo/bar/depth","ishere");
	i_s.overlayNode("/foo", "newval",a);
	
	BOOST_CHECK_EQUAL(i_s.getValue("/foo"), "newval");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/hello"), "frobbisher");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/no"), "way");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/bar/depth"), "ishere");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/empty"), "");
}



BOOST_AUTO_TEST_CASE(OverlayAttributes3)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	sp_I_KVS_attributes a = i_s.createDanglingAttributes();
	a->setAttrib("hello","frobbisher");
	a->setAttrib("no","way");
	
	i_s.overlayNode("/foo", "newval",a);
	
	BOOST_CHECK_EQUAL(i_s.getValue("/foo"), "newval");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/hello"), "frobbisher");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/no"), "way");
}


BOOST_AUTO_TEST_CASE(OverwriteAttributes)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	sp_I_KVS_attributes a = i_s.createDanglingAttributes();
	a->setAttrib("hello","frobbisher");
	a->setAttrib("no","way");
	
	i_s.setValue("/foo","hasvalue");
	i_s.setValue("/foo/bar/depth","ishere");
	i_s.overwriteNode("/foo", "newval", a);
	
	BOOST_CHECK_EQUAL(i_s.getValue("/foo"), "newval");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/hello"), "frobbisher");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/no"), "way");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/bar/depth"), "");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/empty"), "");
}

BOOST_AUTO_TEST_CASE(OverwriteAttributes2)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	sp_I_KVS_attributes a = i_s.createDanglingAttributes();
	a->setAttrib("hello","frobbisher");
	a->setAttrib("no","way");
	
	i_s.setValue("/foo","hasvalue");
	i_s.setValue("/foo/bar/depth","ishere");
	i_s.overwriteNode("/foo", a);
	
	BOOST_CHECK_EQUAL(i_s.getValue("/foo"), "");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/hello"), "frobbisher");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/no"), "way");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/bar/depth"), "");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/empty"), "");
}


BOOST_AUTO_TEST_CASE(OverwriteAttributes3)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	sp_I_KVS_attributes a = i_s.createDanglingAttributes();
	a->setAttrib("hello","frobbisher");
	a->setAttrib("no","way");
	
	i_s.overwriteNode("/foo", a);
	
	BOOST_CHECK_EQUAL(i_s.getValue("/foo"), "");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/hello"), "frobbisher");
	BOOST_CHECK_EQUAL(i_s.getValue("/foo/no"), "way");
}


class AttribProxy_test_mock : private I_KVS_attribproxy {
	
public: 
	
	AttribProxy_test_mock(sp_I_KVS_attributes src) : I_KVS_attribproxy(src)
	{
		
	}
	
	BRING_IN_KVS_ATTRIBS
	
	const std::string & getFoo()
	{
		return getAttrib("foo");
	}
	
	const std::string & setFoo(const std::string & value)
	{
		setAttrib("foo",value);
	}
	
};

BOOST_AUTO_TEST_CASE(AttribProxyMock)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	i_s.setValue("/testnode/foo","value1");
	
	sp_I_KVS_attributes a = i_s.getNode("/testnode")->getAttributesReference();
	
	AttribProxy_test_mock mock(a);
	
	BOOST_CHECK_EQUAL(mock.getFoo(), "value1");
	mock.setFoo("value2");
	BOOST_CHECK_EQUAL(i_s.getValue("/testnode/foo"), "value2");
}


BOOST_AUTO_TEST_CASE(AttribProxyDangling)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	sp_I_KVS_attributes a = i_s.createDanglingAttributes();
	
	a->setAttrib("foo","value1");
	
	AttribProxy_test_mock mock(a);
	
	BOOST_CHECK_EQUAL(mock.getFoo(), "value1");
	mock.setFoo("value2");
	BOOST_CHECK_EQUAL(a->getAttrib("foo"), "value2");
	
	i_s.setValue("/testnode/foo", "INVALID");
	i_s.overlayNode("/testnode", a);
	
	
	BOOST_CHECK_EQUAL(i_s.getValue("/testnode/foo"), "value2");
	i_s.setValue("/testnode/foo", "value3");
	BOOST_CHECK_EQUAL(mock.getFoo(), "value3");
	mock.setFoo("value4");
	BOOST_CHECK_EQUAL(i_s.getValue("/testnode/foo"), "value4");
}


