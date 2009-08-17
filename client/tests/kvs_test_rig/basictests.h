#define CXXTEST_ABORT_TEST_ON_FAIL

#include <unistd.h>

#include <cxxtest/TestSuite.h>

#include "kvs/kvs_node.h"
#include "kvs/i_kvs.h"
#include "kvs/localkvsstore.h"

class MyTestSuite : public CxxTest::TestSuite 
{
	
public:
	void testCreateDanglingAttribute( void )
	{
		LocalKVSStore s;
		
		I_KVS  & i_s = s;
		
		TS_ASSERT(i_s.createDanglingAttributes());
	}
	
	void testGetSetOnDanglingAttributes(void)
	{
		LocalKVSStore s;
		
		I_KVS  & i_s = s;
		
		sp_I_KVS_attributes a = i_s.createDanglingAttributes();
		a->setAttrib("hello","frobbisher");
		a->setAttrib("no","way");
		TS_ASSERT_EQUALS(a->getAttrib("hello"), "frobbisher");
		TS_ASSERT_EQUALS(a->getAttrib("no"), "way");
		TS_ASSERT_EQUALS(a->getAttrib("empty"), "");
	}
	
	
	
	void testGetSetNodeValue()
	{
		
		LocalKVSStore s;
		I_KVS  & i_s = s;
		
		i_s.setValue("/foobar", "hello");
		TS_ASSERT_EQUALS(i_s.getValue("/foobar"), "hello");
		
		sp_I_KVS_node node = i_s.getNode("/foobar");
		TS_ASSERT(node);
		TS_ASSERT_EQUALS(node->getValue(), "hello");
		node->setValue("goodbye");
		TS_ASSERT_EQUALS(node->getValue(), "goodbye");
		TS_ASSERT_EQUALS(i_s.getValue("/foobar"), "goodbye");
	}
	
