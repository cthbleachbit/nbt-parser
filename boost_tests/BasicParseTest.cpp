#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <memory>
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "libnbtp.h"
#include "memstream.h"

BOOST_AUTO_TEST_SUITE(BasicParseTester)

	BOOST_AUTO_TEST_CASE(StringParse) {
		uint8_t bytes[] = {0x00, 0x04, 0x54, 0x65, 0x73, 0x74};
		memstream input(bytes, sizeof(bytes));
		ssize_t run_len = 0;
		std::string parsed = NBTP::StringTag::parseString(input, run_len);
		BOOST_ASSERT(parsed.length() == 4);
		BOOST_ASSERT(run_len == sizeof(bytes));
	}

	BOOST_AUTO_TEST_CASE(StringTagParse) {
		uint8_t bytes[] = {0x00, 0x09, 0x54, 0x65, 0x73, 0x74, 0x20, 0x42, 0x79, 0x74, 0x65};
		memstream input(bytes, sizeof(bytes));
		ssize_t run_len = 0;
		auto parsed = NBTP::StringTag(input, run_len);
		BOOST_ASSERT(parsed.size() == 9);
		BOOST_ASSERT(run_len == sizeof(bytes));
	}

	BOOST_AUTO_TEST_CASE(BytesParse) {
		uint8_t bytes[] = {0x00, 0x00, 0x00, 0x03, 0x01, 0x02, 0x03};
		memstream input(bytes, sizeof(bytes));
		ssize_t run_len = 0;
		auto parsed = NBTP::BytesTag(input, run_len, NBTP::IOFormat::BIN);
		BOOST_ASSERT(parsed.size() == 3);
		BOOST_ASSERT(run_len == sizeof(bytes));
	}

	BOOST_AUTO_TEST_CASE(IntsParse) {
		uint8_t bytes[] = {0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x02, 0xff, 0xff, 0xff,
		                   0xff};
		memstream input(bytes, sizeof(bytes));
		ssize_t run_len = 0;
		auto parsed = NBTP::IntsTag(input, run_len);
		BOOST_ASSERT(parsed.size() == 3);
		BOOST_ASSERT(((NBTP::IntTag *) parsed.getPayload()[0].get())->getPayload() == 255);
		BOOST_ASSERT(((NBTP::IntTag *) parsed.getPayload()[1].get())->getPayload() == 2);
		BOOST_ASSERT(((NBTP::IntTag *) parsed.getPayload()[2].get())->getPayload() == -1);
	}

	BOOST_AUTO_TEST_CASE(EmptyCompound) {
		uint8_t bytes[] = {0x00};
		memstream input(bytes, sizeof(bytes));
		ssize_t run_len = 0;
		auto parsed = NBTP::CompoundTag(input, run_len);
		BOOST_ASSERT(parsed.size() == 0);
	}

	BOOST_AUTO_TEST_CASE(OneSubElemCompound) {
		uint8_t bytes[] = {0x01, 0x00, 0x01, 0x44, 0xff, 0x00};
		memstream input(bytes, sizeof(bytes));
		ssize_t run_len = 0;
		auto parsed = NBTP::CompoundTag(input, run_len);
		BOOST_ASSERT(parsed.size() == 1);
		auto container = parsed.getPayload();
		BOOST_ASSERT(container["D"]->typeCode() == NBTP::TagType::BYTE);
		std::cout << parsed << std::endl;
	}

BOOST_AUTO_TEST_SUITE_END()