#include <gtest/gtest.h>
#include <iostream>
#include "libnbtp.h"
#include "memstream.h"

TEST(BasicParseTest, StringParse) {
	uint8_t bytes[] = {0x00, 0x04, 0x54, 0x65, 0x73, 0x74};
	memstream input(bytes, sizeof(bytes));
	ssize_t run_len = 0;
	std::string parsed = NBTP::StringTag::parseString(input, run_len);
	EXPECT_EQ(parsed.length(), 4);
	EXPECT_EQ(run_len, sizeof(bytes));
}

TEST(BasicParseTest, StringTagParse) {
	uint8_t bytes[] = {0x00, 0x09, 0x54, 0x65, 0x73, 0x74, 0x20, 0x42, 0x79, 0x74, 0x65};
	memstream input(bytes, sizeof(bytes));
	ssize_t run_len = 0;
	auto parsed = NBTP::StringTag(input, run_len);
	EXPECT_EQ(parsed.size(), 9);
	EXPECT_EQ(run_len, sizeof(bytes));
}

TEST(BasicParseTest, BytesParse) {
	uint8_t bytes[] = {0x00, 0x00, 0x00, 0x03, 0x01, 0x02, 0x03};
	memstream input(bytes, sizeof(bytes));
	ssize_t run_len = 0;
	auto parsed = NBTP::BytesTag(input, run_len, NBTP::IOFormat::BIN);
	std::cout << parsed << std::endl;
	EXPECT_EQ(parsed.size(), 3);
	EXPECT_EQ(run_len, sizeof(bytes));
}

TEST(BasicParseTest, IntsParse) {
	uint8_t bytes[] = {
			0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x02, 0xff, 0xff, 0xff,
			0xff
	};
	memstream input(bytes, sizeof(bytes));
	ssize_t run_len = 0;
	auto parsed = NBTP::IntsTag(input, run_len);
	EXPECT_EQ(parsed.size(), 3);
	EXPECT_EQ(((NBTP::IntTag *) parsed.getPayload()[0].get())->getPayload(), 255);
	EXPECT_EQ(((NBTP::IntTag *) parsed.getPayload()[1].get())->getPayload(), 2);
	EXPECT_EQ(((NBTP::IntTag *) parsed.getPayload()[2].get())->getPayload(), -1);
}

TEST(BasicParseTest, EmptyCompound) {
	uint8_t bytes[] = {0x00};
	memstream input(bytes, sizeof(bytes));
	ssize_t run_len = 0;
	auto parsed = NBTP::CompoundTag(input, run_len);
	EXPECT_EQ(parsed.size(), 0);
}

TEST(BasicParseTest, OneSubElemCompound) {
	uint8_t bytes[] = {0x01, 0x00, 0x01, 0x44, 0xff, 0x00};
	memstream input(bytes, sizeof(bytes));
	ssize_t run_len = 0;
	auto parsed = NBTP::CompoundTag(input, run_len);
	EXPECT_EQ(parsed.size(), 1);
	auto container = parsed.getPayload();
	EXPECT_EQ(container["D"]->typeCode(), NBTP::TagType::BYTE);
	std::cout << parsed << std::endl;
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}