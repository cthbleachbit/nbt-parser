#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <memory>
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "libnbtp.h"
#include "memstream.h"

BOOST_AUTO_TEST_SUITE(NestedParseTester)

	BOOST_AUTO_TEST_CASE(EmptyCompound) {
		uint8_t bytes[] = {0x00};
		memstream input(bytes, sizeof(bytes));
		auto parsed = NBTP::CompoundTag(input);
		BOOST_ASSERT(parsed.size() == 0);
	}

	BOOST_AUTO_TEST_CASE(OneSubElemCompound) {
		uint8_t bytes[] = {0x01, 0x00, 0x01, 0x44, 0xff, 0x00};
		memstream input(bytes, sizeof(bytes));
		auto parsed = NBTP::CompoundTag(input);
		BOOST_ASSERT(parsed.size() == 1);
		auto container = parsed.getPayload();
		BOOST_ASSERT(container["D"]->typeCode() == NBTP::TagType::BYTE);
		std::cout << parsed << std::endl;
	}

BOOST_AUTO_TEST_SUITE_END()