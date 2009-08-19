
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


BOOST_AUTO_TEST_CASE(testCreateReadComment)
{
	LocalKVSStore s;
	I_KVS  & i_s = s;
	
	BOOST_CHECK(i_s.createDanglingAttributes());
}


BOOST_AUTO_TEST_SUITE_END()

