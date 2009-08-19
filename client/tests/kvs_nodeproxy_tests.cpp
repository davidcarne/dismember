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
private:
	BRING_IN_KVS_NODE
};

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
