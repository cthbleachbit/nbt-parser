#include "libnbtp.h"
#include <iostream>
#include <gtest/gtest.h>

TEST(FloatBigEndianTest, F) {
	float test;
	uint8_t *test_arr = reinterpret_cast<uint8_t *>(&test);
	test_arr[0] = 0x3f;
	test_arr[1] = 0x00;
	test_arr[2] = 0x00;
	test_arr[3] = 0x00;
	ASSERT_EQ(NBTP::Conversion::conv_f_beh(test), 0.5f);
}

TEST(FloatBigEndianTest, D) {
	double test = 0;
	uint8_t *test_arr = reinterpret_cast<uint8_t *>(&test);
	test_arr[0] = 0x3f;
	test_arr[1] = 0xc0;
	ASSERT_EQ(NBTP::Conversion::conv_d_beh(test), 0.125);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}