#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <memory>
#include <boost/test/unit_test.hpp>
#include "libnbtp.h"
#include "memstream.h"

BOOST_AUTO_TEST_SUITE(BasicParseTester)

	BOOST_AUTO_TEST_CASE(StringParse) {
		uint8_t bytes[] = {0x00, 0x04, 0x54, 0x65, 0x73, 0x74};
		memstream input(bytes, 6);
		std::string parsed = NBTP::StringTag::parseString(input);
		BOOST_ASSERT(parsed.length() == 4);
	}

	BOOST_AUTO_TEST_CASE(StringTagParse) {
		uint8_t bytes[] = {0x00, 0x09, 0x54, 0x65, 0x73, 0x74, 0x20, 0x42, 0x79, 0x74, 0x65, 0x12};
		memstream input(bytes, sizeof(bytes));
		auto parsed = NBTP::StringTag(input);
		BOOST_ASSERT(parsed.size() == 9);
	}

BOOST_AUTO_TEST_SUITE_END()