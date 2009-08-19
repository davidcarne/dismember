
#include <unistd.h>
#include <boost/test/unit_test.hpp>

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
	
	const void setFoo(const std::string & value)
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


BOOST_AUTO_TEST_CASE(EmptyChildIterator)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	
	sp_I_KVS_node hello_node = i_s.setValue("/hello","world");
	
	BOOST_REQUIRE(hello_node);
	
	BOOST_CHECK(hello_node->beginChildren() == hello_node->endChildren());
}


BOOST_AUTO_TEST_CASE(OneChildIterator)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	sp_I_KVS_node hello_node = i_s.setValue("/hello","world");
	sp_I_KVS_node child_orig_node = i_s.setValue("/hello/a","b");
	
	BOOST_REQUIRE(hello_node);
	BOOST_REQUIRE(child_orig_node);
	
	kvs_node_child_ci ci = hello_node->beginChildren();
	
	// Check iterator begin() equality
	BOOST_CHECK(ci == hello_node->beginChildren());
	
	// Check that we can retrieve the child node
	sp_I_KVS_node child_node = *ci;
	BOOST_REQUIRE(child_node);
	BOOST_CHECK(child_node == child_orig_node);
	BOOST_CHECK_EQUAL(child_node->getPath(), "/hello/a");
	BOOST_CHECK_EQUAL(child_node->getKey(), "a");
	BOOST_CHECK_EQUAL(child_node->getValue(), "b");
	
	// Check that we can retrieve the end
	ci++;
	BOOST_CHECK(ci == hello_node->endChildren());
}



