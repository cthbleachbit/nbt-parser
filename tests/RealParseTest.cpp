#include <iostream>
#include <fstream>
#include <gtest/gtest.h>
#include "libnbtp.h"
#include "tags/CompoundTag.h"

#ifndef TEST_BINARY_DATA_DIR
#define TEST_BINARY_DATA_DIR "."
#endif

class BinaryParseTestHelper : public ::testing::Test {
protected:
	auto parseFromFile(const std::string &input_path, ssize_t &size) {
		std::ifstream input(std::string(TEST_BINARY_DATA_DIR) + "/" + input_path, std::ios::in | std::ios::binary);
		return NBTP::TagIO::parseRoot(input, size, NBTP::IOFormat::BIN);
	}
};

TEST_F(BinaryParseTestHelper, World) {
	ssize_t size;
	auto parsed = parseFromFile("World.nbt", size);
	ASSERT_EQ(size, 1212);
	ASSERT_EQ(parsed->typeCode(), NBTP::COMPOUND);
	auto r_tag = (NBTP::CompoundTag*)(parsed.get());
	ASSERT_EQ(r_tag->size(), 1);
	auto data = r_tag->lookup("Data");
	ASSERT_EQ(data->typeCode(), NBTP::COMPOUND);
	auto data_tag = (NBTP::CompoundTag*)(data.get());
	ASSERT_EQ(data_tag->size(), 38);
	ASSERT_EQ(*data_tag->lookup("generatorOptions"), NBTP::StringTag());
}

TEST_F(BinaryParseTestHelper, Player) {
	ssize_t size;
	auto parsed = parseFromFile("Player.nbt", size);
	ASSERT_EQ(parsed->typeCode(), NBTP::COMPOUND);
	auto r_tag = (NBTP::CompoundTag*)(parsed.get());
	ASSERT_EQ(r_tag->size(), 47);
	auto bukkit = r_tag->lookup("bukkit");
	ASSERT_EQ(bukkit->typeCode(), NBTP::COMPOUND);
	auto bukkit_tag = (NBTP::CompoundTag*)(bukkit.get());
	ASSERT_EQ(bukkit_tag->size(), 8);
	ASSERT_EQ(*bukkit_tag->lookup("lastKnownName"), NBTP::StringTag("hundunfensitang"));
}

TEST_F(BinaryParseTestHelper, MapItemNoMarkers) {
	ssize_t size;
	auto parsed = parseFromFile("MapItemNoMarkers.nbt", size);
	ASSERT_EQ(size, 16500);
	ASSERT_EQ(parsed->typeCode(), NBTP::COMPOUND);
	auto r_tag = (NBTP::CompoundTag*)(parsed.get());
	auto data = r_tag->lookup("data");
	ASSERT_EQ(data->typeCode(), NBTP::COMPOUND);
	auto data_tag = (NBTP::CompoundTag*)(data.get());
	ASSERT_EQ(data_tag->size(), 8);
	ASSERT_EQ(*data_tag->lookup("xCenter"), NBTP::IntTag(-576));
}

TEST_F(BinaryParseTestHelper, Malformed) {
	ssize_t size;
	std::shared_ptr<NBTP::Tag> parsed;
	ASSERT_THROW(parseFromFile("Malformed.nbt", size), std::ios_base::failure);
}

int main(int argc, char **argv) {

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}