	void testOverlayDanglingAttributes(void)
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
		
		
		TS_ASSERT_EQUALS(i_s.getValue("/foo/hello"), "frobbisher");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/no"), "way");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/bar"), "frob");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/bar/depth"), "ishere");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/empty"), "");
		
	}
	
	
	
	void testOverwriteDanglingAttributes(void)
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
		
		
		TS_ASSERT_EQUALS(i_s.getValue("/foo"), "value");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/hello"), "frobbisher");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/no"), "way");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/bar"), "");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/bar/depth"), "");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/empty"), "");
		
	}
	
	void testSetValue(void)
	{
		
		LocalKVSStore s;
		
		I_KVS  & i_s = s;
		
		sp_I_KVS_node n = i_s.setValue("/foobar","test");
		TS_ASSERT_EQUALS(i_s.getNode("/foobar"),n);
		TS_ASSERT_EQUALS(i_s.getValue("/foobar"),"test");
		
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
		TS_ASSERT_EQUALS(fbnode->getValue(), "hello");
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
			
			
			TS_ASSERT_EQUALS(i_s.getValue("/meta/description"), 
							 "iPod binary. Pay no attention to the plaintext crypto key @ <a href=\"dsm://ROM/0xDEADBEEF\">0xDEADBEEF</a>");
			TS_ASSERT_EQUALS(i_s.getValue("/meta/author/array/1/name"), "David Carne");
			TS_ASSERT_EQUALS(i_s.getValue("/meta/author/array/1/email"), "davidcarne@gmail.com");
			TS_ASSERT_EQUALS(i_s.getValue("/meta/author/array/1/website"), "carnelec.com");
			TS_ASSERT_EQUALS(i_s.getValue("/meta/author/array/1/company"), "Carne Electronics Ltd.");
			TS_ASSERT_EQUALS(i_s.getValue("/meta/author/array/2/name"), "Courtney Cavin");
			TS_ASSERT_EQUALS(i_s.getValue("/meta/author/array/2/email"), "ccavin@gmail.com");
			s.serializeTo(filename);
		}
		sp_LocalKVSStore pt = LocalKVSStore::createFromFile(filename);
		sp_I_KVS i_pt = pt;
		
		TS_ASSERT(i_pt);
		
		TS_ASSERT_EQUALS(i_pt->getValue("/meta/description"), 
						 "iPod binary. Pay no attention to the plaintext crypto key @ <a href=\"dsm://ROM/0xDEADBEEF\">0xDEADBEEF</a>");
		TS_ASSERT_EQUALS(i_pt->getValue("/meta/author/array/1/name"), "David Carne");
		TS_ASSERT_EQUALS(i_pt->getValue("/meta/author/array/1/email"), "davidcarne@gmail.com");
		TS_ASSERT_EQUALS(i_pt->getValue("/meta/author/array/1/website"), "carnelec.com");
		TS_ASSERT_EQUALS(i_pt->getValue("/meta/author/array/1/company"), "Carne Electronics Ltd.");
		TS_ASSERT_EQUALS(i_pt->getValue("/meta/author/array/2/name"), "Courtney Cavin");
		TS_ASSERT_EQUALS(i_pt->getValue("/meta/author/array/2/email"), "ccavin@gmail.com");
		unlink(filename);
	}
	
	void testOverlayAttributes()
	{
		LocalKVSStore s;
		I_KVS  & i_s = s;
		
		sp_I_KVS_attributes a = i_s.createDanglingAttributes();
		a->setAttrib("hello","frobbisher");
		a->setAttrib("no","way");
		
		i_s.setValue("/foo","hasvalue");
		i_s.setValue("/foo/bar/depth","ishere");
		i_s.overlayNode("/foo", a);
		
		TS_ASSERT_EQUALS(i_s.getValue("/foo"), "hasvalue");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/hello"), "frobbisher");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/no"), "way");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/bar/depth"), "ishere");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/empty"), "");
	}
	
	
	void testOverlayAttributes2()
	{
		LocalKVSStore s;
		I_KVS  & i_s = s;
		
		sp_I_KVS_attributes a = i_s.createDanglingAttributes();
		a->setAttrib("hello","frobbisher");
		a->setAttrib("no","way");
		
		i_s.setValue("/foo","hasvalue");
		i_s.setValue("/foo/bar/depth","ishere");
		i_s.overlayNode("/foo", "newval",a);
		
		TS_ASSERT_EQUALS(i_s.getValue("/foo"), "newval");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/hello"), "frobbisher");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/no"), "way");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/bar/depth"), "ishere");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/empty"), "");
	}
	
	
	
	void testOverlayAttributes3()
	{
		LocalKVSStore s;
		I_KVS  & i_s = s;
		
		sp_I_KVS_attributes a = i_s.createDanglingAttributes();
		a->setAttrib("hello","frobbisher");
		a->setAttrib("no","way");
		
		i_s.overlayNode("/foo", "newval",a);
		
		TS_ASSERT_EQUALS(i_s.getValue("/foo"), "newval");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/hello"), "frobbisher");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/no"), "way");
	}
	
	
	void testOverwriteAttributes()
	{
		LocalKVSStore s;
		I_KVS  & i_s = s;
		
		sp_I_KVS_attributes a = i_s.createDanglingAttributes();
		a->setAttrib("hello","frobbisher");
		a->setAttrib("no","way");
		
		i_s.setValue("/foo","hasvalue");
		i_s.setValue("/foo/bar/depth","ishere");
		i_s.overwriteNode("/foo", "newval", a);
		
		TS_ASSERT_EQUALS(i_s.getValue("/foo"), "newval");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/hello"), "frobbisher");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/no"), "way");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/bar/depth"), "");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/empty"), "");
	}
	
	void testOverwriteAttributes2()
	{
		LocalKVSStore s;
		I_KVS  & i_s = s;
		
		sp_I_KVS_attributes a = i_s.createDanglingAttributes();
		a->setAttrib("hello","frobbisher");
		a->setAttrib("no","way");
		
		i_s.setValue("/foo","hasvalue");
		i_s.setValue("/foo/bar/depth","ishere");
		i_s.overwriteNode("/foo", a);
		
		TS_ASSERT_EQUALS(i_s.getValue("/foo"), "");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/hello"), "frobbisher");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/no"), "way");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/bar/depth"), "");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/empty"), "");
	}
	
	
	void testOverwriteAttributes3()
	{
		LocalKVSStore s;
		I_KVS  & i_s = s;
		
		sp_I_KVS_attributes a = i_s.createDanglingAttributes();
		a->setAttrib("hello","frobbisher");
		a->setAttrib("no","way");
		
		i_s.overwriteNode("/foo", a);
		
		TS_ASSERT_EQUALS(i_s.getValue("/foo"), "");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/hello"), "frobbisher");
		TS_ASSERT_EQUALS(i_s.getValue("/foo/no"), "way");
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
	
	void testAttribProxyMock()
	{
		LocalKVSStore s;
		I_KVS  & i_s = s;
		i_s.setValue("/testnode/foo","value1");
		
		sp_I_KVS_attributes a = i_s.getNode("/testnode")->getAttributesReference();
		
		AttribProxy_test_mock mock(a);
		
		TS_ASSERT_EQUALS(mock.getFoo(), "value1");
		mock.setFoo("value2");
		TS_ASSERT_EQUALS(i_s.getValue("/testnode/foo"), "value2");
	};
	
	
	void testAttribProxyDangling()
	{
		LocalKVSStore s;
		I_KVS  & i_s = s;
		
		sp_I_KVS_attributes a = i_s.createDanglingAttributes();
		
		a->setAttrib("foo","value1");
		
		AttribProxy_test_mock mock(a);
		
		TS_ASSERT_EQUALS(mock.getFoo(), "value1");
		mock.setFoo("value2");
		TS_ASSERT_EQUALS(a->getAttrib("foo"), "value2");
		
		i_s.setValue("/testnode/foo", "INVALID");
		i_s.overlayNode("/testnode", a);
		
		
		TS_ASSERT_EQUALS(i_s.getValue("/testnode/foo"), "value2");
		i_s.setValue("/testnode/foo", "value3");
		TS_ASSERT_EQUALS(mock.getFoo(), "value3");
		mock.setFoo("value4");
		TS_ASSERT_EQUALS(i_s.getValue("/testnode/foo"), "value4");
	};
};


