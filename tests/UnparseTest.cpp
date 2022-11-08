#include <fstream>
#include <gtest/gtest.h>

#include "libnbtp.h"

#ifndef TEST_BINARY_DATA_DIR
#define TEST_BINARY_DATA_DIR "."
#endif

class ParseUnparseComparator : public ::testing::Test {
protected:
	void testFromFile(const std::string &input_path) {
		ssize_t size_of_file;
		/* Read in original input file */
		std::ifstream input_file(std::string(TEST_BINARY_DATA_DIR) + "/" + input_path + ".nbt", std::ios::in | std::ios::binary);
		auto input_tag = NBTP::TagIO::parseRoot(input_file, size_of_file, NBTP::IOFormat::BIN);
		input_file.close();

		/* Dump tag back into memory */
		std::stringstream ss;
		NBTP::TagIO::writeRoot(ss, *input_tag);

		/* Immediately parse that tag again from memory zone */
		ssize_t size_of_in_mem_tag;
		auto reparsed_tag = NBTP::TagIO::parseRoot(ss, size_of_in_mem_tag);

		/* Check invariance */
		ASSERT_EQ(size_of_file, size_of_in_mem_tag);
		ASSERT_EQ(*input_tag, *reparsed_tag);
		ASSERT_EQ(ss.tellg(), ss.tellp());
	}
};

TEST_F(ParseUnparseComparator, World) {
	testFromFile(test_info_->name());
}

TEST_F(ParseUnparseComparator, Player) {
	testFromFile(test_info_->name());
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}