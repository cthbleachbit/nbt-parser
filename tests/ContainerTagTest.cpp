#include <memory>
#include <iostream>
#include <gtest/gtest.h>

#include "libnbtp.h"
#include "tags/Tag.h"
#include "memstream.h"

TEST(ContainerTagTest, Bytes) {
	auto bytes = std::make_shared<NBTP::BytesTag>();
	bytes->insert(5);
	ASSERT_EQ(bytes->size(), 1);
	ASSERT_THROW(bytes->insert(std::shared_ptr<NBTP::Tag>(new NBTP::IntTag(3))), NBTP::ListTypeMismatchException);
}

TEST(ContainerTagTest, Longs) {
	auto longs = std::make_shared<NBTP::LongsTag>();
	longs->insert(3874747474747474774L);
	ASSERT_EQ(longs->size(), 1);
	ASSERT_THROW(longs->insert(std::shared_ptr<NBTP::Tag>(new NBTP::IntTag(3))), NBTP::ListTypeMismatchException);
}

TEST(ContainerTagTest, LongsItr) {
	auto longs = std::make_shared<NBTP::LongsTag>();
	longs->insert(3874747474747474774L);
	longs->insert(0L);
	auto itr = longs->getPayload().begin();
	ASSERT_EQ(itr->get()->typeCode(), NBTP::TagType::LONG);
	ASSERT_EQ(longs->size(), 2);
}

TEST(ContainerTagTest, Str) {
	auto str = std::make_shared<NBTP::StringTag>();
	ASSERT_EQ(str->size(), 0);
	str->setPayload("test");
	ASSERT_EQ(str->size(), 4);
}

TEST(ContainerTagTest, ComplexEqual) {
	uint8_t bytes1[] = {0x01, 0x00, 0x01, 0x45, 0xff, 0x01, 0x00, 0x01, 0x46, 0x55, 0x00};
	memstream input1(bytes1, sizeof(bytes1));
	ssize_t run_len = 0;
	auto parsed1 = NBTP::CompoundTag(input1, run_len);

	uint8_t bytes2[] = {0x01, 0x00, 0x01, 0x46, 0x55, 0x01, 0x00, 0x01, 0x45, 0xff, 0x00};
	memstream input2(bytes2, sizeof(bytes2));
	run_len = 0;
	auto parsed2 = NBTP::CompoundTag(input2, run_len);
	ASSERT_EQ(parsed1.getPayload().size(), 2);
	ASSERT_EQ(parsed1, parsed2);
}

TEST(ContainerTagTest, ComplexInsert) {
	auto tag = std::make_shared<NBTP::CompoundTag>();
	tag->insert("www", std::make_shared<NBTP::IntTag>(20));
	std::cout << *tag << std::endl;
	auto subtag = (NBTP::IntTag &) *(tag->lookup("www"));
	ASSERT_EQ(subtag.getPayload(), 20);
}

TEST(ContainerTagTest, Nonexistent) {
	auto tag = std::make_shared<NBTP::CompoundTag>();
	ASSERT_EQ(tag->lookup("www"), nullptr);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}