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

	BOOST_AUTO_TEST_CASE(Longs) {
		auto longs = std::make_shared<NBTP::LongsTag>();
		longs->insert(3874747474747474774L);
		BOOST_ASSERT(longs->size() == 1);
		BOOST_REQUIRE_THROW(longs->insert(std::shared_ptr<NBTP::Tag>(new NBTP::IntTag(3))), std::runtime_error);
	}

	BOOST_AUTO_TEST_CASE(LongsItr) {
		auto longs = std::make_shared<NBTP::LongsTag>();
		longs->insert(3874747474747474774L);
		longs->insert(0L);
		auto itr = longs->getPayload().begin();
		BOOST_ASSERT(itr->get()->typeCode() == NBTP::TagType::LONG);
		BOOST_ASSERT(longs->size() == 2);
	}

	BOOST_AUTO_TEST_CASE(Str) {
		auto str = std::make_shared<NBTP::StringTag>();
		std::cout << *str << std::endl;
		BOOST_ASSERT(str->size() == 0);
		str->setPayload("test");
		std::cout << *str << std::endl;
		BOOST_ASSERT(str->size() == 4);
	}

BOOST_AUTO_TEST_SUITE_END()