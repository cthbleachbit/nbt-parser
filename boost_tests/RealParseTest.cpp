#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include "TagIO.h"

BOOST_AUTO_TEST_SUITE(RealParseTester)

	BOOST_AUTO_TEST_CASE(World) {
		std::ifstream input("test_data/world.dat.nbt", std::ios::in | std::ios::binary);
		auto tag = NBTP::TagIO::parseRoot(input);
		std::cout << *tag ;
	}

BOOST_AUTO_TEST_SUITE_END()