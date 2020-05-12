#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include "TagIO.h"

BOOST_AUTO_TEST_SUITE(RealParseTester)

	BOOST_AUTO_TEST_CASE(World) {
		std::ifstream input("test_data/world.dat.nbt", std::ios::in | std::ios::binary);
		ssize_t size;
		auto tag = NBTP::TagIO::parseRoot(input, size, NBTP::IOFormat::BIN);
		std::cout << *tag ;
		BOOST_ASSERT(size == 1212);
	}

	BOOST_AUTO_TEST_CASE(Player) {
		std::ifstream input("test_data/playerdata.nbt", std::ios::in | std::ios::binary);
		ssize_t size;
		auto tag = NBTP::TagIO::parseRoot(input, size, NBTP::IOFormat::BIN);
		std::cout << *tag ;
	}

	BOOST_AUTO_TEST_CASE(Map) {
		std::ifstream input("test_data/map_101.nbt", std::ios::in | std::ios::binary);
		ssize_t size;
		auto tag = NBTP::TagIO::parseRoot(input, size, NBTP::IOFormat::BIN);
		std::cout << *tag ;
	}

BOOST_AUTO_TEST_SUITE_END()