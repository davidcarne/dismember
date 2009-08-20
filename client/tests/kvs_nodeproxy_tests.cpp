#include <unistd.h>
#include <boost/test/unit_test.hpp>

#include "kvs/i_kvs.h"
#include "kvs/localkvsstore.h"

#include <string>
class NodeProxyMock1 : private I_KVS_nodeproxy 
{
	public:
	NodeProxyMock1(sp_I_KVS_node ref) : I_KVS_nodeproxy(ref) {
		
	}
	
	void setA(std::string val)
	{
		return setPathValue("a", val);
	}
	
	std::string getA()
	{
		return getPathValue("a");
	}
	
	void setB(std::string val)
	{
		return setPathValue("b", val);
	}
	
	std::string getB()
	{
		return getPathValue("b");
	}
	
	void setSubA(std::string val)
	{
		return setPathValue("sub/a", val);
	}
	
	std::string getSubA()
	{
		return getPathValue("sub/a");
	}
	
	void setSubB(std::string val)
	{
		return setPathValue("sub/b", val);
	}
	
	std::string getSubB()
	{
		return getPathValue("sub/b");
	}
	
	std::string last_subscription_event;
	
	KVS_NODEPROXY_NOTIFYCHANGES(evt)
	{
		last_subscription_event = evt;
	}
	
	BRING_IN_KVS_NODE
};

BOOST_AUTO_TEST_CASE(nodeproxykeyvaluepath)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	i_s.setPathValue("/mockobj","hasavalue");
	NodeProxyMock1 mock(i_s.getPathNode("/mockobj"));
	
	BOOST_CHECK_EQUAL(mock.isDeleted(), false);
	BOOST_CHECK_EQUAL(mock.getKey(), "mockobj");
	BOOST_CHECK_EQUAL(mock.getPath(), "/mockobj");
	BOOST_CHECK_EQUAL(mock.getValue(), "hasavalue");
	mock.setValue("hasavalue2");
	
	BOOST_CHECK_EQUAL(i_s.getPathValue("/mockobj"), "hasavalue2");
}

BOOST_AUTO_TEST_CASE(testNodeProxyAttribReadWrite)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	i_s.setPathValue("/mockobj/a","1");
	i_s.setPathValue("/mockobj/b","2");
	i_s.setPathValue("/mockobj/sub/a","3");
	i_s.setPathValue("/mockobj/sub/b","4");
	
	NodeProxyMock1 mock(i_s.getPathNode("/mockobj"));
	BOOST_CHECK_EQUAL(mock.getA(), "1");
	BOOST_CHECK_EQUAL(mock.getB(), "2");
	BOOST_CHECK_EQUAL(mock.getSubA(), "3");
	BOOST_CHECK_EQUAL(mock.getSubB(), "4");
	
	mock.setA("!");
	mock.setB("@");
	mock.setSubA("#");
	mock.setSubB("$");
	
	BOOST_CHECK_EQUAL(i_s.getPathValue("/mockobj/a"), "!");
	BOOST_CHECK_EQUAL(i_s.getPathValue("/mockobj/b"), "@");
	BOOST_CHECK_EQUAL(i_s.getPathValue("/mockobj/sub/a"), "#");
	BOOST_CHECK_EQUAL(i_s.getPathValue("/mockobj/sub/b"), "$");
}

BOOST_AUTO_TEST_CASE(nodeproxynochildren)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	i_s.setPathValue("/mockobj","hasavalue");
	NodeProxyMock1 mock(i_s.getPathNode("/mockobj"));
	
	BOOST_CHECK_EQUAL(mock.getChildCount(), (unsigned int)0);
	BOOST_CHECK(mock.beginChildren() == mock.endChildren());
}

BOOST_AUTO_TEST_CASE(nodeproxyhaschildren)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	i_s.setPathValue("/mockobj","hasavalue");
	sp_I_KVS_node cn = i_s.setPathValue("/mockobj/snarf","hasavalue");
	NodeProxyMock1 mock(i_s.getPathNode("/mockobj"));
	
	BOOST_CHECK_EQUAL(mock.getChildCount(), (unsigned int)1);
	BOOST_CHECK(mock.beginChildren() != mock.endChildren());
	
	kvs_node_child_ci i = mock.beginChildren();
	kvs_node_child_ci i_end = mock.endChildren();
	
	BOOST_REQUIRE(i != i_end);
	BOOST_CHECK(*i == cn);
	i++;
	BOOST_CHECK(i == i_end);
}

BOOST_AUTO_TEST_CASE(nodeproxydangling)
{	
	sp_I_KVS_node fake;
	NodeProxyMock1 mock(fake);
	
	BOOST_CHECK_EQUAL(mock.getKey(), "");
	BOOST_CHECK_EQUAL(mock.getPath(), "");
	BOOST_CHECK_EQUAL(mock.getPathValue("fakestring/nowheretown"), "");
	mock.setPathValue("fakestring/nowheretown", "val");
	mock.setValue("!!!");
	BOOST_CHECK_EQUAL(mock.getValue(), "");
	
	BOOST_CHECK(mock.beginChildren() == mock.endChildren());
	
	kvs_node_child_ci i = mock.beginChildren();
	kvs_node_child_ci i_end = mock.endChildren();
	
	BOOST_CHECK(i == i_end);

	BOOST_CHECK(!*i);
	
	// Fake iterator, should be able to increment all we want
	i++;
	i++;
	i++;
	i++;
	
	BOOST_CHECK(i == i_end);
	
}

BOOST_AUTO_TEST_CASE(nodesubscription)
{	
	LocalKVSStore s;
	I_KVS  & i_s = s;
	i_s.setPathValue("/mockobj","hasavalue");
	sp_I_KVS_node cn = i_s.setPathValue("/mockobj/snarf","hasavalue");
	NodeProxyMock1 mock(i_s.getPathNode("/mockobj"));
	
	BOOST_CHECK_EQUAL(mock.last_subscription_event, "");
	i_s.setPathValue("/mockobj/snarf","hasavalue");
	BOOST_CHECK_EQUAL(mock.last_subscription_event, "/mockobj/snarf");
	i_s.setPathValue("/mockobj/snarf/barcamp","hasavalue");
	BOOST_CHECK_EQUAL(mock.last_subscription_event, "/mockobj/snarf/barcamp");
	
}