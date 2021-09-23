#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <memory>
#include <boost/test/unit_test.hpp>
#include "libnbtp.h"
#include <iostream>

#include "tags/Tag.h"
#include "memstream.h"

BOOST_AUTO_TEST_SUITE(MultiTagTester)

	BOOST_AUTO_TEST_CASE(Bytes) {
		auto bytes = std::make_shared<NBTP::BytesTag>();
		bytes->insert(5);
		BOOST_ASSERT(bytes->size() == 1);
		BOOST_REQUIRE_THROW(bytes->insert(std::shared_ptr<NBTP::Tag>(new NBTP::IntTag(3))), NBTP::ListTypeMismatchException);
	}

	BOOST_AUTO_TEST_CASE(Longs) {
		auto longs = std::make_shared<NBTP::LongsTag>();
		longs->insert(3874747474747474774L);
		BOOST_ASSERT(longs->size() == 1);
		BOOST_REQUIRE_THROW(longs->insert(std::shared_ptr<NBTP::Tag>(new NBTP::IntTag(3))), NBTP::ListTypeMismatchException);
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
		BOOST_ASSERT(str->size() == 0);
		str->setPayload("test");
		BOOST_ASSERT(str->size() == 4);
	}

	BOOST_AUTO_TEST_CASE(ComplexEqual) {
		uint8_t bytes1[] = {0x01, 0x00, 0x01, 0x45, 0xff, 0x01, 0x00, 0x01, 0x46, 0x55, 0x00};
		memstream input1(bytes1, sizeof(bytes1));
		ssize_t run_len = 0;
		auto parsed1 = NBTP::CompoundTag(input1, run_len);

		uint8_t bytes2[] = {0x01, 0x00, 0x01, 0x46, 0x55, 0x01, 0x00, 0x01, 0x45, 0xff, 0x00};
		memstream input2(bytes2, sizeof(bytes2));
		run_len = 0;
		auto parsed2 = NBTP::CompoundTag(input2, run_len);
		BOOST_ASSERT(parsed1.getPayload().size() == 2);
		BOOST_ASSERT(parsed1 == parsed2);
	}

	BOOST_AUTO_TEST_CASE(ComplexInsert) {
		auto tag = std::make_shared<NBTP::CompoundTag>();
		tag->insert("www", std::make_shared<NBTP::IntTag>(20));
		std::cout << *tag << std::endl;
		auto subtag = (NBTP::IntTag &) *(tag->lookup("www"));
		BOOST_ASSERT(subtag.getPayload() == 20 );
	}

	BOOST_AUTO_TEST_CASE(Nonexistent) {
		auto tag = std::make_shared<NBTP::CompoundTag>();
		BOOST_ASSERT(tag->lookup("www") == nullptr);
	}

BOOST_AUTO_TEST_SUITE_END()