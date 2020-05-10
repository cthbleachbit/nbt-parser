#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include "libnbtp.h"
#include <iostream>
#include <boost/format.hpp>

BOOST_AUTO_TEST_SUITE(FloatTester)

	BOOST_AUTO_TEST_CASE(F) {
		float test;
		uint8_t *test_arr = reinterpret_cast<uint8_t *>(&test);
		test_arr[0] = 0x3f;
		test_arr[1] = 0x00;
		test_arr[2] = 0x00;
		test_arr[3] = 0x00;
		BOOST_ASSERT(NBTP::Conversion::conv_f_beh(test) == 0.5f);
	}

	BOOST_AUTO_TEST_CASE(D) {
		double test = 0;
		uint8_t *test_arr = reinterpret_cast<uint8_t *>(&test);
		test_arr[0] = 0x3f;
		test_arr[1] = 0xc0;
		BOOST_ASSERT(NBTP::Conversion::conv_d_beh(test) == 0.125);
	}

BOOST_AUTO_TEST_SUITE_END()