#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <memory>
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include "libnbtp.h"
#include "memstream.h"

BOOST_AUTO_TEST_SUITE(UnparseTester)

	BOOST_AUTO_TEST_CASE(World) {
		std::ifstream input("test_data/world.dat.nbt", std::ios::in | std::ios::binary);
		ssize_t size;
		auto tag = NBTP::TagIO::parseRoot(input, size);
		input.close();

		std::ofstream output("test_data/world.dat.nbt.out", std::ios::out | std::ios::binary);
		NBTP::TagIO::dumpRoot(output, *tag);
		output.close();

		std::ifstream input_recursion("test_data/world.dat.nbt.out", std::ios::in | std::ios::binary);
		size = 0;
		tag = NBTP::TagIO::parseRoot(input, size);
		std::cout << *tag << std::endl;
	}

BOOST_AUTO_TEST_SUITE_END()