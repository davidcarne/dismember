
#include <unistd.h>

#include <boost/test/unit_test.hpp>

#include "kvs/i_kvs.h"
#include "kvs/localkvsstore.h"
#include "i_comment.h"
#include "kvs_projectmodel.h"

/****************** address_t testing *********************/
BOOST_AUTO_TEST_SUITE(address_t_test_suite)

BOOST_AUTO_TEST_CASE(address_t_default_constructor_invalid)
{
	address_t a;
	BOOST_CHECK_EQUAL(a.isValid(), false);
}

BOOST_AUTO_TEST_SUITE_END()


/****************** memsegment *********************/

struct mem_test_fixture {
    mem_test_fixture() : model( &store ) {}
    ~mem_test_fixture()         {}
	
    LocalKVSStore store;
	KVSBackedProjectModel model;
};


BOOST_FIXTURE_TEST_SUITE(memsegment_man_test_suite, mem_test_fixture)

/*
 * First up - memsegment testing. Api is thus:
 *		 virtual bool addSegment(MemSegment * m);
 *		 virtual bool readByte(address_t taddr, uint8_t * data) const;
 *		 virtual bool readBytes(address_t, u8, u8*) const;
 *		 virtual memsegment_ci memsegs_begin() const;
 *		 virtual memsegment_ci memsegs_end() const;
 *		 virtual address_t locateAddress(uint64_t address) const;
 */
BOOST_AUTO_TEST_CASE(testlocateAddressWithNoSegments)
{
	BOOST_CHECK_EQUAL(model.locateAddress(0x0).isValid(), false);
	
	// 32 bit address
	BOOST_CHECK_EQUAL(model.locateAddress(0xDEADBEEF).isValid(), false);
	
	// 64 bit integer
	BOOST_CHECK_EQUAL(model.locateAddress(0x1337133713371337LL).isValid(), false);
	BOOST_CHECK_EQUAL(model.locateAddress(0xFFFFFFFFFFFFFFFFLL).isValid(), false);
}



BOOST_AUTO_TEST_CASE(test_add_empty_segment)
{
	sp_I_MemSegment s = model.addSegment(0x640, 0x480, "MySegment");
	
	BOOST_REQUIRE(s);
	BOOST_CHECK_EQUAL(s->getName(), "MySegment");
	BOOST_CHECK_EQUAL(s->is_defined(), false);
	BOOST_CHECK_EQUAL(s->get_start(), (unsigned long)0x640);
	BOOST_CHECK_EQUAL(s->get_length(), (unsigned long)0x480);
	
	address_t base = s->getBaseAddress();
	BOOST_CHECK_EQUAL(base.getValue(), (unsigned long)0x640);
	BOOST_CHECK(base.isValid());
	BOOST_CHECK_EQUAL(base.getOffset(), (unsigned long)0);
	BOOST_CHECK(base.getSegment().lock() == s);
	
	address_t locatedBase = model.locateAddress(0x640);
	
	BOOST_CHECK(locatedBase == base);
	
	BOOST_CHECK_EQUAL(locatedBase.readByte(NULL), false);
	
	BOOST_CHECK_EQUAL(s->get_bytes(locatedBase, 4, NULL), false);
}

BOOST_AUTO_TEST_CASE(test_empty_memsegment_iterators)
{
	BOOST_CHECK(model.memsegs_begin() == model.memsegs_end());
}

BOOST_AUTO_TEST_CASE(test_single_memsegment_iterators)
{
	sp_I_MemSegment s = model.addSegment(0x640, 0x480, "MySegment");
	
	BOOST_REQUIRE(s);
	
	memsegment_ci i = model.memsegs_begin();
	memsegment_ci i_end = model.memsegs_end();
	
	BOOST_CHECK(i != i_end);
	
	BOOST_CHECK(*i == s);
	i++;
	BOOST_CHECK(i == i_end);
}




BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(testKVSSaveMemoryLocations)
{
	LocalKVSStore store;
	{
		
		KVSBackedProjectModel model(&store);
		model.addSegment(0x640, 0x480, "MySegment");
		BOOST_REQUIRE(model.locateAddress(0x642).isValid());
	}
	
	{
		KVSBackedProjectModel model(&store);
		address_t da = model.locateAddress(0x642);
		BOOST_REQUIRE(da.isValid());
		sp_cI_MemSegment ms = da.getSegment().lock();
		
		BOOST_CHECK_EQUAL(ms->getName(), "MySegment");
		BOOST_CHECK_EQUAL(ms->get_start(), (unsigned long)0x640);
		BOOST_CHECK_EQUAL(ms->get_length(), (unsigned long)0x480);
		BOOST_CHECK_EQUAL(ms->is_defined(), false);
	}
}
