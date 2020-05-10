#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <memory>
#include <boost/test/unit_test.hpp>
#include "libnbtp.h"
#include <iostream>

#include "tags/Tag.h"

BOOST_AUTO_TEST_SUITE(MultiTagTester)

	BOOST_AUTO_TEST_CASE(Bytes) {
		auto bytes = std::make_shared<NBTP::BytesTag>();
		bytes->insert(5);
		BOOST_ASSERT(bytes->size() == 1);
		BOOST_REQUIRE_THROW(bytes->insert(std::shared_ptr<NBTP::Tag>(new NBTP::IntTag(3))), std::runtime_error);
	}

BOOST_AUTO_TEST_SUITE_END()